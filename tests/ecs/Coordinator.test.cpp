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
#include "Components.hpp"
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

    TEST_F(CoordinatorTest, GetComponentArray) {
        // Register a component type
        coordinator->registerComponent<TestComponent>();

        // Get the component array
        auto componentArray = coordinator->getComponentArray<TestComponent>();
        ASSERT_NE(componentArray, nullptr);

        // Test basic functionality of the component array
        Entity entity = coordinator->createEntity();
        componentArray->insertData(entity, TestComponent{42});
        EXPECT_EQ(componentArray->getData(entity).data, 42);

        // Verify the component was actually added to the entity
        EXPECT_EQ(coordinator->getComponent<TestComponent>(entity).data, 42);
    }

    TEST_F(CoordinatorTest, GetAllComponentTypes) {
        // Register multiple component types
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();

        // Initially, the entity has no components
        auto types = coordinator->getAllComponentTypes(entity);
        EXPECT_TRUE(types.empty());

        // Add components and check types
        coordinator->addComponent(entity, TestComponent{42});
        coordinator->addComponent(entity, ComponentA{10});
        coordinator->addComponent(entity, ComponentB{3.14f});

        types = coordinator->getAllComponentTypes(entity);
        EXPECT_EQ(types.size(), 3);

        // Verify correct types are returned (specific order may vary)
        bool hasTestComponent = false;
        bool hasComponentA = false;
        bool hasComponentB = false;

        for (const auto& typeIndex : types) {
            if (typeIndex == std::type_index(typeid(TestComponent))) hasTestComponent = true;
            else if (typeIndex == std::type_index(typeid(ComponentA))) hasComponentA = true;
            else if (typeIndex == std::type_index(typeid(ComponentB))) hasComponentB = true;
        }

        EXPECT_TRUE(hasTestComponent);
        EXPECT_TRUE(hasComponentA);
        EXPECT_TRUE(hasComponentB);

        // Remove a component and verify types are updated
        coordinator->removeComponent<TestComponent>(entity);
        types = coordinator->getAllComponentTypes(entity);
        EXPECT_EQ(types.size(), 2);

        hasTestComponent = false;
        hasComponentA = false;
        hasComponentB = false;

        for (const auto& typeIndex : types) {
            if (typeIndex == std::type_index(typeid(TestComponent))) hasTestComponent = true;
            else if (typeIndex == std::type_index(typeid(ComponentA))) hasComponentA = true;
            else if (typeIndex == std::type_index(typeid(ComponentB))) hasComponentB = true;
        }

        EXPECT_FALSE(hasTestComponent);
        EXPECT_TRUE(hasComponentA);
        EXPECT_TRUE(hasComponentB);
    }

    TEST_F(CoordinatorTest, EntityHasComponent) {
        coordinator->registerComponent<TestComponent>();
        coordinator->registerComponent<ComponentA>();

        Entity entity = coordinator->createEntity();

        // Initially, the entity has no components
        EXPECT_FALSE(coordinator->entityHasComponent<TestComponent>(entity));
        EXPECT_FALSE(coordinator->entityHasComponent<ComponentA>(entity));

        // Add a component
        coordinator->addComponent(entity, TestComponent{42});
        EXPECT_TRUE(coordinator->entityHasComponent<TestComponent>(entity));
        EXPECT_FALSE(coordinator->entityHasComponent<ComponentA>(entity));

        // Add another component
        coordinator->addComponent(entity, ComponentA{10});
        EXPECT_TRUE(coordinator->entityHasComponent<TestComponent>(entity));
        EXPECT_TRUE(coordinator->entityHasComponent<ComponentA>(entity));

        // Remove a component
        coordinator->removeComponent<TestComponent>(entity);
        EXPECT_FALSE(coordinator->entityHasComponent<TestComponent>(entity));
        EXPECT_TRUE(coordinator->entityHasComponent<ComponentA>(entity));
    }

    TEST_F(CoordinatorTest, GetAllComponentsComprehensive) {
        coordinator->registerComponent<TestComponent>();
        coordinator->registerComponent<ComponentA>();
        coordinator->registerComponent<ComponentB>();

        Entity entity = coordinator->createEntity();

        // Initially, the entity has no components
        auto components = coordinator->getAllComponents(entity);
        EXPECT_TRUE(components.empty());

        // Add components
        coordinator->addComponent(entity, TestComponent{42});
        coordinator->addComponent(entity, ComponentA{10});
        coordinator->addComponent(entity, ComponentB{3.14f});

        components = coordinator->getAllComponents(entity);
        EXPECT_EQ(components.size(), 3);

        // Verify components are correctly returned
        bool hasTestComponent = false;
        bool hasComponentA = false;
        bool hasComponentB = false;

        for (const auto& [type, value] : components) {
            if (type == std::type_index(typeid(TestComponent))) {
                hasTestComponent = true;
                EXPECT_EQ(std::any_cast<TestComponent>(value).data, 42);
            } else if (type == std::type_index(typeid(ComponentA))) {
                hasComponentA = true;
                EXPECT_EQ(std::any_cast<ComponentA>(value).value, 10);
            } else if (type == std::type_index(typeid(ComponentB))) {
                hasComponentB = true;
                EXPECT_FLOAT_EQ(std::any_cast<ComponentB>(value).data, 3.14f);
            }
        }

        EXPECT_TRUE(hasTestComponent);
        EXPECT_TRUE(hasComponentA);
        EXPECT_TRUE(hasComponentB);
    }

    TEST_F(CoordinatorTest, ModifyComponent) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();
        coordinator->addComponent(entity, TestComponent{42});

        // Modify component through getComponent
        coordinator->getComponent<TestComponent>(entity).data = 100;
        EXPECT_EQ(coordinator->getComponent<TestComponent>(entity).data, 100);

        // Modify component through a reference
        TestComponent& component = coordinator->getComponent<TestComponent>(entity);
        component.data = 200;
        EXPECT_EQ(coordinator->getComponent<TestComponent>(entity).data, 200);
    }

    // This test verifies the updateSystemEntities functionality indirectly
    TEST_F(CoordinatorTest, SystemEntityUpdatesWhenComponentsChange) {
        // Setup system with a specific signature
        auto system = coordinator->registerSystem<MockSystem>();
        coordinator->registerComponent<TestComponent>();
        coordinator->registerComponent<ComponentA>();

        Signature signature;
        signature.set(coordinator->getComponentType<TestComponent>());
        signature.set(coordinator->getComponentType<ComponentA>());
        coordinator->setSystemSignature<MockSystem>(signature);

        // Create entity and add only TestComponent - should not be in system
        Entity entity = coordinator->createEntity();
        coordinator->addComponent(entity, TestComponent{42});
        EXPECT_FALSE(system->entities.contains(entity));

        // Add ComponentA - now it should match the system signature
        coordinator->addComponent(entity, ComponentA{10});
        EXPECT_TRUE(system->entities.contains(entity));

        // Remove ComponentA - should no longer match
        coordinator->removeComponent<ComponentA>(entity);
        EXPECT_FALSE(system->entities.contains(entity));
    }

    TEST_F(CoordinatorTest, MultipleSystemsWithDifferentSignatures) {
        // Create two different systems with different signatures
        auto systemA = coordinator->registerSystem<MockSystem>();

        class AnotherMockSystem : public System {
        public:
            MOCK_METHOD(void, update, (), (const));
        };

        auto systemB = coordinator->registerSystem<AnotherMockSystem>();

        coordinator->registerComponent<TestComponent>();
        coordinator->registerComponent<ComponentA>();
        coordinator->registerComponent<ComponentB>();

        // Set different signatures for each system
        Signature signatureA;
        signatureA.set(coordinator->getComponentType<TestComponent>());
        signatureA.set(coordinator->getComponentType<ComponentA>());
        coordinator->setSystemSignature<MockSystem>(signatureA);

        Signature signatureB;
        signatureB.set(coordinator->getComponentType<ComponentB>());
        coordinator->setSystemSignature<AnotherMockSystem>(signatureB);

        // Create entities with different combinations of components
        Entity entity1 = coordinator->createEntity();
        Entity entity2 = coordinator->createEntity();
        Entity entity3 = coordinator->createEntity();

        // Entity1 has TestComponent and ComponentA - should only be in systemA
        coordinator->addComponent(entity1, TestComponent{1});
        coordinator->addComponent(entity1, ComponentA{1});

        // Entity2 has ComponentB - should only be in systemB
        coordinator->addComponent(entity2, ComponentB{2.0f});

        // Entity3 has all components - should be in both systems
        coordinator->addComponent(entity3, TestComponent{3});
        coordinator->addComponent(entity3, ComponentA{3});
        coordinator->addComponent(entity3, ComponentB{3.0f});

        // Check each system has the expected entities
        EXPECT_TRUE(systemA->entities.contains(entity1));
        EXPECT_FALSE(systemA->entities.contains(entity2));
        EXPECT_TRUE(systemA->entities.contains(entity3));

        EXPECT_FALSE(systemB->entities.contains(entity1));
        EXPECT_TRUE(systemB->entities.contains(entity2));
        EXPECT_TRUE(systemB->entities.contains(entity3));
    }

    TEST_F(CoordinatorTest, EntityCreationMaximumLimit) {
        // This test should be adjusted based on your MAX_ENTITIES value
        // For simplicity, let's create a reasonable number of entities
        const int numEntities = MAX_ENTITIES; // Adjust based on your MAX_ENTITIES

        std::vector<Entity> entities;
        coordinator->registerComponent<TestComponent>();

        // Create entities up to the limit
        for (int i = 0; i < numEntities; ++i) {
            EXPECT_NO_THROW({
                Entity entity = coordinator->createEntity();
                entities.push_back(entity);
            });
        }

        // Verify all entities were created correctly
        for (const auto& entity : entities) {
            EXPECT_NO_THROW(coordinator->addComponent(entity, TestComponent{1}));
        }

        // Delete half the entities
        for (size_t i = 0; i < entities.size() / 2; ++i) {
            EXPECT_NO_THROW(coordinator->destroyEntity(entities[i]));
        }

        // Create more entities to replace the deleted ones
        for (size_t i = 0; i < entities.size() / 2; ++i) {
            EXPECT_NO_THROW({
                Entity entity = coordinator->createEntity();
                // Could store these new entities if needed
            });
        }
    }

    TEST_F(CoordinatorTest, ComponentArrayIntegration) {
        coordinator->registerComponent<TestComponent>();

        // Get component array directly
        auto componentArray = coordinator->getComponentArray<TestComponent>();

        // Create entity through coordinator
        Entity entity = coordinator->createEntity();

        // Add component directly through component array
        componentArray->insertData(entity, TestComponent{42});

        // Verify we can access it through the coordinator
        EXPECT_EQ(coordinator->getComponent<TestComponent>(entity).data, 42);

        // Modify through component array
        componentArray->getData(entity).data = 100;

        // Verify change is visible through coordinator
        EXPECT_EQ(coordinator->getComponent<TestComponent>(entity).data, 100);

        // Remove through component array
        componentArray->removeData(entity);

        // Verify it's gone
        EXPECT_THROW(coordinator->getComponent<TestComponent>(entity), ComponentNotFound);
    }

    TEST_F(CoordinatorTest, SingletonComponentEdgeCases) {
        // Try getting a non-existent singleton
        EXPECT_THROW(coordinator->getSingletonComponent<TestSingletonComponent>(), SingletonComponentNotRegistered);

        // Register, remove, and try to get
        coordinator->registerSingletonComponent<TestSingletonComponent>(42);
        coordinator->removeSingletonComponent<TestSingletonComponent>();
        EXPECT_THROW(coordinator->getSingletonComponent<TestSingletonComponent>(), SingletonComponentNotRegistered);

        // Remove a non-existent singleton
        EXPECT_THROW(coordinator->removeSingletonComponent<TestSingletonComponent>(), SingletonComponentNotRegistered);

        // Register multiple singleton types
        struct AnotherSingletonComponent {
            float value;
        };

        coordinator->registerSingletonComponent<TestSingletonComponent>(42);
        coordinator->registerSingletonComponent<AnotherSingletonComponent>(3.14f);

        EXPECT_EQ(coordinator->getSingletonComponent<TestSingletonComponent>().value, 42);
        EXPECT_FLOAT_EQ(coordinator->getSingletonComponent<AnotherSingletonComponent>().value, 3.14f);
    }

    TEST_F(CoordinatorTest, ComplexEntityComponentInteractions) {
        // Register multiple components
        struct Position { float x, y; };
        struct Velocity { float dx, dy; };
        struct Renderable { int spriteId; };
        struct Health { int current, max; };

        coordinator->registerComponent<Position>();
        coordinator->registerComponent<Velocity>();
        coordinator->registerComponent<Renderable>();
        coordinator->registerComponent<Health>();

        // Create a complex entity with multiple components
        Entity entity = coordinator->createEntity();
        coordinator->addComponent(entity, Position{10.0f, 20.0f});
        coordinator->addComponent(entity, Velocity{1.0f, 2.0f});
        coordinator->addComponent(entity, Renderable{5});
        coordinator->addComponent(entity, Health{100, 100});

        // Verify all components are accessible
        EXPECT_FLOAT_EQ(coordinator->getComponent<Position>(entity).x, 10.0f);
        EXPECT_FLOAT_EQ(coordinator->getComponent<Position>(entity).y, 20.0f);
        EXPECT_FLOAT_EQ(coordinator->getComponent<Velocity>(entity).dx, 1.0f);
        EXPECT_FLOAT_EQ(coordinator->getComponent<Velocity>(entity).dy, 2.0f);
        EXPECT_EQ(coordinator->getComponent<Renderable>(entity).spriteId, 5);
        EXPECT_EQ(coordinator->getComponent<Health>(entity).current, 100);
        EXPECT_EQ(coordinator->getComponent<Health>(entity).max, 100);

        // Test entityHasComponent for all components
        EXPECT_TRUE(coordinator->entityHasComponent<Position>(entity));
        EXPECT_TRUE(coordinator->entityHasComponent<Velocity>(entity));
        EXPECT_TRUE(coordinator->entityHasComponent<Renderable>(entity));
        EXPECT_TRUE(coordinator->entityHasComponent<Health>(entity));

        // Get all component types and verify
        auto types = coordinator->getAllComponentTypes(entity);
        EXPECT_EQ(types.size(), 4);

        // Modify components
        coordinator->getComponent<Position>(entity).x += coordinator->getComponent<Velocity>(entity).dx;
        coordinator->getComponent<Position>(entity).y += coordinator->getComponent<Velocity>(entity).dy;
        coordinator->getComponent<Health>(entity).current -= 10;

        // Verify modifications
        EXPECT_FLOAT_EQ(coordinator->getComponent<Position>(entity).x, 11.0f);
        EXPECT_FLOAT_EQ(coordinator->getComponent<Position>(entity).y, 22.0f);
        EXPECT_EQ(coordinator->getComponent<Health>(entity).current, 90);

        // Remove components and verify
        coordinator->removeComponent<Velocity>(entity);
        EXPECT_FALSE(coordinator->entityHasComponent<Velocity>(entity));
        EXPECT_TRUE(coordinator->entityHasComponent<Position>(entity));
        EXPECT_TRUE(coordinator->entityHasComponent<Renderable>(entity));
        EXPECT_TRUE(coordinator->entityHasComponent<Health>(entity));

        types = coordinator->getAllComponentTypes(entity);
        EXPECT_EQ(types.size(), 3);
    }
}
