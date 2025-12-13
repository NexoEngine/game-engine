//// SystemManager.test.cpp ////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        13/12/2025
//  Description: Test file for SystemManager, SparseSet, and System classes
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/System.hpp"
#include "ecs/Definitions.hpp"

namespace nexo::ecs {

// =============================================================================
// Mock Systems for Testing
// =============================================================================

// Mock QuerySystem with configurable signature
class MockQuerySystem : public AQuerySystem {
public:
    explicit MockQuerySystem(Signature sig = Signature{}) : m_signature(sig) {}

    const Signature& getSignature() const override {
        return m_signature;
    }

    void setSignature(Signature sig) {
        m_signature = sig;
    }

private:
    Signature m_signature;
};

// Another mock QuerySystem for testing multiple systems
class AnotherQuerySystem : public AQuerySystem {
public:
    explicit AnotherQuerySystem(Signature sig = Signature{}) : m_signature(sig) {}

    const Signature& getSignature() const override {
        return m_signature;
    }

private:
    Signature m_signature;
};

// Mock QuerySystem with constructor arguments
class ParameterizedQuerySystem : public AQuerySystem {
public:
    ParameterizedQuerySystem(int value, std::string name, Signature sig = Signature{})
        : m_value(value), m_name(std::move(name)), m_signature(sig) {}

    const Signature& getSignature() const override {
        return m_signature;
    }

    int getValue() const { return m_value; }
    const std::string& getName() const { return m_name; }

private:
    int m_value;
    std::string m_name;
    Signature m_signature;
};

// Mock GroupSystem
class MockGroupSystem : public AGroupSystem {
public:
    MockGroupSystem() = default;
    int callCount = 0;
};

// Another mock GroupSystem for testing multiple group systems
class AnotherGroupSystem : public AGroupSystem {
public:
    AnotherGroupSystem() = default;
};

// Mock GroupSystem with constructor arguments
class ParameterizedGroupSystem : public AGroupSystem {
public:
    ParameterizedGroupSystem(int value, std::string name)
        : m_value(value), m_name(std::move(name)) {}

