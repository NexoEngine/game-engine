//// Coordinator.test.cpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/11/2024
//  Description: Test file for the coordinator
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ecs/Coordinator.hpp"
#include "ecs/SingletonComponent.hpp"
#include "ecs/Signature.hpp"
#include "ecs/System.hpp"
#include "ecs/Entity.hpp"

namespace nexo::ecs {
    // Mock Component for testing
    struct TestComponent {
        int data;
    };

    struct ComponentA {
        int value;
    };

    struct ComponentB {
        float data;
    };

    struct TestSingletonComponent {
        int value;
    };

    // Mock System for testing
    class MockSystem : public System {
        public:
        MOCK_METHOD(void, update, (), (const));
    };

    class CoordinatorTest : public ::testing::Test {
        protected:
        void SetUp() override {
            coordinator = std::make_unique<Coordinator>();
            coordinator->init();

            coordinator->registerComponent<ComponentA>();
            coordinator->registerComponent<ComponentB>();
        }

        std::unique_ptr<Coordinator> coordinator;
    };

    TEST_F(CoordinatorTest, Initialization) {
        EXPECT_NO_THROW(coordinator->init());
    }

    TEST_F(CoordinatorTest, CreateAndDestroyEntity) {
        Entity entity = coordinator->createEntity();
        EXPECT_NO_THROW(coordinator->destroyEntity(entity));
    }

    TEST_F(CoordinatorTest, DestroyNonexistentEntity) {
        Entity nonexistentEntity = 99999;
        EXPECT_THROW(coordinator->destroyEntity(nonexistentEntity), OutOfRange);
    }

    TEST_F(CoordinatorTest, RegisterAndAddComponent) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();
        TestComponent component{42};

