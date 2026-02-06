//// EntityManager.test.cpp ////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for EntityManager class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <algorithm>
#include "ecs/Entity.hpp"
#include "ecs/ECSExceptions.hpp"

namespace nexo::ecs {

// =============================================================================
// EntityManager Basic Tests
// =============================================================================

class EntityManagerBasicTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerBasicTest, InitiallyEmpty) {
    EXPECT_EQ(manager.getLivingEntityCount(), 0u);
}

TEST_F(EntityManagerBasicTest, CreateEntityReturnsValidId) {
    Entity e = manager.createEntity();
    EXPECT_LT(e, MAX_ENTITIES);
}

TEST_F(EntityManagerBasicTest, CreateEntityIncrementsCount) {
    manager.createEntity();
    EXPECT_EQ(manager.getLivingEntityCount(), 1u);
}

TEST_F(EntityManagerBasicTest, CreateMultipleEntitiesIncrementsCount) {
    manager.createEntity();
    manager.createEntity();
    manager.createEntity();
    EXPECT_EQ(manager.getLivingEntityCount(), 3u);
}

TEST_F(EntityManagerBasicTest, CreateEntityReturnsUniqueIds) {
    Entity e1 = manager.createEntity();
    Entity e2 = manager.createEntity();
    Entity e3 = manager.createEntity();

    EXPECT_NE(e1, e2);
    EXPECT_NE(e2, e3);
    EXPECT_NE(e1, e3);
}

TEST_F(EntityManagerBasicTest, CreateEntityReturnsSequentialIds) {
    // First entities should be sequential starting from 0
    Entity e1 = manager.createEntity();
    Entity e2 = manager.createEntity();
    Entity e3 = manager.createEntity();

    EXPECT_EQ(e1, 0u);
    EXPECT_EQ(e2, 1u);
    EXPECT_EQ(e3, 2u);
}

// =============================================================================
// EntityManager Destroy Tests
// =============================================================================

class EntityManagerDestroyTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerDestroyTest, DestroyEntityDecrementsCount) {
    Entity e = manager.createEntity();
    EXPECT_EQ(manager.getLivingEntityCount(), 1u);

    manager.destroyEntity(e);
    EXPECT_EQ(manager.getLivingEntityCount(), 0u);
}

TEST_F(EntityManagerDestroyTest, DestroyEntityMultiple) {
    Entity e1 = manager.createEntity();
    Entity e2 = manager.createEntity();
    Entity e3 = manager.createEntity();
    EXPECT_EQ(manager.getLivingEntityCount(), 3u);

    manager.destroyEntity(e2);
    EXPECT_EQ(manager.getLivingEntityCount(), 2u);

    manager.destroyEntity(e1);
    EXPECT_EQ(manager.getLivingEntityCount(), 1u);

    manager.destroyEntity(e3);
    EXPECT_EQ(manager.getLivingEntityCount(), 0u);
}

TEST_F(EntityManagerDestroyTest, DestroyEntityResetsSignature) {
    Entity e = manager.createEntity();

    Signature sig;
    sig.set(0);
    sig.set(3);
    manager.setSignature(e, sig);
    EXPECT_EQ(manager.getSignature(e), sig);

    manager.destroyEntity(e);

    // After destruction and recreation, signature should be reset
    Entity e2 = manager.createEntity();
    EXPECT_EQ(manager.getSignature(e2), Signature{});
}

TEST_F(EntityManagerDestroyTest, DestroyAndRecreateReusesId) {
    Entity e1 = manager.createEntity();
    manager.destroyEntity(e1);

    // The destroyed ID should be reused (pushed to front of queue)
    Entity e2 = manager.createEntity();
    EXPECT_EQ(e1, e2);
}

TEST_F(EntityManagerDestroyTest, DestroyNonExistentEntityNoOp) {
    // Destroying an entity that was never created should not crash
    // (it just won't find it in livingEntities)
    EXPECT_NO_THROW(manager.destroyEntity(0));
}

TEST_F(EntityManagerDestroyTest, DestroyEntityTwiceNoOp) {
    Entity e = manager.createEntity();
    manager.destroyEntity(e);

    // Second destroy should be a no-op (entity not in living list)
    EXPECT_NO_THROW(manager.destroyEntity(e));
}

