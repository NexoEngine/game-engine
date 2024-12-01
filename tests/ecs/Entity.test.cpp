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
        EXPECT_EQ(reusedEntity, 1);
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

        EXPECT_THROW(entityManager->getSignature(invalidEntity), OutOfRange);
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
}