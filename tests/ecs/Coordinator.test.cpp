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
}