// =============================================================================
// EntityManager Signature Tests
// =============================================================================

class EntityManagerSignatureTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerSignatureTest, InitialSignatureEmpty) {
    Entity e = manager.createEntity();
    Signature sig = manager.getSignature(e);
    EXPECT_EQ(sig.count(), 0u);
}

TEST_F(EntityManagerSignatureTest, SetSignature) {
    Entity e = manager.createEntity();

    Signature sig;
    sig.set(0);
    sig.set(5);
    sig.set(10);

    manager.setSignature(e, sig);
    EXPECT_EQ(manager.getSignature(e), sig);
}

TEST_F(EntityManagerSignatureTest, SetSignatureOverwrites) {
    Entity e = manager.createEntity();

    Signature sig1;
    sig1.set(0);
    manager.setSignature(e, sig1);

    Signature sig2;
    sig2.set(5);
    manager.setSignature(e, sig2);

    EXPECT_EQ(manager.getSignature(e), sig2);
    EXPECT_NE(manager.getSignature(e), sig1);
}

TEST_F(EntityManagerSignatureTest, SignaturesIndependent) {
    Entity e1 = manager.createEntity();
    Entity e2 = manager.createEntity();

    Signature sig1;
    sig1.set(0);

    Signature sig2;
    sig2.set(5);

    manager.setSignature(e1, sig1);
    manager.setSignature(e2, sig2);

    EXPECT_EQ(manager.getSignature(e1), sig1);
    EXPECT_EQ(manager.getSignature(e2), sig2);
}

TEST_F(EntityManagerSignatureTest, FullSignature) {
    Entity e = manager.createEntity();

    Signature sig;
    for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; ++i) {
        sig.set(i);
    }

    manager.setSignature(e, sig);
    EXPECT_EQ(manager.getSignature(e), sig);
    EXPECT_EQ(manager.getSignature(e).count(), MAX_COMPONENT_TYPE);
}

// =============================================================================
// EntityManager Living Entities Tests
// =============================================================================

class EntityManagerLivingEntitiesTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerLivingEntitiesTest, GetLivingEntitiesEmpty) {
    auto entities = manager.getLivingEntities();
    EXPECT_TRUE(entities.empty());
}

TEST_F(EntityManagerLivingEntitiesTest, GetLivingEntitiesReturnsAll) {
    Entity e1 = manager.createEntity();
    Entity e2 = manager.createEntity();
    Entity e3 = manager.createEntity();

    auto entities = manager.getLivingEntities();
    EXPECT_EQ(entities.size(), 3u);

    // Check all entities are present
    std::vector<Entity> entityList(entities.begin(), entities.end());
    EXPECT_NE(std::find(entityList.begin(), entityList.end(), e1), entityList.end());
    EXPECT_NE(std::find(entityList.begin(), entityList.end(), e2), entityList.end());
    EXPECT_NE(std::find(entityList.begin(), entityList.end(), e3), entityList.end());
}

TEST_F(EntityManagerLivingEntitiesTest, GetLivingEntitiesAfterDestroy) {
    Entity e1 = manager.createEntity();
    Entity e2 = manager.createEntity();
    Entity e3 = manager.createEntity();

    manager.destroyEntity(e2);

    auto entities = manager.getLivingEntities();
    EXPECT_EQ(entities.size(), 2u);

    std::vector<Entity> entityList(entities.begin(), entities.end());
    EXPECT_NE(std::find(entityList.begin(), entityList.end(), e1), entityList.end());
    EXPECT_EQ(std::find(entityList.begin(), entityList.end(), e2), entityList.end()); // e2 should not be present
    EXPECT_NE(std::find(entityList.begin(), entityList.end(), e3), entityList.end());
}

// =============================================================================
// EntityManager Exception Tests
// =============================================================================

class EntityManagerExceptionTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerExceptionTest, DestroyOutOfRangeThrows) {
    EXPECT_THROW(manager.destroyEntity(MAX_ENTITIES), OutOfRange);
    EXPECT_THROW(manager.destroyEntity(MAX_ENTITIES + 1), OutOfRange);
}

