//// Entity.test.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/11/2024
//  Description: Test file for the entity manager
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ecs/Entity.hpp"
#include "Components.hpp"

namespace nexo::ecs {
    class EntityManagerTest : public ::testing::Test {
        protected:
        void SetUp() override {
            entityManager = std::make_unique<EntityManager>();
        }

        std::unique_ptr<EntityManager> entityManager;
    };

    TEST_F(EntityManagerTest, CreateAndDestroyEntity) {
        Entity entity = entityManager->createEntity();
        EXPECT_EQ(entity, 0); // First entity should have ID 0

        entityManager->destroyEntity(entity);

        // Recreate the entity, it should use the ID in front
        Entity reusedEntity = entityManager->createEntity();
        EXPECT_EQ(reusedEntity, 0);
    }

    TEST_F(EntityManagerTest, TooManyEntities) {
        for (Entity i = 0; i < MAX_ENTITIES; ++i) {
            EXPECT_NO_THROW(entityManager->createEntity());
        }

        EXPECT_THROW(entityManager->createEntity(), TooManyEntities);
    }

    TEST_F(EntityManagerTest, SetAndGetSignature) {
        Entity entity = entityManager->createEntity();
        Signature signature;
        signature.set(1); // Set the 1st bit

        entityManager->setSignature(entity, signature);

        Signature retrieved = entityManager->getSignature(entity);
        EXPECT_EQ(retrieved, signature);
    }

    TEST_F(EntityManagerTest, SetSignatureOutOfRange) {
        Entity invalidEntity = MAX_ENTITIES; // Invalid entity ID
        Signature signature;

        EXPECT_THROW(entityManager->setSignature(invalidEntity, signature), OutOfRange);
    }

    TEST_F(EntityManagerTest, GetSignatureOutOfRange) {
        Entity invalidEntity = MAX_ENTITIES; // Invalid entity ID

        EXPECT_THROW(static_cast<void>(entityManager->getSignature(invalidEntity)), OutOfRange);
    }

    TEST_F(EntityManagerTest, DestroyEntityResetsSignature) {
        Entity entity = entityManager->createEntity();
        Signature signature;
        signature.set(1); // Set the 1st bit

        entityManager->setSignature(entity, signature);

        entityManager->destroyEntity(entity);

        Signature resetSignature = entityManager->getSignature(entity);
        EXPECT_TRUE(resetSignature.none()); // Signature should be reset
    }

    TEST_F(EntityManagerTest, DestroyEntityOutOfRange) {
        Entity invalidEntity = MAX_ENTITIES; // Invalid entity ID

        EXPECT_THROW(entityManager->destroyEntity(invalidEntity), OutOfRange);
    }

    TEST_F(EntityManagerTest, CreateAndDestroyAllEntities) {
        std::vector<Entity> entities;

        for (Entity i = 0; i < MAX_ENTITIES; ++i) {
            entities.push_back(entityManager->createEntity());
        }

        EXPECT_THROW(entityManager->createEntity(), TooManyEntities);

        for (Entity entity : entities) {
            EXPECT_NO_THROW(entityManager->destroyEntity(entity));
        }

        EXPECT_NO_THROW(entityManager->createEntity());
    }

    TEST_F(EntityManagerTest, GetLivingEntityCount) {
        // Initially should be 0
        EXPECT_EQ(entityManager->getLivingEntityCount(), 0);

        // Create one entity
        Entity entity1 = entityManager->createEntity();
        EXPECT_EQ(entityManager->getLivingEntityCount(), 1);

        // Create another entity
        Entity entity2 = entityManager->createEntity();
        EXPECT_EQ(entityManager->getLivingEntityCount(), 2);

        // Destroy one entity
        entityManager->destroyEntity(entity1);
        EXPECT_EQ(entityManager->getLivingEntityCount(), 1);

        // Destroy the other entity
        entityManager->destroyEntity(entity2);
        EXPECT_EQ(entityManager->getLivingEntityCount(), 0);
    }

    TEST_F(EntityManagerTest, EntityIdSequence) {
        // Create multiple entities and check the ID sequence
        Entity entity1 = entityManager->createEntity();
        Entity entity2 = entityManager->createEntity();
        Entity entity3 = entityManager->createEntity();

        // IDs should be sequential starting from 0
        EXPECT_EQ(entity1, 0);
        EXPECT_EQ(entity2, 1);
        EXPECT_EQ(entity3, 2);
    }

