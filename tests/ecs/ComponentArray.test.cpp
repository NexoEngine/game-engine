//// ComponentArray.test.cpp //////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/04/2025
//  Description: Test file for the component array class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ComponentArray.hpp"
#include "ECSExceptions.hpp"

namespace nexo::ecs {

    struct TestComponent {
        int value;

        TestComponent() = default;
        TestComponent(int v) : value(v) {}

        bool operator==(const TestComponent& other) const {
            return value == other.value;
        }
    };

    class ComponentArrayTest : public ::testing::Test {
    protected:
        std::shared_ptr<ComponentArray<TestComponent>> componentArray = nullptr;

        void SetUp() override {
        	componentArray = std::make_shared<ComponentArray<TestComponent>>();
            for (Entity i = 0; i < 5; ++i) {
            	TestComponent newComp;
             	newComp.value = i * 10;
                componentArray->insert(i, newComp);
            }
        }
    };

    // =========================================================
    // ================== BASIC OPERATIONS =====================
    // =========================================================

    TEST_F(ComponentArrayTest, InsertAddsComponentCorrectly) {
        const Entity testEntity = 10;
        const TestComponent testComponent(100);

        componentArray->insert(testEntity, testComponent);

        EXPECT_TRUE(componentArray->hasComponent(testEntity));
        EXPECT_EQ(componentArray->get(testEntity), testComponent);
        EXPECT_EQ(componentArray->size(), 6); // 5 from setup + 1 new
    }

    TEST_F(ComponentArrayTest, InsertDuplicateEntityIsIgnored) {
        const Entity testEntity = 1; // Already exists from setup
        const TestComponent newComponent(999);

        // Original component should be preserved
        TestComponent originalComponent = componentArray->get(testEntity);

        componentArray->insert(testEntity, newComponent);

        // Size should not change
        EXPECT_EQ(componentArray->size(), 5);
        // Component should not be updated
        EXPECT_EQ(componentArray->get(testEntity), originalComponent);
    }

    TEST_F(ComponentArrayTest, RemoveRemovesComponentCorrectly) {
        const Entity testEntity = 2;

        EXPECT_TRUE(componentArray->hasComponent(testEntity));
        componentArray->remove(testEntity);
        EXPECT_FALSE(componentArray->hasComponent(testEntity));
        EXPECT_EQ(componentArray->size(), 4); // 5 from setup - 1 removed
    }

    TEST_F(ComponentArrayTest, GetReturnsCorrectComponent) {
        for (Entity i = 0; i < 5; ++i) {
            EXPECT_EQ(componentArray->get(i).value, i * 10);
        }
    }

    TEST_F(ComponentArrayTest, GetAllowsModification) {
        const Entity testEntity = 3;
        TestComponent& component = componentArray->get(testEntity);
        component.value = 999;

        EXPECT_EQ(componentArray->get(testEntity).value, 999);
    }

    TEST_F(ComponentArrayTest, EntityDestroyedRemovesComponent) {
        const Entity testEntity = 4;

        EXPECT_TRUE(componentArray->hasComponent(testEntity));
        componentArray->entityDestroyed(testEntity);
        EXPECT_FALSE(componentArray->hasComponent(testEntity));
        EXPECT_EQ(componentArray->size(), 4);
    }

    TEST_F(ComponentArrayTest, EntityDestroyedIgnoresNonExistentEntity) {
        const Entity nonExistentEntity = 100;

        EXPECT_FALSE(componentArray->hasComponent(nonExistentEntity));
        componentArray->entityDestroyed(nonExistentEntity);
        EXPECT_EQ(componentArray->size(), 5); // Size unchanged
    }

