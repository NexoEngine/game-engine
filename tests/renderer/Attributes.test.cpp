//// Attributes.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for RequiredAttributes (bitfield operations)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/Attributes.hpp"

namespace nexo::renderer {

class RequiredAttributesTest : public ::testing::Test {
protected:
    RequiredAttributes attrs;

    void SetUp() override {
        // Reset all bits to 0
        attrs.bitsUnion.bits = 0;
    }
};

// =============================================================================
// Default State Tests
// =============================================================================

TEST_F(RequiredAttributesTest, DefaultBitsAreZero) {
    RequiredAttributes defaultAttrs;
    EXPECT_EQ(defaultAttrs.bitsUnion.bits, 0);
}

TEST_F(RequiredAttributesTest, DefaultFlagsAreFalse) {
    RequiredAttributes defaultAttrs;
    EXPECT_FALSE(defaultAttrs.bitsUnion.flags.position);
    EXPECT_FALSE(defaultAttrs.bitsUnion.flags.normal);
    EXPECT_FALSE(defaultAttrs.bitsUnion.flags.tangent);
    EXPECT_FALSE(defaultAttrs.bitsUnion.flags.bitangent);
    EXPECT_FALSE(defaultAttrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(defaultAttrs.bitsUnion.flags.lightmapUV);
}

// =============================================================================
// Individual Flag Tests
// =============================================================================

TEST_F(RequiredAttributesTest, SetPositionFlag) {
    attrs.bitsUnion.flags.position = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.position);
    EXPECT_EQ(attrs.bitsUnion.bits & 0x01, 0x01);
}

TEST_F(RequiredAttributesTest, SetNormalFlag) {
    attrs.bitsUnion.flags.normal = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.normal);
    EXPECT_EQ(attrs.bitsUnion.bits & 0x02, 0x02);
}

TEST_F(RequiredAttributesTest, SetTangentFlag) {
    attrs.bitsUnion.flags.tangent = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.tangent);
    EXPECT_EQ(attrs.bitsUnion.bits & 0x04, 0x04);
}

TEST_F(RequiredAttributesTest, SetBitangentFlag) {
    attrs.bitsUnion.flags.bitangent = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.bitangent);
    EXPECT_EQ(attrs.bitsUnion.bits & 0x08, 0x08);
}

TEST_F(RequiredAttributesTest, SetUv0Flag) {
    attrs.bitsUnion.flags.uv0 = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.uv0);
    EXPECT_EQ(attrs.bitsUnion.bits & 0x10, 0x10);
}

TEST_F(RequiredAttributesTest, SetLightmapUVFlag) {
    attrs.bitsUnion.flags.lightmapUV = true;
    EXPECT_TRUE(attrs.bitsUnion.flags.lightmapUV);
    EXPECT_EQ(attrs.bitsUnion.bits & 0x20, 0x20);
}

// =============================================================================
// Multiple Flags Tests
// =============================================================================

