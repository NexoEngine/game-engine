//// SparseSet.test.cpp ///////////////////////////////////////////////////////
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
//  Date:        28/03/2025
//  Description: Test file for the sparse set class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ecs/System.hpp"

namespace nexo::ecs {
    class SparseSetTest : public ::testing::Test {
    protected:
        SparseSet sparseSet;
    };

    TEST_F(SparseSetTest, InitiallyEmpty) {
        EXPECT_TRUE(sparseSet.empty());
        EXPECT_TRUE(sparseSet.getDense().empty());
        EXPECT_EQ(sparseSet.begin(), sparseSet.end());
    }

    TEST_F(SparseSetTest, InsertSingleEntity) {
        Entity entity = 42;
        sparseSet.insert(entity);

        EXPECT_FALSE(sparseSet.empty());
        EXPECT_TRUE(sparseSet.contains(entity));
        EXPECT_EQ(sparseSet.getDense().size(), 1);
        EXPECT_EQ(sparseSet.getDense()[0], entity);
    }

    TEST_F(SparseSetTest, InsertMultipleEntities) {
        Entity entity1 = 42;
        Entity entity2 = 100;
        Entity entity3 = 255;

        sparseSet.insert(entity1);
        sparseSet.insert(entity2);
        sparseSet.insert(entity3);

        EXPECT_FALSE(sparseSet.empty());
        EXPECT_TRUE(sparseSet.contains(entity1));
        EXPECT_TRUE(sparseSet.contains(entity2));
        EXPECT_TRUE(sparseSet.contains(entity3));
        EXPECT_EQ(sparseSet.getDense().size(), 3);

        // Check that all entities are in the dense array
        auto& dense = sparseSet.getDense();
        EXPECT_THAT(dense, ::testing::UnorderedElementsAre(entity1, entity2, entity3));
    }

    TEST_F(SparseSetTest, EraseSingleEntity) {
        Entity entity = 42;
        sparseSet.insert(entity);
        EXPECT_TRUE(sparseSet.contains(entity));

        sparseSet.erase(entity);
        EXPECT_FALSE(sparseSet.contains(entity));
        EXPECT_TRUE(sparseSet.empty());
    }

    TEST_F(SparseSetTest, EraseMultipleEntities) {
        // Insert multiple entities
        Entity entity1 = 42;
        Entity entity2 = 100;
        Entity entity3 = 255;

        sparseSet.insert(entity1);
        sparseSet.insert(entity2);
        sparseSet.insert(entity3);

        // Erase middle entity
        sparseSet.erase(entity2);
        EXPECT_FALSE(sparseSet.contains(entity2));
        EXPECT_TRUE(sparseSet.contains(entity1));
        EXPECT_TRUE(sparseSet.contains(entity3));
        EXPECT_EQ(sparseSet.getDense().size(), 2);

        // Erase first entity
        sparseSet.erase(entity1);
        EXPECT_FALSE(sparseSet.contains(entity1));
        EXPECT_TRUE(sparseSet.contains(entity3));
        EXPECT_EQ(sparseSet.getDense().size(), 1);

        // Erase last entity
        sparseSet.erase(entity3);
        EXPECT_FALSE(sparseSet.contains(entity3));
        EXPECT_TRUE(sparseSet.empty());
    }

    TEST_F(SparseSetTest, SwapAndPopMechanism) {
        // Insert entities in order to test the swap-and-pop mechanism
        Entity entity1 = 1;
        Entity entity2 = 2;
        Entity entity3 = 3;

        sparseSet.insert(entity1);
        sparseSet.insert(entity2);
        sparseSet.insert(entity3);

        // Erase the first entity
        sparseSet.erase(entity1);

        // The last entity should now be in the first entity's position
        auto& dense = sparseSet.getDense();
        EXPECT_EQ(dense.size(), 2);

        // Since order isn't guaranteed with unordered_map, we just check that both
        // remaining entities are in the dense array
        EXPECT_THAT(dense, ::testing::UnorderedElementsAre(entity2, entity3));
        EXPECT_TRUE(sparseSet.contains(entity2));
        EXPECT_TRUE(sparseSet.contains(entity3));
    }

