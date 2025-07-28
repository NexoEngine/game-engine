//// Entity.test.cpp ///////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//
//  Author:      iMeaNz
//  Date:        2025-04-09
//  Description: Test file for Entity Manager class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "Entity.hpp"
#include "ECSExceptions.hpp"
#include <vector>
#include <set>
#include <algorithm>

namespace nexo::ecs {

	class EntityManagerTest : public ::testing::Test {
	protected:
	    EntityManager entityManager;

	    // Helper to create multiple entities
	    std::vector<Entity> createMultipleEntities(size_t count) {
	        std::vector<Entity> entities;
	        entities.reserve(count);
	        for (size_t i = 0; i < count; ++i) {
	            entities.push_back(entityManager.createEntity());
	        }
	        return entities;
	    }
	};

	// Test constructor
	TEST_F(EntityManagerTest, ConstructorInitializesCorrectly) {
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 0);

	    // Create and then destroy an entity to verify the pool works
	    Entity e = entityManager.createEntity();
	    EXPECT_EQ(e, 0); // First entity should be 0
	    entityManager.destroyEntity(e);
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 0);

	    // Creating again should reuse the entity ID
	    Entity e2 = entityManager.createEntity();
	    EXPECT_EQ(e2, 0);
	}

	TEST_F(EntityManagerTest, CreateEntityReturnsUniqueIDs) {
	    const size_t numEntities = 100;
	    std::set<Entity> uniqueEntities;

	    for (size_t i = 0; i < numEntities; ++i) {
	        Entity e = entityManager.createEntity();
	        EXPECT_EQ(uniqueEntities.count(e), 0) << "Entity ID " << e << " was issued multiple times";
	        uniqueEntities.insert(e);
	    }

	    EXPECT_EQ(entityManager.getLivingEntityCount(), numEntities);
	}

	// Test destroyEntity
	TEST_F(EntityManagerTest, DestroyEntityRemovesFromLivingEntities) {
	    // Create some entities
	    auto entities = createMultipleEntities(5);
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 5);

	    // Destroy middle entity
	    entityManager.destroyEntity(entities[2]);
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 4);

	    // Check it's removed from living entities
	    auto livingEntities = entityManager.getLivingEntities();
	    EXPECT_FALSE(std::find(livingEntities.begin(), livingEntities.end(), entities[2]) != livingEntities.end());

	    // Other entities should still be there
	    EXPECT_TRUE(std::find(livingEntities.begin(), livingEntities.end(), entities[0]) != livingEntities.end());
	    EXPECT_TRUE(std::find(livingEntities.begin(), livingEntities.end(), entities[1]) != livingEntities.end());
	    EXPECT_TRUE(std::find(livingEntities.begin(), livingEntities.end(), entities[3]) != livingEntities.end());
	    EXPECT_TRUE(std::find(livingEntities.begin(), livingEntities.end(), entities[4]) != livingEntities.end());
	}

	TEST_F(EntityManagerTest, DestroyEntityThrowsWithInvalidEntity) {
	    EXPECT_THROW({ entityManager.destroyEntity(MAX_ENTITIES); }, OutOfRange);
	}

	// Test ID recycling
	TEST_F(EntityManagerTest, DestroyedEntityIDsAreRecycled) {
	    // Create entities
	    auto entities = createMultipleEntities(5);

	    // Destroy entities in reverse order
	    entityManager.destroyEntity(entities[4]);
	    entityManager.destroyEntity(entities[2]);
	    entityManager.destroyEntity(entities[0]);

	    // New entities should reuse IDs in LIFO order (stack behavior)
	    Entity e1 = entityManager.createEntity();
	    Entity e2 = entityManager.createEntity();
	    Entity e3 = entityManager.createEntity();

	    // The IDs should be reused in reverse order of destruction
	    EXPECT_EQ(e1, entities[0]);
	    EXPECT_EQ(e2, entities[2]);
	    EXPECT_EQ(e3, entities[4]);
	}

	TEST_F(EntityManagerTest, SetAndGetSignatureWorkCorrectly) {
	    // Create an entity
	    Entity e = entityManager.createEntity();

	    // Create a test signature
	    Signature signature;
	    signature.set(1); // Set bit 1
	    signature.set(3); // Set bit 3

	    // Set signature
	    entityManager.setSignature(e, signature);

	    // Get signature and verify it matches
	    Signature retrievedSignature = entityManager.getSignature(e);
	    EXPECT_EQ(retrievedSignature, signature);
	    EXPECT_TRUE(retrievedSignature.test(1));
	    EXPECT_TRUE(retrievedSignature.test(3));
	    EXPECT_FALSE(retrievedSignature.test(0));
	    EXPECT_FALSE(retrievedSignature.test(2));
	}

	// Test setting signature for invalid entity
	TEST_F(EntityManagerTest, SetSignatureThrowsWithInvalidEntity) {
	    Signature signature;
	    EXPECT_THROW({ entityManager.setSignature(MAX_ENTITIES, signature); }, OutOfRange);
	}

	// Test getting signature for invalid entity
	TEST_F(EntityManagerTest, GetSignatureThrowsWithInvalidEntity) {
	    EXPECT_THROW({ static_cast<void>(entityManager.getSignature(MAX_ENTITIES)); }, OutOfRange);
	}

	// Test signature is reset on entity destruction
	TEST_F(EntityManagerTest, DestroyEntityResetsSignature) {
	    // Create an entity and set signature
	    Entity e = entityManager.createEntity();

	    Signature signature;
	    signature.set(1);
	    signature.set(3);

	    entityManager.setSignature(e, signature);

	    // Destroy entity
	    entityManager.destroyEntity(e);

	    // Create a new entity (should reuse the ID)
	    Entity newE = entityManager.createEntity();
	    EXPECT_EQ(newE, e);

	    // Signature should be reset
	    Signature newSignature = entityManager.getSignature(newE);
	    EXPECT_EQ(newSignature.count(), 0);
	    EXPECT_FALSE(newSignature.test(1));
	    EXPECT_FALSE(newSignature.test(3));
	}

	// Test get living entities
	TEST_F(EntityManagerTest, GetLivingEntitiesReturnsCorrectEntities) {
	    // Empty at start
	    EXPECT_EQ(entityManager.getLivingEntities().size(), 0);

	    // Create some entities
	    auto entities = createMultipleEntities(5);

	    // Check living entities
	    auto livingEntities = entityManager.getLivingEntities();
	    EXPECT_EQ(livingEntities.size(), 5);

	    // Verify all created entities are in the living set
	    for (auto e : entities) {
	        bool found = false;
	        for (auto le : livingEntities) {
	            if (le == e) {
	                found = true;
	                break;
	            }
	        }
	        EXPECT_TRUE(found) << "Entity " << e << " not found in living entities";
	    }

	    // Remove an entity
	    entityManager.destroyEntity(entities[2]);

	    // Check updated living entities
	    livingEntities = entityManager.getLivingEntities();
	    EXPECT_EQ(livingEntities.size(), 4);

	    // Verify entity was removed
	    bool found = false;
	    for (auto le : livingEntities) {
	        if (le == entities[2]) {
	            found = true;
	            break;
	        }
	    }
	    EXPECT_FALSE(found) << "Destroyed entity " << entities[2] << " still found in living entities";
	}

	// Test get living entity count
	TEST_F(EntityManagerTest, GetLivingEntityCountReturnsCorrectCount) {
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 0);

	    auto e1 = entityManager.createEntity();
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 1);

	    auto e2 = entityManager.createEntity();
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 2);

	    entityManager.destroyEntity(e1);
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 1);

	    entityManager.destroyEntity(e2);
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 0);
	}

	// Complex scenario test
	TEST_F(EntityManagerTest, ComplexEntityLifecycleScenario) {
	    // Create a batch of entities
	    std::vector<Entity> batch1 = createMultipleEntities(10);
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 10);

	    // Set signatures for some entities
	    for (size_t i = 0; i < batch1.size(); i += 2) {
	        Signature sig;
	        sig.set(i % MAX_COMPONENT_TYPE);
	        entityManager.setSignature(batch1[i], sig);
	    }

	    // Destroy some entities
	    for (size_t i = 0; i < batch1.size(); i += 3) {
	        entityManager.destroyEntity(batch1[i]);
	    }

	    // Create new entities (should reuse some IDs)
	    std::vector<Entity> batch2 = createMultipleEntities(5);

	    // Verify expected count
	    EXPECT_EQ(entityManager.getLivingEntityCount(), 11); // 10 - 4 + 5 = 11 (10 original - 4 destroyed + 5 new)

	    // Check recycled IDs have clean signatures
	    for (Entity e : batch2) {
	        Signature sig = entityManager.getSignature(e);
	        EXPECT_EQ(sig.count(), 0) << "Recycled entity " << e << " has non-empty signature";
	    }

	    // Destroy all entities
	    auto livingEntities = entityManager.getLivingEntities();
		std::vector<Entity> toDestroy;
		toDestroy.reserve(livingEntities.size());
		for (Entity e : livingEntities) {
			toDestroy.push_back(e);
		}
		for (Entity e : toDestroy) {
			entityManager.destroyEntity(e);
		}

	    EXPECT_EQ(entityManager.getLivingEntityCount(), 0);
	}

	// Edge case - destroy entity that's already destroyed
	TEST_F(EntityManagerTest, DestroyAlreadyDestroyedEntity) {
	    // This doesn't throw, just performs the operation
	    Entity e = entityManager.createEntity();
	    entityManager.destroyEntity(e);

	    // Should not throw, signature already reset
	    entityManager.destroyEntity(e);

	    EXPECT_EQ(entityManager.getLivingEntityCount(), 0);

	    // Create a new entity, should still get the same ID back
	    Entity newE = entityManager.createEntity();
	    EXPECT_EQ(newE, e);
	}

}
