//// Components.test.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/04/2025
//  Description: Test file for the component manager
//
///////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include "Components.hpp"
#include "Definitions.hpp"
#include "ECSExceptions.hpp"
#include <string>

namespace nexo::ecs {

	struct TestComponentA {
	    int value;

	    TestComponentA(int v = 0) : value(v) {}

	    bool operator==(const TestComponentA& other) const {
	        return value == other.value;
	    }
	};

	struct TestComponentB {
	    float x, y;

	    TestComponentB(float _x = 0.0f, float _y = 0.0f) : x(_x), y(_y) {}

	    bool operator==(const TestComponentB& other) const {
	        return x == other.x && y == other.y;
	    }
	};

	struct TestComponentC {
	    std::string name;

	    TestComponentC(const std::string& n = "") : name(n) {}

	    bool operator==(const TestComponentC& other) const {
	        return name == other.name;
	    }
	};

	struct TestComponentD {
	    bool flag;

	    TestComponentD(bool f = false) : flag(f) {}

	    bool operator==(const TestComponentD& other) const {
	        return flag == other.flag;
	    }
	};

	class ComponentManagerTest : public ::testing::Test {
	protected:
	    ComponentManager componentManager;

	    void SetUp() override {
	        componentManager.registerComponent<TestComponentA>();
	        componentManager.registerComponent<TestComponentB>();
	        componentManager.registerComponent<TestComponentC>();
	    }
	};

	class GroupKeyTest : public ::testing::Test {
	protected:
	    GroupKey createGroupKey() {
	        GroupKey key;
	        key.ownedSignature.set(0);
	        key.ownedSignature.set(2);
	        key.nonOwnedSignature.set(1);
	        key.nonOwnedSignature.set(3);
	        return key;
	    }
	};

	// =========================================================
	// ================== COMPONENT REGISTRATION ===============
	// =========================================================

	TEST_F(ComponentManagerTest, RegisterComponentCreatesComponentArray) {
	    // Register a new component type
	    componentManager.registerComponent<TestComponentD>();

	    // Check that the component array was created
	    auto componentArray = componentManager.getComponentArray<TestComponentD>();
	    EXPECT_NE(componentArray, nullptr);
	    EXPECT_EQ(componentArray->size(), 0);
	}

	TEST_F(ComponentManagerTest, GetComponentTypeReturnsConsistentTypeID) {
	    // Get component types for registered components
	    ComponentType typeA1 = componentManager.getComponentType<TestComponentA>();
	    ComponentType typeA2 = componentManager.getComponentType<TestComponentA>();
	    ComponentType typeB = componentManager.getComponentType<TestComponentB>();

	    // Same component type should return the same ID
	    EXPECT_EQ(typeA1, typeA2);

	    // Different component types should return different IDs
	    EXPECT_NE(typeA1, typeB);
	}

	TEST_F(ComponentManagerTest, GetComponentTypeThrowsForUnregisteredComponent) {
	    // Try to get type ID for an unregistered component
	    EXPECT_THROW({
	        static_cast<void>(componentManager.getComponentType<TestComponentD>());
	    }, ComponentNotRegistered);
	}

	TEST_F(ComponentManagerTest, GetComponentArrayThrowsForUnregisteredComponent) {
	    // Try to get component array for an unregistered component
	    EXPECT_THROW({
	        static_cast<void>(componentManager.getComponentArray<TestComponentD>());
	    }, ComponentNotRegistered);
	}

	// =========================================================
	// ============= COMPONENT ADDITION/REMOVAL ================
	// =========================================================

	TEST_F(ComponentManagerTest, AddComponentAddsComponentToEntity) {
	    const Entity entity = 1;
	    const TestComponentA component(42);
	    Signature signature;
		Signature oldSignature = signature;
		signature.set(componentManager.getComponentType<TestComponentA>(), true);

	    // Add component to entity
	    componentManager.addComponent<TestComponentA>(entity, component, oldSignature, signature);

	    // Check that the component was added
	    auto& retrievedComponent = componentManager.getComponent<TestComponentA>(entity);
	    EXPECT_EQ(retrievedComponent.value, component.value);

	    // Check the size of the component array
	    auto componentArray = componentManager.getComponentArray<TestComponentA>();
	    EXPECT_EQ(componentArray->size(), 1);
	    EXPECT_TRUE(componentArray->hasComponent(entity));
	}

