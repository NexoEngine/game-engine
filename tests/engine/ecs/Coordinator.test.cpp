//// Coordinator.test.cpp /////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for ECS Coordinator edge cases
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/Coordinator.hpp"
#include "ecs/ECSExceptions.hpp"

namespace nexo::ecs {

// =============================================================================
// Test Component Types
// =============================================================================

struct Position {
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    bool operator==(const Position& other) const {
        return x == other.x && y == other.y && z == other.z;
    }
};

struct Velocity {
    float dx = 0.0f;
    float dy = 0.0f;
    float dz = 0.0f;

    bool operator==(const Velocity& other) const {
        return dx == other.dx && dy == other.dy && dz == other.dz;
    }
};

struct Health {
    int current = 100;
    int maximum = 100;

    bool operator==(const Health& other) const {
        return current == other.current && maximum == other.maximum;
    }
};

struct Tag {
    std::string name;

    bool operator==(const Tag& other) const {
        return name == other.name;
    }
};

// Component with memento pattern support
struct Transform {
    float x = 0.0f;
    float y = 0.0f;
    float rotation = 0.0f;

    struct Memento {
        float x;
        float y;
        float rotation;
    };

    Memento save() const {
        return {x, y, rotation};
    }

    void restore(const Memento& m) {
        x = m.x;
        y = m.y;
        rotation = m.rotation;
    }

    bool operator==(const Transform& other) const {
        return x == other.x && y == other.y && rotation == other.rotation;
    }
};

// =============================================================================
// Coordinator Initialization Tests
// =============================================================================

class CoordinatorInitTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
    }
};

TEST_F(CoordinatorInitTest, InitSucceeds) {
    EXPECT_NO_THROW(coordinator.createEntity());
}

TEST_F(CoordinatorInitTest, MultipleInitCallsSafe) {
    // Re-initializing should work without crashing
    EXPECT_NO_THROW(coordinator.init());
    EXPECT_NO_THROW(coordinator.createEntity());
}

// =============================================================================
// Entity Duplication Tests
// =============================================================================

class CoordinatorDuplicateEntityTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
        coordinator.registerComponent<Position>();
        coordinator.registerComponent<Velocity>();
        coordinator.registerComponent<Health>();
        coordinator.registerComponent<Tag>();
    }
};

TEST_F(CoordinatorDuplicateEntityTest, DuplicateEntityWithNoComponents) {
    // Create entity with no components
    Entity source = coordinator.createEntity();

    // Duplicate should succeed even with no components
    Entity duplicate = coordinator.duplicateEntity(source);

    EXPECT_NE(source, duplicate);

    // Verify both entities exist with empty signatures
    Signature sourceSignature = coordinator.getSignature(source);
    Signature duplicateSignature = coordinator.getSignature(duplicate);

    EXPECT_EQ(sourceSignature.count(), 0u);
    EXPECT_EQ(duplicateSignature.count(), 0u);
}

TEST_F(CoordinatorDuplicateEntityTest, DuplicateEntityWithSingleComponent) {
    Entity source = coordinator.createEntity();
    coordinator.addComponent<Position>(source, {1.0f, 2.0f, 3.0f});

    Entity duplicate = coordinator.duplicateEntity(source);

    EXPECT_NE(source, duplicate);
    EXPECT_TRUE(coordinator.entityHasComponent<Position>(duplicate));

    Position& duplicatePos = coordinator.getComponent<Position>(duplicate);
    EXPECT_FLOAT_EQ(duplicatePos.x, 1.0f);
    EXPECT_FLOAT_EQ(duplicatePos.y, 2.0f);
    EXPECT_FLOAT_EQ(duplicatePos.z, 3.0f);
}