    TEST_F(SparseSetTest, InsertDuplicateEntity) {
        Entity entity = 42;

        sparseSet.insert(entity);
        // Try to insert the same entity again
        sparseSet.insert(entity);

        // Should only be one entity in the set
        EXPECT_EQ(sparseSet.getDense().size(), 1);
        EXPECT_TRUE(sparseSet.contains(entity));
    }

    TEST_F(SparseSetTest, EraseNonExistentEntity) {
        Entity entity = 42;

        // Try to erase an entity that doesn't exist
        sparseSet.erase(entity);

        // Should still be empty
        EXPECT_TRUE(sparseSet.empty());

        // Insert and then erase
        sparseSet.insert(entity);
        sparseSet.erase(entity);

        // Try to erase again
        sparseSet.erase(entity);

        // Should still be empty
        EXPECT_TRUE(sparseSet.empty());
    }

    TEST_F(SparseSetTest, IteratorFunctionality) {
        Entity entity1 = 42;
        Entity entity2 = 100;

        sparseSet.insert(entity1);
        sparseSet.insert(entity2);

        // Test begin/end iteration
        std::vector<Entity> entities;
        for (auto it = sparseSet.begin(); it != sparseSet.end(); ++it) {
            entities.push_back(*it);
        }

        EXPECT_EQ(entities.size(), 2);
        EXPECT_THAT(entities, ::testing::UnorderedElementsAre(entity1, entity2));

        // Test range-based for loop
        entities.clear();
        for (Entity e : sparseSet) {
            entities.push_back(e);
        }

        EXPECT_EQ(entities.size(), 2);
        EXPECT_THAT(entities, ::testing::UnorderedElementsAre(entity1, entity2));
    }

    TEST_F(SparseSetTest, GetDenseArray) {
        Entity entity1 = 42;
        Entity entity2 = 100;

        sparseSet.insert(entity1);
        sparseSet.insert(entity2);

        const auto& dense = sparseSet.getDense();
        EXPECT_EQ(dense.size(), 2);
        EXPECT_THAT(dense, ::testing::UnorderedElementsAre(entity1, entity2));
    }

    TEST_F(SparseSetTest, LargeNumberOfEntities) {
        // Insert a large number of entities
        const size_t numEntities = 1000;

        for (Entity i = 0; i < numEntities; ++i) {
            sparseSet.insert(i);
        }

        EXPECT_EQ(sparseSet.getDense().size(), numEntities);

        // Check each entity is contained
        for (Entity i = 0; i < numEntities; ++i) {
            EXPECT_TRUE(sparseSet.contains(i));
        }

        // Remove all entities in reverse order
        for (Entity i = numEntities - 1; i != static_cast<Entity>(-1); --i) {
            sparseSet.erase(i);
        }

        EXPECT_TRUE(sparseSet.empty());
    }

    TEST_F(SparseSetTest, MixedOperations) {
        // Perform a mix of insert and erase operations
        sparseSet.insert(1);
        sparseSet.insert(2);
        sparseSet.insert(3);
        sparseSet.erase(2);
        sparseSet.insert(4);
        sparseSet.erase(1);
        sparseSet.insert(5);
        sparseSet.erase(3);
        sparseSet.insert(6);

        EXPECT_EQ(sparseSet.getDense().size(), 3);
        EXPECT_TRUE(sparseSet.contains(4));
        EXPECT_TRUE(sparseSet.contains(5));
        EXPECT_TRUE(sparseSet.contains(6));
        EXPECT_FALSE(sparseSet.contains(1));
        EXPECT_FALSE(sparseSet.contains(2));
        EXPECT_FALSE(sparseSet.contains(3));
    }