	TEST_F(ComponentManagerTest, RemoveComponentRemovesComponentFromEntity) {
	    const Entity entity = 1;
	    const TestComponentA component(42);
	    Signature signature;
		Signature oldSignature = signature;
		signature.set(componentManager.getComponentType<TestComponentA>(), true);

	    // Add component to entity
	    componentManager.addComponent<TestComponentA>(entity, component, oldSignature, signature);

	    // Verify component exists
	    EXPECT_TRUE(componentManager.getComponentArray<TestComponentA>()->hasComponent(entity));

	    // Remove component
		Signature newSignature = signature;
		newSignature.set(componentManager.getComponentType<TestComponentA>(), false);
	    componentManager.removeComponent<TestComponentA>(entity, signature, newSignature);

	    // Check that the component was removed
	    EXPECT_FALSE(componentManager.getComponentArray<TestComponentA>()->hasComponent(entity));
	    EXPECT_EQ(componentManager.getComponentArray<TestComponentA>()->size(), 0);
	}

	TEST_F(ComponentManagerTest, TryRemoveComponentReturnsTrueIfRemoved) {
	    const Entity entity = 1;
	    const TestComponentA component(42);
	    Signature signature;
		Signature oldSignature = signature;
		signature.set(componentManager.getComponentType<TestComponentA>(), true);

	    // Add component to entity
	    componentManager.addComponent<TestComponentA>(entity, component, oldSignature, signature);

	    // Try to remove component
		Signature newSignature = signature;
		newSignature.set(componentManager.getComponentType<TestComponentA>(), false);
	    bool removed = componentManager.tryRemoveComponent<TestComponentA>(entity, signature, newSignature);

	    // Check that the component was removed and function returned true
	    EXPECT_TRUE(removed);
	    EXPECT_FALSE(componentManager.getComponentArray<TestComponentA>()->hasComponent(entity));
	}

	TEST_F(ComponentManagerTest, TryRemoveComponentReturnsFalseIfNotExist) {
	    const Entity entity = 1;
	    Signature signature;
		Signature oldSignature = signature;
		signature.set(componentManager.getComponentType<TestComponentA>(), true);

	    // Try to remove a component that doesn't exist
	    bool removed = componentManager.tryRemoveComponent<TestComponentA>(entity, oldSignature, signature);

	    // Check that the function returned false
	    EXPECT_FALSE(removed);
	}

	TEST_F(ComponentManagerTest, GetComponentReturnsCorrectComponent) {
	    const Entity entity = 1;
	    const TestComponentA component(42);
	    Signature signature;
		Signature oldSignature = signature;
		signature.set(componentManager.getComponentType<TestComponentA>(), true);

	    // Add component to entity
	    componentManager.addComponent<TestComponentA>(entity, component, oldSignature, signature);

	    // Get component
	    auto& retrievedComponent = componentManager.getComponent<TestComponentA>(entity);

	    // Check that the correct component was returned
	    EXPECT_EQ(retrievedComponent.value, component.value);

	    // Modify the component and verify it was changed in the array
	    retrievedComponent.value = 100;
	    auto& verifyComponent = componentManager.getComponent<TestComponentA>(entity);
	    EXPECT_EQ(verifyComponent.value, 100);
	}

	TEST_F(ComponentManagerTest, TryGetComponentReturnsComponentIfExists) {
	    const Entity entity = 1;
	    const TestComponentA component(42);
	    Signature signature;
		Signature oldSignature = signature;
		signature.set(componentManager.getComponentType<TestComponentA>(), true);

	    // Add component to entity
	    componentManager.addComponent<TestComponentA>(entity, component, oldSignature, signature);

	    // Try to get component
	    auto optComponent = componentManager.tryGetComponent<TestComponentA>(entity);

	    // Check that the component was returned
	    EXPECT_TRUE(optComponent.has_value());
	    EXPECT_EQ(optComponent.value().get().value, component.value);
	}