TEST_F(CoordinatorDuplicateEntityTest, DuplicateEntityWithMultipleComponents) {
    Entity source = coordinator.createEntity();
    coordinator.addComponent<Position>(source, {10.0f, 20.0f, 30.0f});
    coordinator.addComponent<Velocity>(source, {1.0f, 2.0f, 3.0f});
    coordinator.addComponent<Health>(source, {50, 100});

    Entity duplicate = coordinator.duplicateEntity(source);

    EXPECT_NE(source, duplicate);

    // Verify all components are duplicated
    EXPECT_TRUE(coordinator.entityHasComponent<Position>(duplicate));
    EXPECT_TRUE(coordinator.entityHasComponent<Velocity>(duplicate));
    EXPECT_TRUE(coordinator.entityHasComponent<Health>(duplicate));

    // Verify component values
    Position& pos = coordinator.getComponent<Position>(duplicate);
    EXPECT_EQ(pos, Position(10.0f, 20.0f, 30.0f));

    Velocity& vel = coordinator.getComponent<Velocity>(duplicate);
    EXPECT_EQ(vel, Velocity(1.0f, 2.0f, 3.0f));

    Health& health = coordinator.getComponent<Health>(duplicate);
    EXPECT_EQ(health, Health(50, 100));
}

TEST_F(CoordinatorDuplicateEntityTest, DuplicateEntitySignaturePropagation) {
    Entity source = coordinator.createEntity();
    coordinator.addComponent<Position>(source, {1.0f, 2.0f, 3.0f});
    coordinator.addComponent<Velocity>(source, {4.0f, 5.0f, 6.0f});

    Signature sourceSignature = coordinator.getSignature(source);

    Entity duplicate = coordinator.duplicateEntity(source);
    Signature duplicateSignature = coordinator.getSignature(duplicate);

    // Signatures should be identical
    EXPECT_EQ(sourceSignature, duplicateSignature);
    EXPECT_EQ(sourceSignature.count(), duplicateSignature.count());
}

TEST_F(CoordinatorDuplicateEntityTest, DuplicatedEntityIsIndependent) {
    Entity source = coordinator.createEntity();
    coordinator.addComponent<Position>(source, {1.0f, 2.0f, 3.0f});

    Entity duplicate = coordinator.duplicateEntity(source);

    // Modify source
    coordinator.getComponent<Position>(source).x = 100.0f;

    // Duplicate should remain unchanged
    EXPECT_FLOAT_EQ(coordinator.getComponent<Position>(duplicate).x, 1.0f);
}

TEST_F(CoordinatorDuplicateEntityTest, DuplicateMultipleTimes) {
    Entity source = coordinator.createEntity();
    coordinator.addComponent<Position>(source, {1.0f, 2.0f, 3.0f});

    Entity dup1 = coordinator.duplicateEntity(source);
    Entity dup2 = coordinator.duplicateEntity(source);
    Entity dup3 = coordinator.duplicateEntity(source);

    // All should be unique
    EXPECT_NE(dup1, dup2);
    EXPECT_NE(dup2, dup3);
    EXPECT_NE(dup1, dup3);

    // All should have the component
    EXPECT_TRUE(coordinator.entityHasComponent<Position>(dup1));
    EXPECT_TRUE(coordinator.entityHasComponent<Position>(dup2));
    EXPECT_TRUE(coordinator.entityHasComponent<Position>(dup3));
}

// =============================================================================
// getAllComponentTypes and getAllComponentTypeIndices Tests
// =============================================================================

class CoordinatorGetAllComponentTypesTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
        coordinator.registerComponent<Position>();
        coordinator.registerComponent<Velocity>();
        coordinator.registerComponent<Health>();
        coordinator.registerComponent<Tag>();
    }
};

TEST_F(CoordinatorGetAllComponentTypesTest, EntityWithNoComponents) {
    Entity entity = coordinator.createEntity();

    std::vector<ComponentType> types = coordinator.getAllComponentTypes(entity);

    EXPECT_TRUE(types.empty());
}

TEST_F(CoordinatorGetAllComponentTypesTest, EntityWithSingleComponent) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {});

    std::vector<ComponentType> types = coordinator.getAllComponentTypes(entity);

    EXPECT_EQ(types.size(), 1u);
    EXPECT_EQ(types[0], coordinator.getComponentType<Position>());
}

