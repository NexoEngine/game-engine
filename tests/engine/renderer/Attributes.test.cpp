//// Attributes.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for RequiredAttributes struct
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/Attributes.hpp"

namespace nexo::renderer {

// =============================================================================
// RequiredAttributes Default Initialization Tests
// =============================================================================

class RequiredAttributesDefaultTest : public ::testing::Test {};

TEST_F(RequiredAttributesDefaultTest, DefaultInitializationAllFlagsFalse) {
    RequiredAttributes attrs{};
    EXPECT_FALSE(attrs.bitsUnion.flags.position);
    EXPECT_FALSE(attrs.bitsUnion.flags.normal);
    EXPECT_FALSE(attrs.bitsUnion.flags.tangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.bitangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(attrs.bitsUnion.flags.lightmapUV);
}

TEST_F(RequiredAttributesDefaultTest, DefaultInitializationBitsAreZero) {
    RequiredAttributes attrs{};
    EXPECT_EQ(attrs.bitsUnion.bits, 0);
}

TEST_F(RequiredAttributesDefaultTest, DefaultConstructedAttributesAreEqual) {
    RequiredAttributes attrs1{};
    RequiredAttributes attrs2{};
    EXPECT_TRUE(attrs1 == attrs2);
}

// =============================================================================
// RequiredAttributes Single Flag Tests
// =============================================================================

class RequiredAttributesSingleFlagTest : public ::testing::Test {};

TEST_F(RequiredAttributesSingleFlagTest, SetPositionFlagOnly) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.position = true;

    EXPECT_TRUE(attrs.bitsUnion.flags.position);
    EXPECT_FALSE(attrs.bitsUnion.flags.normal);
    EXPECT_FALSE(attrs.bitsUnion.flags.tangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.bitangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(attrs.bitsUnion.flags.lightmapUV);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00000001);
}

TEST_F(RequiredAttributesSingleFlagTest, SetNormalFlagOnly) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.normal = true;

    EXPECT_FALSE(attrs.bitsUnion.flags.position);
    EXPECT_TRUE(attrs.bitsUnion.flags.normal);
    EXPECT_FALSE(attrs.bitsUnion.flags.tangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.bitangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(attrs.bitsUnion.flags.lightmapUV);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00000010);
}

TEST_F(RequiredAttributesSingleFlagTest, SetTangentFlagOnly) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.tangent = true;

    EXPECT_FALSE(attrs.bitsUnion.flags.position);
    EXPECT_FALSE(attrs.bitsUnion.flags.normal);
    EXPECT_TRUE(attrs.bitsUnion.flags.tangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.bitangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(attrs.bitsUnion.flags.lightmapUV);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00000100);
}

TEST_F(RequiredAttributesSingleFlagTest, SetBitangentFlagOnly) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.bitangent = true;

    EXPECT_FALSE(attrs.bitsUnion.flags.position);
    EXPECT_FALSE(attrs.bitsUnion.flags.normal);
    EXPECT_FALSE(attrs.bitsUnion.flags.tangent);
    EXPECT_TRUE(attrs.bitsUnion.flags.bitangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(attrs.bitsUnion.flags.lightmapUV);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00001000);
}

TEST_F(RequiredAttributesSingleFlagTest, SetUV0FlagOnly) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.uv0 = true;

    EXPECT_FALSE(attrs.bitsUnion.flags.position);
    EXPECT_FALSE(attrs.bitsUnion.flags.normal);
    EXPECT_FALSE(attrs.bitsUnion.flags.tangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.bitangent);
    EXPECT_TRUE(attrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(attrs.bitsUnion.flags.lightmapUV);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00010000);
}

TEST_F(RequiredAttributesSingleFlagTest, SetLightmapUVFlagOnly) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.lightmapUV = true;

    EXPECT_FALSE(attrs.bitsUnion.flags.position);
    EXPECT_FALSE(attrs.bitsUnion.flags.normal);
    EXPECT_FALSE(attrs.bitsUnion.flags.tangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.bitangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.uv0);
    EXPECT_TRUE(attrs.bitsUnion.flags.lightmapUV);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00100000);
}

// =============================================================================
// RequiredAttributes Multiple Flags Tests
// =============================================================================

class RequiredAttributesMultipleFlagsTest : public ::testing::Test {};

TEST_F(RequiredAttributesMultipleFlagsTest, SetPositionAndNormal) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.normal = true;

    EXPECT_TRUE(attrs.bitsUnion.flags.position);
    EXPECT_TRUE(attrs.bitsUnion.flags.normal);
    EXPECT_FALSE(attrs.bitsUnion.flags.tangent);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00000011);
}

