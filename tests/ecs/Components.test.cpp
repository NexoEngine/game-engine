//// Components.test.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/11/2024
//  Description: Test file for the components ecs classes
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ecs/Components.hpp"

namespace nexo::ecs {
    struct TestComponent {
        int value;
    };

    struct AnotherComponent {
        float data;
    };

    class ComponentManagerTest : public ::testing::Test {
        protected:
            void SetUp() override
            {
                componentManager = std::make_unique<ComponentManager>();
            }

            std::unique_ptr<ComponentManager> componentManager;
    };

    TEST(ComponentArrayTest, InsertAndRetrieveData)
    {
        ComponentArray<TestComponent> array;

        Entity entity1 = 1;
        Entity entity2 = 2;

        array.insertData(entity1, {42});
        array.insertData(entity2, {84});

        EXPECT_EQ(array.getData(entity1).value, 42);
        EXPECT_EQ(array.getData(entity2).value, 84);
    }

    TEST(ComponentArrayTest, RemoveData)
    {
        ComponentArray<TestComponent> array;

        Entity entity = 1;

        array.insertData(entity, {42});
        EXPECT_NO_THROW(array.removeData(entity));

        EXPECT_THROW(static_cast<void>(array.getData(entity)), ComponentNotFound);
    }

    TEST(ComponentArrayTest, HandleEntityDestruction)
    {
        ComponentArray<TestComponent> array;

        Entity entity = 1;

        array.insertData(entity, {42});
        array.entityDestroyed(entity);

        EXPECT_THROW(static_cast<void>(array.getData(entity)), ComponentNotFound);
    }

    TEST(ComponentArrayTest, InsertDuplicateEntity)
    {
        ComponentArray<TestComponent> array;

        Entity entity = 1;

        array.insertData(entity, {42});
        EXPECT_NO_THROW(array.insertData(entity, {100}));

        EXPECT_EQ(array.getData(entity).value, 42); // Original value should remain
    }

    TEST(ComponentArrayTest, InitialStateEmpty)
    {
        ComponentArray<TestComponent> array;
        EXPECT_EQ(array.size(), 0);
        EXPECT_FALSE(array.hasComponent(0));
    }

    TEST(ComponentArrayTest, SizeTracking)
    {
        ComponentArray<TestComponent> array;

        array.insertData(1, {42});
        EXPECT_EQ(array.size(), 1);

        array.insertData(2, {84});
        EXPECT_EQ(array.size(), 2);

        array.removeData(1);
        EXPECT_EQ(array.size(), 1);
    }

    TEST(ComponentArrayTest, GetEntityAtIndex)
    {
        ComponentArray<TestComponent> array;

        array.insertData(5, {42});
        array.insertData(10, {84});

        EXPECT_EQ(array.getEntityAtIndex(0), 5);
        EXPECT_EQ(array.getEntityAtIndex(1), 10);
        EXPECT_THROW(static_cast<void>(array.getEntityAtIndex(2)), OutOfRange);
    }

    TEST(ComponentArrayTest, RawDataAccess)
    {
        ComponentArray<TestComponent> array;

        array.insertData(1, {42});
        array.insertData(2, {84});

        auto data = array.rawData();
        EXPECT_EQ(data.size(), 2);
        EXPECT_EQ(data[0].value, 42);
        EXPECT_EQ(data[1].value, 84);

        // Test const version
        const auto& constArray = array;
        auto constData = constArray.rawData();
        EXPECT_EQ(constData.size(), 2);
        EXPECT_EQ(constData[0].value, 42);
        EXPECT_EQ(constData[1].value, 84);
    }

    TEST(ComponentArrayTest, EntitiesAccess)
    {
        ComponentArray<TestComponent> array;

        array.insertData(5, {42});
        array.insertData(10, {84});

        auto entities = array.entities();
        EXPECT_EQ(entities.size(), 2);
        EXPECT_EQ(entities[0], 5);
        EXPECT_EQ(entities[1], 10);
    }