	TEST_F(ComponentManagerTest, TryGetComponentReturnsNulloptIfNotExists) {
	    const Entity entity = 1;

	    // Try to get a component that doesn't exist
	    auto optComponent = componentManager.tryGetComponent<TestComponentA>(entity);

	    // Check that nullopt was returned
	    EXPECT_FALSE(optComponent.has_value());
	}

	TEST_F(ComponentManagerTest, EntityDestroyedRemovesAllComponents) {
	    const Entity entity = 1;
	    Signature signature;
		Signature oldSignature = signature;
		signature.set(componentManager.getComponentType<TestComponentA>(), true);

	    // Add multiple components to the entity
		signature.set(getComponentTypeID<TestComponentA>());
	    componentManager.addComponent<TestComponentA>(entity, TestComponentA(42), oldSignature, signature);

		Signature newSignature = signature;
	    newSignature.set(getComponentTypeID<TestComponentB>());
	    componentManager.addComponent<TestComponentB>(entity, TestComponentB(1.0f, 2.0f), signature, newSignature);

	    // Verify the components exist
	    EXPECT_TRUE(componentManager.getComponentArray<TestComponentA>()->hasComponent(entity));
	    EXPECT_TRUE(componentManager.getComponentArray<TestComponentB>()->hasComponent(entity));

	    // Destroy the entity
	    componentManager.entityDestroyed(entity, signature);

	    // Check that all components were removed
	    EXPECT_FALSE(componentManager.getComponentArray<TestComponentA>()->hasComponent(entity));
	    EXPECT_FALSE(componentManager.getComponentArray<TestComponentB>()->hasComponent(entity));
	}

	// =========================================================
	// =================== GROUP KEYS ==========================
	// =========================================================

	TEST_F(GroupKeyTest, GroupKeyEquality) {
	    GroupKey key1 = createGroupKey();
	    GroupKey key2 = createGroupKey();

	    // Same keys should be equal
	    EXPECT_EQ(key1, key2);

	    // Different owned signature should make keys not equal
	    GroupKey key3 = createGroupKey();
	    key3.ownedSignature.set(4);
	    EXPECT_NE(key1, key3);

	    // Different non-owned signature should make keys not equal
	    GroupKey key4 = createGroupKey();
	    key4.nonOwnedSignature.set(5);
	    EXPECT_NE(key1, key4);
	}

	TEST_F(GroupKeyTest, GroupKeyHash) {
	    GroupKey key1 = createGroupKey();
	    GroupKey key2 = createGroupKey();

	    // Same keys should have the same hash
	    std::hash<GroupKey> hasher;
	    EXPECT_EQ(hasher(key1), hasher(key2));

	    // Different keys should have different hashes (not guaranteed, but likely)
	    GroupKey key3 = createGroupKey();
	    key3.ownedSignature.set(4);
	    EXPECT_NE(hasher(key1), hasher(key3));
	}

	TEST_F(ComponentManagerTest, HasCommonOwnedComponents) {
	    GroupKey key1, key2, key3;

	    // Set up signatures with overlapping components
	    key1.ownedSignature.set(0);
	    key1.ownedSignature.set(1);

	    key2.ownedSignature.set(1);
	    key2.ownedSignature.set(2);

	    key3.ownedSignature.set(3);
	    key3.ownedSignature.set(4);

	    // Check for overlapping components
	    EXPECT_TRUE(componentManager.hasCommonOwnedComponents(key1, key2)); // Both have component 1
	    EXPECT_FALSE(componentManager.hasCommonOwnedComponents(key1, key3)); // No common components
	    EXPECT_FALSE(componentManager.hasCommonOwnedComponents(key2, key3)); // No common components
	}

