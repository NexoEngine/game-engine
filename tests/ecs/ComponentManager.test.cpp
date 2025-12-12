//// ComponentManager.test.cpp ///////////////////////////////////////////////
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
//  Author:      Claude Code
//  Date:        13/12/2025
//  Description: Comprehensive edge case test file for ComponentManager class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "Components.hpp"
#include "ECSExceptions.hpp"
#include "Definitions.hpp"

namespace nexo::ecs {

    // Test component structures (unique names to avoid ODR violations with Components.test.cpp)
    struct EdgeTestComponentA {
        int value;
        bool operator==(const EdgeTestComponentA& other) const {
            return value == other.value;
        }
    };

    struct EdgeTestComponentB {
        float data;
        bool operator==(const EdgeTestComponentB& other) const {
            return data == other.data;
        }
    };

    struct EdgeTestComponentC {
        double x, y, z;
        bool operator==(const EdgeTestComponentC& other) const {
            return x == other.x && y == other.y && z == other.z;
        }
    };

    struct EdgeTestComponentD {
        char letter;
        bool operator==(const EdgeTestComponentD& other) const {
            return letter == other.letter;
        }
    };

    class ComponentManagerEdgeCaseTest : public ::testing::Test {
    protected:
        std::unique_ptr<ComponentManager> componentManager;

        void SetUp() override {
            componentManager = std::make_unique<ComponentManager>();
        }

        void TearDown() override {
            componentManager.reset();
        }
    };

    // =========================================================
    // ============== COMPONENT REGISTRATION ===================
    // =========================================================

    TEST_F(ComponentManagerEdgeCaseTest, RegisterComponentSucceeds) {
        EXPECT_NO_THROW(componentManager->registerComponent<EdgeTestComponentA>());
    }

    TEST_F(ComponentManagerEdgeCaseTest, RegisterSameComponentTwiceIsAllowed) {
        componentManager->registerComponent<EdgeTestComponentA>();
        // Should log a warning but not throw
        EXPECT_NO_THROW(componentManager->registerComponent<EdgeTestComponentA>());
    }

    TEST_F(ComponentManagerEdgeCaseTest, RegisterMultipleDifferentComponents) {
        EXPECT_NO_THROW(componentManager->registerComponent<EdgeTestComponentA>());
        EXPECT_NO_THROW(componentManager->registerComponent<EdgeTestComponentB>());
        EXPECT_NO_THROW(componentManager->registerComponent<EdgeTestComponentC>());
        EXPECT_NO_THROW(componentManager->registerComponent<EdgeTestComponentD>());
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetComponentTypeConsistency) {
        componentManager->registerComponent<EdgeTestComponentA>();

        ComponentType type1 = componentManager->getComponentType<EdgeTestComponentA>();
        ComponentType type2 = componentManager->getComponentType<EdgeTestComponentA>();

        EXPECT_EQ(type1, type2);
    }

    TEST_F(ComponentManagerEdgeCaseTest, DifferentComponentsHaveDifferentTypes) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        ComponentType typeA = componentManager->getComponentType<EdgeTestComponentA>();
        ComponentType typeB = componentManager->getComponentType<EdgeTestComponentB>();