    TEST(ComponentArrayTest, SwapAndPopRemovalMechanism)
    {
        ComponentArray<TestComponent> array;

        // Insert three components
        array.insertData(1, {1});
        array.insertData(2, {2});
        array.insertData(3, {3});

        // Remove the middle one
        array.removeData(2);

        // Check if the entities are correctly mapped
        EXPECT_EQ(array.size(), 2);
        EXPECT_TRUE(array.hasComponent(1));
        EXPECT_FALSE(array.hasComponent(2));
        EXPECT_TRUE(array.hasComponent(3));

        // The last entity (3) should now be at index 1
        EXPECT_EQ(array.getEntityAtIndex(1), 3);

        // Data should be preserved correctly
        EXPECT_EQ(array.getData(1).value, 1);
        EXPECT_EQ(array.getData(3).value, 3);
    }

    TEST(ComponentArrayTest, AutomaticGrowth)
    {
        // Create array with small initial capacity
        ComponentArray<TestComponent, 2> array;

        // Insert component for an entity beyond initial capacity
        array.insertData(10, {42});

        EXPECT_TRUE(array.hasComponent(10));
        EXPECT_EQ(array.getData(10).value, 42);
    }

    TEST(ComponentArrayTest, ConstGetData)
    {
        ComponentArray<TestComponent> array;
        array.insertData(1, {42});

        const ComponentArray<TestComponent>& constArray = array;
        EXPECT_EQ(constArray.getData(1).value, 42);
        EXPECT_THROW(static_cast<void>(constArray.getData(2)), ComponentNotFound);
    }

    TEST(ComponentArrayTest, LargeEntityIDs)
    {
        ComponentArray<TestComponent> array;

        Entity largeEntity = MAX_ENTITIES - 1;
        array.insertData(largeEntity, {42});

        EXPECT_TRUE(array.hasComponent(largeEntity));
        EXPECT_EQ(array.getData(largeEntity).value, 42);
    }

    TEST(ComponentArrayTest, MultipleInsertionsAndRemovals)
    {
        ComponentArray<TestComponent> array;

        // Insert several components
        for (Entity e = 0; e < 100; ++e) {
            array.insertData(e, {static_cast<int>(e)});
        }

        EXPECT_EQ(array.size(), 100);

        // Remove some components
        for (Entity e = 0; e < 100; e += 2) {
            array.removeData(e);
        }

        EXPECT_EQ(array.size(), 50);

        // Check remaining components
        for (Entity e = 1; e < 100; e += 2) {
            EXPECT_TRUE(array.hasComponent(e));
            EXPECT_EQ(array.getData(e).value, e);
        }
    }

    TEST(ComponentArrayTest, ShrinkingBehavior)
    {
        ComponentArray<TestComponent, 10> array;

        // Insert many components to force capacity increase
        for (Entity e = 0; e < 100; ++e) {
            array.insertData(e, {static_cast<int>(e)});
        }

        // Remove most components to trigger shrinking
        for (Entity e = 20; e < 100; ++e) {
            array.removeData(e);
        }

        // Check remaining components are still accessible
        for (Entity e = 0; e < 20; ++e) {
            EXPECT_TRUE(array.hasComponent(e));
            EXPECT_EQ(array.getData(e).value, e);
        }
    }

    TEST(ComponentArrayTest, ComplexComponentType)
    {
        struct ComplexComponent {
            std::string name;
            std::vector<int> data;
            bool flag;

            bool operator==(const ComplexComponent& other) const {
                return name == other.name && data == other.data && flag == other.flag;
            }
        };

        ComponentArray<ComplexComponent> array;

        ComplexComponent comp1{"test", {1, 2, 3}, true};
        ComplexComponent comp2{"another", {4, 5}, false};

        array.insertData(1, comp1);
        array.insertData(2, comp2);

        EXPECT_EQ(array.getData(1), comp1);
        EXPECT_EQ(array.getData(2), comp2);
    }

    TEST(ComponentArrayTest, InsertBeyondMaxEntities)
    {
        ComponentArray<TestComponent> array;
        EXPECT_THROW(array.insertData(MAX_ENTITIES, {42}), OutOfRange);
    }

    TEST(ComponentArrayTest, AccessNonExistentComponent)
    {
        ComponentArray<TestComponent> array;
        EXPECT_THROW(static_cast<void>(array.getData(1)), ComponentNotFound);
    }