TEST_F(CoordinatorGetAllComponentTypesTest, EntityWithMultipleComponents) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {});
    coordinator.addComponent<Velocity>(entity, {});
    coordinator.addComponent<Health>(entity, {});

    std::vector<ComponentType> types = coordinator.getAllComponentTypes(entity);

    EXPECT_EQ(types.size(), 3u);

    // Verify all component types are present
    ComponentType posType = coordinator.getComponentType<Position>();
    ComponentType velType = coordinator.getComponentType<Velocity>();
    ComponentType healthType = coordinator.getComponentType<Health>();

    EXPECT_NE(std::find(types.begin(), types.end(), posType), types.end());
    EXPECT_NE(std::find(types.begin(), types.end(), velType), types.end());
    EXPECT_NE(std::find(types.begin(), types.end(), healthType), types.end());
}

TEST_F(CoordinatorGetAllComponentTypesTest, GetAllComponentTypeIndices) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {});
    coordinator.addComponent<Velocity>(entity, {});

    std::vector<std::type_index> typeIndices = coordinator.getAllComponentTypeIndices(entity);

    EXPECT_EQ(typeIndices.size(), 2u);

    // Verify type indices match
    bool foundPosition = false;
    bool foundVelocity = false;

    for (const auto& typeIdx : typeIndices) {
        if (typeIdx == typeid(Position)) foundPosition = true;
        if (typeIdx == typeid(Velocity)) foundVelocity = true;
    }

    EXPECT_TRUE(foundPosition);
    EXPECT_TRUE(foundVelocity);
}

TEST_F(CoordinatorGetAllComponentTypesTest, GetAllComponentTypeIndicesEmpty) {
    Entity entity = coordinator.createEntity();

    std::vector<std::type_index> typeIndices = coordinator.getAllComponentTypeIndices(entity);

    EXPECT_TRUE(typeIndices.empty());
}

TEST_F(CoordinatorGetAllComponentTypesTest, ComponentTypesAfterRemoval) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {});
    coordinator.addComponent<Velocity>(entity, {});
    coordinator.addComponent<Health>(entity, {});

    EXPECT_EQ(coordinator.getAllComponentTypes(entity).size(), 3u);

    coordinator.removeComponent<Velocity>(entity);

    std::vector<ComponentType> types = coordinator.getAllComponentTypes(entity);
    EXPECT_EQ(types.size(), 2u);

    ComponentType velType = coordinator.getComponentType<Velocity>();
    EXPECT_EQ(std::find(types.begin(), types.end(), velType), types.end());
}

// =============================================================================
// Entity ID Reuse Tests
// =============================================================================

class CoordinatorEntityIdReuseTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
        coordinator.registerComponent<Position>();
        coordinator.registerComponent<Velocity>();
    }
};

TEST_F(CoordinatorEntityIdReuseTest, DestroyedEntityIdIsReused) {
    Entity e1 = coordinator.createEntity();
    Entity firstId = e1;

    coordinator.destroyEntity(e1);

    Entity e2 = coordinator.createEntity();

    // The destroyed ID should be reused
    EXPECT_EQ(e2, firstId);
}

TEST_F(CoordinatorEntityIdReuseTest, MultipleDestroyedIdsReusedInLIFOOrder) {
    // Create and destroy several entities
    Entity e1 = coordinator.createEntity();
    Entity e2 = coordinator.createEntity();
    Entity e3 = coordinator.createEntity();

    coordinator.destroyEntity(e1);
    coordinator.destroyEntity(e2);
    coordinator.destroyEntity(e3);

    // IDs should be reused in LIFO order (stack-like: e3, e2, e1)
    Entity new1 = coordinator.createEntity();
    Entity new2 = coordinator.createEntity();
    Entity new3 = coordinator.createEntity();

    EXPECT_EQ(new1, e3);
    EXPECT_EQ(new2, e2);
    EXPECT_EQ(new3, e1);
}