TEST_F(RequiredAttributesTest, SetMultipleFlags) {
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.normal = true;
    attrs.bitsUnion.flags.uv0 = true;

    EXPECT_TRUE(attrs.bitsUnion.flags.position);
    EXPECT_TRUE(attrs.bitsUnion.flags.normal);
    EXPECT_FALSE(attrs.bitsUnion.flags.tangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.bitangent);
    EXPECT_TRUE(attrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(attrs.bitsUnion.flags.lightmapUV);

    // position (0x01) + normal (0x02) + uv0 (0x10) = 0x13
    EXPECT_EQ(attrs.bitsUnion.bits & 0x3F, 0x13);
}

TEST_F(RequiredAttributesTest, SetAllFlags) {
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.normal = true;
    attrs.bitsUnion.flags.tangent = true;
    attrs.bitsUnion.flags.bitangent = true;
    attrs.bitsUnion.flags.uv0 = true;
    attrs.bitsUnion.flags.lightmapUV = true;

    EXPECT_EQ(attrs.bitsUnion.bits & 0x3F, 0x3F);
}

// =============================================================================
// Equality Operator Tests
// =============================================================================

TEST_F(RequiredAttributesTest, EqualityWithSameBits) {
    RequiredAttributes a, b;
    a.bitsUnion.bits = 0x15;
    b.bitsUnion.bits = 0x15;

    EXPECT_TRUE(a == b);
}

TEST_F(RequiredAttributesTest, EqualityWithDifferentBits) {
    RequiredAttributes a, b;
    a.bitsUnion.bits = 0x15;
    b.bitsUnion.bits = 0x16;

    EXPECT_FALSE(a == b);
}

TEST_F(RequiredAttributesTest, EqualityWithZeroBits) {
    RequiredAttributes a, b;
    a.bitsUnion.bits = 0;
    b.bitsUnion.bits = 0;

    EXPECT_TRUE(a == b);
}

TEST_F(RequiredAttributesTest, EqualityUsingFlags) {
    RequiredAttributes a, b;
    a.bitsUnion.flags.position = true;
    a.bitsUnion.flags.uv0 = true;

    b.bitsUnion.flags.position = true;
    b.bitsUnion.flags.uv0 = true;

    EXPECT_TRUE(a == b);
}

// =============================================================================
// CompatibleWith Tests
// =============================================================================

TEST_F(RequiredAttributesTest, CompatibleWithSameBits) {
    RequiredAttributes required, available;
    required.bitsUnion.bits = 0x13;  // position + normal + uv0
    available.bitsUnion.bits = 0x13;

    EXPECT_TRUE(required.compatibleWith(available));
}

TEST_F(RequiredAttributesTest, CompatibleWithSupersetBits) {
    RequiredAttributes required, available;
    required.bitsUnion.bits = 0x03;   // position + normal
    available.bitsUnion.bits = 0x13;  // position + normal + uv0

    EXPECT_TRUE(required.compatibleWith(available));
}

TEST_F(RequiredAttributesTest, NotCompatibleWithSubsetBits) {
    RequiredAttributes required, available;
    required.bitsUnion.bits = 0x13;  // position + normal + uv0
    available.bitsUnion.bits = 0x03;  // position + normal (missing uv0)

    EXPECT_FALSE(required.compatibleWith(available));
}

TEST_F(RequiredAttributesTest, CompatibleWithZeroRequired) {
    RequiredAttributes required, available;
    required.bitsUnion.bits = 0x00;  // no requirements
    available.bitsUnion.bits = 0x3F;  // all attributes

    EXPECT_TRUE(required.compatibleWith(available));
}

TEST_F(RequiredAttributesTest, CompatibleWithZeroAvailable) {
    RequiredAttributes required, available;
    required.bitsUnion.bits = 0x00;  // no requirements
    available.bitsUnion.bits = 0x00;  // nothing available

    EXPECT_TRUE(required.compatibleWith(available));
}

TEST_F(RequiredAttributesTest, NotCompatibleWhenRequiredAndNothingAvailable) {
    RequiredAttributes required, available;
    required.bitsUnion.bits = 0x01;  // requires position
    available.bitsUnion.bits = 0x00;  // nothing available

    EXPECT_FALSE(required.compatibleWith(available));
}

TEST_F(RequiredAttributesTest, CompatibleWithDifferentNonOverlappingBits) {
    RequiredAttributes required, available;
    required.bitsUnion.flags.position = true;
    available.bitsUnion.flags.normal = true;

    // required has position, available has normal - not compatible
    EXPECT_FALSE(required.compatibleWith(available));
}

TEST_F(RequiredAttributesTest, CompatibilityIsNotSymmetric) {
    RequiredAttributes a, b;
    a.bitsUnion.bits = 0x01;  // position only
    b.bitsUnion.bits = 0x03;  // position + normal

    // a requires only position, b has position - compatible
    EXPECT_TRUE(a.compatibleWith(b));

    // b requires position + normal, a only has position - NOT compatible
    EXPECT_FALSE(b.compatibleWith(a));
}

// =============================================================================
// Bitfield Union Consistency Tests
// =============================================================================

TEST_F(RequiredAttributesTest, BitsAndFlagsAreConsistent) {
    // Set via flags
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.tangent = true;

    // Check bits directly
    uint8_t expected = 0x01 | 0x04;  // position + tangent
    EXPECT_EQ(attrs.bitsUnion.bits & 0x3F, expected);
}

TEST_F(RequiredAttributesTest, SetViaBitsReadViaFlags) {
    attrs.bitsUnion.bits = 0x12;  // normal (0x02) + uv0 (0x10)

    EXPECT_FALSE(attrs.bitsUnion.flags.position);
    EXPECT_TRUE(attrs.bitsUnion.flags.normal);
    EXPECT_FALSE(attrs.bitsUnion.flags.tangent);
    EXPECT_FALSE(attrs.bitsUnion.flags.bitangent);
    EXPECT_TRUE(attrs.bitsUnion.flags.uv0);
    EXPECT_FALSE(attrs.bitsUnion.flags.lightmapUV);
}

// =============================================================================
// Common Attribute Combinations
// =============================================================================

TEST_F(RequiredAttributesTest, BasicMeshAttributes) {
    // Typical basic mesh: position + normal + uv0
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.normal = true;
    attrs.bitsUnion.flags.uv0 = true;

    RequiredAttributes available;
    available.bitsUnion.flags.position = true;
    available.bitsUnion.flags.normal = true;
    available.bitsUnion.flags.uv0 = true;

    EXPECT_TRUE(attrs.compatibleWith(available));
}

TEST_F(RequiredAttributesTest, NormalMappedMeshAttributes) {
    // Normal mapped mesh: position + normal + tangent + bitangent + uv0
    attrs.bitsUnion.flags.position = true;
    attrs.bitsUnion.flags.normal = true;
    attrs.bitsUnion.flags.tangent = true;
    attrs.bitsUnion.flags.bitangent = true;
    attrs.bitsUnion.flags.uv0 = true;

    RequiredAttributes basicMesh;
    basicMesh.bitsUnion.flags.position = true;
    basicMesh.bitsUnion.flags.normal = true;
    basicMesh.bitsUnion.flags.uv0 = true;

    // Normal mapped requires more than basic mesh has
    EXPECT_FALSE(attrs.compatibleWith(basicMesh));

    // Basic mesh requirements are satisfied by normal mapped mesh
    EXPECT_TRUE(basicMesh.compatibleWith(attrs));
}

}  // namespace nexo::renderer