	// =========================================================
	// ================ GROUP REGISTRATION =====================
	// =========================================================

	TEST_F(ComponentManagerTest, RegisterGroupCreatesNewGroup) {
	    // Register a new group
	    auto group = componentManager.registerGroup<TestComponentA>(get<TestComponentB, TestComponentC>());

	    // Check that the group was created
	    EXPECT_NE(group, nullptr);

	    // Verify group properties
	    EXPECT_EQ(group->size(), 0);

	    // Verify group signature
	    auto allSignature = group->allSignature();
	    EXPECT_TRUE(allSignature.test(getComponentTypeID<TestComponentA>()));
	    EXPECT_TRUE(allSignature.test(getComponentTypeID<TestComponentB>()));
	    EXPECT_TRUE(allSignature.test(getComponentTypeID<TestComponentC>()));
	}

	TEST_F(ComponentManagerTest, RegisterGroupReturnsSameGroupWhenCalledTwice) {
	    // Register group twice
	    auto group1 = componentManager.registerGroup<TestComponentA>(get<TestComponentB, TestComponentC>());
	    auto group2 = componentManager.registerGroup<TestComponentA>(get<TestComponentB, TestComponentC>());

	    // Should return the same group object
	    EXPECT_EQ(group1, group2);
	}

	TEST_F(ComponentManagerTest, RegisterGroupThrowsOnOverlappingOwnedComponents) {
	    // Register first group
	    componentManager.registerGroup<TestComponentA, TestComponentB>(get<TestComponentC>());

		// The EXPECT_THROW macro seems to dislike templating for some reason
		bool exceptionThrown = false;
		try {
		    componentManager.registerGroup<TestComponentA, TestComponentC>(get<TestComponentB>());
		} catch (const OverlappingGroupsException& e) {
		    exceptionThrown = true;
		}
		EXPECT_TRUE(exceptionThrown);
	}

	TEST_F(ComponentManagerTest, RegisterGroupThrowsOnUnregisteredComponents) {
		bool exceptionThrown = false;
	    try {
	        componentManager.registerGroup<TestComponentA, TestComponentD>(get<TestComponentB>());
	    } catch (const ComponentNotRegistered& e) {
			exceptionThrown = true;
	    }
		EXPECT_TRUE(exceptionThrown);
	}

	TEST_F(ComponentManagerTest, GetGroupReturnsRegisteredGroup) {
	    // Register a group
	    auto registeredGroup = componentManager.registerGroup<TestComponentA>(get<TestComponentB, TestComponentC>());

	    // Get the group
	    auto retrievedGroup = componentManager.getGroup<TestComponentA>(get<TestComponentB, TestComponentC>());

	    // Should return the same group object
	    EXPECT_EQ(registeredGroup, retrievedGroup);
	}

	TEST_F(ComponentManagerTest, GetGroupThrowsOnNonexistentGroup) {
		bool exceptionThrown = false;
	    try {
	        componentManager.getGroup<TestComponentA, TestComponentB>(get<TestComponentC>());
	    } catch (const GroupNotFound& e) {
      		exceptionThrown = true;
	    }
		EXPECT_TRUE(exceptionThrown);
	}

	TEST_F(ComponentManagerTest, GroupAddsEntitiesOnComponentsAdded) {
	    auto group = componentManager.registerGroup<TestComponentA>(get<TestComponentB>());

	    // Create required entity components
	    const Entity entity = 1;
	    Signature signature;
		Signature oldSignature = signature;

	    signature.set(getComponentTypeID<TestComponentA>());
	    componentManager.addComponent<TestComponentA>(entity, TestComponentA(42), oldSignature, signature);

		Signature newSignature = signature;
	    newSignature.set(getComponentTypeID<TestComponentB>());
	    componentManager.addComponent<TestComponentB>(entity, TestComponentB(1.0f, 2.0f), signature, newSignature);

	    // Entity should be automatically added to the group
	    EXPECT_EQ(group->size(), 1);

	    // Verify the entity is in the group
	    auto entitySpan = group->entities();
	    EXPECT_EQ(entitySpan.size(), 1);
	    EXPECT_EQ(entitySpan[0], entity);
	}