TEST_F(CoordinatorEntityIdReuseTest, ReusedEntityHasCleanSignature) {
    Entity e1 = coordinator.createEntity();
    coordinator.addComponent<Position>(e1, {1.0f, 2.0f, 3.0f});
    coordinator.addComponent<Velocity>(e1, {4.0f, 5.0f, 6.0f});

    Signature oldSignature = coordinator.getSignature(e1);
    EXPECT_GT(oldSignature.count(), 0u);

    coordinator.destroyEntity(e1);

    Entity e2 = coordinator.createEntity();
    EXPECT_EQ(e1, e2); // Same ID reused

    Signature newSignature = coordinator.getSignature(e2);
    EXPECT_EQ(newSignature.count(), 0u);
    EXPECT_NE(newSignature, oldSignature);
}

TEST_F(CoordinatorEntityIdReuseTest, ReusedEntityCanHaveNewComponents) {
    Entity e1 = coordinator.createEntity();
    coordinator.addComponent<Position>(e1, {1.0f, 2.0f, 3.0f});
    coordinator.destroyEntity(e1);

    Entity e2 = coordinator.createEntity();
    EXPECT_EQ(e1, e2);

    // Should be able to add different components
    EXPECT_NO_THROW(coordinator.addComponent<Velocity>(e2, {10.0f, 20.0f, 30.0f}));

    EXPECT_TRUE(coordinator.entityHasComponent<Velocity>(e2));
    EXPECT_FALSE(coordinator.entityHasComponent<Position>(e2));
}

TEST_F(CoordinatorEntityIdReuseTest, PartialDestructionWithReuse) {
    std::vector<Entity> entities;
    for (int i = 0; i < 10; ++i) {
        entities.push_back(coordinator.createEntity());
    }

    // Destroy every other entity
    for (size_t i = 0; i < entities.size(); i += 2) {
        coordinator.destroyEntity(entities[i]);
    }

    // Create new entities - should reuse destroyed IDs
    for (int i = 0; i < 5; ++i) {
        Entity newEntity = coordinator.createEntity();
        // Should be one of the destroyed IDs
        bool isReused = false;
        for (size_t j = 0; j < entities.size(); j += 2) {
            if (newEntity == entities[j]) {
                isReused = true;
                break;
            }
        }
        EXPECT_TRUE(isReused);
    }
}

// =============================================================================
// Signature Edge Case Tests
// =============================================================================

class CoordinatorSignatureEdgeCaseTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
        coordinator.registerComponent<Position>();
        coordinator.registerComponent<Velocity>();
        coordinator.registerComponent<Health>();
    }
};

TEST_F(CoordinatorSignatureEdgeCaseTest, GetSignatureForNewEntity) {
    Entity entity = coordinator.createEntity();

    Signature sig = coordinator.getSignature(entity);

    EXPECT_EQ(sig.count(), 0u);
    EXPECT_FALSE(sig.any());
}

TEST_F(CoordinatorSignatureEdgeCaseTest, GetSignatureAfterAddingComponents) {
    Entity entity = coordinator.createEntity();

    coordinator.addComponent<Position>(entity, {});
    Signature sig1 = coordinator.getSignature(entity);
    EXPECT_EQ(sig1.count(), 1u);

    coordinator.addComponent<Velocity>(entity, {});
    Signature sig2 = coordinator.getSignature(entity);
    EXPECT_EQ(sig2.count(), 2u);

    coordinator.addComponent<Health>(entity, {});
    Signature sig3 = coordinator.getSignature(entity);
    EXPECT_EQ(sig3.count(), 3u);
}

TEST_F(CoordinatorSignatureEdgeCaseTest, GetSignatureAfterRemovingComponents) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {});
    coordinator.addComponent<Velocity>(entity, {});
    coordinator.addComponent<Health>(entity, {});

    EXPECT_EQ(coordinator.getSignature(entity).count(), 3u);

    coordinator.removeComponent<Velocity>(entity);
    EXPECT_EQ(coordinator.getSignature(entity).count(), 2u);

    coordinator.removeComponent<Health>(entity);
    EXPECT_EQ(coordinator.getSignature(entity).count(), 1u);

    coordinator.removeComponent<Position>(entity);
    EXPECT_EQ(coordinator.getSignature(entity).count(), 0u);
}