    TEST_F(SparseSetTest, EmptyAfterEraseAll) {
        // Insert and then erase all
        sparseSet.insert(1);
        sparseSet.insert(2);
        sparseSet.insert(3);

        sparseSet.erase(1);
        sparseSet.erase(2);
        sparseSet.erase(3);

        EXPECT_TRUE(sparseSet.empty());
        EXPECT_EQ(sparseSet.begin(), sparseSet.end());
    }

    TEST_F(SparseSetTest, NonSequentialEntities) {
        // Test with large, scattered entity IDs
        Entity entity1 = 1000000;
        Entity entity2 = 2000000;
        Entity entity3 = 3000000;

        sparseSet.insert(entity1);
        sparseSet.insert(entity2);
        sparseSet.insert(entity3);

        EXPECT_TRUE(sparseSet.contains(entity1));
        EXPECT_TRUE(sparseSet.contains(entity2));
        EXPECT_TRUE(sparseSet.contains(entity3));

        sparseSet.erase(entity2);

        EXPECT_TRUE(sparseSet.contains(entity1));
        EXPECT_FALSE(sparseSet.contains(entity2));
        EXPECT_TRUE(sparseSet.contains(entity3));
    }

    // Edge Case Tests

    TEST_F(SparseSetTest, CapacityGrowthBehavior) {
        // Test that the sparse set can grow beyond initial capacity
        const size_t targetSize = 1000;

        // Insert entities beyond typical initial capacity
        for (Entity i = 0; i < targetSize; ++i) {
            sparseSet.insert(i);
        }

        EXPECT_EQ(sparseSet.size(), targetSize);

        // Verify all entities are still accessible
        for (Entity i = 0; i < targetSize; ++i) {
            EXPECT_TRUE(sparseSet.contains(i));
        }

        // Check that the dense array is valid
        const auto& dense = sparseSet.getDense();
        EXPECT_EQ(dense.size(), targetSize);
    }

    TEST_F(SparseSetTest, RemoveAndReAddSameEntityMultipleTimes) {
        Entity entity = 100;

        // Add, remove, add pattern multiple times
        for (int cycle = 0; cycle < 10; ++cycle) {
            sparseSet.insert(entity);
            EXPECT_TRUE(sparseSet.contains(entity));
            EXPECT_EQ(sparseSet.size(), 1);

            sparseSet.erase(entity);
            EXPECT_FALSE(sparseSet.contains(entity));
            EXPECT_TRUE(sparseSet.empty());
        }
    }

    TEST_F(SparseSetTest, RemoveAndReAddWithOtherEntities) {
        Entity entity1 = 10;
        Entity entity2 = 20;
        Entity entity3 = 30;

        // Insert multiple entities
        sparseSet.insert(entity1);
        sparseSet.insert(entity2);
        sparseSet.insert(entity3);

        // Remove and re-add entity2 multiple times
        for (int cycle = 0; cycle < 5; ++cycle) {
            sparseSet.erase(entity2);
            EXPECT_FALSE(sparseSet.contains(entity2));
            EXPECT_EQ(sparseSet.size(), 2);

            sparseSet.insert(entity2);
            EXPECT_TRUE(sparseSet.contains(entity2));
            EXPECT_EQ(sparseSet.size(), 3);
        }

        // Verify all entities are still present
        EXPECT_TRUE(sparseSet.contains(entity1));
        EXPECT_TRUE(sparseSet.contains(entity2));
        EXPECT_TRUE(sparseSet.contains(entity3));
    }

    TEST_F(SparseSetTest, IteratorAfterInserts) {
        // Test iterator behavior after multiple insertions
        std::vector<Entity> entities = {1, 5, 10, 15, 20};

        for (Entity e : entities) {
            sparseSet.insert(e);
        }

        std::vector<Entity> iterated;
        for (Entity e : sparseSet) {
            iterated.push_back(e);
        }

        EXPECT_EQ(iterated.size(), entities.size());
        EXPECT_THAT(iterated, ::testing::UnorderedElementsAreArray(entities));
    }