	TEST_F(ComponentManagerTest, GroupRemovesEntitiesOnComponentsRemoved) {
	    // Create entity with required components first
	    const Entity entity = 1;
	    Signature signature;
		Signature oldSignature = signature;
	    signature.set(getComponentTypeID<TestComponentA>());

	    componentManager.addComponent<TestComponentA>(entity, TestComponentA(42), oldSignature, signature);

		Signature newSignature = signature;
	    newSignature.set(getComponentTypeID<TestComponentB>());
	    componentManager.addComponent<TestComponentB>(entity, TestComponentB(1.0f, 2.0f), signature, newSignature);
		signature = newSignature;

	    // Register group
	    auto group = componentManager.registerGroup<TestComponentA>(get<TestComponentB>());

	    // Verify entity is in the group
	    EXPECT_EQ(group->size(), 1);


	    // Remove a required component
		newSignature.set(getComponentTypeID<TestComponentB>(), false);
	    componentManager.removeComponent<TestComponentB>(entity, signature, newSignature);

	    // Entity should be automatically removed from the group
	    EXPECT_EQ(group->size(), 0);
	}

	TEST_F(ComponentManagerTest, GroupHandlesMultipleEntities) {
	    // Register group
	    auto group = componentManager.registerGroup<TestComponentA>(get<TestComponentB>());

	    for (Entity e = 1; e <= 5; ++e) {
			Signature signature;
			Signature oldSignature = signature;
      		signature.set(getComponentTypeID<TestComponentA>());
	        componentManager.addComponent<TestComponentA>(e, TestComponentA(e * 10), oldSignature, signature);

			oldSignature = signature;
	        signature.set(getComponentTypeID<TestComponentB>());
	        componentManager.addComponent<TestComponentB>(e, TestComponentB(e * 1.0f, e * 2.0f), oldSignature, signature);
	    }

	    // All entities should be in the group
	    EXPECT_EQ(group->size(), 5);
		Signature signature;
		signature.set(getComponentTypeID<TestComponentA>());
		signature.set(getComponentTypeID<TestComponentB>());

		Signature newSignature = signature;
		signature.set(getComponentTypeID<TestComponentA>(), false);

	    // Remove some entities
	    for (Entity e = 1; e <= 5; e += 2) {
	        componentManager.removeComponent<TestComponentA>(e, signature, newSignature);
	    }

	    // Only remaining entities should be in the group
	    EXPECT_EQ(group->size(), 2);

	    // Verify the correct entities remain
	    auto entitySpan = group->entities();
	    std::set<Entity> expectedEntities = {2, 4};
	    std::set<Entity> actualEntities(entitySpan.begin(), entitySpan.end());
	    EXPECT_EQ(actualEntities, expectedEntities);
	}

	TEST_F(ComponentManagerTest, EntityDestroyedRemovesFromGroups) {
	    // Create entity with required components
	    const Entity entity = 1;
	    Signature signature;
		Signature oldSignature = signature;

		signature.set(getComponentTypeID<TestComponentA>());
	    componentManager.addComponent<TestComponentA>(entity, TestComponentA(42), oldSignature, signature);

		oldSignature = signature;
		signature.set(getComponentTypeID<TestComponentB>());
	    componentManager.addComponent<TestComponentB>(entity, TestComponentB(1.0f, 2.0f), oldSignature, signature);

	    // Register group
	    auto group = componentManager.registerGroup<TestComponentA>(get<TestComponentB>());

	    // Verify entity is in the group
	    EXPECT_EQ(group->size(), 1);

	    // Destroy the entity
	    componentManager.entityDestroyed(entity, signature);

	    // Entity should be removed from the group
	    EXPECT_EQ(group->size(), 0);
	}

	// =========================================================
	// ================ INTEGRATION TEST ======================
	// =========================================================