    TEST_F(EntityManagerTest, ComplexEntityRecycling) {
        // Create several entities
        std::vector<Entity> entities;
        for (int i = 0; i < 5; ++i) {
            entities.push_back(entityManager->createEntity());
        }

        // Destroy entities in a non-sequential order
        entityManager->destroyEntity(entities[2]); // Destroy entity with ID 2
        entityManager->destroyEntity(entities[0]); // Destroy entity with ID 0

        // Create new entities and check the recycling order
        // Entities should be reused in FIFO order (first destroyed first reused)
        Entity newEntity1 = entityManager->createEntity();
        Entity newEntity2 = entityManager->createEntity();

        // The first available entity should be recycled first,
        // which depends on the order they were added to the availability queue
        EXPECT_EQ(newEntity1, 0); // ID 2 should be recycled first (FIFO)
        EXPECT_EQ(newEntity2, 2); // ID 0 should be recycled next

        // Destroy all entities and check if count reaches zero
        entityManager->destroyEntity(entities[1]);
        entityManager->destroyEntity(entities[3]);
        entityManager->destroyEntity(entities[4]);
        entityManager->destroyEntity(newEntity1);
        entityManager->destroyEntity(newEntity2);

        EXPECT_EQ(entityManager->getLivingEntityCount(), 0);
    }

    TEST_F(EntityManagerTest, EntityCountTracking) {
        // Test edge cases in entity count tracking

        // Create MAX_ENTITIES entities
        std::vector<Entity> entities;
        for (Entity i = 0; i < MAX_ENTITIES; ++i) {
            entities.push_back(entityManager->createEntity());
        }

        EXPECT_EQ(entityManager->getLivingEntityCount(), MAX_ENTITIES);

        // Destroy half of the entities
        for (Entity i = 0; i < MAX_ENTITIES / 2; ++i) {
            entityManager->destroyEntity(entities[i]);
        }

        if (MAX_ENTITIES % 2 == 0) {
        	EXPECT_EQ(entityManager->getLivingEntityCount(), MAX_ENTITIES / 2);
        } else {
        	EXPECT_EQ(entityManager->getLivingEntityCount(), MAX_ENTITIES / 2 + 1);
        }

        // Create half of them again
        for (Entity i = 0; i < MAX_ENTITIES / 2; ++i) {
            entityManager->createEntity();
        }

        EXPECT_EQ(entityManager->getLivingEntityCount(), MAX_ENTITIES);
    }

    TEST_F(EntityManagerTest, DestroyAlreadyDestroyedEntity) {
        // Create an entity
        Entity entity = entityManager->createEntity();
        EXPECT_EQ(entityManager->getLivingEntityCount(), 1);

        // Destroy it once
        entityManager->destroyEntity(entity);
        EXPECT_EQ(entityManager->getLivingEntityCount(), 0);

        // Destroy it again - this should work without errors
        // and not affect the living entity count (which should remain 0)
        EXPECT_NO_THROW(entityManager->destroyEntity(entity));
        EXPECT_EQ(entityManager->getLivingEntityCount(), 0);
    }

    TEST_F(EntityManagerTest, SignatureManipulation) {
        // Create an entity
        Entity entity = entityManager->createEntity();

        // Initial signature should be empty
        EXPECT_TRUE(entityManager->getSignature(entity).none());

        // Set some bits
        Signature signature;
        signature.set(0);
        signature.set(5);
        signature.set(10);
        entityManager->setSignature(entity, signature);

        // Check that signature was properly set
        Signature retrieved = entityManager->getSignature(entity);
        EXPECT_TRUE(retrieved.test(0));
        EXPECT_TRUE(retrieved.test(5));
        EXPECT_TRUE(retrieved.test(10));
        EXPECT_FALSE(retrieved.test(1));

        // Modify signature
        Signature newSignature = retrieved;
        newSignature.reset(0);
        newSignature.set(3);
        entityManager->setSignature(entity, newSignature);

        // Check updated signature
        retrieved = entityManager->getSignature(entity);
        EXPECT_FALSE(retrieved.test(0));
        EXPECT_TRUE(retrieved.test(3));
        EXPECT_TRUE(retrieved.test(5));
        EXPECT_TRUE(retrieved.test(10));
    }
}