    TEST_F(SparseSetTest, IteratorAfterRemoval) {
        // Insert several entities
        sparseSet.insert(1);
        sparseSet.insert(2);
        sparseSet.insert(3);
        sparseSet.insert(4);

        // Remove middle entity
        sparseSet.erase(2);

        // Verify iterator still works correctly
        std::vector<Entity> remaining;
        for (Entity e : sparseSet) {
            remaining.push_back(e);
        }

        EXPECT_EQ(remaining.size(), 3);
        EXPECT_THAT(remaining, ::testing::UnorderedElementsAre(1, 3, 4));
    }

    TEST_F(SparseSetTest, IteratorOnEmptySet) {
        // Verify iterators on empty set
        EXPECT_EQ(sparseSet.begin(), sparseSet.end());

        int count = 0;
        for ([[maybe_unused]] Entity e : sparseSet) {
            ++count;
        }
        EXPECT_EQ(count, 0);
    }

    TEST_F(SparseSetTest, SwapOperationEdgeCaseLastElement) {
        // Test swap-and-pop when removing the last element
        sparseSet.insert(1);
        sparseSet.insert(2);
        sparseSet.insert(3);

        // Remove last element - should not cause a swap
        sparseSet.erase(3);

        EXPECT_EQ(sparseSet.size(), 2);
        EXPECT_TRUE(sparseSet.contains(1));
        EXPECT_TRUE(sparseSet.contains(2));
        EXPECT_FALSE(sparseSet.contains(3));
    }

    TEST_F(SparseSetTest, SwapOperationEdgeCaseFirstElement) {
        // Test swap-and-pop when removing the first element
        sparseSet.insert(1);
        sparseSet.insert(2);
        sparseSet.insert(3);

        // Remove first element - should swap with last
        sparseSet.erase(1);

        EXPECT_EQ(sparseSet.size(), 2);
        EXPECT_FALSE(sparseSet.contains(1));
        EXPECT_TRUE(sparseSet.contains(2));
        EXPECT_TRUE(sparseSet.contains(3));

        // Verify the dense array is still valid
        const auto& dense = sparseSet.getDense();
        EXPECT_THAT(dense, ::testing::UnorderedElementsAre(2, 3));
    }

    TEST_F(SparseSetTest, SwapOperationSingleElement) {
        // Test swap-and-pop with only one element
        Entity entity = 42;
        sparseSet.insert(entity);

        sparseSet.erase(entity);

        EXPECT_TRUE(sparseSet.empty());
        EXPECT_FALSE(sparseSet.contains(entity));
    }

    TEST_F(SparseSetTest, ClearAndRepopulate) {
        // Insert initial entities
        std::vector<Entity> initialEntities = {1, 2, 3, 4, 5};
        for (Entity e : initialEntities) {
            sparseSet.insert(e);
        }

        EXPECT_EQ(sparseSet.size(), initialEntities.size());

        // Clear by removing all entities
        for (Entity e : initialEntities) {
            sparseSet.erase(e);
        }

        EXPECT_TRUE(sparseSet.empty());

        // Repopulate with different entities
        std::vector<Entity> newEntities = {10, 20, 30, 40, 50};
        for (Entity e : newEntities) {
            sparseSet.insert(e);
        }

        EXPECT_EQ(sparseSet.size(), newEntities.size());

        // Verify old entities are gone and new ones are present
        for (Entity e : initialEntities) {
            EXPECT_FALSE(sparseSet.contains(e));
        }
        for (Entity e : newEntities) {
            EXPECT_TRUE(sparseSet.contains(e));
        }
    }

    TEST_F(SparseSetTest, ClearAndRepopulateSameEntities) {
        // Insert entities
        std::vector<Entity> entities = {100, 200, 300};
        for (Entity e : entities) {
            sparseSet.insert(e);
        }

        // Clear
        for (Entity e : entities) {
            sparseSet.erase(e);
        }

        EXPECT_TRUE(sparseSet.empty());

        // Re-insert same entities
        for (Entity e : entities) {
            sparseSet.insert(e);
        }

        EXPECT_EQ(sparseSet.size(), entities.size());
        for (Entity e : entities) {
            EXPECT_TRUE(sparseSet.contains(e));
        }
    }