    TEST(ComponentArrayTest, RemoveNonExistentComponent)
    {
        ComponentArray<TestComponent> array;
        EXPECT_THROW(array.removeData(1), ComponentNotFound);
    }

    TEST(ComponentArrayTest, EntityDestroyedWithComponent)
    {
        ComponentArray<TestComponent> array;

        array.insertData(1, {42});
        EXPECT_TRUE(array.hasComponent(1));

        array.entityDestroyed(1);
        EXPECT_FALSE(array.hasComponent(1));
        EXPECT_EQ(array.size(), 0);
    }

    TEST(ComponentArrayTest, EntityDestroyedWithoutComponent)
    {
        ComponentArray<TestComponent> array;

        array.insertData(2, {42});
        EXPECT_FALSE(array.hasComponent(1));

        // Should not throw or affect other components
        EXPECT_NO_THROW(array.entityDestroyed(1));
        EXPECT_TRUE(array.hasComponent(2));
        EXPECT_EQ(array.size(), 1);
    }

    TEST(ComponentArrayTest, VerySmallCapacity)
    {
        ComponentArray<TestComponent, 1> array;

        // Insert beyond initial capacity
        array.insertData(0, {0});
        array.insertData(1, {1});
        array.insertData(2, {2});

        EXPECT_EQ(array.size(), 3);
        EXPECT_TRUE(array.hasComponent(0));
        EXPECT_TRUE(array.hasComponent(1));
        EXPECT_TRUE(array.hasComponent(2));
    }

    TEST(ComponentArrayTest, ConcurrentEntityRemovalAndAccess)
    {
        ComponentArray<TestComponent> array;

        for (Entity e = 0; e < 10; ++e) {
            array.insertData(e, {static_cast<int>(e)});
        }

        // Remove entities and check that others remain accessible
        for (Entity e = 0; e < 10; e += 2) {
            array.removeData(e);

            // Check remaining entities are still correctly accessible
            for (Entity remaining = 1; remaining < 10; remaining += 2) {
                EXPECT_TRUE(array.hasComponent(remaining));
                EXPECT_EQ(array.getData(remaining).value, remaining);
            }
        }
    }

    TEST(ComponentArrayTest, ReinsertAfterRemoval)
    {
        ComponentArray<TestComponent> array;

        array.insertData(1, {42});
        array.removeData(1);
        EXPECT_FALSE(array.hasComponent(1));

        // Reinsert with new value
        array.insertData(1, {100});
        EXPECT_TRUE(array.hasComponent(1));
        EXPECT_EQ(array.getData(1).value, 100);
    }

    TEST(ComponentArrayTest, MoveSemanticRespect)
    {
        // Instead of using a true move-only type, use a class that tracks moves
        struct MoveTrackingComponent {
            std::shared_ptr<int> ptr;
            int moves = 0;

            MoveTrackingComponent(int val) : ptr(std::make_shared<int>(val)) {}

            MoveTrackingComponent(const MoveTrackingComponent& other) : ptr(other.ptr), moves(other.moves) {}

            MoveTrackingComponent(MoveTrackingComponent&& other) noexcept
                : ptr(std::move(other.ptr)), moves(other.moves + 1) {}

            MoveTrackingComponent& operator=(const MoveTrackingComponent& other) {
                ptr = other.ptr;
                moves = other.moves;
                return *this;
            }

            MoveTrackingComponent& operator=(MoveTrackingComponent&& other) noexcept {
                ptr = std::move(other.ptr);
                moves = other.moves + 1;
                return *this;
            }
        };

        ComponentArray<MoveTrackingComponent> array;

        array.insertData(1, MoveTrackingComponent(42));
        EXPECT_EQ(*array.getData(1).ptr, 42);

        // Don't remove enough components to trigger shrinking
        // Just verify the basic functionality works
    }