	TEST_F(ComponentManagerTest, ComplexIntegrationTest) {
	    // Register additional component
	    componentManager.registerComponent<TestComponentD>();

	    // Register multiple groups
	    auto groupAB = componentManager.registerGroup<TestComponentA>(get<TestComponentB>());
	    auto groupCD = componentManager.registerGroup<TestComponentC>(get<TestComponentD>());

	    // Create entities with various component combinations
	    Signature signature1, signature2, signature3;
		Signature oldSignature1, oldSignature2, oldSignature3;

	    // Entity 1: A + B + C
		oldSignature1 = signature1;
	    signature1.set(getComponentTypeID<TestComponentA>());
	    componentManager.addComponent<TestComponentA>(1, TestComponentA(10), oldSignature1, signature1);

		oldSignature1 = signature1;
	    signature1.set(getComponentTypeID<TestComponentB>());
	    componentManager.addComponent<TestComponentB>(1, TestComponentB(1.0f, 2.0f), oldSignature1, signature1);

		oldSignature1 = signature1;
	    signature1.set(getComponentTypeID<TestComponentC>());
	    componentManager.addComponent<TestComponentC>(1, TestComponentC("Entity1"), oldSignature1, signature1);

	    // Entity 2: A + B + D
		oldSignature2 = signature2;
	    signature2.set(getComponentTypeID<TestComponentA>());
	    componentManager.addComponent<TestComponentA>(2, TestComponentA(20), oldSignature2, signature2);

		oldSignature2 = signature2;
	    signature2.set(getComponentTypeID<TestComponentB>());
	    componentManager.addComponent<TestComponentB>(2, TestComponentB(3.0f, 4.0f), oldSignature2, signature2);

	    oldSignature2 = signature2;
	    signature2.set(getComponentTypeID<TestComponentD>());
	    componentManager.addComponent<TestComponentD>(2, TestComponentD(true), oldSignature2, signature2);

	    // Entity 3: C + D
		oldSignature3 = signature3;
	    signature3.set(getComponentTypeID<TestComponentC>());
	    componentManager.addComponent<TestComponentC>(3, TestComponentC("Entity3"), oldSignature3, signature3);

		oldSignature3 = signature3;
	    signature3.set(getComponentTypeID<TestComponentD>());
	    componentManager.addComponent<TestComponentD>(3, TestComponentD(false), oldSignature3, signature3);

	    // Verify group membership
	    EXPECT_EQ(groupAB->size(), 2); // Entities 1 and 2
	    EXPECT_EQ(groupCD->size(), 1); // Entity 3

	    // Remove components to change group membership
		Signature newSignature1 = signature1;
		newSignature1.set(getComponentTypeID<TestComponentB>(), false);
	    componentManager.removeComponent<TestComponentB>(1, signature1, newSignature1);
	    signature1.reset(getComponentTypeID<TestComponentB>());

		oldSignature1 = signature1;
	    signature1.set(getComponentTypeID<TestComponentD>());
	    componentManager.addComponent<TestComponentD>(1, TestComponentD(true), oldSignature1, signature1);

	    // Check updated group membership
	    EXPECT_EQ(groupAB->size(), 1); // Only Entity 2 now
	    EXPECT_EQ(groupCD->size(), 2); // Entities 1 and 3 now

	    // Destroy an entity
	    componentManager.entityDestroyed(2, signature2);

	    // Verify final state
	    EXPECT_EQ(groupAB->size(), 0);
	    EXPECT_EQ(groupCD->size(), 2);

	    // Check component arrays
	    EXPECT_EQ(componentManager.getComponentArray<TestComponentA>()->size(), 1);
	    EXPECT_EQ(componentManager.getComponentArray<TestComponentB>()->size(), 0);
	    EXPECT_EQ(componentManager.getComponentArray<TestComponentC>()->size(), 2);
	    EXPECT_EQ(componentManager.getComponentArray<TestComponentD>()->size(), 2);
	}
}