TEST_F(CoordinatorSignatureEdgeCaseTest, SignatureConsistencyAcrossEntities) {
    Entity e1 = coordinator.createEntity();
    Entity e2 = coordinator.createEntity();
    Entity e3 = coordinator.createEntity();

    coordinator.addComponent<Position>(e1, {});
    coordinator.addComponent<Position>(e2, {});
    coordinator.addComponent<Velocity>(e3, {});

    Signature sig1 = coordinator.getSignature(e1);
    Signature sig2 = coordinator.getSignature(e2);
    Signature sig3 = coordinator.getSignature(e3);

    // e1 and e2 should have same signature
    EXPECT_EQ(sig1, sig2);

    // e3 should have different signature
    EXPECT_NE(sig1, sig3);
}

TEST_F(CoordinatorSignatureEdgeCaseTest, SignatureMatchesComponentPresence) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {});
    coordinator.addComponent<Velocity>(entity, {});

    Signature sig = coordinator.getSignature(entity);

    ComponentType posType = coordinator.getComponentType<Position>();
    ComponentType velType = coordinator.getComponentType<Velocity>();
    ComponentType healthType = coordinator.getComponentType<Health>();

    EXPECT_TRUE(sig.test(posType));
    EXPECT_TRUE(sig.test(velType));
    EXPECT_FALSE(sig.test(healthType));
}

// =============================================================================
// entityHasComponent Tests
// =============================================================================

class CoordinatorEntityHasComponentTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
        coordinator.registerComponent<Position>();
        coordinator.registerComponent<Velocity>();
        coordinator.registerComponent<Health>();
    }
};

TEST_F(CoordinatorEntityHasComponentTest, NewEntityHasNoComponents) {
    Entity entity = coordinator.createEntity();

    EXPECT_FALSE(coordinator.entityHasComponent<Position>(entity));
    EXPECT_FALSE(coordinator.entityHasComponent<Velocity>(entity));
    EXPECT_FALSE(coordinator.entityHasComponent<Health>(entity));
}

TEST_F(CoordinatorEntityHasComponentTest, EntityHasAddedComponent) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {});

    EXPECT_TRUE(coordinator.entityHasComponent<Position>(entity));
    EXPECT_FALSE(coordinator.entityHasComponent<Velocity>(entity));
}

TEST_F(CoordinatorEntityHasComponentTest, EntityDoesNotHaveRemovedComponent) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {});
    coordinator.addComponent<Velocity>(entity, {});

    EXPECT_TRUE(coordinator.entityHasComponent<Position>(entity));
    EXPECT_TRUE(coordinator.entityHasComponent<Velocity>(entity));

    coordinator.removeComponent<Position>(entity);

    EXPECT_FALSE(coordinator.entityHasComponent<Position>(entity));
    EXPECT_TRUE(coordinator.entityHasComponent<Velocity>(entity));
}

TEST_F(CoordinatorEntityHasComponentTest, EntityHasComponentByTypeId) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {});

    ComponentType posType = coordinator.getComponentType<Position>();

    EXPECT_TRUE(coordinator.entityHasComponent(entity, posType));
}

// =============================================================================
// getAllComponents Tests
// =============================================================================

class CoordinatorGetAllComponentsTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
        coordinator.registerComponent<Position>();
        coordinator.registerComponent<Velocity>();
        coordinator.registerComponent<Health>();
    }
};

TEST_F(CoordinatorGetAllComponentsTest, EntityWithNoComponentsReturnsEmpty) {
    Entity entity = coordinator.createEntity();

    std::vector<std::any> components = coordinator.getAllComponents(entity);

    EXPECT_TRUE(components.empty());
}