TEST_F(RequiredAttributesMultipleFlagsTest, SetTangentAndBitangent) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.tangent = true;
    attrs.bitsUnion.flags.bitangent = true;

    EXPECT_TRUE(attrs.bitsUnion.flags.tangent);
    EXPECT_TRUE(attrs.bitsUnion.flags.bitangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.normal);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00001100);
}

TEST_F(RequiredAttributesMultipleFlagsTest, SetUVFlags) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.uv0 = true;
    attrs.bitsUnion.flags.lightmapUV = true;

    EXPECT_TRUE(attrs.bitsUnion.flags.uv0);
    EXPECT_TRUE(attrs.bitsUnion.flags.lightmapUV);
    EXPECT_FALSE(attrs.bitsUnion.flags.position);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00110000);
}

TEST_F(RequiredAttributesMultipleFlagsTest, SetPositionNormalTangent) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.normal = true;
    attrs.bitsUnion.flags.tangent = true;

    EXPECT_TRUE(attrs.bitsUnion.flags.position);
    EXPECT_TRUE(attrs.bitsUnion.flags.normal);
    EXPECT_TRUE(attrs.bitsUnion.flags.tangent);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00000111);
}

TEST_F(RequiredAttributesMultipleFlagsTest, SetAlternatingFlags) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.tangent = true;
    attrs.bitsUnion.flags.uv0 = true;

    EXPECT_TRUE(attrs.bitsUnion.flags.position);
    EXPECT_FALSE(attrs.bitsUnion.flags.normal);
    EXPECT_TRUE(attrs.bitsUnion.flags.tangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.bitangent);
    EXPECT_TRUE(attrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(attrs.bitsUnion.flags.lightmapUV);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00010101);
}

// =============================================================================
// RequiredAttributes All Flags Tests
// =============================================================================

class RequiredAttributesAllFlagsTest : public ::testing::Test {};

TEST_F(RequiredAttributesAllFlagsTest, SetAllFlags) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.normal = true;
    attrs.bitsUnion.flags.tangent = true;
    attrs.bitsUnion.flags.bitangent = true;
    attrs.bitsUnion.flags.uv0 = true;
    attrs.bitsUnion.flags.lightmapUV = true;

    EXPECT_TRUE(attrs.bitsUnion.flags.position);
    EXPECT_TRUE(attrs.bitsUnion.flags.normal);
    EXPECT_TRUE(attrs.bitsUnion.flags.tangent);
    EXPECT_TRUE(attrs.bitsUnion.flags.bitangent);
    EXPECT_TRUE(attrs.bitsUnion.flags.uv0);
    EXPECT_TRUE(attrs.bitsUnion.flags.lightmapUV);
    EXPECT_EQ(attrs.bitsUnion.bits, 0b00111111);
}

TEST_F(RequiredAttributesAllFlagsTest, SetAllFlagsViaBits) {
    RequiredAttributes attrs{};
    attrs.bitsUnion.bits = 0b00111111;

    EXPECT_TRUE(attrs.bitsUnion.flags.position);
    EXPECT_TRUE(attrs.bitsUnion.flags.normal);
    EXPECT_TRUE(attrs.bitsUnion.flags.tangent);
    EXPECT_TRUE(attrs.bitsUnion.flags.bitangent);
    EXPECT_TRUE(attrs.bitsUnion.flags.uv0);
    EXPECT_TRUE(attrs.bitsUnion.flags.lightmapUV);
}

// =============================================================================
// RequiredAttributes Equality Operator Tests
// =============================================================================

class RequiredAttributesEqualityTest : public ::testing::Test {};

TEST_F(RequiredAttributesEqualityTest, EmptyAttributesAreEqual) {
    RequiredAttributes attrs1{};
    RequiredAttributes attrs2{};

    EXPECT_TRUE(attrs1 == attrs2);
}

TEST_F(RequiredAttributesEqualityTest, SameFlagsAreEqual) {
    RequiredAttributes attrs1{};
    RequiredAttributes attrs2{};

    attrs1.bitsUnion.flags.position = true;
    attrs1.bitsUnion.flags.normal = true;

    attrs2.bitsUnion.flags.position = true;
    attrs2.bitsUnion.flags.normal = true;

    EXPECT_TRUE(attrs1 == attrs2);
}

TEST_F(RequiredAttributesEqualityTest, DifferentFlagsAreNotEqual) {
    RequiredAttributes attrs1{};
    RequiredAttributes attrs2{};

    attrs1.bitsUnion.flags.position = true;
    attrs2.bitsUnion.flags.normal = true;

    EXPECT_FALSE(attrs1 == attrs2);
}

