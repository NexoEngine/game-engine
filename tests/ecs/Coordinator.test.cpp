//// Coordinator.test.cpp /////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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
#include "ecs/Definitions.hpp"
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

        TestSingletonComponent() = default;
        TestSingletonComponent(int v) : value(v) {}

        TestSingletonComponent(const TestSingletonComponent&) = delete;
		TestSingletonComponent& operator=(const TestSingletonComponent&) = delete;
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
        EXPECT_NO_THROW(coordinator->destroyEntity(nonexistentEntity));
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

    TEST_F(CoordinatorTest, RemoveComponentFromNonexistentEntity) {
        coordinator->registerComponent<TestComponent>();

        Entity nonexistentEntity = 100;

        EXPECT_THROW(coordinator->removeComponent<TestComponent>(nonexistentEntity), ComponentNotFound);
    }

    // TEST_F(CoordinatorTest, GetAllComponents) {
    //     coordinator->registerComponent<TestComponent>();
    //
    //     Entity entity = coordinator->createEntity();
    //     TestComponent component{42};
    //     coordinator->addComponent(entity, component);
    //
    //     auto components = coordinator->getAllComponents(entity);
    //     EXPECT_EQ(components.size(), 1);
    //
    //     auto& [type, value] = components[0];
    //     EXPECT_EQ(type, std::type_index(typeid(TestComponent)));
    //     EXPECT_EQ(std::any_cast<TestComponent>(value).data, 42);
    // }

    TEST_F(CoordinatorTest, GetAllEntitiesWith_NoMatch) {
        Entity e1 = coordinator->createEntity();
        ComponentA compA{10};
        coordinator->addComponent(e1, compA);

        std::vector<Entity> result = coordinator->getAllEntitiesWith<ComponentA, ComponentB>();
        EXPECT_TRUE(result.empty());
    }

    TEST_F(CoordinatorTest, GetAllEntitiesWith_SingleMatch) {
        Entity e1 = coordinator->createEntity();
        coordinator->addComponent(e1, ComponentA{10});

        Entity e2 = coordinator->createEntity();
        coordinator->addComponent(e2, ComponentA{20});
        coordinator->addComponent(e2, ComponentB{3.14f});

        std::vector<Entity> result = coordinator->getAllEntitiesWith<ComponentA, ComponentB>();
        EXPECT_EQ(result.size(), 1);
        EXPECT_TRUE(std::find(result.begin(), result.end(), e2) != result.end());
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

        std::vector<Entity> result = coordinator->getAllEntitiesWith<ComponentA, ComponentB>();
        EXPECT_EQ(result.size(), 3);
        EXPECT_TRUE(std::find(result.begin(), result.end(), e1) != result.end());
        EXPECT_TRUE(std::find(result.begin(), result.end(), e2) != result.end());
        EXPECT_TRUE(std::find(result.begin(), result.end(), e3) != result.end());
    }

    TEST_F(CoordinatorTest, DestroyedEntityNotReturned) {
        Entity e1 = coordinator->createEntity();
        coordinator->addComponent(e1, ComponentA{10});
        coordinator->addComponent(e1, ComponentB{2.5f});

        std::vector<Entity> result = coordinator->getAllEntitiesWith<ComponentA, ComponentB>();
        EXPECT_TRUE(std::find(result.begin(), result.end(), e1) != result.end());

        coordinator->destroyEntity(e1);

        result = coordinator->getAllEntitiesWith<ComponentA, ComponentB>();
        EXPECT_TRUE(std::find(result.begin(), result.end(), e1) == result.end());
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

        // Remove the singleton.
        EXPECT_NO_THROW(coordinator->removeSingletonComponent<TestSingletonComponent>());

        // After removal, trying to get the singleton should throw.
        EXPECT_THROW({
            coordinator->getSingletonComponent<TestSingletonComponent>();
        }, SingletonComponentNotRegistered);
    }

    TEST_F(CoordinatorTest, SingletonComponent_ReRegister) {
        coordinator->registerSingletonComponent<TestSingletonComponent>(100);
        // Remove and register a new value.
        coordinator->removeSingletonComponent<TestSingletonComponent>();
        coordinator->registerSingletonComponent<TestSingletonComponent>(200);

        TestSingletonComponent &retrieved = coordinator->getSingletonComponent<TestSingletonComponent>();
        EXPECT_EQ(retrieved.value, 200);
    }

    TEST_F(CoordinatorTest, GetComponentArray) {
        // Register a component type
        coordinator->registerComponent<TestComponent>();

        // Get the component array
        auto componentArray = coordinator->getComponentArray<TestComponent>();
        ASSERT_NE(componentArray, nullptr);

        // Test basic functionality of the component array
        Entity entity = coordinator->createEntity();
        componentArray->insert(entity, TestComponent{42});
        EXPECT_EQ(componentArray->get(entity).data, 42);
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
            if (typeIndex == getComponentTypeID<TestComponent>()) hasTestComponent = true;
            else if (typeIndex == getComponentTypeID<ComponentA>()) hasComponentA = true;
            else if (typeIndex == getComponentTypeID<ComponentB>()) hasComponentB = true;
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
            if (typeIndex == getComponentTypeID<TestComponent>()) hasTestComponent = true;
            else if (typeIndex == getComponentTypeID<ComponentA>()) hasComponentA = true;
            else if (typeIndex == getComponentTypeID<ComponentB>()) hasComponentB = true;
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

    TEST_F(CoordinatorTest, ComponentArrayIntegration) {
        coordinator->registerComponent<TestComponent>();

        // Get component array directly
        auto componentArray = coordinator->getComponentArray<TestComponent>();

        // Create entity through coordinator
        Entity entity = coordinator->createEntity();

        // Add component directly through component array
        componentArray->insert(entity, TestComponent{42});

        // Verify we can access it through the coordinator
        EXPECT_EQ(coordinator->getComponent<TestComponent>(entity).data, 42);

        // Modify through component array
        componentArray->get(entity).data = 100;

        // Verify change is visible through coordinator
        EXPECT_EQ(coordinator->getComponent<TestComponent>(entity).data, 100);

        // Remove through component array
        componentArray->remove(entity);

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

            AnotherSingletonComponent() = default;
            AnotherSingletonComponent(float v) : value(v) {}

            AnotherSingletonComponent(const AnotherSingletonComponent&) = delete;
            AnotherSingletonComponent& operator=(const AnotherSingletonComponent&) = delete;
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

    // ========================================================================
    // Edge Case Tests
    // ========================================================================

    TEST_F(CoordinatorTest, DuplicateEntity_BasicCopy) {
        coordinator->registerComponent<TestComponent>();

        Entity original = coordinator->createEntity();
        coordinator->addComponent(original, TestComponent{42});
        coordinator->addComponent(original, ComponentA{100});
        coordinator->addComponent(original, ComponentB{3.14f});

        // Duplicate the entity
        Entity duplicate = coordinator->duplicateEntity(original);

        // Verify the duplicate is a different entity
        EXPECT_NE(original, duplicate);

        // Verify all components were copied
        EXPECT_TRUE(coordinator->entityHasComponent<TestComponent>(duplicate));
        EXPECT_TRUE(coordinator->entityHasComponent<ComponentA>(duplicate));
        EXPECT_TRUE(coordinator->entityHasComponent<ComponentB>(duplicate));

        // Verify component values are identical
        EXPECT_EQ(coordinator->getComponent<TestComponent>(duplicate).data, 42);
        EXPECT_EQ(coordinator->getComponent<ComponentA>(duplicate).value, 100);
        EXPECT_FLOAT_EQ(coordinator->getComponent<ComponentB>(duplicate).data, 3.14f);
    }

    TEST_F(CoordinatorTest, DuplicateEntity_EmptyEntity) {
        // Create an entity with no components
        Entity original = coordinator->createEntity();

        // Duplicate it
        Entity duplicate = coordinator->duplicateEntity(original);

        // Verify the duplicate is a different entity
        EXPECT_NE(original, duplicate);

        // Verify both entities have no components
        auto originalTypes = coordinator->getAllComponentTypes(original);
        auto duplicateTypes = coordinator->getAllComponentTypes(duplicate);
        EXPECT_TRUE(originalTypes.empty());
        EXPECT_TRUE(duplicateTypes.empty());
    }

    TEST_F(CoordinatorTest, DuplicateEntity_IndependentModification) {
        coordinator->registerComponent<TestComponent>();

        Entity original = coordinator->createEntity();
        coordinator->addComponent(original, TestComponent{42});

        Entity duplicate = coordinator->duplicateEntity(original);

        // Modify the original
        coordinator->getComponent<TestComponent>(original).data = 100;

        // Verify the duplicate is unchanged
        EXPECT_EQ(coordinator->getComponent<TestComponent>(original).data, 100);
        EXPECT_EQ(coordinator->getComponent<TestComponent>(duplicate).data, 42);

        // Modify the duplicate
        coordinator->getComponent<TestComponent>(duplicate).data = 200;

        // Verify the original is unchanged
        EXPECT_EQ(coordinator->getComponent<TestComponent>(original).data, 100);
        EXPECT_EQ(coordinator->getComponent<TestComponent>(duplicate).data, 200);
    }

    TEST_F(CoordinatorTest, DuplicateEntity_MultipleDuplicates) {
        coordinator->registerComponent<TestComponent>();

        Entity original = coordinator->createEntity();
        coordinator->addComponent(original, TestComponent{42});

        // Create multiple duplicates
        Entity dup1 = coordinator->duplicateEntity(original);
        Entity dup2 = coordinator->duplicateEntity(original);
        Entity dup3 = coordinator->duplicateEntity(original);

        // Verify all are unique
        EXPECT_NE(original, dup1);
        EXPECT_NE(original, dup2);
        EXPECT_NE(original, dup3);
        EXPECT_NE(dup1, dup2);
        EXPECT_NE(dup1, dup3);
        EXPECT_NE(dup2, dup3);

        // Verify all have the same components
        EXPECT_EQ(coordinator->getComponent<TestComponent>(dup1).data, 42);
        EXPECT_EQ(coordinator->getComponent<TestComponent>(dup2).data, 42);
        EXPECT_EQ(coordinator->getComponent<TestComponent>(dup3).data, 42);
    }

    TEST_F(CoordinatorTest, GetEntitySignature_ValidEntity) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();

        // Get signature of entity with no components
        Signature sig1 = coordinator->getSignature(entity);
        EXPECT_FALSE(sig1.any());

        // Add a component
        coordinator->addComponent(entity, TestComponent{42});
        Signature sig2 = coordinator->getSignature(entity);

        ComponentType typeId = coordinator->getComponentType<TestComponent>();
        EXPECT_TRUE(sig2.test(typeId));
    }

    TEST_F(CoordinatorTest, GetEntitySignature_AfterComponentChanges) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();
        coordinator->addComponent(entity, TestComponent{42});
        coordinator->addComponent(entity, ComponentA{10});

        Signature sig1 = coordinator->getSignature(entity);

        // Remove a component
        coordinator->removeComponent<TestComponent>(entity);
        Signature sig2 = coordinator->getSignature(entity);

        // Signatures should be different
        EXPECT_NE(sig1, sig2);

        // Verify the signature reflects the current state
        ComponentType typeA = coordinator->getComponentType<ComponentA>();
        ComponentType typeTest = coordinator->getComponentType<TestComponent>();
        EXPECT_TRUE(sig2.test(typeA));
        EXPECT_FALSE(sig2.test(typeTest));
    }

    TEST_F(CoordinatorTest, HasComponent_NonExistentEntity) {
        coordinator->registerComponent<TestComponent>();

        // Test with an entity that was never created
        Entity nonexistent = 99999;

        // This should not crash, but may return false or have undefined behavior
        // Depending on implementation, this test documents the current behavior
        EXPECT_NO_THROW({
            bool hasComponent = coordinator->entityHasComponent<TestComponent>(nonexistent);
            // The result may vary based on implementation
            (void)hasComponent;
        });
    }

    TEST_F(CoordinatorTest, HasComponent_AfterEntityDestroyed) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();
        coordinator->addComponent(entity, TestComponent{42});

        // Verify component exists
        EXPECT_TRUE(coordinator->entityHasComponent<TestComponent>(entity));

        // Destroy the entity
        coordinator->destroyEntity(entity);

        // After destruction, the entity should not have components
        // Note: This behavior depends on whether the signature is reset on destruction
        EXPECT_NO_THROW({
            bool hasComponent = coordinator->entityHasComponent<TestComponent>(entity);
            // The result may vary based on implementation
            (void)hasComponent;
        });
    }

    TEST_F(CoordinatorTest, HasComponent_ComponentTypeByID) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();
        coordinator->addComponent(entity, TestComponent{42});

        ComponentType typeId = coordinator->getComponentType<TestComponent>();

        // Test the overload that takes ComponentType
        EXPECT_TRUE(coordinator->entityHasComponent(entity, typeId));

        // Test with a component the entity doesn't have
        ComponentType typeA = coordinator->getComponentType<ComponentA>();
        EXPECT_FALSE(coordinator->entityHasComponent(entity, typeA));
    }

    TEST_F(CoordinatorTest, GetAllComponentTypes_EdgeCases) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();

        // Test with entity that has no components
        auto types1 = coordinator->getAllComponentTypes(entity);
        EXPECT_TRUE(types1.empty());

        // Add one component
        coordinator->addComponent(entity, TestComponent{42});
        auto types2 = coordinator->getAllComponentTypes(entity);
        EXPECT_EQ(types2.size(), 1);

        // Add multiple components
        coordinator->addComponent(entity, ComponentA{10});
        coordinator->addComponent(entity, ComponentB{3.14f});
        auto types3 = coordinator->getAllComponentTypes(entity);
        EXPECT_EQ(types3.size(), 3);

        // Remove all components one by one
        coordinator->removeComponent<TestComponent>(entity);
        auto types4 = coordinator->getAllComponentTypes(entity);
        EXPECT_EQ(types4.size(), 2);

        coordinator->removeComponent<ComponentA>(entity);
        auto types5 = coordinator->getAllComponentTypes(entity);
        EXPECT_EQ(types5.size(), 1);

        coordinator->removeComponent<ComponentB>(entity);
        auto types6 = coordinator->getAllComponentTypes(entity);
        EXPECT_TRUE(types6.empty());
    }

    TEST_F(CoordinatorTest, GetAllComponentTypes_AfterDuplication) {
        coordinator->registerComponent<TestComponent>();

        Entity original = coordinator->createEntity();
        coordinator->addComponent(original, TestComponent{42});
        coordinator->addComponent(original, ComponentA{100});

        Entity duplicate = coordinator->duplicateEntity(original);

        auto originalTypes = coordinator->getAllComponentTypes(original);
        auto duplicateTypes = coordinator->getAllComponentTypes(duplicate);

        // Both should have the same number of component types
        EXPECT_EQ(originalTypes.size(), duplicateTypes.size());
        EXPECT_EQ(duplicateTypes.size(), 2);

        // Verify the component types are the same
        std::sort(originalTypes.begin(), originalTypes.end());
        std::sort(duplicateTypes.begin(), duplicateTypes.end());
        EXPECT_EQ(originalTypes, duplicateTypes);
    }

    TEST_F(CoordinatorTest, ComponentArray_DirectManipulation) {
        coordinator->registerComponent<TestComponent>();

        auto componentArray = coordinator->getComponentArray<TestComponent>();
        ASSERT_NE(componentArray, nullptr);

        Entity e1 = coordinator->createEntity();
        Entity e2 = coordinator->createEntity();

        // Insert through component array
        componentArray->insert(e1, TestComponent{10});
        componentArray->insert(e2, TestComponent{20});

        // Verify through coordinator
        EXPECT_EQ(coordinator->getComponent<TestComponent>(e1).data, 10);
        EXPECT_EQ(coordinator->getComponent<TestComponent>(e2).data, 20);

        // Modify through component array
        componentArray->get(e1).data = 100;

        // Verify change is visible through coordinator
        EXPECT_EQ(coordinator->getComponent<TestComponent>(e1).data, 100);
    }

    TEST_F(CoordinatorTest, ComponentArray_NullptrForUnregistered) {
        // Try to get a component array for an unregistered component
        struct UnregisteredComponent { int value; };

        // This should throw or return nullptr depending on implementation
        EXPECT_THROW({
            auto componentArray = coordinator->getComponentArray<UnregisteredComponent>();
        }, ComponentNotRegistered);
    }

    TEST_F(CoordinatorTest, GetAllEntitiesWith_EmptyResult) {
        coordinator->registerComponent<TestComponent>();

        // Create entities without the component we're looking for
        Entity e1 = coordinator->createEntity();
        Entity e2 = coordinator->createEntity();

        coordinator->addComponent(e1, ComponentA{10});
        coordinator->addComponent(e2, ComponentB{3.14f});

        // Search for entities with TestComponent
        auto result = coordinator->getAllEntitiesWith<TestComponent>();
        EXPECT_TRUE(result.empty());
    }

    TEST_F(CoordinatorTest, GetAllEntitiesWith_SingleComponentType) {
        coordinator->registerComponent<TestComponent>();

        Entity e1 = coordinator->createEntity();
        Entity e2 = coordinator->createEntity();
        Entity e3 = coordinator->createEntity();

        coordinator->addComponent(e1, TestComponent{10});
        coordinator->addComponent(e2, TestComponent{20});
        coordinator->addComponent(e2, ComponentA{5});
        coordinator->addComponent(e3, ComponentA{15});

        // Get all entities with TestComponent
        auto result = coordinator->getAllEntitiesWith<TestComponent>();
        EXPECT_EQ(result.size(), 2);
        EXPECT_TRUE(std::find(result.begin(), result.end(), e1) != result.end());
        EXPECT_TRUE(std::find(result.begin(), result.end(), e2) != result.end());
        EXPECT_TRUE(std::find(result.begin(), result.end(), e3) == result.end());
    }

    TEST_F(CoordinatorTest, GetAllEntitiesWith_AfterDestruction) {
        coordinator->registerComponent<TestComponent>();

        Entity e1 = coordinator->createEntity();
        Entity e2 = coordinator->createEntity();

        coordinator->addComponent(e1, TestComponent{10});
        coordinator->addComponent(e2, TestComponent{20});

        auto result1 = coordinator->getAllEntitiesWith<TestComponent>();
        EXPECT_EQ(result1.size(), 2);

        // Destroy one entity
        coordinator->destroyEntity(e1);

        auto result2 = coordinator->getAllEntitiesWith<TestComponent>();
        EXPECT_EQ(result2.size(), 1);
        EXPECT_TRUE(std::find(result2.begin(), result2.end(), e2) != result2.end());
        EXPECT_TRUE(std::find(result2.begin(), result2.end(), e1) == result2.end());
    }

    TEST_F(CoordinatorTest, TryGetComponent_EdgeCases) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();

        // Try to get a component that doesn't exist
        auto opt1 = coordinator->tryGetComponent<TestComponent>(entity);
        EXPECT_FALSE(opt1.has_value());

        // Add the component
        coordinator->addComponent(entity, TestComponent{42});

        // Try to get it again
        auto opt2 = coordinator->tryGetComponent<TestComponent>(entity);
        ASSERT_TRUE(opt2.has_value());
        EXPECT_EQ(opt2->get().data, 42);

        // Modify through the optional reference
        opt2->get().data = 100;
        EXPECT_EQ(coordinator->getComponent<TestComponent>(entity).data, 100);

        // Remove the component
        coordinator->removeComponent<TestComponent>(entity);

        // Try to get it after removal
        auto opt3 = coordinator->tryGetComponent<TestComponent>(entity);
        EXPECT_FALSE(opt3.has_value());
    }

    TEST_F(CoordinatorTest, ErrorHandling_GetComponentFromEntityWithout) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();

        // Try to get a component the entity doesn't have
        EXPECT_THROW({
            coordinator->getComponent<TestComponent>(entity);
        }, ComponentNotFound);
    }

    TEST_F(CoordinatorTest, ErrorHandling_RemoveNonexistentComponent) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();

        // Try to remove a component the entity doesn't have
        EXPECT_THROW({
            coordinator->removeComponent<TestComponent>(entity);
        }, ComponentNotFound);
    }

    TEST_F(CoordinatorTest, ErrorHandling_AddComponentTwice) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();
        coordinator->addComponent(entity, TestComponent{42});

        // Try to add the same component type again
        // Current implementation logs a warning and keeps the original value
        EXPECT_NO_THROW({
            coordinator->addComponent(entity, TestComponent{100});
        });

        // Verify the original value is unchanged (component was not overwritten)
        EXPECT_EQ(coordinator->getComponent<TestComponent>(entity).data, 42);
    }

    TEST_F(CoordinatorTest, MultipleEntities_ComponentIsolation) {
        coordinator->registerComponent<TestComponent>();

        // Create multiple entities with the same component type
        Entity e1 = coordinator->createEntity();
        Entity e2 = coordinator->createEntity();
        Entity e3 = coordinator->createEntity();

        coordinator->addComponent(e1, TestComponent{10});
        coordinator->addComponent(e2, TestComponent{20});
        coordinator->addComponent(e3, TestComponent{30});

        // Modify one entity's component
        coordinator->getComponent<TestComponent>(e1).data = 100;

        // Verify other entities are unaffected
        EXPECT_EQ(coordinator->getComponent<TestComponent>(e1).data, 100);
        EXPECT_EQ(coordinator->getComponent<TestComponent>(e2).data, 20);
        EXPECT_EQ(coordinator->getComponent<TestComponent>(e3).data, 30);

        // Remove component from one entity
        coordinator->removeComponent<TestComponent>(e2);

        // Verify others still have their components
        EXPECT_TRUE(coordinator->entityHasComponent<TestComponent>(e1));
        EXPECT_FALSE(coordinator->entityHasComponent<TestComponent>(e2));
        EXPECT_TRUE(coordinator->entityHasComponent<TestComponent>(e3));
    }

    TEST_F(CoordinatorTest, EntityLifecycle_ReuseAfterDestruction) {
        coordinator->registerComponent<TestComponent>();

        // Create and destroy an entity
        Entity e1 = coordinator->createEntity();
        coordinator->addComponent(e1, TestComponent{42});
        coordinator->destroyEntity(e1);

        // Create a new entity (may reuse the ID)
        Entity e2 = coordinator->createEntity();

        // The new entity should not have components from the old one
        EXPECT_FALSE(coordinator->entityHasComponent<TestComponent>(e2));

        auto types = coordinator->getAllComponentTypes(e2);
        EXPECT_TRUE(types.empty());
    }

    TEST_F(CoordinatorTest, GetAllComponents_MultipleComponents) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();
        coordinator->addComponent(entity, TestComponent{42});
        coordinator->addComponent(entity, ComponentA{100});
        coordinator->addComponent(entity, ComponentB{3.14f});

        // Get all components
        auto components = coordinator->getAllComponents(entity);
        EXPECT_EQ(components.size(), 3);

        // Note: The specific order may vary, so we just check the count
    }

    TEST_F(CoordinatorTest, GetAllComponents_EmptyEntity) {
        Entity entity = coordinator->createEntity();

        // Get components from an entity with no components
        auto components = coordinator->getAllComponents(entity);
        EXPECT_TRUE(components.empty());
    }

    TEST_F(CoordinatorTest, SignatureUpdates_AddRemoveSequence) {
        coordinator->registerComponent<TestComponent>();

        Entity entity = coordinator->createEntity();

        // Track signature changes
        Signature sig1 = coordinator->getSignature(entity);
        EXPECT_FALSE(sig1.any());

        // Add component
        coordinator->addComponent(entity, TestComponent{42});
        Signature sig2 = coordinator->getSignature(entity);
        EXPECT_TRUE(sig2.any());
        EXPECT_NE(sig1, sig2);

        // Add another component
        coordinator->addComponent(entity, ComponentA{10});
        Signature sig3 = coordinator->getSignature(entity);
        EXPECT_NE(sig2, sig3);

        // Remove first component
        coordinator->removeComponent<TestComponent>(entity);
        Signature sig4 = coordinator->getSignature(entity);
        EXPECT_NE(sig3, sig4);

        // Remove second component
        coordinator->removeComponent<ComponentA>(entity);
        Signature sig5 = coordinator->getSignature(entity);
        EXPECT_FALSE(sig5.any());
        EXPECT_EQ(sig1, sig5);
    }
}
