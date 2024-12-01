//// System.test.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/11/2024
//  Description: Test file for the system manager
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "ecs/System.hpp"
#include "ecs/Signature.hpp"

namespace nexo::ecs {
    // Mock system for testing
    class MockSystem : public System {
        public:
        MOCK_METHOD(void, update, (), (const));
    };

    class MockSystem2 : public System {
        public:
        MOCK_METHOD(void, update, (), (const));
    };

    class SystemManagerTest : public ::testing::Test {
        protected:
        void SetUp() override {
            systemManager = std::make_unique<SystemManager>();
        }

        std::unique_ptr<SystemManager> systemManager;
    };

    TEST_F(SystemManagerTest, RegisterSystem) {
        auto mockSystem = systemManager->registerSystem<MockSystem>();

        EXPECT_NE(mockSystem, nullptr);
        EXPECT_TRUE(mockSystem->entities.empty());
    }

    TEST_F(SystemManagerTest, RegisterSystemTwice) {
        systemManager->registerSystem<MockSystem>();

        // Attempting to register the same system twice
        EXPECT_NO_THROW(systemManager->registerSystem<MockSystem>());
    }

    TEST_F(SystemManagerTest, SetSignatureForSystem) {
        auto mockSystem = systemManager->registerSystem<MockSystem>();

        Signature signature;
        signature.set(1); // Set the 1st bit

        EXPECT_NO_THROW(systemManager->setSignature<MockSystem>(signature));
    }

    TEST_F(SystemManagerTest, SetSignatureForUnregisteredSystem) {
        Signature signature;
        signature.set(1);

        EXPECT_THROW(systemManager->setSignature<MockSystem>(signature), SystemNotRegistered);
    }

    TEST_F(SystemManagerTest, EntityDestroyed) {
        auto mockSystem = systemManager->registerSystem<MockSystem>();

        Signature signature;
        signature.set(1); // Set the 1st bit
        systemManager->setSignature<MockSystem>(signature);

        // Simulate an entity with the matching signature
        Entity entity = 1;
        mockSystem->entities.insert(entity);

        systemManager->entityDestroyed(entity);

        EXPECT_TRUE(mockSystem->entities.empty());
    }

    TEST_F(SystemManagerTest, EntitySignatureChanged_AddEntityToSystem) {
        auto mockSystem = systemManager->registerSystem<MockSystem>();

        Signature systemSignature;
        systemSignature.set(1); // Set the 1st bit
        systemManager->setSignature<MockSystem>(systemSignature);

        Entity entity = 1;
        Signature entitySignature;
        entitySignature.set(1); // Matches the system's signature

        systemManager->entitySignatureChanged(entity, entitySignature);

        EXPECT_TRUE(mockSystem->entities.contains(entity));
    }

    TEST_F(SystemManagerTest, EntitySignatureChanged_RemoveEntityFromSystem) {
        auto mockSystem = systemManager->registerSystem<MockSystem>();

        Signature systemSignature;
        systemSignature.set(1); // Set the 1st bit
        systemManager->setSignature<MockSystem>(systemSignature);

        Entity entity = 1;
        Signature entitySignature;
        entitySignature.set(1); // Matches the system's signature

        // Add the entity
        systemManager->entitySignatureChanged(entity, entitySignature);
        EXPECT_TRUE(mockSystem->entities.contains(entity));

        // Change signature to no longer match
        entitySignature.reset(1); // Clear the 1st bit
        systemManager->entitySignatureChanged(entity, entitySignature);

        EXPECT_FALSE(mockSystem->entities.contains(entity));
    }

    TEST_F(SystemManagerTest, EntitySignatureChanged_IrrelevantEntity) {
        auto mockSystem = systemManager->registerSystem<MockSystem>();

        Signature systemSignature;
        systemSignature.set(1); // Set the 1st bit
        systemManager->setSignature<MockSystem>(systemSignature);

        Entity entity = 1;
        Signature entitySignature;
        entitySignature.set(2); // Does not match the system's signature

        systemManager->entitySignatureChanged(entity, entitySignature);

        EXPECT_FALSE(mockSystem->entities.contains(entity));
    }

    TEST_F(SystemManagerTest, EntityDestroyedTwice) {
        auto mockSystem = systemManager->registerSystem<MockSystem>();

        Signature signature;
        signature.set(1); // Set the 1st bit
        systemManager->setSignature<MockSystem>(signature);

        Entity entity = 1;
        mockSystem->entities.insert(entity);

        systemManager->entityDestroyed(entity);
        EXPECT_TRUE(mockSystem->entities.empty());

        // Destroying the same entity again should not throw an error
        EXPECT_NO_THROW(systemManager->entityDestroyed(entity));
    }

    TEST_F(SystemManagerTest, AddAndRemoveEntitiesFromMultipleSystems) {
        auto system1 = systemManager->registerSystem<MockSystem>();
        auto system2 = systemManager->registerSystem<MockSystem2>();

        Signature signature1;
        signature1.set(1); // Matches entities with component 1
        systemManager->setSignature<MockSystem>(signature1);

        Signature signature2;
        signature2.set(2); // Matches entities with component 2
        systemManager->setSignature<MockSystem2>(signature2);

        // Add entities with different signatures
        Entity entity1 = 1;
        Entity entity2 = 2;

        Signature entitySignature1;
        entitySignature1.set(1); // Matches system1

        Signature entitySignature2;
        entitySignature2.set(2); // Matches system2

        systemManager->entitySignatureChanged(entity1, entitySignature1);
        systemManager->entitySignatureChanged(entity2, entitySignature2);

        EXPECT_TRUE(system1->entities.contains(entity1));
        EXPECT_FALSE(system1->entities.contains(entity2));
        EXPECT_FALSE(system2->entities.contains(entity1));
        EXPECT_TRUE(system2->entities.contains(entity2));

        // Change signature of entity1 to match system2
        systemManager->entitySignatureChanged(entity1, entitySignature2);
        EXPECT_FALSE(system1->entities.contains(entity1));
        EXPECT_TRUE(system2->entities.contains(entity1));
    }
}