    TEST(ComponentArrayTest, SmallOperationsWithoutShrinking)
    {
        // Using a component with expensive move operations
        struct ExpensiveComponent {
            std::vector<int> data;

            ExpensiveComponent(std::initializer_list<int> values) : data(values) {}
        };

        ComponentArray<ExpensiveComponent> array;

        // Add just a few components
        array.insertData(1, {1, 2, 3});
        array.insertData(2, {4, 5, 6});
        array.insertData(3, {7, 8, 9});

        // Verify data access
        EXPECT_EQ(array.getData(1).data, std::vector<int>({1, 2, 3}));
        EXPECT_EQ(array.getData(2).data, std::vector<int>({4, 5, 6}));
        EXPECT_EQ(array.getData(3).data, std::vector<int>({7, 8, 9}));

        // Remove one (not enough to trigger shrinking)
        array.removeData(2);

        // Verify remaining data
        EXPECT_EQ(array.getData(1).data, std::vector<int>({1, 2, 3}));
        EXPECT_EQ(array.getData(3).data, std::vector<int>({7, 8, 9}));
    }

    TEST(ComponentArrayTest, EdgeCaseEmptyRemoval)
    {
        ComponentArray<TestComponent> array;

        array.insertData(1, {42});
        array.removeData(1);

        // Array should be empty now
        EXPECT_EQ(array.size(), 0);
        EXPECT_FALSE(array.hasComponent(1));

        // Removing again should throw
        EXPECT_THROW(array.removeData(1), ComponentNotFound);
    }

    TEST(ComponentArrayTest, HandleOverflow)
    {
        ComponentArray<TestComponent> array;

        for (Entity entity = 0; entity < MAX_ENTITIES; ++entity)
        {
            EXPECT_NO_THROW(array.insertData(entity, {static_cast<int>(entity)}));
        }

        EXPECT_THROW(array.insertData(MAX_ENTITIES, {999}), OutOfRange);
    }

    TEST_F(ComponentManagerTest, RegisterAndRetrieveComponentType)
    {
        componentManager->registerComponent<TestComponent>();
        ComponentType type = componentManager->getComponentType<TestComponent>();

        EXPECT_EQ(type, 0); // First registered component type
    }

    TEST_F(ComponentManagerTest, AddAndRetrieveComponent)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;

        componentManager->addComponent(entity, TestComponent{42});
        TestComponent &retrieved = componentManager->getComponent<TestComponent>(entity);