TEST_F(EntityManagerExceptionTest, SetSignatureOutOfRangeThrows) {
    EXPECT_THROW(manager.setSignature(MAX_ENTITIES, Signature{}), OutOfRange);
}

TEST_F(EntityManagerExceptionTest, GetSignatureOutOfRangeThrows) {
    EXPECT_THROW({ [[maybe_unused]] auto sig = manager.getSignature(MAX_ENTITIES); }, OutOfRange);
}

// =============================================================================
// EntityManager Stress Tests
// =============================================================================

class EntityManagerStressTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerStressTest, CreateManyEntities) {
    constexpr size_t NUM_ENTITIES = 1000;

    std::vector<Entity> entities;
    entities.reserve(NUM_ENTITIES);

    for (size_t i = 0; i < NUM_ENTITIES; ++i) {
        entities.push_back(manager.createEntity());
    }

    EXPECT_EQ(manager.getLivingEntityCount(), NUM_ENTITIES);

    // Verify all entities are unique
    std::sort(entities.begin(), entities.end());
    auto last = std::unique(entities.begin(), entities.end());
    EXPECT_EQ(last, entities.end());
}

TEST_F(EntityManagerStressTest, CreateDestroyPattern) {
    // Create some entities
    std::vector<Entity> entities;
    for (int i = 0; i < 100; ++i) {
        entities.push_back(manager.createEntity());
    }

    // Destroy every other entity
    for (size_t i = 0; i < entities.size(); i += 2) {
        manager.destroyEntity(entities[i]);
    }

    EXPECT_EQ(manager.getLivingEntityCount(), 50u);

    // Create new entities (should reuse destroyed IDs)
    for (int i = 0; i < 50; ++i) {
        manager.createEntity();
    }

    EXPECT_EQ(manager.getLivingEntityCount(), 100u);
}

TEST_F(EntityManagerStressTest, SignatureOperationsOnManyEntities) {
    constexpr size_t NUM_ENTITIES = 500;

    std::vector<Entity> entities;
    for (size_t i = 0; i < NUM_ENTITIES; ++i) {
        Entity e = manager.createEntity();
        entities.push_back(e);

        // Set a unique signature based on index
        Signature sig;
        sig.set(i % MAX_COMPONENT_TYPE);
        manager.setSignature(e, sig);
    }

    // Verify signatures
    for (size_t i = 0; i < NUM_ENTITIES; ++i) {
        Signature expected;
        expected.set(i % MAX_COMPONENT_TYPE);
        EXPECT_EQ(manager.getSignature(entities[i]), expected);
    }
}

// =============================================================================
// EntityManager Edge Case Tests
// =============================================================================

class EntityManagerEdgeCaseTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerEdgeCaseTest, EntityIdZeroValid) {
    Entity e = manager.createEntity();
    EXPECT_EQ(e, 0u);

    Signature sig;
    sig.set(1);
    EXPECT_NO_THROW(manager.setSignature(e, sig));
    EXPECT_EQ(manager.getSignature(e), sig);

    EXPECT_NO_THROW(manager.destroyEntity(e));
}

TEST_F(EntityManagerEdgeCaseTest, EmptySignature) {
    Entity e = manager.createEntity();

    Signature empty;
    manager.setSignature(e, empty);

    EXPECT_EQ(manager.getSignature(e).count(), 0u);
}

TEST_F(EntityManagerEdgeCaseTest, SignatureWithSingleBit) {
    Entity e = manager.createEntity();

    for (ComponentType bit = 0; bit < MAX_COMPONENT_TYPE; ++bit) {
        Signature sig;
        sig.set(bit);
        manager.setSignature(e, sig);

        EXPECT_TRUE(manager.getSignature(e).test(bit));
        EXPECT_EQ(manager.getSignature(e).count(), 1u);
    }
}

// =============================================================================
// EntityManager MAX_ENTITIES Limit Tests
// =============================================================================

class EntityManagerLimitTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerLimitTest, CreateEntitiesUpToMaxLimit) {
    // This test creates MAX_ENTITIES entities (warning: may be slow)
    // Creating entities up to the limit should succeed
    std::vector<Entity> entities;
    entities.reserve(MAX_ENTITIES);

