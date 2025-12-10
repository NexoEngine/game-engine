//// EntityManager.test.cpp ////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for EntityManager class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
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
    EXPECT_THROW(manager.getSignature(MAX_ENTITIES), OutOfRange);
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

}  // namespace nexo::ecs