TEST_F(RequiredAttributesEqualityTest, AllFlagsSetAreEqual) {
    RequiredAttributes attrs1{};
    RequiredAttributes attrs2{};

    attrs1.bitsUnion.bits = 0b00111111;
    attrs2.bitsUnion.bits = 0b00111111;

    EXPECT_TRUE(attrs1 == attrs2);
}

TEST_F(RequiredAttributesEqualityTest, SubsetIsNotEqual) {
    RequiredAttributes attrs1{};
    RequiredAttributes attrs2{};

    attrs1.bitsUnion.flags.position = true;
    attrs1.bitsUnion.flags.normal = true;

    attrs2.bitsUnion.flags.position = true;
    attrs2.bitsUnion.flags.normal = true;
    attrs2.bitsUnion.flags.tangent = true;

    EXPECT_FALSE(attrs1 == attrs2);
}

// =============================================================================
// RequiredAttributes compatibleWith - Basic Tests
// =============================================================================

class RequiredAttributesCompatibleBasicTest : public ::testing::Test {};

TEST_F(RequiredAttributesCompatibleBasicTest, EmptyIsCompatibleWithEmpty) {
    RequiredAttributes attrs1{};
    RequiredAttributes attrs2{};

    EXPECT_TRUE(attrs1.compatibleWith(attrs2));
    EXPECT_TRUE(attrs2.compatibleWith(attrs1));
}

TEST_F(RequiredAttributesCompatibleBasicTest, EmptyIsCompatibleWithAny) {
    RequiredAttributes empty{};
    RequiredAttributes attrs{};
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.normal = true;

    // Empty requires nothing, so it's compatible with anything
    EXPECT_TRUE(empty.compatibleWith(attrs));
}

TEST_F(RequiredAttributesCompatibleBasicTest, IdenticalAttributesAreCompatible) {
    RequiredAttributes attrs1{};
    RequiredAttributes attrs2{};

    attrs1.bitsUnion.flags.position = true;
    attrs1.bitsUnion.flags.normal = true;

    attrs2.bitsUnion.flags.position = true;
    attrs2.bitsUnion.flags.normal = true;

    EXPECT_TRUE(attrs1.compatibleWith(attrs2));
    EXPECT_TRUE(attrs2.compatibleWith(attrs1));
}

// =============================================================================
// RequiredAttributes compatibleWith - Subset Tests
// =============================================================================

class RequiredAttributesCompatibleSubsetTest : public ::testing::Test {};

TEST_F(RequiredAttributesCompatibleSubsetTest, SubsetIsCompatibleWithSuperset) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    // Required: position
    required.bitsUnion.flags.position = true;

    // Provided: position + normal
    provided.bitsUnion.flags.position = true;
    provided.bitsUnion.flags.normal = true;

    // Required is subset of provided, so it's compatible
    EXPECT_TRUE(required.compatibleWith(provided));
}

TEST_F(RequiredAttributesCompatibleSubsetTest, SupersetIsNotCompatibleWithSubset) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    // Required: position + normal
    required.bitsUnion.flags.position = true;
    required.bitsUnion.flags.normal = true;

    // Provided: only position
    provided.bitsUnion.flags.position = true;

    // Required is NOT a subset of provided (normal is missing)
    EXPECT_FALSE(required.compatibleWith(provided));
}

TEST_F(RequiredAttributesCompatibleSubsetTest, SingleFlagSubsetCompatibility) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    required.bitsUnion.flags.position = true;

    provided.bitsUnion.flags.position = true;
    provided.bitsUnion.flags.normal = true;
    provided.bitsUnion.flags.tangent = true;
    provided.bitsUnion.flags.bitangent = true;
    provided.bitsUnion.flags.uv0 = true;
    provided.bitsUnion.flags.lightmapUV = true;

    EXPECT_TRUE(required.compatibleWith(provided));
}

TEST_F(RequiredAttributesCompatibleSubsetTest, MultipleFlagsSubsetCompatibility) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    required.bitsUnion.flags.position = true;
    required.bitsUnion.flags.normal = true;
    required.bitsUnion.flags.uv0 = true;

    provided.bitsUnion.bits = 0b00111111; // All flags set

    EXPECT_TRUE(required.compatibleWith(provided));
}

TEST_F(RequiredAttributesCompatibleSubsetTest, PartialOverlapNotCompatible) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    // Required: position + normal
    required.bitsUnion.flags.position = true;
    required.bitsUnion.flags.normal = true;

    // Provided: position + tangent (missing normal)
    provided.bitsUnion.flags.position = true;
    provided.bitsUnion.flags.tangent = true;

    EXPECT_FALSE(required.compatibleWith(provided));
}

// =============================================================================
// RequiredAttributes compatibleWith - All Flags Tests
// =============================================================================

