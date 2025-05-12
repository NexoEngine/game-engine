//// SparseSet.test.cpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
}