    TEST_F(SparseSetTest, MaxEntitiesBoundary) {
        // Test near the MAX_ENTITIES boundary
        const Entity nearMax = MAX_ENTITIES - 1;

        // These should work
        sparseSet.insert(nearMax - 2);
        sparseSet.insert(nearMax - 1);
        sparseSet.insert(nearMax);

        EXPECT_TRUE(sparseSet.contains(nearMax - 2));
        EXPECT_TRUE(sparseSet.contains(nearMax - 1));
        EXPECT_TRUE(sparseSet.contains(nearMax));

        // Test operations at boundary
        sparseSet.erase(nearMax);
        EXPECT_FALSE(sparseSet.contains(nearMax));

        sparseSet.insert(nearMax);
        EXPECT_TRUE(sparseSet.contains(nearMax));
    }

    TEST_F(SparseSetTest, EmptySetContains) {
        // Verify contains returns false for any entity on empty set
        EXPECT_FALSE(sparseSet.contains(0));
        EXPECT_FALSE(sparseSet.contains(1));
        EXPECT_FALSE(sparseSet.contains(100));
        EXPECT_FALSE(sparseSet.contains(MAX_ENTITIES - 1));
    }

    TEST_F(SparseSetTest, EmptySetSize) {
        // Verify size is 0 for empty set
        EXPECT_EQ(sparseSet.size(), 0);

        // Insert and remove
        sparseSet.insert(1);
        EXPECT_EQ(sparseSet.size(), 1);

        sparseSet.erase(1);
        EXPECT_EQ(sparseSet.size(), 0);
    }

    TEST_F(SparseSetTest, EmptySetGetDense) {
        // Verify getDense returns empty vector for empty set
        const auto& dense = sparseSet.getDense();
        EXPECT_TRUE(dense.empty());
        EXPECT_EQ(dense.size(), 0);
    }

    TEST_F(SparseSetTest, SingleElementInsertAndCheck) {
        // Test all operations with a single element
        Entity entity = 777;

        sparseSet.insert(entity);

        EXPECT_FALSE(sparseSet.empty());
        EXPECT_TRUE(sparseSet.contains(entity));
        EXPECT_EQ(sparseSet.size(), 1);

        const auto& dense = sparseSet.getDense();
        EXPECT_EQ(dense.size(), 1);
        EXPECT_EQ(dense[0], entity);
    }

    TEST_F(SparseSetTest, SingleElementIteration) {
        // Test iteration with single element
        Entity entity = 42;
        sparseSet.insert(entity);

        std::vector<Entity> iterated;
        for (Entity e : sparseSet) {
            iterated.push_back(e);
        }

        EXPECT_EQ(iterated.size(), 1);
        EXPECT_EQ(iterated[0], entity);
    }

    TEST_F(SparseSetTest, SingleElementRemoval) {
        // Test removal of single element
        Entity entity = 999;
        sparseSet.insert(entity);

        EXPECT_TRUE(sparseSet.contains(entity));

        sparseSet.erase(entity);

        EXPECT_FALSE(sparseSet.contains(entity));
        EXPECT_TRUE(sparseSet.empty());
    }

    TEST_F(SparseSetTest, IndexConsistencyAfterInserts) {
        // Verify index mapping is consistent after insertions
        std::vector<Entity> entities = {10, 20, 30, 40, 50};

        for (Entity e : entities) {
            sparseSet.insert(e);
        }

        // All entities should be findable
        for (Entity e : entities) {
            EXPECT_TRUE(sparseSet.contains(e));
        }

        // Dense array should contain all entities
        const auto& dense = sparseSet.getDense();
        EXPECT_EQ(dense.size(), entities.size());
        EXPECT_THAT(dense, ::testing::UnorderedElementsAreArray(entities));
    }