    for (size_t i = 0; i < MAX_ENTITIES; ++i) {
        EXPECT_NO_THROW({
            Entity e = manager.createEntity();
            entities.push_back(e);
        });
    }

    EXPECT_EQ(manager.getLivingEntityCount(), MAX_ENTITIES);
}

TEST_F(EntityManagerLimitTest, CreateEntityBeyondMaxThrows) {
    // Fill up to max
    for (size_t i = 0; i < MAX_ENTITIES; ++i) {
        manager.createEntity();
    }

    // Trying to create one more should throw
    EXPECT_THROW(manager.createEntity(), TooManyEntities);
}

TEST_F(EntityManagerLimitTest, LivingEntityCountAccurateAtLimit) {
    // Create entities up to the limit
    for (size_t i = 0; i < MAX_ENTITIES; ++i) {
        manager.createEntity();
    }

    EXPECT_EQ(manager.getLivingEntityCount(), MAX_ENTITIES);

    // Destroy one and verify count
    manager.destroyEntity(100);
    EXPECT_EQ(manager.getLivingEntityCount(), MAX_ENTITIES - 1);

    // Create one more and verify we're back at max
    manager.createEntity();
    EXPECT_EQ(manager.getLivingEntityCount(), MAX_ENTITIES);
}

TEST_F(EntityManagerLimitTest, MaxEntitiesMinusOneIsValid) {
    // MAX_ENTITIES - 1 is the highest valid entity ID
    // Fill all entities
    for (size_t i = 0; i < MAX_ENTITIES; ++i) {
        manager.createEntity();
    }

    // MAX_ENTITIES - 1 should be valid
    EXPECT_NO_THROW({ [[maybe_unused]] auto sig = manager.getSignature(MAX_ENTITIES - 1); });
    EXPECT_NO_THROW(manager.setSignature(MAX_ENTITIES - 1, Signature{}));
}

// =============================================================================
// EntityManager Entity ID Recycling Tests
// =============================================================================

class EntityManagerRecyclingTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerRecyclingTest, DestroyAndRecreateReusesIdLifo) {
    // Create entities 0, 1, 2
    Entity e0 = manager.createEntity();
    Entity e1 = manager.createEntity();
    Entity e2 = manager.createEntity();

    // Destroy in order: 0, 1, 2
    manager.destroyEntity(e0);
    manager.destroyEntity(e1);
    manager.destroyEntity(e2);

    // Recreate - should reuse in LIFO order (2, 1, 0)
    // because destroyEntity uses push_front
    Entity new0 = manager.createEntity();
    Entity new1 = manager.createEntity();
    Entity new2 = manager.createEntity();

    EXPECT_EQ(new0, e2); // Last destroyed, first reused
    EXPECT_EQ(new1, e1);
    EXPECT_EQ(new2, e0);
}

TEST_F(EntityManagerRecyclingTest, MultipleDestroyRecyclesCycles) {
    std::vector<Entity> first_batch;
    std::vector<Entity> second_batch;
    std::vector<Entity> third_batch;

    // First cycle: create 10 entities
    for (int i = 0; i < 10; ++i) {
        first_batch.push_back(manager.createEntity());
    }

    // Destroy all
    for (Entity e : first_batch) {
        manager.destroyEntity(e);
    }

    EXPECT_EQ(manager.getLivingEntityCount(), 0u);

    // Second cycle: create 10 more (should reuse IDs)
    for (int i = 0; i < 10; ++i) {
        second_batch.push_back(manager.createEntity());
    }

    EXPECT_EQ(manager.getLivingEntityCount(), 10u);

    // Destroy all again
    for (Entity e : second_batch) {
        manager.destroyEntity(e);
    }

    // Third cycle: create 10 more (should reuse IDs again)
    for (int i = 0; i < 10; ++i) {
        third_batch.push_back(manager.createEntity());
    }

    EXPECT_EQ(manager.getLivingEntityCount(), 10u);

    // All batches should have the same set of IDs
    std::sort(first_batch.begin(), first_batch.end());
    std::sort(second_batch.begin(), second_batch.end());
    std::sort(third_batch.begin(), third_batch.end());

    EXPECT_EQ(first_batch, second_batch);
    EXPECT_EQ(second_batch, third_batch);
}