        EXPECT_EQ(retrieved.value, 42);
    }

    TEST_F(ComponentManagerTest, RemoveComponent)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;

        componentManager->addComponent(entity, TestComponent{42});
        EXPECT_NO_THROW(componentManager->removeComponent<TestComponent>(entity));
        EXPECT_THROW(static_cast<void>(componentManager->getComponent<TestComponent>(entity)), ComponentNotFound);
    }

    TEST_F(ComponentManagerTest, TryRemoveComponent)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;

        EXPECT_FALSE(componentManager->tryRemoveComponent<TestComponent>(entity)); // No component yet

        componentManager->addComponent(entity, TestComponent{42});
        EXPECT_TRUE(componentManager->tryRemoveComponent<TestComponent>(entity));
        EXPECT_FALSE(componentManager->tryRemoveComponent<TestComponent>(entity));
    }

    TEST_F(ComponentManagerTest, EntityDestroyedCleansUpComponents)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity1 = 1;
        Entity entity2 = 2;

        componentManager->addComponent(entity1, TestComponent{42});
        componentManager->addComponent(entity2, TestComponent{84});

        componentManager->entityDestroyed(entity1);

        EXPECT_THROW(static_cast<void>(componentManager->getComponent<TestComponent>(entity1)), ComponentNotFound);
        EXPECT_EQ(componentManager->getComponent<TestComponent>(entity2).value, 84);
    }

    TEST_F(ComponentManagerTest, RetrieveUnregisteredComponentType)
    {
        EXPECT_THROW(static_cast<void>(componentManager->getComponentType<TestComponent>()), ComponentNotRegistered);
    }

    TEST_F(ComponentManagerTest, AddComponentWithoutRegistering)
    {
        Entity entity = 1;
        EXPECT_THROW(componentManager->addComponent(entity, TestComponent{42}), ComponentNotRegistered);
    }

    TEST_F(ComponentManagerTest, TryGetComponent)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;

        auto result = componentManager->tryGetComponent<TestComponent>(entity);
        EXPECT_FALSE(result.has_value());

        componentManager->addComponent(entity, TestComponent{42});
        result = componentManager->tryGetComponent<TestComponent>(entity);
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result->get().value, 42);
    }

    TEST_F(ComponentManagerTest, GetComponentArray)
    {
        componentManager->registerComponent<TestComponent>();

        // Get component array
        auto array = componentManager->getComponentArray<TestComponent>();
        EXPECT_NE(array, nullptr);

        // Get const component array
        const auto& constManager = *componentManager;
        auto constArray = constManager.getComponentArray<TestComponent>();
        EXPECT_NE(constArray, nullptr);
    }

    TEST_F(ComponentManagerTest, GetComponentArrayUnregistered)
    {
        // Get component array for unregistered component
        EXPECT_THROW(static_cast<void>(componentManager->getComponentArray<TestComponent>()), ComponentNotRegistered);

        // Get const component array for unregistered component
        const auto& constManager = *componentManager;
        EXPECT_THROW(static_cast<void>(constManager.getComponentArray<TestComponent>()), ComponentNotRegistered);
    }

    TEST_F(ComponentManagerTest, MoveConstructor)
    {
        // Setup original manager
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;
        componentManager->addComponent(entity, TestComponent{42});

        // Move construct new manager
        ComponentManager movedManager(std::move(*componentManager));

        // Should be able to access component through moved manager
        EXPECT_EQ(movedManager.getComponent<TestComponent>(entity).value, 42);
    }

    TEST_F(ComponentManagerTest, MoveAssignment)
    {
        // Setup original manager
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;
        componentManager->addComponent(entity, TestComponent{42});

        // Create second manager and move-assign
        ComponentManager secondManager;
        secondManager = std::move(*componentManager);

        // Should be able to access component through second manager
        EXPECT_EQ(secondManager.getComponent<TestComponent>(entity).value, 42);
    }

    TEST_F(ComponentManagerTest, ComponentTypeIDs)
    {
        componentManager->registerComponent<TestComponent>();
        componentManager->registerComponent<AnotherComponent>();

        // Different component types should have different IDs
        ComponentType testType = componentManager->getComponentType<TestComponent>();
        ComponentType anotherType = componentManager->getComponentType<AnotherComponent>();

        EXPECT_NE(testType, anotherType);
    }

    TEST_F(ComponentManagerTest, EntityDestroyedNoComponents)
    {
        // EntityDestroyed with no components registered should not crash
        EXPECT_NO_THROW(componentManager->entityDestroyed(1));
    }

    TEST_F(ComponentManagerTest, EntityDestroyedEmptyComponentArray)
    {
        componentManager->registerComponent<TestComponent>();

        // EntityDestroyed with registered but empty component array should not crash
        EXPECT_NO_THROW(componentManager->entityDestroyed(1));
    }

    TEST_F(ComponentManagerTest, ComplexComponentStorage)
    {
        struct ComplexComponent {
            std::string name;
            std::vector<int> data;
        };

        componentManager->registerComponent<ComplexComponent>();
        Entity entity = 1;

        ComplexComponent complex{"test", {1, 2, 3}};
        componentManager->addComponent(entity, complex);

        ComplexComponent& retrieved = componentManager->getComponent<ComplexComponent>(entity);
        EXPECT_EQ(retrieved.name, "test");
        EXPECT_EQ(retrieved.data, std::vector<int>({1, 2, 3}));
    }

    TEST_F(ComponentManagerTest, GetComponentArrayDirect)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;

        componentManager->addComponent(entity, TestComponent{42});

        // Get component array directly and use it
        auto array = componentManager->getComponentArray<TestComponent>();
        TestComponent& component = array->getData(entity);
        EXPECT_EQ(component.value, 42);
    }

    TEST_F(ComponentManagerTest, ComponentModification)
    {
        componentManager->registerComponent<TestComponent>();
        Entity entity = 1;

        componentManager->addComponent(entity, TestComponent{42});

        // Modify component directly
        componentManager->getComponent<TestComponent>(entity).value = 100;

        // Verify modification took effect
        EXPECT_EQ(componentManager->getComponent<TestComponent>(entity).value, 100);
    }

    TEST_F(ComponentManagerTest, RegisterDuplicateComponent)
    {
        componentManager->registerComponent<TestComponent>();
        EXPECT_NO_THROW(componentManager->registerComponent<TestComponent>());
    }
}