    int getValue() const { return m_value; }
    const std::string& getName() const { return m_name; }

private:
    int m_value;
    std::string m_name;
};

// =============================================================================
// SparseSet Basic Tests
// =============================================================================

class SparseSetBasicTest : public ::testing::Test {
protected:
    SparseSet sparseSet;
};

TEST_F(SparseSetBasicTest, InitiallyEmpty) {
    EXPECT_TRUE(sparseSet.empty());
    EXPECT_EQ(sparseSet.size(), 0u);
}

TEST_F(SparseSetBasicTest, InsertAddsEntity) {
    sparseSet.insert(0);
    EXPECT_FALSE(sparseSet.empty());
    EXPECT_EQ(sparseSet.size(), 1u);
    EXPECT_TRUE(sparseSet.contains(0));
}

TEST_F(SparseSetBasicTest, InsertMultipleEntities) {
    sparseSet.insert(0);
    sparseSet.insert(5);
    sparseSet.insert(10);

    EXPECT_EQ(sparseSet.size(), 3u);
    EXPECT_TRUE(sparseSet.contains(0));
    EXPECT_TRUE(sparseSet.contains(5));
    EXPECT_TRUE(sparseSet.contains(10));
}

TEST_F(SparseSetBasicTest, ContainsReturnsFalseForMissing) {
    sparseSet.insert(0);
    EXPECT_FALSE(sparseSet.contains(1));
    EXPECT_FALSE(sparseSet.contains(100));
}

TEST_F(SparseSetBasicTest, EraseRemovesEntity) {
    sparseSet.insert(0);
    EXPECT_TRUE(sparseSet.contains(0));

    sparseSet.erase(0);
    EXPECT_FALSE(sparseSet.contains(0));
    EXPECT_EQ(sparseSet.size(), 0u);
    EXPECT_TRUE(sparseSet.empty());
}

TEST_F(SparseSetBasicTest, EraseDecrementsSize) {
    sparseSet.insert(0);
    sparseSet.insert(1);
    sparseSet.insert(2);
    EXPECT_EQ(sparseSet.size(), 3u);

    sparseSet.erase(1);
    EXPECT_EQ(sparseSet.size(), 2u);
}

TEST_F(SparseSetBasicTest, GetDenseReturnsAllEntities) {
    sparseSet.insert(5);
    sparseSet.insert(10);
    sparseSet.insert(15);

    const auto& dense = sparseSet.getDense();
    EXPECT_EQ(dense.size(), 3u);

    // All entities should be in the dense array
    EXPECT_NE(std::find(dense.begin(), dense.end(), 5), dense.end());
    EXPECT_NE(std::find(dense.begin(), dense.end(), 10), dense.end());
    EXPECT_NE(std::find(dense.begin(), dense.end(), 15), dense.end());
}

// =============================================================================
// SparseSet Edge Case Tests
// =============================================================================

class SparseSetEdgeCaseTest : public ::testing::Test {
protected:
    SparseSet sparseSet;
};

TEST_F(SparseSetEdgeCaseTest, DuplicateInsertWarns) {
    sparseSet.insert(0);
    EXPECT_EQ(sparseSet.size(), 1u);

    // Second insert should be a no-op with warning
    sparseSet.insert(0);
    EXPECT_EQ(sparseSet.size(), 1u);
    EXPECT_TRUE(sparseSet.contains(0));
}

TEST_F(SparseSetEdgeCaseTest, EraseNonExistentWarns) {
    // Erasing non-existent entity should warn but not crash
    EXPECT_NO_THROW(sparseSet.erase(999));
    EXPECT_TRUE(sparseSet.empty());
}

TEST_F(SparseSetEdgeCaseTest, EraseFromMiddlePreservesOthers) {
    sparseSet.insert(0);
    sparseSet.insert(1);
    sparseSet.insert(2);
    sparseSet.insert(3);

    sparseSet.erase(1);

    EXPECT_TRUE(sparseSet.contains(0));
    EXPECT_FALSE(sparseSet.contains(1));
    EXPECT_TRUE(sparseSet.contains(2));
    EXPECT_TRUE(sparseSet.contains(3));
    EXPECT_EQ(sparseSet.size(), 3u);
}

TEST_F(SparseSetEdgeCaseTest, EraseLastEntity) {
    sparseSet.insert(0);
    sparseSet.insert(1);
    sparseSet.insert(2);

    sparseSet.erase(2);

    EXPECT_TRUE(sparseSet.contains(0));
    EXPECT_TRUE(sparseSet.contains(1));
    EXPECT_FALSE(sparseSet.contains(2));
    EXPECT_EQ(sparseSet.size(), 2u);
}

TEST_F(SparseSetEdgeCaseTest, IterationOrder) {
    // Insert in specific order
    sparseSet.insert(10);
    sparseSet.insert(5);
    sparseSet.insert(15);

    // Collect entities via iteration
    std::vector<Entity> entities;
    for (Entity entity : sparseSet) {
        entities.push_back(entity);
    }

    EXPECT_EQ(entities.size(), 3u);
    // All entities should be present
    EXPECT_NE(std::find(entities.begin(), entities.end(), 10), entities.end());
    EXPECT_NE(std::find(entities.begin(), entities.end(), 5), entities.end());
    EXPECT_NE(std::find(entities.begin(), entities.end(), 15), entities.end());
}

TEST_F(SparseSetEdgeCaseTest, IterationWithEmptySet) {
    int count = 0;
    for ([[maybe_unused]] Entity entity : sparseSet) {
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST_F(SparseSetEdgeCaseTest, LargeEntityIds) {
    Entity largeId = 100000;
    sparseSet.insert(largeId);

    EXPECT_TRUE(sparseSet.contains(largeId));
    EXPECT_EQ(sparseSet.size(), 1u);
}

// =============================================================================
// SparseSet Iterator Tests
// =============================================================================

class SparseSetIteratorTest : public ::testing::Test {
protected:
    SparseSet sparseSet;
};

TEST_F(SparseSetIteratorTest, BeginAndEndWork) {
    auto begin = sparseSet.begin();
    auto end = sparseSet.end();
    EXPECT_EQ(begin, end);
}

TEST_F(SparseSetIteratorTest, IterateOverEntities) {
    sparseSet.insert(1);
    sparseSet.insert(2);
    sparseSet.insert(3);

    int sum = 0;
    for (Entity entity : sparseSet) {
        sum += entity;
    }

    EXPECT_EQ(sum, 6);
}

TEST_F(SparseSetIteratorTest, IterateWithDenseAccessor) {
    sparseSet.insert(10);
    sparseSet.insert(20);
    sparseSet.insert(30);

    const auto& dense = sparseSet.getDense();
    int count = 0;
    for (size_t i = 0; i < dense.size(); ++i) {
        count++;
    }

    EXPECT_EQ(count, 3);
}

// =============================================================================
// SystemManager Registration Tests
// =============================================================================

class SystemManagerRegistrationTest : public ::testing::Test {
protected:
    SystemManager systemManager;
};

TEST_F(SystemManagerRegistrationTest, RegisterQuerySystemSucceeds) {
    auto system = systemManager.registerQuerySystem<MockQuerySystem>();

    EXPECT_NE(system, nullptr);
}

TEST_F(SystemManagerRegistrationTest, RegisterGroupSystemSucceeds) {
    auto system = systemManager.registerGroupSystem<MockGroupSystem>();

    EXPECT_NE(system, nullptr);
}

TEST_F(SystemManagerRegistrationTest, RegisterQuerySystemWithParameters) {
    auto system = systemManager.registerQuerySystem<ParameterizedQuerySystem>(42, "test");

    ASSERT_NE(system, nullptr);
    EXPECT_EQ(system->getValue(), 42);
    EXPECT_EQ(system->getName(), "test");
}

TEST_F(SystemManagerRegistrationTest, RegisterGroupSystemWithParameters) {
    auto system = systemManager.registerGroupSystem<ParameterizedGroupSystem>(100, "group");

    ASSERT_NE(system, nullptr);
    EXPECT_EQ(system->getValue(), 100);
    EXPECT_EQ(system->getName(), "group");
}

TEST_F(SystemManagerRegistrationTest, RegisterMultipleQuerySystems) {
    auto system1 = systemManager.registerQuerySystem<MockQuerySystem>();
    auto system2 = systemManager.registerQuerySystem<AnotherQuerySystem>();

    EXPECT_NE(system1, nullptr);
    EXPECT_NE(system2, nullptr);
    // Different types, so they are definitely different systems
    EXPECT_NE(static_cast<void*>(system1.get()), static_cast<void*>(system2.get()));
}

TEST_F(SystemManagerRegistrationTest, RegisterMultipleGroupSystems) {
    auto system1 = systemManager.registerGroupSystem<MockGroupSystem>();
    auto system2 = systemManager.registerGroupSystem<AnotherGroupSystem>();

    EXPECT_NE(system1, nullptr);
    EXPECT_NE(system2, nullptr);
    // Different types, so they are definitely different systems
    EXPECT_NE(static_cast<void*>(system1.get()), static_cast<void*>(system2.get()));
}

// =============================================================================
// SystemManager Duplicate Registration Tests
// =============================================================================

class SystemManagerDuplicateTest : public ::testing::Test {
protected:
    SystemManager systemManager;
};

TEST_F(SystemManagerDuplicateTest, DuplicateQuerySystemReturnsNull) {
    auto first = systemManager.registerQuerySystem<MockQuerySystem>();
    EXPECT_NE(first, nullptr);

    auto second = systemManager.registerQuerySystem<MockQuerySystem>();
    EXPECT_EQ(second, nullptr);
}

TEST_F(SystemManagerDuplicateTest, DuplicateGroupSystemReturnsNull) {
    auto first = systemManager.registerGroupSystem<MockGroupSystem>();
    EXPECT_NE(first, nullptr);

    auto second = systemManager.registerGroupSystem<MockGroupSystem>();
    EXPECT_EQ(second, nullptr);
}

TEST_F(SystemManagerDuplicateTest, DuplicateRegistrationPreservesOriginal) {
    auto original = systemManager.registerQuerySystem<ParameterizedQuerySystem>(42, "original");
    ASSERT_NE(original, nullptr);

    auto duplicate = systemManager.registerQuerySystem<ParameterizedQuerySystem>(100, "duplicate");
    EXPECT_EQ(duplicate, nullptr);

    // Original should still be accessible and unchanged
    EXPECT_EQ(original->getValue(), 42);
    EXPECT_EQ(original->getName(), "original");
}

// =============================================================================
// SystemManager Signature Tests
// =============================================================================

class SystemManagerSignatureTest : public ::testing::Test {
protected:
    SystemManager systemManager;

    void SetUp() override {
        systemManager.registerQuerySystem<MockQuerySystem>();
        systemManager.registerQuerySystem<AnotherQuerySystem>();
    }
};

TEST_F(SystemManagerSignatureTest, SetSignatureSucceeds) {
    Signature sig;
    sig.set(0);
    sig.set(1);

    EXPECT_NO_THROW(systemManager.setSignature<MockQuerySystem>(sig));
}

TEST_F(SystemManagerSignatureTest, SetDifferentSignaturesForDifferentSystems) {
    Signature sig1;
    sig1.set(0);

    Signature sig2;
    sig2.set(1);
    sig2.set(2);

    EXPECT_NO_THROW(systemManager.setSignature<MockQuerySystem>(sig1));
    EXPECT_NO_THROW(systemManager.setSignature<AnotherQuerySystem>(sig2));
}

TEST_F(SystemManagerSignatureTest, SetEmptySignature) {
    Signature emptySig;
    EXPECT_NO_THROW(systemManager.setSignature<MockQuerySystem>(emptySig));
}

TEST_F(SystemManagerSignatureTest, SetFullSignature) {
    Signature fullSig;
    fullSig.set(); // Sets all bits

    EXPECT_NO_THROW(systemManager.setSignature<MockQuerySystem>(fullSig));
}

// =============================================================================
// SystemManager EntitySignatureChanged Tests
// =============================================================================

class SystemManagerSignatureChangeTest : public ::testing::Test {
protected:
    SystemManager systemManager;
    std::shared_ptr<MockQuerySystem> system1;
    std::shared_ptr<AnotherQuerySystem> system2;

    void SetUp() override {
        Signature sig1;
        sig1.set(0); // Requires component type 0

        Signature sig2;
        sig2.set(1); // Requires component type 1
        sig2.set(2); // Requires component type 2

        system1 = systemManager.registerQuerySystem<MockQuerySystem>(sig1);
        system2 = systemManager.registerQuerySystem<AnotherQuerySystem>(sig2);

        systemManager.setSignature<MockQuerySystem>(sig1);
        systemManager.setSignature<AnotherQuerySystem>(sig2);
    }
};

TEST_F(SystemManagerSignatureChangeTest, EntityAddedToMatchingSystem) {
    Entity entity = 0;
    Signature oldSig; // No components
    Signature newSig;
    newSig.set(0); // Now has component type 0

    systemManager.entitySignatureChanged(entity, oldSig, newSig);

    EXPECT_TRUE(system1->entities.contains(entity));
    EXPECT_FALSE(system2->entities.contains(entity));
}

TEST_F(SystemManagerSignatureChangeTest, EntityRemovedFromSystem) {
    Entity entity = 0;
    Signature oldSig;
    oldSig.set(0); // Had component type 0
    Signature newSig; // Component removed

    // First add entity to system
    systemManager.entitySignatureChanged(entity, Signature{}, oldSig);
    EXPECT_TRUE(system1->entities.contains(entity));

    // Now remove component
    systemManager.entitySignatureChanged(entity, oldSig, newSig);
    EXPECT_FALSE(system1->entities.contains(entity));
}

TEST_F(SystemManagerSignatureChangeTest, EntityMovedBetweenSystems) {
    Entity entity = 0;
    Signature sig1;
    sig1.set(0); // Component type 0

    Signature sig2;
    sig2.set(1); // Component type 1
    sig2.set(2); // Component type 2

    // Add to system1
    systemManager.entitySignatureChanged(entity, Signature{}, sig1);
    EXPECT_TRUE(system1->entities.contains(entity));
    EXPECT_FALSE(system2->entities.contains(entity));

    // Move to system2
    systemManager.entitySignatureChanged(entity, sig1, sig2);
    EXPECT_FALSE(system1->entities.contains(entity));
    EXPECT_TRUE(system2->entities.contains(entity));
}

TEST_F(SystemManagerSignatureChangeTest, EntityAddedToMultipleSystems) {
    Entity entity = 0;
    Signature oldSig;
    Signature newSig;
    newSig.set(0); // Component type 0
    newSig.set(1); // Component type 1
    newSig.set(2); // Component type 2

    systemManager.entitySignatureChanged(entity, oldSig, newSig);

    // Should match both systems (system1 requires 0, system2 requires 1&2)
    EXPECT_TRUE(system1->entities.contains(entity));
    EXPECT_TRUE(system2->entities.contains(entity));
}

TEST_F(SystemManagerSignatureChangeTest, PartialSignatureMatch) {
    Entity entity = 0;
    Signature sig;
    sig.set(1); // Only has component type 1 (system2 requires 1 AND 2)

    systemManager.entitySignatureChanged(entity, Signature{}, sig);

    EXPECT_FALSE(system1->entities.contains(entity)); // Doesn't have 0
    EXPECT_FALSE(system2->entities.contains(entity)); // Missing component 2
}

TEST_F(SystemManagerSignatureChangeTest, NoChangeInSignature) {
    Entity entity = 0;
    Signature sig;
    sig.set(0);

    // Add entity
    systemManager.entitySignatureChanged(entity, Signature{}, sig);
    EXPECT_TRUE(system1->entities.contains(entity));

    // "Change" with same signature - should remain
    systemManager.entitySignatureChanged(entity, sig, sig);
    EXPECT_TRUE(system1->entities.contains(entity));
}

TEST_F(SystemManagerSignatureChangeTest, MultipleEntitiesIndependent) {
    Entity entity1 = 0;
    Entity entity2 = 1;

    Signature sig1;
    sig1.set(0);

    Signature sig2;
    sig2.set(1);
    sig2.set(2);

    systemManager.entitySignatureChanged(entity1, Signature{}, sig1);
    systemManager.entitySignatureChanged(entity2, Signature{}, sig2);

    EXPECT_TRUE(system1->entities.contains(entity1));
    EXPECT_FALSE(system1->entities.contains(entity2));
    EXPECT_FALSE(system2->entities.contains(entity1));
    EXPECT_TRUE(system2->entities.contains(entity2));
}

// =============================================================================
// SystemManager EntityDestroyed Tests
// =============================================================================

class SystemManagerEntityDestroyedTest : public ::testing::Test {
protected:
    SystemManager systemManager;
    std::shared_ptr<MockQuerySystem> system1;
    std::shared_ptr<AnotherQuerySystem> system2;

    void SetUp() override {
        Signature sig1;
        sig1.set(0);

        Signature sig2;
        sig2.set(1);

        system1 = systemManager.registerQuerySystem<MockQuerySystem>(sig1);
        system2 = systemManager.registerQuerySystem<AnotherQuerySystem>(sig2);

        systemManager.setSignature<MockQuerySystem>(sig1);
        systemManager.setSignature<AnotherQuerySystem>(sig2);
    }
};

TEST_F(SystemManagerEntityDestroyedTest, RemovesEntityFromMatchingSystem) {
    Entity entity = 0;
    Signature sig;
    sig.set(0);

    // Add entity to system1
    systemManager.entitySignatureChanged(entity, Signature{}, sig);
    EXPECT_TRUE(system1->entities.contains(entity));

    // Destroy entity
    systemManager.entityDestroyed(entity, sig);
    EXPECT_FALSE(system1->entities.contains(entity));
}

TEST_F(SystemManagerEntityDestroyedTest, RemovesFromMultipleSystems) {
    Entity entity = 0;
    Signature sig;
    sig.set(0);
    sig.set(1);

    // Add entity to both systems
    systemManager.entitySignatureChanged(entity, Signature{}, sig);
    EXPECT_TRUE(system1->entities.contains(entity));
    EXPECT_TRUE(system2->entities.contains(entity));

    // Destroy entity
    systemManager.entityDestroyed(entity, sig);
    EXPECT_FALSE(system1->entities.contains(entity));
    EXPECT_FALSE(system2->entities.contains(entity));
}

TEST_F(SystemManagerEntityDestroyedTest, DoesNotAffectOtherEntities) {
    Entity entity1 = 0;
    Entity entity2 = 1;

    Signature sig;
    sig.set(0);

    // Add both entities
    systemManager.entitySignatureChanged(entity1, Signature{}, sig);
    systemManager.entitySignatureChanged(entity2, Signature{}, sig);

    EXPECT_TRUE(system1->entities.contains(entity1));
    EXPECT_TRUE(system1->entities.contains(entity2));

    // Destroy only entity1
    systemManager.entityDestroyed(entity1, sig);

    EXPECT_FALSE(system1->entities.contains(entity1));
    EXPECT_TRUE(system1->entities.contains(entity2)); // entity2 still present
}

TEST_F(SystemManagerEntityDestroyedTest, DestroyNonExistentEntity) {
    Entity entity = 999;
    Signature sig;
    sig.set(0);

    // Should not crash when destroying entity not in any system
    EXPECT_NO_THROW(systemManager.entityDestroyed(entity, sig));
}

TEST_F(SystemManagerEntityDestroyedTest, DestroyEntityWithEmptySignature) {
    Entity entity = 0;
    Signature emptySig;

    // Entity not in any system
    EXPECT_NO_THROW(systemManager.entityDestroyed(entity, emptySig));
}

TEST_F(SystemManagerEntityDestroyedTest, DestroyOnlyFromMatchingSystems) {
    Entity entity = 0;
    Signature sig;
    sig.set(0); // Only matches system1

    // Add to system1
    systemManager.entitySignatureChanged(entity, Signature{}, sig);
    EXPECT_TRUE(system1->entities.contains(entity));
    EXPECT_FALSE(system2->entities.contains(entity));

    // Destroy - should only affect system1
    systemManager.entityDestroyed(entity, sig);
    EXPECT_FALSE(system1->entities.contains(entity));
}

// =============================================================================
// SystemManager Complex Scenario Tests
// =============================================================================

class SystemManagerComplexTest : public ::testing::Test {
protected:
    SystemManager systemManager;
    std::shared_ptr<MockQuerySystem> renderSystem;
    std::shared_ptr<AnotherQuerySystem> physicsSystem;
    std::shared_ptr<MockGroupSystem> groupSystem;

    void SetUp() override {
        // RenderSystem requires Transform(0) and Mesh(1)
        Signature renderSig;
        renderSig.set(0);
        renderSig.set(1);

        // PhysicsSystem requires Transform(0) and RigidBody(2)
        Signature physicsSig;
        physicsSig.set(0);
        physicsSig.set(2);

        renderSystem = systemManager.registerQuerySystem<MockQuerySystem>(renderSig);
        physicsSystem = systemManager.registerQuerySystem<AnotherQuerySystem>(physicsSig);
        groupSystem = systemManager.registerGroupSystem<MockGroupSystem>();

        systemManager.setSignature<MockQuerySystem>(renderSig);
        systemManager.setSignature<AnotherQuerySystem>(physicsSig);
    }
};

TEST_F(SystemManagerComplexTest, EntityLifecycleAcrossSystems) {
    Entity entity = 0;
    Signature emptySig;

    // Start with just Transform
    Signature withTransform;
    withTransform.set(0);
    systemManager.entitySignatureChanged(entity, emptySig, withTransform);
    EXPECT_FALSE(renderSystem->entities.contains(entity));
    EXPECT_FALSE(physicsSystem->entities.contains(entity));

    // Add Mesh - now in render system
    Signature withMesh = withTransform;
    withMesh.set(1);
    systemManager.entitySignatureChanged(entity, withTransform, withMesh);
    EXPECT_TRUE(renderSystem->entities.contains(entity));
    EXPECT_FALSE(physicsSystem->entities.contains(entity));

    // Add RigidBody - now in both systems
    Signature withRigidBody = withMesh;
    withRigidBody.set(2);
    systemManager.entitySignatureChanged(entity, withMesh, withRigidBody);
    EXPECT_TRUE(renderSystem->entities.contains(entity));
    EXPECT_TRUE(physicsSystem->entities.contains(entity));

    // Remove Mesh - only in physics now
    Signature withoutMesh = withRigidBody;
    withoutMesh.reset(1);
    systemManager.entitySignatureChanged(entity, withRigidBody, withoutMesh);
    EXPECT_FALSE(renderSystem->entities.contains(entity));
    EXPECT_TRUE(physicsSystem->entities.contains(entity));

    // Destroy entity
    systemManager.entityDestroyed(entity, withoutMesh);
    EXPECT_FALSE(renderSystem->entities.contains(entity));
    EXPECT_FALSE(physicsSystem->entities.contains(entity));
}

TEST_F(SystemManagerComplexTest, MultipleEntitiesWithDifferentSignatures) {
    Entity staticMesh = 0;  // Transform + Mesh
    Entity dynamicObject = 1;  // Transform + Mesh + RigidBody
    Entity trigger = 2;  // Transform + RigidBody

    Signature staticSig;
    staticSig.set(0);
    staticSig.set(1);

    Signature dynamicSig;
    dynamicSig.set(0);
    dynamicSig.set(1);
    dynamicSig.set(2);

    Signature triggerSig;
    triggerSig.set(0);
    triggerSig.set(2);

    systemManager.entitySignatureChanged(staticMesh, Signature{}, staticSig);
    systemManager.entitySignatureChanged(dynamicObject, Signature{}, dynamicSig);
    systemManager.entitySignatureChanged(trigger, Signature{}, triggerSig);

    // Verify render system
    EXPECT_TRUE(renderSystem->entities.contains(staticMesh));
    EXPECT_TRUE(renderSystem->entities.contains(dynamicObject));
    EXPECT_FALSE(renderSystem->entities.contains(trigger));
    EXPECT_EQ(renderSystem->entities.size(), 2u);

    // Verify physics system
    EXPECT_FALSE(physicsSystem->entities.contains(staticMesh));
    EXPECT_TRUE(physicsSystem->entities.contains(dynamicObject));
    EXPECT_TRUE(physicsSystem->entities.contains(trigger));
    EXPECT_EQ(physicsSystem->entities.size(), 2u);
}

TEST_F(SystemManagerComplexTest, BulkEntityOperations) {
    const size_t entityCount = 100;
    std::vector<Entity> entities(entityCount);

    Signature renderSig;
    renderSig.set(0);
    renderSig.set(1);

    // Add many entities
    for (size_t i = 0; i < entityCount; ++i) {
        entities[i] = static_cast<Entity>(i);
        systemManager.entitySignatureChanged(entities[i], Signature{}, renderSig);
    }

    EXPECT_EQ(renderSystem->entities.size(), entityCount);

    // Remove half of them
    for (size_t i = 0; i < entityCount / 2; ++i) {
        systemManager.entityDestroyed(entities[i], renderSig);
    }

    EXPECT_EQ(renderSystem->entities.size(), entityCount / 2);
}

}  // namespace nexo::ecs