    TEST_F(SparseSetTest, IndexConsistencyAfterRemovals) {
        // Verify index mapping is consistent after removals
        std::vector<Entity> entities = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

        for (Entity e : entities) {
            sparseSet.insert(e);
        }

        // Remove every other entity
        std::vector<Entity> toRemove = {2, 4, 6, 8, 10};
        std::vector<Entity> remaining = {1, 3, 5, 7, 9};

        for (Entity e : toRemove) {
            sparseSet.erase(e);
        }

        // Verify remaining entities
        EXPECT_EQ(sparseSet.size(), remaining.size());

        for (Entity e : remaining) {
            EXPECT_TRUE(sparseSet.contains(e));
        }

        for (Entity e : toRemove) {
            EXPECT_FALSE(sparseSet.contains(e));
        }

        // Verify dense array
        const auto& dense = sparseSet.getDense();
        EXPECT_THAT(dense, ::testing::UnorderedElementsAreArray(remaining));
    }

    TEST_F(SparseSetTest, IndexConsistencyAfterMixedOperations) {
        // Test consistency after complex insert/remove patterns
        sparseSet.insert(1);
        sparseSet.insert(2);
        sparseSet.insert(3);
        EXPECT_EQ(sparseSet.size(), 3);

        sparseSet.erase(2);
        EXPECT_EQ(sparseSet.size(), 2);

        sparseSet.insert(4);
        sparseSet.insert(5);
        EXPECT_EQ(sparseSet.size(), 4);

        sparseSet.erase(1);
        sparseSet.erase(3);
        EXPECT_EQ(sparseSet.size(), 2);

        // Remaining should be 4 and 5
        EXPECT_TRUE(sparseSet.contains(4));
        EXPECT_TRUE(sparseSet.contains(5));
        EXPECT_FALSE(sparseSet.contains(1));
        EXPECT_FALSE(sparseSet.contains(2));
        EXPECT_FALSE(sparseSet.contains(3));

        // Verify dense array
        const auto& dense = sparseSet.getDense();
        EXPECT_THAT(dense, ::testing::UnorderedElementsAre(4, 5));
    }

    TEST_F(SparseSetTest, ContainsConsistencyAfterSwap) {
        // Verify contains is correct after swap-and-pop operations
        sparseSet.insert(100);
        sparseSet.insert(200);
        sparseSet.insert(300);

        // Remove middle element (should swap with last)
        sparseSet.erase(200);

        // Both remaining entities should be findable
        EXPECT_TRUE(sparseSet.contains(100));
        EXPECT_FALSE(sparseSet.contains(200));
        EXPECT_TRUE(sparseSet.contains(300));
    }

    TEST_F(SparseSetTest, MemoryEfficiencyScenarioSparseEntityIDs) {
        // Test with very sparse entity IDs
        std::vector<Entity> sparseIds = {0, 1000, 50000, 100000, 499999};

        for (Entity e : sparseIds) {
            sparseSet.insert(e);
        }

        EXPECT_EQ(sparseSet.size(), sparseIds.size());

        // Verify all are accessible
        for (Entity e : sparseIds) {
            EXPECT_TRUE(sparseSet.contains(e));
        }

        // Dense array should only contain these entities
        const auto& dense = sparseSet.getDense();
        EXPECT_EQ(dense.size(), sparseIds.size());
        EXPECT_THAT(dense, ::testing::UnorderedElementsAreArray(sparseIds));
    }

    TEST_F(SparseSetTest, MemoryEfficiencyScenarioGrowthAndShrink) {
        // Test memory efficiency with growth and shrinkage
        const size_t largeSize = 10000;

        // Grow to large size
        for (Entity i = 0; i < largeSize; ++i) {
            sparseSet.insert(i);
        }

        EXPECT_EQ(sparseSet.size(), largeSize);

        // Shrink back down
        for (Entity i = 0; i < largeSize; ++i) {
            sparseSet.erase(i);
        }

        EXPECT_TRUE(sparseSet.empty());

        // Add a few entities again
        sparseSet.insert(1);
        sparseSet.insert(2);
        sparseSet.insert(3);

        EXPECT_EQ(sparseSet.size(), 3);
        EXPECT_TRUE(sparseSet.contains(1));
        EXPECT_TRUE(sparseSet.contains(2));
        EXPECT_TRUE(sparseSet.contains(3));
    }