TEST_F(EntityManagerRecyclingTest, PartialDestroyAndRecreate) {
    // Create 20 entities
    std::vector<Entity> entities;
    for (int i = 0; i < 20; ++i) {
        entities.push_back(manager.createEntity());
    }

    // Destroy every third entity
    std::vector<Entity> destroyed;
    for (size_t i = 0; i < entities.size(); i += 3) {
        destroyed.push_back(entities[i]);
        manager.destroyEntity(entities[i]);
    }

    size_t destroyed_count = destroyed.size();
    EXPECT_EQ(manager.getLivingEntityCount(), 20u - destroyed_count);

    // Recreate same number as destroyed
    std::vector<Entity> recreated;
    for (size_t i = 0; i < destroyed_count; ++i) {
        recreated.push_back(manager.createEntity());
    }

    EXPECT_EQ(manager.getLivingEntityCount(), 20u);

    // Recreated IDs should match destroyed IDs (in reverse order due to LIFO)
    std::sort(destroyed.begin(), destroyed.end());
    std::sort(recreated.begin(), recreated.end());
    EXPECT_EQ(destroyed, recreated);
}

TEST_F(EntityManagerRecyclingTest, RecycledEntityHasCleanSignature) {
    Entity e = manager.createEntity();

    // Set complex signature
    Signature sig;
    sig.set(0);
    sig.set(5);
    sig.set(10);
    sig.set(15);
    sig.set(20);
    manager.setSignature(e, sig);

    EXPECT_EQ(manager.getSignature(e), sig);

    // Destroy entity
    manager.destroyEntity(e);

    // Recreate - should reuse same ID
    Entity e2 = manager.createEntity();
    EXPECT_EQ(e, e2);

    // Signature should be clean
    EXPECT_EQ(manager.getSignature(e2), Signature{});
    EXPECT_EQ(manager.getSignature(e2).count(), 0u);
}

// =============================================================================
// EntityManager Rapid Create/Destroy Tests
// =============================================================================

class EntityManagerRapidOperationsTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerRapidOperationsTest, AlternatingCreateDestroy) {
    // Rapidly alternate between creating and destroying
    for (int i = 0; i < 100; ++i) {
        Entity e = manager.createEntity();
        EXPECT_EQ(manager.getLivingEntityCount(), 1u);
        manager.destroyEntity(e);
        EXPECT_EQ(manager.getLivingEntityCount(), 0u);
    }
}

TEST_F(EntityManagerRapidOperationsTest, CreateDestroyBatch) {
    // Create batches and destroy them repeatedly
    for (int batch = 0; batch < 50; ++batch) {
        std::vector<Entity> entities;

        // Create batch of 20
        for (int i = 0; i < 20; ++i) {
            entities.push_back(manager.createEntity());
        }

        EXPECT_EQ(manager.getLivingEntityCount(), 20u);

        // Destroy entire batch
        for (Entity e : entities) {
            manager.destroyEntity(e);
        }

        EXPECT_EQ(manager.getLivingEntityCount(), 0u);
    }
}

TEST_F(EntityManagerRapidOperationsTest, InterleavedCreateDestroy) {
    std::vector<Entity> alive;

    // Interleaved pattern: create 5, destroy 2, create 3, destroy 4, etc.
    for (int i = 0; i < 5; ++i) {
        alive.push_back(manager.createEntity());
    }
    EXPECT_EQ(manager.getLivingEntityCount(), 5u);

    manager.destroyEntity(alive[0]);
    manager.destroyEntity(alive[1]);
    alive.erase(alive.begin(), alive.begin() + 2);
    EXPECT_EQ(manager.getLivingEntityCount(), 3u);

    for (int i = 0; i < 3; ++i) {
        alive.push_back(manager.createEntity());
    }
    EXPECT_EQ(manager.getLivingEntityCount(), 6u);

    manager.destroyEntity(alive[0]);
    manager.destroyEntity(alive[1]);
    manager.destroyEntity(alive[2]);
    manager.destroyEntity(alive[3]);
    alive.erase(alive.begin(), alive.begin() + 4);
    EXPECT_EQ(manager.getLivingEntityCount(), 2u);
}