        EXPECT_NO_THROW(coordinator->addComponent(entity, component));
        EXPECT_EQ(coordinator->getComponent<TestComponent>(entity).data, 42);
    }

    TEST_F(CoordinatorTest, RemoveComponent) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();
        TestComponent component{42};

        coordinator->addComponent(entity, component);
        EXPECT_NO_THROW(coordinator->removeComponent<TestComponent>(entity));
    }

    TEST_F(CoordinatorTest, TryRemoveNonexistentComponent) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();

        EXPECT_NO_THROW(coordinator->tryRemoveComponent<TestComponent>(entity));
    }

    TEST_F(CoordinatorTest, AddComponentToNonexistentEntity) {
        coordinator->registerComponent<TestComponent>();

        Entity nonexistentEntity = 99999;
        TestComponent component{42};

        EXPECT_THROW(coordinator->addComponent(nonexistentEntity, component), OutOfRange);
    }

    TEST_F(CoordinatorTest, RemoveComponentFromNonexistentEntity) {
        coordinator->registerComponent<TestComponent>();

        Entity nonexistentEntity = 99999;

        EXPECT_THROW(coordinator->removeComponent<TestComponent>(nonexistentEntity), ComponentNotFound);
    }

    TEST_F(CoordinatorTest, RegisterSystemAndSetSignature) {
        auto system = coordinator->registerSystem<MockSystem>();

        EXPECT_NE(system, nullptr);

        Signature signature;
        signature.set(1);

        EXPECT_NO_THROW(coordinator->setSystemSignature<MockSystem>(signature));
    }

    TEST_F(CoordinatorTest, UpdateSystemEntities) {
        auto system = coordinator->registerSystem<MockSystem>();
        coordinator->registerComponent<TestComponent>();

        Signature signature;
        signature.set(coordinator->getComponentType<TestComponent>());

        coordinator->setSystemSignature<MockSystem>(signature);

        Entity entity = coordinator->createEntity();
        TestComponent component{42};

        coordinator->registerComponent<TestComponent>();
        coordinator->addComponent(entity, component);

        EXPECT_TRUE(system->entities.contains(entity));
    }

    TEST_F(CoordinatorTest, SystemDoesNotIncludeMismatchedEntity) {
        auto system = coordinator->registerSystem<MockSystem>();

        Signature signature;
        signature.set(1);

        coordinator->setSystemSignature<MockSystem>(signature);

        Entity entity = coordinator->createEntity();
        TestComponent component{42};

        coordinator->registerComponent<TestComponent>();
        // Entity signature does not match the system
        Signature entitySignature;
        entitySignature.set(2);
        coordinator->setSystemSignature<MockSystem>(entitySignature);

        coordinator->addComponent(entity, component);
        EXPECT_FALSE(system->entities.contains(entity));
    }

    TEST_F(CoordinatorTest, GetAllComponents) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();
        TestComponent component{42};
        coordinator->addComponent(entity, component);

        auto components = coordinator->getAllComponents(entity);
        EXPECT_EQ(components.size(), 1);

        auto& [type, value] = components[0];
        EXPECT_EQ(type, std::type_index(typeid(TestComponent)));
        EXPECT_EQ(std::any_cast<TestComponent>(value).data, 42);
    }

    TEST_F(CoordinatorTest, GetAllEntitiesWith_NoMatch) {
        Entity e1 = coordinator->createEntity();
        ComponentA compA{10};
        coordinator->addComponent(e1, compA);

        std::set<Entity> result = coordinator->getAllEntitiesWith<ComponentA, ComponentB>();
        EXPECT_TRUE(result.empty());
    }

    TEST_F(CoordinatorTest, GetAllEntitiesWith_SingleMatch) {
        Entity e1 = coordinator->createEntity();
        coordinator->addComponent(e1, ComponentA{10});

        Entity e2 = coordinator->createEntity();
        coordinator->addComponent(e2, ComponentA{20});
        coordinator->addComponent(e2, ComponentB{3.14f});

        std::set<Entity> result = coordinator->getAllEntitiesWith<ComponentA, ComponentB>();
        EXPECT_EQ(result.size(), 1);
        EXPECT_TRUE(result.find(e2) != result.end());
    }

    TEST_F(CoordinatorTest, GetAllEntitiesWith_MultipleMatches) {
        Entity e1 = coordinator->createEntity();
        coordinator->addComponent(e1, ComponentA{1});
        coordinator->addComponent(e1, ComponentB{1.0f});

        Entity e2 = coordinator->createEntity();
        coordinator->addComponent(e2, ComponentA{2});
        coordinator->addComponent(e2, ComponentB{2.0f});

        Entity e3 = coordinator->createEntity();
        coordinator->addComponent(e3, ComponentA{3});
        coordinator->addComponent(e3, ComponentB{3.0f});

        std::set<Entity> result = coordinator->getAllEntitiesWith<ComponentA, ComponentB>();
        EXPECT_EQ(result.size(), 3);
        EXPECT_TRUE(result.find(e1) != result.end());
        EXPECT_TRUE(result.find(e2) != result.end());
        EXPECT_TRUE(result.find(e3) != result.end());
    }

    TEST_F(CoordinatorTest, DestroyedEntityNotReturned) {
        // Create an entity with both components.
        Entity e1 = coordinator->createEntity();
        coordinator->addComponent(e1, ComponentA{10});
        coordinator->addComponent(e1, ComponentB{2.5f});

        // Verify it is returned.
        std::set<Entity> result = coordinator->getAllEntitiesWith<ComponentA, ComponentB>();
        EXPECT_TRUE(result.find(e1) != result.end());

        // Destroy the entity.
        coordinator->destroyEntity(e1);

        result = coordinator->getAllEntitiesWith<ComponentA, ComponentB>();
        EXPECT_TRUE(result.find(e1) == result.end());
    }

    TEST_F(CoordinatorTest, TryGetComponentWorks) {
        Entity e1 = coordinator->createEntity();
        ComponentA compA{100};
        coordinator->addComponent(e1, compA);

        std::optional<std::reference_wrapper<ComponentA>> optComp = coordinator->tryGetComponent<ComponentA>(e1);
        ASSERT_TRUE(optComp.has_value());
        EXPECT_EQ(optComp->get().value, 100);

        // For an entity without ComponentB, tryGetComponent should return empty.
        std::optional<std::reference_wrapper<ComponentB>> optCompB = coordinator->tryGetComponent<ComponentB>(e1);
        EXPECT_FALSE(optCompB.has_value());
    }

    TEST_F(CoordinatorTest, SingletonComponent_RegisterAndGet) {
        // Register the singleton.
        EXPECT_NO_THROW(coordinator->registerSingletonComponent<TestSingletonComponent>(42));

        // Retrieve it and check its value.
        TestSingletonComponent &retrieved = coordinator->getSingletonComponent<TestSingletonComponent>();
        EXPECT_EQ(retrieved.value, 42);
    }

    TEST_F(CoordinatorTest, SingletonComponent_Remove) {
        // Register the singleton.
        coordinator->registerSingletonComponent<TestSingletonComponent>(77);

        // Check that it can be retrieved.
        {
            TestSingletonComponent &retrieved = coordinator->getSingletonComponent<TestSingletonComponent>();
            EXPECT_EQ(retrieved.value, 77);
        }

        // Remove the singleton.
        EXPECT_NO_THROW(coordinator->removeSingletonComponent<TestSingletonComponent>());

        // After removal, trying to get the singleton should throw.
        EXPECT_THROW({
            coordinator->getSingletonComponent<TestSingletonComponent>();
        }, std::exception);
    }

    TEST_F(CoordinatorTest, SingletonComponent_ReRegister) {
        coordinator->registerSingletonComponent<TestSingletonComponent>(100);
        {
            TestSingletonComponent &retrieved = coordinator->getSingletonComponent<TestSingletonComponent>();
            EXPECT_EQ(retrieved.value, 100);
        }
        // Remove and register a new value.
        coordinator->removeSingletonComponent<TestSingletonComponent>();
        coordinator->registerSingletonComponent<TestSingletonComponent>(200);
        {
            TestSingletonComponent &retrieved = coordinator->getSingletonComponent<TestSingletonComponent>();
            EXPECT_EQ(retrieved.value, 200);
        }
    }
}