        EXPECT_NE(typeA, typeB);
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetComponentTypeThrowsWhenNotRegistered) {
        EXPECT_THROW(
            componentManager->getComponentType<EdgeTestComponentA>(),
            ComponentNotRegistered
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, RegisterManyComponentTypes) {
        // Test that the system doesn't break with multiple registrations
        // Note: We can't register MAX_COMPONENT_TYPE because it's a global counter
        // shared across all tests, so we register a reasonable number instead
        for (int i = 0; i < 10; ++i) {
            // Register TypeErasedComponents to avoid template limit issues
            EXPECT_NO_THROW(componentManager->registerComponent(sizeof(int), 32));
        }
    }

    // =========================================================
    // ============== COMPONENT OPERATIONS =====================
    // =========================================================

    TEST_F(ComponentManagerEdgeCaseTest, AddComponentToEntity) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        EdgeTestComponentA component{42};
        Signature oldSig, newSig;
        newSig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        EXPECT_NO_THROW(componentManager->addComponent(entity, component, oldSig, newSig));
    }

    TEST_F(ComponentManagerEdgeCaseTest, AddMultipleComponentsToSameEntity) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        Entity entity = 1;
        EdgeTestComponentA compA{42};
        EdgeTestComponentB compB{3.14f};

        Signature sig;
        Signature oldSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);
        componentManager->addComponent(entity, compA, oldSig, sig);

        oldSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), true);
        componentManager->addComponent(entity, compB, oldSig, sig);

        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(entity).value, 42);
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentB>(entity).data, 3.14f);
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetComponentFromEntity) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 5;
        EdgeTestComponentA component{99};
        Signature oldSig, newSig;
        newSig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        componentManager->addComponent(entity, component, oldSig, newSig);

        EdgeTestComponentA& retrieved = componentManager->getComponent<EdgeTestComponentA>(entity);
        EXPECT_EQ(retrieved.value, 99);
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetComponentAllowsModification) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        EdgeTestComponentA component{42};
        Signature oldSig, newSig;
        newSig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        componentManager->addComponent(entity, component, oldSig, newSig);

        componentManager->getComponent<EdgeTestComponentA>(entity).value = 100;
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(entity).value, 100);
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetComponentThrowsForNonExistentComponent) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;

        EXPECT_THROW(
            componentManager->getComponent<EdgeTestComponentA>(entity),
            ComponentNotFound
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, TryGetComponentReturnsNulloptForNonExistent) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;

        auto result = componentManager->tryGetComponent<EdgeTestComponentA>(entity);
        EXPECT_FALSE(result.has_value());
    }

    TEST_F(ComponentManagerEdgeCaseTest, TryGetComponentReturnsValueWhenExists) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        EdgeTestComponentA component{42};
        Signature oldSig, newSig;
        newSig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        componentManager->addComponent(entity, component, oldSig, newSig);

        auto result = componentManager->tryGetComponent<EdgeTestComponentA>(entity);
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value().get().value, 42);
    }

    TEST_F(ComponentManagerEdgeCaseTest, RemoveComponentFromEntity) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        EdgeTestComponentA component{42};
        Signature oldSig, newSig;
        newSig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        componentManager->addComponent(entity, component, oldSig, newSig);

        Signature prevSig = newSig;
        newSig.set(componentManager->getComponentType<EdgeTestComponentA>(), false);

        EXPECT_NO_THROW(componentManager->removeComponent<EdgeTestComponentA>(entity, prevSig, newSig));
    }

    TEST_F(ComponentManagerEdgeCaseTest, RemoveComponentThrowsForNonExistent) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        Signature oldSig, newSig;

        EXPECT_THROW(
            componentManager->removeComponent<EdgeTestComponentA>(entity, oldSig, newSig),
            ComponentNotFound
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, TryRemoveComponentReturnsFalseForNonExistent) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        Signature oldSig, newSig;

        bool result = componentManager->tryRemoveComponent<EdgeTestComponentA>(entity, oldSig, newSig);
        EXPECT_FALSE(result);
    }

    TEST_F(ComponentManagerEdgeCaseTest, TryRemoveComponentReturnsTrueWhenExists) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        EdgeTestComponentA component{42};
        Signature oldSig, newSig;
        newSig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        componentManager->addComponent(entity, component, oldSig, newSig);

        Signature prevSig = newSig;
        newSig.set(componentManager->getComponentType<EdgeTestComponentA>(), false);

        bool result = componentManager->tryRemoveComponent<EdgeTestComponentA>(entity, prevSig, newSig);
        EXPECT_TRUE(result);
    }

    TEST_F(ComponentManagerEdgeCaseTest, RemoveOneComponentLeavesOthersIntact) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();
        componentManager->registerComponent<EdgeTestComponentC>();

        Entity entity = 1;
        EdgeTestComponentA compA{42};
        EdgeTestComponentB compB{3.14f};
        EdgeTestComponentC compC{1.0, 2.0, 3.0};

        Signature sig;
        Signature oldSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);
        componentManager->addComponent(entity, compA, oldSig, sig);

        oldSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), true);
        componentManager->addComponent(entity, compB, oldSig, sig);

        oldSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentC>(), true);
        componentManager->addComponent(entity, compC, oldSig, sig);

        // Remove component B
        Signature prevSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), false);
        componentManager->removeComponent<EdgeTestComponentB>(entity, prevSig, sig);

        // A and C should still exist
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(entity).value, 42);
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentC>(entity).x, 1.0);
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentC>(entity).y, 2.0);
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentC>(entity).z, 3.0);

        // B should not exist
        EXPECT_THROW(
            componentManager->getComponent<EdgeTestComponentB>(entity),
            ComponentNotFound
        );
    }

    // =========================================================
    // ============== ENTITY DESTRUCTION =======================
    // =========================================================

    TEST_F(ComponentManagerEdgeCaseTest, EntityDestroyedRemovesAllComponents) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        Entity entity = 1;
        EdgeTestComponentA compA{42};
        EdgeTestComponentB compB{3.14f};

        Signature sig;
        Signature oldSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);
        componentManager->addComponent(entity, compA, oldSig, sig);

        oldSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), true);
        componentManager->addComponent(entity, compB, oldSig, sig);

        // Destroy entity
        componentManager->entityDestroyed(entity, sig);

        // Both components should be removed
        EXPECT_THROW(
            componentManager->getComponent<EdgeTestComponentA>(entity),
            ComponentNotFound
        );
        EXPECT_THROW(
            componentManager->getComponent<EdgeTestComponentB>(entity),
            ComponentNotFound
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, EntityDestroyedWithNoComponents) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        Signature sig;

        // Should not throw
        EXPECT_NO_THROW(componentManager->entityDestroyed(entity, sig));
    }

    TEST_F(ComponentManagerEdgeCaseTest, DestroyMultipleEntities) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity1 = 1;
        Entity entity2 = 2;
        Entity entity3 = 3;

        EdgeTestComponentA comp1{10};
        EdgeTestComponentA comp2{20};
        EdgeTestComponentA comp3{30};

        Signature sig;
        Signature oldSig;
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        componentManager->addComponent(entity1, comp1, oldSig, sig);
        componentManager->addComponent(entity2, comp2, oldSig, sig);
        componentManager->addComponent(entity3, comp3, oldSig, sig);

        // Destroy entity 2
        componentManager->entityDestroyed(entity2, sig);

        // Entity 1 and 3 should still exist
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(entity1).value, 10);
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(entity3).value, 30);

        // Entity 2 should not exist
        EXPECT_THROW(
            componentManager->getComponent<EdgeTestComponentA>(entity2),
            ComponentNotFound
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, ReuseDestroyedEntityID) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        EdgeTestComponentA comp1{42};

        Signature sig;
        Signature oldSig;
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        componentManager->addComponent(entity, comp1, oldSig, sig);
        componentManager->entityDestroyed(entity, sig);

        // Reuse the same entity ID
        EdgeTestComponentA comp2{99};
        componentManager->addComponent(entity, comp2, oldSig, sig);

        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(entity).value, 99);
    }

    // =========================================================
    // ============== COMPONENT DUPLICATION ====================
    // =========================================================

    TEST_F(ComponentManagerEdgeCaseTest, DuplicateComponentCreatesExactCopy) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity sourceEntity = 1;
        Entity destEntity = 2;
        EdgeTestComponentA component{42};

        Signature sig;
        Signature oldSig;
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        componentManager->addComponent(sourceEntity, component, oldSig, sig);
        componentManager->duplicateComponent<EdgeTestComponentA>(sourceEntity, destEntity, oldSig, sig);

        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(destEntity).value, 42);

        // Modify duplicate and ensure source is unaffected
        componentManager->getComponent<EdgeTestComponentA>(destEntity).value = 999;
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(sourceEntity).value, 42);
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(destEntity).value, 999);
    }

    TEST_F(ComponentManagerEdgeCaseTest, DuplicateComponentThrowsOnNonExistentSource) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity sourceEntity = 999;
        Entity destEntity = 1;
        Signature oldSig, newSig;

        EXPECT_THROW(
            componentManager->duplicateComponent<EdgeTestComponentA>(sourceEntity, destEntity, oldSig, newSig),
            ComponentNotFound
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, DuplicateComponentWithTypeID) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity sourceEntity = 1;
        Entity destEntity = 2;
        EdgeTestComponentA component{42};

        Signature sig;
        Signature oldSig;
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        componentManager->addComponent(sourceEntity, component, oldSig, sig);

        ComponentType typeID = componentManager->getComponentType<EdgeTestComponentA>();
        componentManager->duplicateComponent(typeID, sourceEntity, destEntity, oldSig, sig);

        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(destEntity).value, 42);
    }

    // =========================================================
    // ============== COMPONENT ARRAYS =========================
    // =========================================================

    TEST_F(ComponentManagerEdgeCaseTest, GetComponentArrayReturnsValidArray) {
        componentManager->registerComponent<EdgeTestComponentA>();

        auto array = componentManager->getComponentArray<EdgeTestComponentA>();
        EXPECT_NE(array, nullptr);
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetComponentArrayThrowsWhenNotRegistered) {
        EXPECT_THROW(
            componentManager->getComponentArray<EdgeTestComponentA>(),
            ComponentNotRegistered
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetComponentArrayConstVersion) {
        componentManager->registerComponent<EdgeTestComponentA>();

        const ComponentManager* constManager = componentManager.get();
        auto array = constManager->getComponentArray<EdgeTestComponentA>();
        EXPECT_NE(array, nullptr);
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetComponentArrayByTypeID) {
        componentManager->registerComponent<EdgeTestComponentA>();

        ComponentType typeID = componentManager->getComponentType<EdgeTestComponentA>();
        auto array = componentManager->getComponentArray(typeID);
        EXPECT_NE(array, nullptr);
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetComponentArrayByTypeIDThrowsWhenNotRegistered) {
        ComponentType invalidTypeID = 99;

        EXPECT_THROW(
            componentManager->getComponentArray(invalidTypeID),
            ComponentNotRegistered
        );
    }

    // =========================================================
    // ============== TYPE ERASED COMPONENTS ===================
    // =========================================================

    TEST_F(ComponentManagerEdgeCaseTest, RegisterTypeErasedComponent) {
        size_t componentSize = sizeof(int);
        size_t initialCapacity = 128;

        ComponentType typeID = componentManager->registerComponent(componentSize, initialCapacity);
        EXPECT_LT(typeID, MAX_COMPONENT_TYPE);
    }

    TEST_F(ComponentManagerEdgeCaseTest, AddTypeErasedComponentByTypeID) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        EdgeTestComponentA component{42};
        Signature oldSig, newSig;
        ComponentType typeID = componentManager->getComponentType<EdgeTestComponentA>();
        newSig.set(typeID, true);

        EXPECT_NO_THROW(
            componentManager->addComponent(entity, typeID, &component, oldSig, newSig)
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, RemoveTypeErasedComponentByTypeID) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        EdgeTestComponentA component{42};
        Signature oldSig, newSig;
        ComponentType typeID = componentManager->getComponentType<EdgeTestComponentA>();
        newSig.set(typeID, true);

        componentManager->addComponent(entity, typeID, &component, oldSig, newSig);

        Signature prevSig = newSig;
        newSig.set(typeID, false);

        EXPECT_NO_THROW(
            componentManager->removeComponent(entity, typeID, prevSig, newSig)
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, TryGetComponentByTypeIDReturnsNullptrForNonExistent) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        ComponentType typeID = componentManager->getComponentType<EdgeTestComponentA>();

        void* result = componentManager->tryGetComponent(entity, typeID);
        EXPECT_EQ(result, nullptr);
    }

    TEST_F(ComponentManagerEdgeCaseTest, TryGetComponentByTypeIDReturnsPointerWhenExists) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entity = 1;
        EdgeTestComponentA component{42};
        Signature oldSig, newSig;
        ComponentType typeID = componentManager->getComponentType<EdgeTestComponentA>();
        newSig.set(typeID, true);

        componentManager->addComponent(entity, component, oldSig, newSig);

        void* result = componentManager->tryGetComponent(entity, typeID);
        ASSERT_NE(result, nullptr);

        EdgeTestComponentA* compPtr = static_cast<EdgeTestComponentA*>(result);
        EXPECT_EQ(compPtr->value, 42);
    }

    // =========================================================
    // ============== GROUP OPERATIONS =========================
    // =========================================================

    TEST_F(ComponentManagerEdgeCaseTest, RegisterGroupWithOwnedComponents) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        auto group = componentManager->registerGroup<EdgeTestComponentA, EdgeTestComponentB>(get<>());
        EXPECT_NE(group, nullptr);
    }

    TEST_F(ComponentManagerEdgeCaseTest, RegisterGroupWithNonOwnedComponents) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();
        componentManager->registerComponent<EdgeTestComponentC>();

        auto group = componentManager->registerGroup<EdgeTestComponentA>(get<EdgeTestComponentB, EdgeTestComponentC>());
        EXPECT_NE(group, nullptr);
    }

    TEST_F(ComponentManagerEdgeCaseTest, RegisterSameGroupTwiceReturnsSameInstance) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        auto group1 = componentManager->registerGroup<EdgeTestComponentA>(get<EdgeTestComponentB>());
        auto group2 = componentManager->registerGroup<EdgeTestComponentA>(get<EdgeTestComponentB>());

        EXPECT_EQ(group1, group2);
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetGroupReturnsExistingGroup) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        auto registeredGroup = componentManager->registerGroup<EdgeTestComponentA>(get<EdgeTestComponentB>());
        auto retrievedGroup = componentManager->getGroup<EdgeTestComponentA>(get<EdgeTestComponentB>());

        EXPECT_EQ(registeredGroup, retrievedGroup);
    }

    TEST_F(ComponentManagerEdgeCaseTest, GetGroupThrowsWhenNotRegistered) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        EXPECT_THROW(
            componentManager->getGroup<EdgeTestComponentA>(get<EdgeTestComponentB>()),
            GroupNotFound
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, RegisterOverlappingGroupsThrows) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();
        componentManager->registerComponent<EdgeTestComponentC>();

        // Register first group with A as owned
        componentManager->registerGroup<EdgeTestComponentA>(get<EdgeTestComponentB>());

        // Try to register second group with A as owned (should throw)
        EXPECT_THROW(
            componentManager->registerGroup<EdgeTestComponentA>(get<EdgeTestComponentC>()),
            OverlappingGroupsException
        );
    }

    TEST_F(ComponentManagerEdgeCaseTest, RegisterOverlappingGroupsWithMultipleOwnedThrows) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();
        componentManager->registerComponent<EdgeTestComponentC>();
        componentManager->registerComponent<EdgeTestComponentD>();

        // Register first group with A and B as owned
        auto emptyTag = get<>();
        componentManager->registerGroup<EdgeTestComponentA, EdgeTestComponentB>(emptyTag);

        // Try to register second group with B and C as owned (should throw because B overlaps)
        bool threw = false;
        try {
            componentManager->registerGroup<EdgeTestComponentB, EdgeTestComponentC>(emptyTag);
        } catch (const OverlappingGroupsException&) {
            threw = true;
        }
        EXPECT_TRUE(threw);
    }

    TEST_F(ComponentManagerEdgeCaseTest, HasCommonOwnedComponentsDetectsOverlap) {
        GroupKey key1, key2;

        key1.ownedSignature.set(0, true);
        key1.ownedSignature.set(1, true);

        key2.ownedSignature.set(1, true);
        key2.ownedSignature.set(2, true);

        EXPECT_TRUE(ComponentManager::hasCommonOwnedComponents(key1, key2));
    }

    TEST_F(ComponentManagerEdgeCaseTest, HasCommonOwnedComponentsDetectsNoOverlap) {
        GroupKey key1, key2;

        key1.ownedSignature.set(0, true);
        key1.ownedSignature.set(1, true);

        key2.ownedSignature.set(2, true);
        key2.ownedSignature.set(3, true);

        EXPECT_FALSE(ComponentManager::hasCommonOwnedComponents(key1, key2));
    }

    TEST_F(ComponentManagerEdgeCaseTest, AddComponentUpdatesGroups) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        auto group = componentManager->registerGroup<EdgeTestComponentA, EdgeTestComponentB>(get<>());

        Entity entity = 1;
        EdgeTestComponentA compA{42};
        EdgeTestComponentB compB{3.14f};

        Signature sig;
        Signature oldSig = sig;

        // Add first component (entity doesn't qualify for group yet)
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);
        componentManager->addComponent(entity, compA, oldSig, sig);

        // Add second component (entity now qualifies for group)
        oldSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), true);
        componentManager->addComponent(entity, compB, oldSig, sig);

        // Group should contain the entity
        EXPECT_EQ(group->size(), 1);
    }

    TEST_F(ComponentManagerEdgeCaseTest, RemoveComponentUpdatesGroups) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        auto group = componentManager->registerGroup<EdgeTestComponentA, EdgeTestComponentB>(get<>());

        Entity entity = 1;
        EdgeTestComponentA compA{42};
        EdgeTestComponentB compB{3.14f};

        Signature sig;
        Signature oldSig = sig;

        // Add both components
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);
        componentManager->addComponent(entity, compA, oldSig, sig);

        oldSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), true);
        componentManager->addComponent(entity, compB, oldSig, sig);

        EXPECT_EQ(group->size(), 1);

        // Remove one component (entity no longer qualifies)
        Signature prevSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), false);
        componentManager->removeComponent<EdgeTestComponentB>(entity, prevSig, sig);

        // Group should be empty
        EXPECT_EQ(group->size(), 0);
    }

    TEST_F(ComponentManagerEdgeCaseTest, EntityDestroyedUpdatesGroups) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        auto group = componentManager->registerGroup<EdgeTestComponentA, EdgeTestComponentB>(get<>());

        Entity entity = 1;
        EdgeTestComponentA compA{42};
        EdgeTestComponentB compB{3.14f};

        Signature sig;
        Signature oldSig = sig;

        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);
        componentManager->addComponent(entity, compA, oldSig, sig);

        oldSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), true);
        componentManager->addComponent(entity, compB, oldSig, sig);

        EXPECT_EQ(group->size(), 1);

        // Destroy entity
        componentManager->entityDestroyed(entity, sig);

        // Group should be empty
        EXPECT_EQ(group->size(), 0);
    }

    // =========================================================
    // ============== EDGE CASES AND STRESS TESTS ==============
    // =========================================================

    TEST_F(ComponentManagerEdgeCaseTest, ComponentDataIntegrityAfterManyOperations) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();

        // Add many entities with components
        for (Entity i = 0; i < 100; ++i) {
            EdgeTestComponentA compA{static_cast<int>(i * 10)};
            EdgeTestComponentB compB{static_cast<float>(i * 0.5f)};

            Signature sig;
            Signature oldSig = sig;

            sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);
            componentManager->addComponent(i, compA, oldSig, sig);

            oldSig = sig;
            sig.set(componentManager->getComponentType<EdgeTestComponentB>(), true);
            componentManager->addComponent(i, compB, oldSig, sig);
        }

        // Verify all components
        for (Entity i = 0; i < 100; ++i) {
            EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(i).value, i * 10);
            EXPECT_FLOAT_EQ(componentManager->getComponent<EdgeTestComponentB>(i).data, i * 0.5f);
        }

        // Remove every other entity
        for (Entity i = 0; i < 100; i += 2) {
            Signature sig;
            sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);
            sig.set(componentManager->getComponentType<EdgeTestComponentB>(), true);
            componentManager->entityDestroyed(i, sig);
        }

        // Verify remaining entities
        for (Entity i = 1; i < 100; i += 2) {
            EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(i).value, i * 10);
            EXPECT_FLOAT_EQ(componentManager->getComponent<EdgeTestComponentB>(i).data, i * 0.5f);
        }
    }

    TEST_F(ComponentManagerEdgeCaseTest, SparseEntityDistribution) {
        componentManager->registerComponent<EdgeTestComponentA>();

        Entity entities[] = {1, 100, 1000, 10000, 50000};

        Signature oldSig, newSig;
        newSig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);

        for (Entity entity : entities) {
            EdgeTestComponentA comp{static_cast<int>(entity)};
            componentManager->addComponent(entity, comp, oldSig, newSig);
        }

        for (Entity entity : entities) {
            EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(entity).value, static_cast<int>(entity));
        }
    }

    TEST_F(ComponentManagerEdgeCaseTest, ComponentDataIntegrityAfterMixedOperations) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();
        componentManager->registerComponent<EdgeTestComponentC>();

        Entity entity = 1;
        Signature sig;
        Signature oldSig;

        // Add all three components
        EdgeTestComponentA compA{42};
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);
        componentManager->addComponent(entity, compA, oldSig, sig);

        oldSig = sig;
        EdgeTestComponentB compB{3.14f};
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), true);
        componentManager->addComponent(entity, compB, oldSig, sig);

        oldSig = sig;
        EdgeTestComponentC compC{1.0, 2.0, 3.0};
        sig.set(componentManager->getComponentType<EdgeTestComponentC>(), true);
        componentManager->addComponent(entity, compC, oldSig, sig);

        // Remove middle component
        Signature prevSig = sig;
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), false);
        componentManager->removeComponent<EdgeTestComponentB>(entity, prevSig, sig);

        // Verify remaining components
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(entity).value, 42);
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentC>(entity).x, 1.0);

        // Modify remaining components
        componentManager->getComponent<EdgeTestComponentA>(entity).value = 999;
        componentManager->getComponent<EdgeTestComponentC>(entity).z = 99.0;

        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentA>(entity).value, 999);
        EXPECT_EQ(componentManager->getComponent<EdgeTestComponentC>(entity).z, 99.0);
    }

    TEST_F(ComponentManagerEdgeCaseTest, MultipleGroupsWithSameEntity) {
        componentManager->registerComponent<EdgeTestComponentA>();
        componentManager->registerComponent<EdgeTestComponentB>();
        componentManager->registerComponent<EdgeTestComponentC>();

        // Create groups with different non-owned components (no overlap in owned)
        auto group1 = componentManager->registerGroup<EdgeTestComponentA>(get<EdgeTestComponentB>());
        auto group2 = componentManager->registerGroup<EdgeTestComponentC>(get<EdgeTestComponentB>());

        Entity entity = 1;
        Signature sig;
        Signature oldSig;

        // Add components to qualify for both groups
        EdgeTestComponentA compA{42};
        sig.set(componentManager->getComponentType<EdgeTestComponentA>(), true);
        componentManager->addComponent(entity, compA, oldSig, sig);

        oldSig = sig;
        EdgeTestComponentB compB{3.14f};
        sig.set(componentManager->getComponentType<EdgeTestComponentB>(), true);
        componentManager->addComponent(entity, compB, oldSig, sig);

        oldSig = sig;
        EdgeTestComponentC compC{1.0, 2.0, 3.0};
        sig.set(componentManager->getComponentType<EdgeTestComponentC>(), true);
        componentManager->addComponent(entity, compC, oldSig, sig);

        // Entity should be in both groups
        EXPECT_EQ(group1->size(), 1);
        EXPECT_EQ(group2->size(), 1);
    }

}