TEST_F(EntityManagerRapidOperationsTest, RapidSignatureChanges) {
    Entity e = manager.createEntity();

    // Rapidly change signatures
    for (int i = 0; i < 100; ++i) {
        Signature sig;
        sig.set(i % MAX_COMPONENT_TYPE);
        manager.setSignature(e, sig);
        EXPECT_EQ(manager.getSignature(e), sig);
    }

    manager.destroyEntity(e);
}

// =============================================================================
// EntityManager Signature Edge Case Tests
// =============================================================================

class EntityManagerSignatureEdgeCaseTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerSignatureEdgeCaseTest, AllBitsSetThenClear) {
    Entity e = manager.createEntity();

    // Set all bits
    Signature full;
    for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; ++i) {
        full.set(i);
    }
    manager.setSignature(e, full);
    EXPECT_EQ(manager.getSignature(e).count(), MAX_COMPONENT_TYPE);

    // Clear all bits
    Signature empty;
    manager.setSignature(e, empty);
    EXPECT_EQ(manager.getSignature(e).count(), 0u);
}

TEST_F(EntityManagerSignatureEdgeCaseTest, SignatureFirstBit) {
    Entity e = manager.createEntity();

    Signature sig;
    sig.set(0);
    manager.setSignature(e, sig);

    EXPECT_TRUE(manager.getSignature(e).test(0));
    EXPECT_EQ(manager.getSignature(e).count(), 1u);
}

TEST_F(EntityManagerSignatureEdgeCaseTest, SignatureLastBit) {
    Entity e = manager.createEntity();

    Signature sig;
    sig.set(MAX_COMPONENT_TYPE - 1);
    manager.setSignature(e, sig);

    EXPECT_TRUE(manager.getSignature(e).test(MAX_COMPONENT_TYPE - 1));
    EXPECT_EQ(manager.getSignature(e).count(), 1u);
}

TEST_F(EntityManagerSignatureEdgeCaseTest, SignatureAlternatingBits) {
    Entity e = manager.createEntity();

    Signature sig;
    for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; i += 2) {
        sig.set(i);
    }
    manager.setSignature(e, sig);

    for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; ++i) {
        if (i % 2 == 0) {
            EXPECT_TRUE(manager.getSignature(e).test(i));
        } else {
            EXPECT_FALSE(manager.getSignature(e).test(i));
        }
    }
}

TEST_F(EntityManagerSignatureEdgeCaseTest, MultipleEntitiesDistinctSignatures) {
    constexpr size_t NUM_ENTITIES = MAX_COMPONENT_TYPE;
    std::vector<Entity> entities;

    // Create entities with unique signatures
    for (size_t i = 0; i < NUM_ENTITIES; ++i) {
        Entity e = manager.createEntity();
        entities.push_back(e);

        Signature sig;
        sig.set(i);
        manager.setSignature(e, sig);
    }

    // Verify each entity has correct signature
    for (size_t i = 0; i < NUM_ENTITIES; ++i) {
        Signature expected;
        expected.set(i);
        EXPECT_EQ(manager.getSignature(entities[i]), expected);
    }
}

// =============================================================================
// EntityManager Entity ID Distribution Tests
// =============================================================================

class EntityManagerDistributionTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerDistributionTest, SequentialIdAfterNoDestroy) {
    std::vector<Entity> entities;

    for (int i = 0; i < 100; ++i) {
        entities.push_back(manager.createEntity());
    }

    // IDs should be sequential
    for (int i = 0; i < 100; ++i) {
        EXPECT_EQ(entities[i], static_cast<Entity>(i));
    }
}

TEST_F(EntityManagerDistributionTest, IdDistributionAfterManyOperations) {
    std::vector<Entity> entities;

    // Create 50 entities
    for (int i = 0; i < 50; ++i) {
        entities.push_back(manager.createEntity());
    }

    // Destroy every other one
    for (int i = 0; i < 50; i += 2) {
        manager.destroyEntity(entities[i]);
    }

    // Create 25 more - should reuse destroyed IDs
    std::vector<Entity> new_entities;
    for (int i = 0; i < 25; ++i) {
        new_entities.push_back(manager.createEntity());
    }

    // All new entities should be within 0-49 range (reused IDs)
    for (Entity e : new_entities) {
        EXPECT_LT(e, 50u);
    }
}