    TEST_F(SparseSetTest, ZeroEntityID) {
        // Test with entity ID 0
        Entity entity = 0;

        sparseSet.insert(entity);
        EXPECT_TRUE(sparseSet.contains(entity));
        EXPECT_EQ(sparseSet.size(), 1);

        const auto& dense = sparseSet.getDense();
        EXPECT_EQ(dense[0], entity);

        sparseSet.erase(entity);
        EXPECT_FALSE(sparseSet.contains(entity));
        EXPECT_TRUE(sparseSet.empty());
    }

    TEST_F(SparseSetTest, SequentialInsertAndRandomRemoval) {
        // Insert sequentially, remove randomly
        const size_t count = 100;
        std::vector<Entity> entities;

        for (Entity i = 0; i < count; ++i) {
            entities.push_back(i);
            sparseSet.insert(i);
        }

        EXPECT_EQ(sparseSet.size(), count);

        // Remove entities at specific indices
        std::vector<Entity> toRemove = {5, 15, 25, 35, 45, 55, 65, 75, 85, 95};

        for (Entity e : toRemove) {
            sparseSet.erase(e);
        }

        EXPECT_EQ(sparseSet.size(), count - toRemove.size());

        // Verify removed entities are gone
        for (Entity e : toRemove) {
            EXPECT_FALSE(sparseSet.contains(e));
        }

        // Verify remaining entities are present
        for (Entity i = 0; i < count; ++i) {
            bool shouldExist = std::find(toRemove.begin(), toRemove.end(), i) == toRemove.end();
            EXPECT_EQ(sparseSet.contains(i), shouldExist);
        }
    }

    TEST_F(SparseSetTest, AlternatingInsertRemove) {
        // Alternate between insert and remove operations
        for (int i = 0; i < 20; ++i) {
            sparseSet.insert(i);
            EXPECT_EQ(sparseSet.size(), 1);
            EXPECT_TRUE(sparseSet.contains(i));

            sparseSet.erase(i);
            EXPECT_EQ(sparseSet.size(), 0);
            EXPECT_FALSE(sparseSet.contains(i));
        }

        EXPECT_TRUE(sparseSet.empty());
    }

    TEST_F(SparseSetTest, GetDenseConsistency) {
        // Verify getDense always returns consistent view
        std::vector<Entity> entities = {10, 20, 30};

        for (Entity e : entities) {
            sparseSet.insert(e);
            const auto& dense = sparseSet.getDense();
            EXPECT_TRUE(sparseSet.contains(e));
            EXPECT_TRUE(std::find(dense.begin(), dense.end(), e) != dense.end());
        }

        for (Entity e : entities) {
            const auto& denseBefore = sparseSet.getDense();
            size_t sizeBefore = denseBefore.size();

            sparseSet.erase(e);

            const auto& denseAfter = sparseSet.getDense();
            EXPECT_EQ(denseAfter.size(), sizeBefore - 1);
            EXPECT_FALSE(std::find(denseAfter.begin(), denseAfter.end(), e) != denseAfter.end());
        }
    }

    TEST_F(SparseSetTest, StressTestRapidOperations) {
        // Stress test with many rapid operations
        const size_t operations = 1000;

        for (size_t i = 0; i < operations; ++i) {
            Entity entity = i % 100; // Reuse entity IDs

            if (!sparseSet.contains(entity)) {
                sparseSet.insert(entity);
            } else {
                sparseSet.erase(entity);
            }
        }

        // Verify integrity
        const auto& dense = sparseSet.getDense();
        for (Entity e : dense) {
            EXPECT_TRUE(sparseSet.contains(e));
        }
    }
}