TEST_F(CoordinatorGetAllComponentsTest, EntityWithSingleComponentReturnsOne) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {1.0f, 2.0f, 3.0f});

    std::vector<std::any> components = coordinator.getAllComponents(entity);

    EXPECT_EQ(components.size(), 1u);
}

TEST_F(CoordinatorGetAllComponentsTest, EntityWithMultipleComponentsReturnsAll) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {1.0f, 2.0f, 3.0f});
    coordinator.addComponent<Velocity>(entity, {4.0f, 5.0f, 6.0f});
    coordinator.addComponent<Health>(entity, {50, 100});

    std::vector<std::any> components = coordinator.getAllComponents(entity);

    EXPECT_EQ(components.size(), 3u);
}

TEST_F(CoordinatorGetAllComponentsTest, ComponentsCanBeCast) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {10.0f, 20.0f, 30.0f});

    std::vector<std::any> components = coordinator.getAllComponents(entity);

    ASSERT_EQ(components.size(), 1u);

    // Should be able to cast back to Position
    EXPECT_NO_THROW({
        Position pos = std::any_cast<Position>(components[0]);
        EXPECT_FLOAT_EQ(pos.x, 10.0f);
    });
}

// =============================================================================
// Component Type Management Tests
// =============================================================================

class CoordinatorComponentTypeTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
    }
};

TEST_F(CoordinatorComponentTypeTest, RegisteredComponentsHaveUniqueTypes) {
    coordinator.registerComponent<Position>();
    coordinator.registerComponent<Velocity>();
    coordinator.registerComponent<Health>();

    ComponentType posType = coordinator.getComponentType<Position>();
    ComponentType velType = coordinator.getComponentType<Velocity>();
    ComponentType healthType = coordinator.getComponentType<Health>();

    EXPECT_NE(posType, velType);
    EXPECT_NE(velType, healthType);
    EXPECT_NE(posType, healthType);
}

TEST_F(CoordinatorComponentTypeTest, SameComponentTypeAcrossCalls) {
    coordinator.registerComponent<Position>();

    ComponentType type1 = coordinator.getComponentType<Position>();
    ComponentType type2 = coordinator.getComponentType<Position>();

    EXPECT_EQ(type1, type2);
}

// =============================================================================
// Memento Pattern Tests
// =============================================================================

class CoordinatorMementoTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
        coordinator.registerComponent<Transform>();
        coordinator.registerComponent<Position>();
    }
};

TEST_F(CoordinatorMementoTest, ComponentSupportsMementoPattern) {
    Transform transform{1.0f, 2.0f, 45.0f};
    std::any componentAny = transform;

    EXPECT_TRUE(coordinator.supportsMementoPattern(componentAny));
}

TEST_F(CoordinatorMementoTest, ComponentDoesNotSupportMementoPattern) {
    Position position{1.0f, 2.0f, 3.0f};
    std::any componentAny = position;

    EXPECT_FALSE(coordinator.supportsMementoPattern(componentAny));
}

TEST_F(CoordinatorMementoTest, SaveAndRestoreComponent) {
    Transform original{10.0f, 20.0f, 90.0f};
    std::any componentAny = original;

    // Save
    std::any mementoAny = coordinator.saveComponent(componentAny);
    EXPECT_TRUE(mementoAny.has_value());

    // Restore
    std::any restoredAny = coordinator.restoreComponent(mementoAny, typeid(Transform));
    EXPECT_TRUE(restoredAny.has_value());

    Transform restored = std::any_cast<Transform>(restoredAny);
    EXPECT_EQ(restored, original);
}

// =============================================================================
// Stress Tests
// =============================================================================

class CoordinatorStressTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
        coordinator.registerComponent<Position>();
        coordinator.registerComponent<Velocity>();
        coordinator.registerComponent<Health>();
        coordinator.registerComponent<Tag>();
    }
};