TEST_F(EntityManagerDistributionTest, NoIdDuplicationAfterComplexPattern) {
    std::vector<Entity> all_entities;

    // Create 100 entities
    for (int i = 0; i < 100; ++i) {
        all_entities.push_back(manager.createEntity());
    }

    // Destroy random pattern
    manager.destroyEntity(all_entities[10]);
    manager.destroyEntity(all_entities[25]);
    manager.destroyEntity(all_entities[50]);
    manager.destroyEntity(all_entities[75]);
    manager.destroyEntity(all_entities[99]);

    // Remove destroyed entities from tracking
    std::vector<Entity> alive_entities;
    for (size_t i = 0; i < all_entities.size(); ++i) {
        if (i != 10 && i != 25 && i != 50 && i != 75 && i != 99) {
            alive_entities.push_back(all_entities[i]);
        }
    }

    // Create 5 new entities
    for (int i = 0; i < 5; ++i) {
        alive_entities.push_back(manager.createEntity());
    }

    // Check all living entities are unique
    std::sort(alive_entities.begin(), alive_entities.end());
    auto last = std::unique(alive_entities.begin(), alive_entities.end());
    EXPECT_EQ(last, alive_entities.end());

    // Verify count
    EXPECT_EQ(manager.getLivingEntityCount(), 100u);
}

// =============================================================================
// EntityManager Validation Edge Cases
// =============================================================================

class EntityManagerValidationTest : public ::testing::Test {
protected:
    EntityManager manager;
};

TEST_F(EntityManagerValidationTest, GetSignatureForNeverCreatedEntity) {
    // Entity ID 500 was never created (assuming we haven't created that many)
    // Getting signature should not throw (just returns the default signature)
    EXPECT_NO_THROW({
        Signature sig = manager.getSignature(500);
        EXPECT_EQ(sig.count(), 0u);
    });
}

TEST_F(EntityManagerValidationTest, SetSignatureForNeverCreatedEntity) {
    // Setting signature for an entity that hasn't been created yet
    // is allowed (signature is stored in array)
    Signature sig;
    sig.set(5);

    EXPECT_NO_THROW(manager.setSignature(100, sig));
    EXPECT_EQ(manager.getSignature(100), sig);
}

TEST_F(EntityManagerValidationTest, InvalidEntityId) {
    // INVALID_ENTITY constant should throw OutOfRange
    EXPECT_THROW(manager.destroyEntity(INVALID_ENTITY), OutOfRange);
    EXPECT_THROW({ [[maybe_unused]] auto sig = manager.getSignature(INVALID_ENTITY); }, OutOfRange);
    EXPECT_THROW(manager.setSignature(INVALID_ENTITY, Signature{}), OutOfRange);
}

TEST_F(EntityManagerValidationTest, EntityIdMaxEntitiesThrows) {
    // MAX_ENTITIES is out of range
    EXPECT_THROW(manager.destroyEntity(MAX_ENTITIES), OutOfRange);
    EXPECT_THROW({ [[maybe_unused]] auto sig = manager.getSignature(MAX_ENTITIES); }, OutOfRange);
    EXPECT_THROW(manager.setSignature(MAX_ENTITIES, Signature{}), OutOfRange);
}

TEST_F(EntityManagerValidationTest, LivingEntitiesDoesNotContainDestroyed) {
    // Create several entities
    Entity e1 = manager.createEntity();
    Entity e2 = manager.createEntity();
    Entity e3 = manager.createEntity();

    // Destroy middle one
    manager.destroyEntity(e2);

    // Get living entities
    auto living = manager.getLivingEntities();
    std::vector<Entity> living_vec(living.begin(), living.end());

    // Should contain e1 and e3, but not e2
    EXPECT_NE(std::find(living_vec.begin(), living_vec.end(), e1), living_vec.end());
    EXPECT_EQ(std::find(living_vec.begin(), living_vec.end(), e2), living_vec.end());
    EXPECT_NE(std::find(living_vec.begin(), living_vec.end(), e3), living_vec.end());
}

}  // namespace nexo::ecs