    TEST_F(ComponentArrayTest, GetEntityAtIndexReturnsCorrectEntity) {
        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ(componentArray->getEntityAtIndex(i), i);
        }
    }

    TEST_F(ComponentArrayTest, GetAllComponentsReturnsCorrectSpan) {
        auto span = componentArray->getAllComponents();
        EXPECT_EQ(span.size(), 5);

        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ(span[i].value, static_cast<int>(i * 10));
        }
    }

    TEST_F(ComponentArrayTest, EntitiesReturnsCorrectEntitySpan) {
        auto entitiesSpan = componentArray->entities();
        EXPECT_EQ(entitiesSpan.size(), 5);

        for (size_t i = 0; i < 5; ++i) {
            EXPECT_EQ(entitiesSpan[i], i);
        }
    }

    TEST_F(ComponentArrayTest, SizeReturnsCorrectCount) {
        EXPECT_EQ(componentArray->size(), 5);
        componentArray->remove(0);
        EXPECT_EQ(componentArray->size(), 4);
        TestComponent newComp;
        newComp.value = 100;
        componentArray->insert(10, newComp);
        EXPECT_EQ(componentArray->size(), 5);
    }

    // =========================================================
    // ================== GROUP OPERATIONS =====================
    // =========================================================

    TEST_F(ComponentArrayTest, AddToGroupMovesEntityToGroupRegion) {
        EXPECT_EQ(componentArray->groupSize(), 0);

        // Add entity 3 to group
        componentArray->addToGroup(3);
        EXPECT_EQ(componentArray->groupSize(), 1);
        EXPECT_EQ(componentArray->getEntityAtIndex(0), 3);

        // Add entity 1 to group
        componentArray->addToGroup(1);
        EXPECT_EQ(componentArray->groupSize(), 2);

        // Verify both entities are in the group region (first 2 entries)
        auto entities = componentArray->entities();
        EXPECT_TRUE(entities[0] == 3 || entities[1] == 3);
        EXPECT_TRUE(entities[0] == 1 || entities[1] == 1);
    }

    TEST_F(ComponentArrayTest, AddToGroupIgnoresAlreadyGroupedEntity) {
        componentArray->addToGroup(2);
        EXPECT_EQ(componentArray->groupSize(), 1);

        // Add the same entity again
        componentArray->addToGroup(2);
        EXPECT_EQ(componentArray->groupSize(), 1); // Size should not change
        EXPECT_EQ(componentArray->getEntityAtIndex(0), 2);
    }

    TEST_F(ComponentArrayTest, RemoveFromGroupMovesEntityOutOfGroupRegion) {
        // First add some entities to the group
        componentArray->addToGroup(1);
        componentArray->addToGroup(3);
        EXPECT_EQ(componentArray->groupSize(), 2);

        // Remove entity 1 from group
        componentArray->removeFromGroup(1);
        EXPECT_EQ(componentArray->groupSize(), 1);
        EXPECT_EQ(componentArray->getEntityAtIndex(0), 3);

        // Remove entity 3 from group
        componentArray->removeFromGroup(3);
        EXPECT_EQ(componentArray->groupSize(), 0);
    }

    TEST_F(ComponentArrayTest, RemoveFromGroupIgnoresNonGroupedEntity) {
        componentArray->addToGroup(2);
        EXPECT_EQ(componentArray->groupSize(), 1);

        // Try to remove an entity that's not in the group
        componentArray->removeFromGroup(4);
        EXPECT_EQ(componentArray->groupSize(), 1); // Size should not change
    }

    TEST_F(ComponentArrayTest, RemoveHandlesGroupedEntityCorrectly) {
        // Add some entities to group
        componentArray->addToGroup(1);
        componentArray->addToGroup(3);
        EXPECT_EQ(componentArray->groupSize(), 2);

        // Remove a grouped entity
        componentArray->remove(1);

        // Group size should decrease
        EXPECT_EQ(componentArray->groupSize(), 1);
        // Total size should decrease
        EXPECT_EQ(componentArray->size(), 4);
        // Entity 3 should still be in the group
        EXPECT_EQ(componentArray->getEntityAtIndex(0), 3);
    }

    // =========================================================
    // ================== ERROR HANDLING =======================
    // =========================================================

    TEST_F(ComponentArrayTest, InsertThrowsOnEntityBeyondMaxEntities) {
        const Entity invalidEntity = MAX_ENTITIES;
        TestComponent testComp;
        testComp.value = 1;
        EXPECT_THROW(componentArray->insert(invalidEntity, testComp), OutOfRange);
    }

    TEST_F(ComponentArrayTest, RemoveThrowsOnNonExistentComponent) {
        const Entity nonExistentEntity = 100;
        EXPECT_THROW(componentArray->remove(nonExistentEntity), ComponentNotFound);
    }

    TEST_F(ComponentArrayTest, GetThrowsOnNonExistentComponent) {
        const Entity nonExistentEntity = 100;
        EXPECT_THROW(static_cast<void>(componentArray->get(nonExistentEntity)), ComponentNotFound);
    }

    TEST_F(ComponentArrayTest, GetEntityAtIndexThrowsOnInvalidIndex) {
        const size_t invalidIndex = 100;
        EXPECT_THROW(static_cast<void>(componentArray->getEntityAtIndex(invalidIndex)), OutOfRange);
    }

    TEST_F(ComponentArrayTest, AddToGroupThrowsOnNonExistentComponent) {
        const Entity nonExistentEntity = 100;
        EXPECT_THROW(componentArray->addToGroup(nonExistentEntity), ComponentNotFound);
    }

    TEST_F(ComponentArrayTest, RemoveFromGroupThrowsOnNonExistentComponent) {
        const Entity nonExistentEntity = 100;
        EXPECT_THROW(componentArray->removeFromGroup(nonExistentEntity), ComponentNotFound);
    }

    // =========================================================
    // ================== CAPACITY AND MEMORY ==================
    // =========================================================

    TEST_F(ComponentArrayTest, ArrayShrinksWhenManyElementsRemoved) {
        // First, add more elements to increase capacity
        for (Entity i = 5; i < 20; ++i) {
            componentArray->insert(i, TestComponent(i * 10));
        }
        size_t largeSize = componentArray->size();

        // Now remove many elements to trigger shrink
        for (Entity i = 0; i < 15; ++i) {
            componentArray->remove(i);
        }

        // Verify remaining elements are still accessible
        for (Entity i = 15; i < 20; ++i) {
            EXPECT_TRUE(componentArray->hasComponent(i));
            EXPECT_EQ(componentArray->get(i).value, i * 10);
        }
    }

    TEST_F(ComponentArrayTest, HandlesSparseEntityDistribution) {
        // Insert components with large gaps between entity IDs
        componentArray->insert(100, TestComponent(100));
        componentArray->insert(1000, TestComponent(1000));
        componentArray->insert(10000, TestComponent(10000));

        // Verify components were stored correctly
        EXPECT_TRUE(componentArray->hasComponent(100));
        EXPECT_TRUE(componentArray->hasComponent(1000));
        EXPECT_TRUE(componentArray->hasComponent(10000));

        EXPECT_EQ(componentArray->get(100).value, 100);
        EXPECT_EQ(componentArray->get(1000).value, 1000);
        EXPECT_EQ(componentArray->get(10000).value, 10000);

        // Make sure non-existent entities in gaps return false
        EXPECT_FALSE(componentArray->hasComponent(101));
        EXPECT_FALSE(componentArray->hasComponent(9999));
    }

    // =========================================================
    // ================== COMPLEX TESTS ========================
    // =========================================================

    TEST_F(ComponentArrayTest, ComplexEntityLifecycle) {
        // Initial state: 5 entities with components
        EXPECT_EQ(componentArray->size(), 5);

        // Add some to group
        componentArray->addToGroup(1);
        componentArray->addToGroup(3);
        EXPECT_EQ(componentArray->groupSize(), 2);

        // Remove a non-grouped entity
        componentArray->remove(4);
        EXPECT_EQ(componentArray->size(), 4);
        EXPECT_EQ(componentArray->groupSize(), 2);

        // Remove a grouped entity
        componentArray->remove(1);
        EXPECT_EQ(componentArray->size(), 3);
        EXPECT_EQ(componentArray->groupSize(), 1);

        // Add new entities
        componentArray->insert(6, TestComponent(60));
        componentArray->insert(7, TestComponent(70));
        EXPECT_EQ(componentArray->size(), 5);

        // Destroy an entity via entityDestroyed
        componentArray->entityDestroyed(0);
        EXPECT_EQ(componentArray->size(), 4);

        // Add the new entities to group
        componentArray->addToGroup(6);
        componentArray->addToGroup(7);
        EXPECT_EQ(componentArray->groupSize(), 3);

        // Verify the final state
        EXPECT_TRUE(componentArray->hasComponent(2));
        EXPECT_TRUE(componentArray->hasComponent(3));
        EXPECT_TRUE(componentArray->hasComponent(6));
        EXPECT_TRUE(componentArray->hasComponent(7));

        EXPECT_FALSE(componentArray->hasComponent(0));
        EXPECT_FALSE(componentArray->hasComponent(1));
        EXPECT_FALSE(componentArray->hasComponent(4));

        // Check grouped entities through entity indexes
        auto entitiesSpan = componentArray->entities();
        bool found3 = false, found6 = false, found7 = false;

        for (size_t i = 0; i < componentArray->groupSize(); ++i) {
            Entity e = entitiesSpan[i];
            if (e == 3) found3 = true;
            if (e == 6) found6 = true;
            if (e == 7) found7 = true;
        }

        EXPECT_TRUE(found3);
        EXPECT_TRUE(found6);
        EXPECT_TRUE(found7);
    }

    TEST_F(ComponentArrayTest, ComplexForEachWithGroupOperations) {
        // Add entities to group
        componentArray->addToGroup(1);
        componentArray->addToGroup(3);
        EXPECT_EQ(componentArray->groupSize(), 2);

        // Modify only grouped components
        int groupSum = 0;
        componentArray->forEach([&](Entity e, TestComponent& comp) {
            if (e == 1 || e == 3) {
                comp.value *= 2;
                groupSum += comp.value;
            }
        });

        EXPECT_EQ(groupSum, 80); // (10*2) + (30*2)
        EXPECT_EQ(componentArray->get(1).value, 20);
        EXPECT_EQ(componentArray->get(3).value, 60);

        // Non-grouped components should be unchanged
        EXPECT_EQ(componentArray->get(0).value, 0);
        EXPECT_EQ(componentArray->get(2).value, 20);
        EXPECT_EQ(componentArray->get(4).value, 40);
    }
}