class RequiredAttributesCompatibleAllFlagsTest : public ::testing::Test {};

TEST_F(RequiredAttributesCompatibleAllFlagsTest, AllFlagsCompatibleWithAllFlags) {
    RequiredAttributes attrs1{};
    RequiredAttributes attrs2{};

    attrs1.bitsUnion.bits = 0b00111111;
    attrs2.bitsUnion.bits = 0b00111111;

    EXPECT_TRUE(attrs1.compatibleWith(attrs2));
}

TEST_F(RequiredAttributesCompatibleAllFlagsTest, AllFlagsNotCompatibleWithSubset) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    required.bitsUnion.bits = 0b00111111; // All flags
    provided.bitsUnion.flags.position = true; // Only position

    EXPECT_FALSE(required.compatibleWith(provided));
}

TEST_F(RequiredAttributesCompatibleAllFlagsTest, AnySubsetCompatibleWithAllFlags) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    required.bitsUnion.flags.tangent = true;
    required.bitsUnion.flags.uv0 = true;

    provided.bitsUnion.bits = 0b00111111; // All flags

    EXPECT_TRUE(required.compatibleWith(provided));
}

// =============================================================================
// RequiredAttributes compatibleWith - Bit Manipulation Tests
// =============================================================================

class RequiredAttributesCompatibleBitManipulationTest : public ::testing::Test {};

TEST_F(RequiredAttributesCompatibleBitManipulationTest, DirectBitManipulation) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    required.bitsUnion.bits = 0b00001010; // normal + bitangent
    provided.bitsUnion.bits = 0b00011111; // position + normal + tangent + bitangent + uv0

    EXPECT_TRUE(required.compatibleWith(provided));
}

TEST_F(RequiredAttributesCompatibleBitManipulationTest, BitMaskLogic) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    required.bitsUnion.bits = 0b00000101; // position + tangent
    provided.bitsUnion.bits = 0b00010101; // position + tangent + uv0

    // (required.bits & provided.bits) == required.bits
    // (0b00000101 & 0b00010101) == 0b00000101 ✓
    EXPECT_TRUE(required.compatibleWith(provided));
}

TEST_F(RequiredAttributesCompatibleBitManipulationTest, MissingBitNotCompatible) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    required.bitsUnion.bits = 0b00001111; // position + normal + tangent + bitangent
    provided.bitsUnion.bits = 0b00000111; // position + normal + tangent (missing bitangent)

    // (required.bits & provided.bits) != required.bits
    // (0b00001111 & 0b00000111) == 0b00000111 ≠ 0b00001111
    EXPECT_FALSE(required.compatibleWith(provided));
}

TEST_F(RequiredAttributesCompatibleBitManipulationTest, NonOverlappingBitsNotCompatible) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    required.bitsUnion.bits = 0b00000011; // position + normal
    provided.bitsUnion.bits = 0b00111100; // tangent + bitangent + uv0 + lightmapUV

    // (required.bits & provided.bits) == 0 ≠ required.bits
    EXPECT_FALSE(required.compatibleWith(provided));
}

// =============================================================================
// RequiredAttributes compatibleWith - Edge Cases
// =============================================================================

class RequiredAttributesCompatibleEdgeCasesTest : public ::testing::Test {};

TEST_F(RequiredAttributesCompatibleEdgeCasesTest, ZeroRequiredCompatibleWithAnything) {
    RequiredAttributes required{};
    RequiredAttributes provided1{};
    RequiredAttributes provided2{};
    RequiredAttributes provided3{};

    required.bitsUnion.bits = 0;
    provided1.bitsUnion.bits = 0;
    provided2.bitsUnion.bits = 0b00010101;
    provided3.bitsUnion.bits = 0b00111111;

    EXPECT_TRUE(required.compatibleWith(provided1));
    EXPECT_TRUE(required.compatibleWith(provided2));
    EXPECT_TRUE(required.compatibleWith(provided3));
}

TEST_F(RequiredAttributesCompatibleEdgeCasesTest, NonZeroRequiredNotCompatibleWithZero) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    required.bitsUnion.flags.position = true;
    provided.bitsUnion.bits = 0;

    EXPECT_FALSE(required.compatibleWith(provided));
}

TEST_F(RequiredAttributesCompatibleEdgeCasesTest, SingleBitDifferenceCausesIncompatibility) {
    RequiredAttributes required{};
    RequiredAttributes provided{};

    required.bitsUnion.bits = 0b00111111; // All 6 flags
    provided.bitsUnion.bits = 0b00111110; // All except position

    EXPECT_FALSE(required.compatibleWith(provided));
}

}  // namespace nexo::renderer
