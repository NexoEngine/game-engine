//// Definitions.test.cpp /////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for ECS type definitions and constants
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/Definitions.hpp"
#include <type_traits>
#include <limits>

namespace nexo::ecs {

// =============================================================================
// Entity Type Tests
// =============================================================================

class EntityTypeTest : public ::testing::Test {};

TEST_F(EntityTypeTest, EntityIsUint32) {
    static_assert(std::is_same_v<Entity, std::uint32_t>);
    SUCCEED();
}

TEST_F(EntityTypeTest, EntityIsUnsigned) {
    static_assert(std::is_unsigned_v<Entity>);
    SUCCEED();
}

TEST_F(EntityTypeTest, MaxEntitiesIsReasonable) {
    EXPECT_GT(MAX_ENTITIES, 0u);
    EXPECT_LE(MAX_ENTITIES, std::numeric_limits<Entity>::max());
}

TEST_F(EntityTypeTest, MaxEntitiesValue) {
    EXPECT_EQ(MAX_ENTITIES, 500000u);
}

TEST_F(EntityTypeTest, InvalidEntityIsMax) {
    EXPECT_EQ(INVALID_ENTITY, std::numeric_limits<Entity>::max());
}

TEST_F(EntityTypeTest, InvalidEntityGreaterThanMaxEntities) {
    EXPECT_GT(INVALID_ENTITY, MAX_ENTITIES);
}

TEST_F(EntityTypeTest, ValidEntityRangeDoesNotOverlapInvalid) {
    for (Entity e = 0; e < 100; ++e) {
        EXPECT_NE(e, INVALID_ENTITY);
    }
}

// =============================================================================
// ComponentType Tests
// =============================================================================

class ComponentTypeTest : public ::testing::Test {};

TEST_F(ComponentTypeTest, ComponentTypeIsUint8) {
    static_assert(std::is_same_v<ComponentType, std::uint8_t>);
    SUCCEED();
}

TEST_F(ComponentTypeTest, ComponentTypeIsUnsigned) {
    static_assert(std::is_unsigned_v<ComponentType>);
    SUCCEED();
}

TEST_F(ComponentTypeTest, MaxComponentTypeValue) {
    EXPECT_EQ(MAX_COMPONENT_TYPE, 32u);
}

TEST_F(ComponentTypeTest, MaxComponentTypeFitsInUint8) {
    EXPECT_LE(MAX_COMPONENT_TYPE, std::numeric_limits<ComponentType>::max());
}

// =============================================================================
// GroupType Tests
// =============================================================================

class GroupTypeTest : public ::testing::Test {};

TEST_F(GroupTypeTest, GroupTypeIsUint8) {
    static_assert(std::is_same_v<GroupType, std::uint8_t>);
    SUCCEED();
}

TEST_F(GroupTypeTest, GroupTypeIsUnsigned) {
    static_assert(std::is_unsigned_v<GroupType>);
    SUCCEED();
}

TEST_F(GroupTypeTest, MaxGroupNumberValue) {
    EXPECT_EQ(MAX_GROUP_NUMBER, 32u);
}

TEST_F(GroupTypeTest, MaxGroupNumberFitsInGroupType) {
    EXPECT_LE(MAX_GROUP_NUMBER, std::numeric_limits<GroupType>::max());
}

// =============================================================================
// Signature Tests
// =============================================================================

class SignatureTest : public ::testing::Test {};

TEST_F(SignatureTest, SignatureSize) {
    EXPECT_EQ(Signature{}.size(), MAX_COMPONENT_TYPE);
}

TEST_F(SignatureTest, DefaultSignatureAllFalse) {
    Signature sig;
    EXPECT_EQ(sig.count(), 0u);
}

TEST_F(SignatureTest, CanSetAllBits) {
    Signature sig;
    for (ComponentType i = 0; i < MAX_COMPONENT_TYPE; ++i) {
        sig.set(i);
    }
    EXPECT_EQ(sig.count(), MAX_COMPONENT_TYPE);
}

TEST_F(SignatureTest, CanTestIndividualBits) {
    Signature sig;
    sig.set(0);
    sig.set(15);
    sig.set(31);

    EXPECT_TRUE(sig.test(0));
    EXPECT_FALSE(sig.test(1));
    EXPECT_TRUE(sig.test(15));
    EXPECT_FALSE(sig.test(16));
    EXPECT_TRUE(sig.test(31));
}

TEST_F(SignatureTest, CanResetBits) {
    Signature sig;
    sig.set(5);
    EXPECT_TRUE(sig.test(5));

    sig.reset(5);
    EXPECT_FALSE(sig.test(5));
}

TEST_F(SignatureTest, BitwiseAnd) {
    Signature sig1;
    sig1.set(0);
    sig1.set(1);
    sig1.set(2);

    Signature sig2;
    sig2.set(1);
    sig2.set(2);
    sig2.set(3);

    Signature result = sig1 & sig2;
    EXPECT_FALSE(result.test(0));
    EXPECT_TRUE(result.test(1));
    EXPECT_TRUE(result.test(2));
    EXPECT_FALSE(result.test(3));
}

TEST_F(SignatureTest, BitwiseOr) {
    Signature sig1;
    sig1.set(0);
    sig1.set(1);

    Signature sig2;
    sig2.set(2);
    sig2.set(3);

    Signature result = sig1 | sig2;
    EXPECT_TRUE(result.test(0));
    EXPECT_TRUE(result.test(1));
    EXPECT_TRUE(result.test(2));
    EXPECT_TRUE(result.test(3));
}

TEST_F(SignatureTest, SignatureEquality) {
    Signature sig1;
    sig1.set(0);
    sig1.set(5);

    Signature sig2;
    sig2.set(0);
    sig2.set(5);

    Signature sig3;
    sig3.set(0);

    EXPECT_EQ(sig1, sig2);
    EXPECT_NE(sig1, sig3);
}

TEST_F(SignatureTest, SubsetCheck) {
    Signature system;
    system.set(0);
    system.set(2);

    Signature entity;
    entity.set(0);
    entity.set(1);
    entity.set(2);

    // Entity has all components required by system
    EXPECT_EQ((entity & system), system);
}

TEST_F(SignatureTest, NotSubsetCheck) {
    Signature system;
    system.set(0);
    system.set(2);

    Signature entity;
    entity.set(0);

    // Entity does NOT have all components required by system
    EXPECT_NE((entity & system), system);
}

// =============================================================================
// Component Type ID Tests
// =============================================================================

struct TestComponentA { int a; };
struct TestComponentB { float b; };
struct TestComponentC { double c; };

class ComponentTypeIDTest : public ::testing::Test {};

TEST_F(ComponentTypeIDTest, SameTypeGetsSameID) {
    ComponentType id1 = getComponentTypeID<TestComponentA>();
    ComponentType id2 = getComponentTypeID<TestComponentA>();
    EXPECT_EQ(id1, id2);
}

TEST_F(ComponentTypeIDTest, DifferentTypesGetDifferentIDs) {
    ComponentType idA = getComponentTypeID<TestComponentA>();
    ComponentType idB = getComponentTypeID<TestComponentB>();
    ComponentType idC = getComponentTypeID<TestComponentC>();

    EXPECT_NE(idA, idB);
    EXPECT_NE(idB, idC);
    EXPECT_NE(idA, idC);
}

TEST_F(ComponentTypeIDTest, IDsAreConsistent) {
    ComponentType first = getComponentTypeID<TestComponentA>();

    // Call multiple times
    for (int i = 0; i < 10; ++i) {
        EXPECT_EQ(getComponentTypeID<TestComponentA>(), first);
    }
}

TEST_F(ComponentTypeIDTest, ConstVariantGetsSameID) {
    ComponentType regular = getComponentTypeID<TestComponentA>();
    ComponentType constType = getComponentTypeID<const TestComponentA>();

    EXPECT_EQ(regular, constType);
}

TEST_F(ComponentTypeIDTest, ReferenceVariantGetsSameID) {
    ComponentType regular = getComponentTypeID<TestComponentA>();
    ComponentType refType = getComponentTypeID<TestComponentA&>();

    EXPECT_EQ(regular, refType);
}

TEST_F(ComponentTypeIDTest, ConstRefVariantGetsSameID) {
    ComponentType regular = getComponentTypeID<TestComponentA>();
    ComponentType constRefType = getComponentTypeID<const TestComponentA&>();

    EXPECT_EQ(regular, constRefType);
}

TEST_F(ComponentTypeIDTest, IDsAreWithinRange) {
    ComponentType idA = getComponentTypeID<TestComponentA>();
    ComponentType idB = getComponentTypeID<TestComponentB>();
    ComponentType idC = getComponentTypeID<TestComponentC>();

    EXPECT_LT(idA, MAX_COMPONENT_TYPE);
    EXPECT_LT(idB, MAX_COMPONENT_TYPE);
    EXPECT_LT(idC, MAX_COMPONENT_TYPE);
}

// =============================================================================
// Signature and Component Type Integration Tests
// =============================================================================

class SignatureComponentIntegrationTest : public ::testing::Test {};

TEST_F(SignatureComponentIntegrationTest, CanUseComponentTypeWithSignature) {
    Signature sig;
    sig.set(getComponentTypeID<TestComponentA>());
    sig.set(getComponentTypeID<TestComponentB>());

    EXPECT_TRUE(sig.test(getComponentTypeID<TestComponentA>()));
    EXPECT_TRUE(sig.test(getComponentTypeID<TestComponentB>()));
    EXPECT_FALSE(sig.test(getComponentTypeID<TestComponentC>()));
}

TEST_F(SignatureComponentIntegrationTest, BuildSystemSignature) {
    Signature systemSignature;
    systemSignature.set(getComponentTypeID<TestComponentA>());
    systemSignature.set(getComponentTypeID<TestComponentB>());

    // Entity with both components
    Signature entityWithBoth;
    entityWithBoth.set(getComponentTypeID<TestComponentA>());
    entityWithBoth.set(getComponentTypeID<TestComponentB>());
    entityWithBoth.set(getComponentTypeID<TestComponentC>());

    // Entity missing one component
    Signature entityMissing;
    entityMissing.set(getComponentTypeID<TestComponentA>());

    // Check if entity matches system requirements
    EXPECT_EQ((entityWithBoth & systemSignature), systemSignature);
    EXPECT_NE((entityMissing & systemSignature), systemSignature);
}

// =============================================================================
// Edge Cases
// =============================================================================

class DefinitionsEdgeCaseTest : public ::testing::Test {};

TEST_F(DefinitionsEdgeCaseTest, SignatureFlipBit) {
    Signature sig;
    sig.flip(0);
    EXPECT_TRUE(sig.test(0));

    sig.flip(0);
    EXPECT_FALSE(sig.test(0));
}

TEST_F(DefinitionsEdgeCaseTest, SignatureNone) {
    Signature sig;
    EXPECT_TRUE(sig.none());

    sig.set(0);
    EXPECT_FALSE(sig.none());
}

TEST_F(DefinitionsEdgeCaseTest, SignatureAny) {
    Signature sig;
    EXPECT_FALSE(sig.any());

    sig.set(5);
    EXPECT_TRUE(sig.any());
}

TEST_F(DefinitionsEdgeCaseTest, SignatureAll) {
    Signature sig;
    EXPECT_FALSE(sig.all());

    sig.set();  // Set all bits
    EXPECT_TRUE(sig.all());
}

TEST_F(DefinitionsEdgeCaseTest, SignatureToString) {
    Signature sig;
    sig.set(0);
    sig.set(31);

    std::string str = sig.to_string();
    EXPECT_EQ(str.length(), MAX_COMPONENT_TYPE);
}

}  // namespace nexo::ecs