TEST_F(CoordinatorStressTest, CreateAndDestroyManyEntities) {
    constexpr int NUM_ITERATIONS = 1000;

    for (int i = 0; i < NUM_ITERATIONS; ++i) {
        Entity entity = coordinator.createEntity();
        coordinator.addComponent<Position>(entity, {});
        coordinator.destroyEntity(entity);
    }

    SUCCEED();
}

TEST_F(CoordinatorStressTest, DuplicateManyEntities) {
    Entity template_entity = coordinator.createEntity();
    coordinator.addComponent<Position>(template_entity, {1.0f, 2.0f, 3.0f});
    coordinator.addComponent<Velocity>(template_entity, {4.0f, 5.0f, 6.0f});

    std::vector<Entity> duplicates;
    for (int i = 0; i < 100; ++i) {
        duplicates.push_back(coordinator.duplicateEntity(template_entity));
    }

    // Verify all duplicates have components
    for (Entity dup : duplicates) {
        EXPECT_TRUE(coordinator.entityHasComponent<Position>(dup));
        EXPECT_TRUE(coordinator.entityHasComponent<Velocity>(dup));
    }
}

TEST_F(CoordinatorStressTest, GetAllComponentTypesOnManyEntities) {
    std::vector<Entity> entities;

    for (int i = 0; i < 100; ++i) {
        Entity e = coordinator.createEntity();
        coordinator.addComponent<Position>(e, {});
        coordinator.addComponent<Velocity>(e, {});
        entities.push_back(e);
    }

    for (Entity e : entities) {
        std::vector<ComponentType> types = coordinator.getAllComponentTypes(e);
        EXPECT_EQ(types.size(), 2u);
    }
}

// =============================================================================
// Edge Case Integration Tests
// =============================================================================

class CoordinatorIntegrationTest : public ::testing::Test {
protected:
    Coordinator coordinator;

    void SetUp() override {
        coordinator.init();
        coordinator.registerComponent<Position>();
        coordinator.registerComponent<Velocity>();
        coordinator.registerComponent<Health>();
    }
};

TEST_F(CoordinatorIntegrationTest, DestroyAndRecreateWithSameComponents) {
    Entity e1 = coordinator.createEntity();
    coordinator.addComponent<Position>(e1, {1.0f, 2.0f, 3.0f});

    Signature sig1 = coordinator.getSignature(e1);

    coordinator.destroyEntity(e1);

    Entity e2 = coordinator.createEntity();
    EXPECT_EQ(e1, e2);

    coordinator.addComponent<Position>(e2, {4.0f, 5.0f, 6.0f});

    Signature sig2 = coordinator.getSignature(e2);

    // Signatures should match (same component types)
    EXPECT_EQ(sig1, sig2);

    // But values should be different
    Position& pos = coordinator.getComponent<Position>(e2);
    EXPECT_FLOAT_EQ(pos.x, 4.0f);
}

TEST_F(CoordinatorIntegrationTest, DuplicateAfterModification) {
    Entity original = coordinator.createEntity();
    coordinator.addComponent<Position>(original, {1.0f, 2.0f, 3.0f});
    coordinator.addComponent<Health>(original, {100, 100});

    // Modify
    coordinator.getComponent<Health>(original).current = 50;

    // Duplicate should get modified values
    Entity duplicate = coordinator.duplicateEntity(original);

    Health& dupHealth = coordinator.getComponent<Health>(duplicate);
    EXPECT_EQ(dupHealth.current, 50);
    EXPECT_EQ(dupHealth.maximum, 100);
}

TEST_F(CoordinatorIntegrationTest, GetAllComponentsAfterPartialRemoval) {
    Entity entity = coordinator.createEntity();
    coordinator.addComponent<Position>(entity, {});
    coordinator.addComponent<Velocity>(entity, {});
    coordinator.addComponent<Health>(entity, {});

    EXPECT_EQ(coordinator.getAllComponents(entity).size(), 3u);

    coordinator.removeComponent<Velocity>(entity);

    EXPECT_EQ(coordinator.getAllComponents(entity).size(), 2u);
}

}  // namespace nexo::ecs
