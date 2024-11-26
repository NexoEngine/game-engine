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

    //TODO: Add tests for the singleton components
}