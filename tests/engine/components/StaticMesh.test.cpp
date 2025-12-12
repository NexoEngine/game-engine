//// StaticMesh.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for StaticMeshComponent
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/StaticMesh.hpp"

namespace nexo::components {

class StaticMeshComponentTest : public ::testing::Test {};

// =============================================================================
// Default Initialization Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, DefaultVaoIsNull) {
    StaticMeshComponent mesh;
    EXPECT_EQ(mesh.vao, nullptr);
}

TEST_F(StaticMeshComponentTest, DefaultMeshAttributesAreZero) {
    StaticMeshComponent mesh;
    EXPECT_EQ(mesh.meshAttributes.bitsUnion.bits, 0);
}

// =============================================================================
// Memento Pattern Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, SaveCreatesMemento) {
    StaticMeshComponent mesh;
    auto memento = mesh.save();
    EXPECT_EQ(memento.vao, nullptr);
}

TEST_F(StaticMeshComponentTest, RestoreFromMementoNull) {
    StaticMeshComponent mesh;
    auto memento = mesh.save();

    StaticMeshComponent mesh2;
    mesh2.restore(memento);
    EXPECT_EQ(mesh2.vao, nullptr);
}

TEST_F(StaticMeshComponentTest, MementoPreservesNullVao) {
    StaticMeshComponent mesh;
    auto memento = mesh.save();

    mesh.restore(memento);
    EXPECT_EQ(mesh.vao, nullptr);
}

// =============================================================================
// RequiredAttributes Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, MeshAttributesDefaultFlags) {
    StaticMeshComponent mesh;
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.position);
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.normal);
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.tangent);
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.bitangent);
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.uv0);
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.lightmapUV);
}

TEST_F(StaticMeshComponentTest, MeshAttributesCanBeModified) {
    StaticMeshComponent mesh;
    mesh.meshAttributes.bitsUnion.flags.position = true;
    mesh.meshAttributes.bitsUnion.flags.normal = true;

    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.position);
    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.normal);
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.tangent);
}

TEST_F(StaticMeshComponentTest, MeshAttributesBitsReflectFlags) {
    StaticMeshComponent mesh;
    mesh.meshAttributes.bitsUnion.flags.position = true;
    EXPECT_NE(mesh.meshAttributes.bitsUnion.bits, 0);
}

// =============================================================================
// Type Traits Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, IsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<StaticMeshComponent>);
}

TEST_F(StaticMeshComponentTest, IsCopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<StaticMeshComponent>);
}

TEST_F(StaticMeshComponentTest, IsCopyAssignable) {
    EXPECT_TRUE(std::is_copy_assignable_v<StaticMeshComponent>);
}

TEST_F(StaticMeshComponentTest, IsMoveConstructible) {
    EXPECT_TRUE(std::is_move_constructible_v<StaticMeshComponent>);
}

TEST_F(StaticMeshComponentTest, IsMoveAssignable) {
    EXPECT_TRUE(std::is_move_assignable_v<StaticMeshComponent>);
}

// =============================================================================
// Memento Type Traits Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, MementoIsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<StaticMeshComponent::Memento>);
}

TEST_F(StaticMeshComponentTest, MementoIsCopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<StaticMeshComponent::Memento>);
}

// =============================================================================
// Copy Semantics Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, CopyConstruction) {
    StaticMeshComponent original;
    original.meshAttributes.bitsUnion.flags.position = true;
    original.meshAttributes.bitsUnion.flags.normal = true;

    StaticMeshComponent copy = original;
    EXPECT_EQ(copy.vao, original.vao);  // Shared pointer is copied
    EXPECT_TRUE(copy.meshAttributes.bitsUnion.flags.position);
    EXPECT_TRUE(copy.meshAttributes.bitsUnion.flags.normal);
}

TEST_F(StaticMeshComponentTest, CopyAssignment) {
    StaticMeshComponent original;
    original.meshAttributes.bitsUnion.flags.uv0 = true;

    StaticMeshComponent copy;
    copy = original;
    EXPECT_EQ(copy.vao, original.vao);
    EXPECT_TRUE(copy.meshAttributes.bitsUnion.flags.uv0);
}

// =============================================================================
// Move Semantics Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, MoveConstruction) {
    StaticMeshComponent original;
    original.meshAttributes.bitsUnion.flags.position = true;
    original.meshAttributes.bitsUnion.flags.normal = true;

    StaticMeshComponent moved = std::move(original);
    EXPECT_TRUE(moved.meshAttributes.bitsUnion.flags.position);
    EXPECT_TRUE(moved.meshAttributes.bitsUnion.flags.normal);
}

TEST_F(StaticMeshComponentTest, MoveAssignment) {
    StaticMeshComponent original;
    original.meshAttributes.bitsUnion.flags.tangent = true;

    StaticMeshComponent moved;
    moved = std::move(original);
    EXPECT_TRUE(moved.meshAttributes.bitsUnion.flags.tangent);
}

// =============================================================================
// Memento Save/Restore Comprehensive Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, MementoDoesNotCaptureAttributes) {
    StaticMeshComponent mesh;
    mesh.meshAttributes.bitsUnion.flags.position = true;
    mesh.meshAttributes.bitsUnion.flags.normal = true;

    auto memento = mesh.save();

    // Memento only stores vao, not meshAttributes
    // After restore, meshAttributes should remain unchanged
    StaticMeshComponent mesh2;
    mesh2.meshAttributes.bitsUnion.flags.uv0 = true;

    mesh2.restore(memento);

    // meshAttributes should not be affected by restore
    EXPECT_TRUE(mesh2.meshAttributes.bitsUnion.flags.uv0);
    EXPECT_FALSE(mesh2.meshAttributes.bitsUnion.flags.position);
}

TEST_F(StaticMeshComponentTest, SaveRestoreRoundTripVao) {
    StaticMeshComponent mesh;
    // Set vao to nullptr initially
    mesh.vao = nullptr;

    auto memento = mesh.save();

    // Verify memento captured the state
    EXPECT_EQ(memento.vao, nullptr);

    // Restore and verify
    mesh.restore(memento);
    EXPECT_EQ(mesh.vao, nullptr);
}

TEST_F(StaticMeshComponentTest, RestoreOverwritesVao) {
    StaticMeshComponent mesh;
    mesh.vao = nullptr;

    StaticMeshComponent::Memento memento;
    memento.vao = nullptr;

    mesh.restore(memento);
    EXPECT_EQ(mesh.vao, nullptr);
}

// =============================================================================
// MeshAttributes Comprehensive Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, MeshAttributesAllFlagsCanBeSet) {
    StaticMeshComponent mesh;

    mesh.meshAttributes.bitsUnion.flags.position = true;
    mesh.meshAttributes.bitsUnion.flags.normal = true;
    mesh.meshAttributes.bitsUnion.flags.tangent = true;
    mesh.meshAttributes.bitsUnion.flags.bitangent = true;
    mesh.meshAttributes.bitsUnion.flags.uv0 = true;
    mesh.meshAttributes.bitsUnion.flags.lightmapUV = true;

    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.position);
    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.normal);
    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.tangent);
    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.bitangent);
    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.uv0);
    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.lightmapUV);
}

TEST_F(StaticMeshComponentTest, MeshAttributesAllFlagsSetBitsNonZero) {
    StaticMeshComponent mesh;

    mesh.meshAttributes.bitsUnion.flags.position = true;
    mesh.meshAttributes.bitsUnion.flags.normal = true;
    mesh.meshAttributes.bitsUnion.flags.tangent = true;
    mesh.meshAttributes.bitsUnion.flags.bitangent = true;
    mesh.meshAttributes.bitsUnion.flags.uv0 = true;
    mesh.meshAttributes.bitsUnion.flags.lightmapUV = true;

    // When all flags are set, bits should be non-zero
    EXPECT_NE(mesh.meshAttributes.bitsUnion.bits, 0);
}

TEST_F(StaticMeshComponentTest, MeshAttributesBitsManipulation) {
    StaticMeshComponent mesh;

    // Set via bits directly
    mesh.meshAttributes.bitsUnion.bits = 0b00000001;  // position only
    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.position);
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.normal);
}

TEST_F(StaticMeshComponentTest, MeshAttributesIndividualFlagToggle) {
    StaticMeshComponent mesh;

    // Set a flag
    mesh.meshAttributes.bitsUnion.flags.position = true;
    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.position);

    // Unset the flag
    mesh.meshAttributes.bitsUnion.flags.position = false;
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.position);
    EXPECT_EQ(mesh.meshAttributes.bitsUnion.bits, 0);
}

TEST_F(StaticMeshComponentTest, MeshAttributesMultipleFlagsToggle) {
    StaticMeshComponent mesh;

    mesh.meshAttributes.bitsUnion.flags.position = true;
    mesh.meshAttributes.bitsUnion.flags.normal = true;
    mesh.meshAttributes.bitsUnion.flags.uv0 = true;

    uint8_t initialBits = mesh.meshAttributes.bitsUnion.bits;
    EXPECT_NE(initialBits, 0);

    // Toggle one flag off
    mesh.meshAttributes.bitsUnion.flags.normal = false;
    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.position);
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.normal);
    EXPECT_TRUE(mesh.meshAttributes.bitsUnion.flags.uv0);
    EXPECT_NE(mesh.meshAttributes.bitsUnion.bits, initialBits);
}

// =============================================================================
// RequiredAttributes Equality Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, MeshAttributesEqualityEmpty) {
    StaticMeshComponent mesh1;
    StaticMeshComponent mesh2;

    EXPECT_TRUE(mesh1.meshAttributes == mesh2.meshAttributes);
}

TEST_F(StaticMeshComponentTest, MeshAttributesEqualitySame) {
    StaticMeshComponent mesh1;
    mesh1.meshAttributes.bitsUnion.flags.position = true;
    mesh1.meshAttributes.bitsUnion.flags.normal = true;

    StaticMeshComponent mesh2;
    mesh2.meshAttributes.bitsUnion.flags.position = true;
    mesh2.meshAttributes.bitsUnion.flags.normal = true;

    EXPECT_TRUE(mesh1.meshAttributes == mesh2.meshAttributes);
}

TEST_F(StaticMeshComponentTest, MeshAttributesEqualityDifferent) {
    StaticMeshComponent mesh1;
    mesh1.meshAttributes.bitsUnion.flags.position = true;

    StaticMeshComponent mesh2;
    mesh2.meshAttributes.bitsUnion.flags.normal = true;

    EXPECT_FALSE(mesh1.meshAttributes == mesh2.meshAttributes);
}

// =============================================================================
// RequiredAttributes Compatibility Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, MeshAttributesCompatibilitySubset) {
    StaticMeshComponent required;
    required.meshAttributes.bitsUnion.flags.position = true;

    StaticMeshComponent provided;
    provided.meshAttributes.bitsUnion.flags.position = true;
    provided.meshAttributes.bitsUnion.flags.normal = true;
    provided.meshAttributes.bitsUnion.flags.uv0 = true;

    // Provided has all required attributes (and more)
    EXPECT_TRUE(required.meshAttributes.compatibleWith(provided.meshAttributes));
}

TEST_F(StaticMeshComponentTest, MeshAttributesCompatibilityExact) {
    StaticMeshComponent mesh1;
    mesh1.meshAttributes.bitsUnion.flags.position = true;
    mesh1.meshAttributes.bitsUnion.flags.normal = true;

    StaticMeshComponent mesh2;
    mesh2.meshAttributes.bitsUnion.flags.position = true;
    mesh2.meshAttributes.bitsUnion.flags.normal = true;

    EXPECT_TRUE(mesh1.meshAttributes.compatibleWith(mesh2.meshAttributes));
}

TEST_F(StaticMeshComponentTest, MeshAttributesCompatibilityMissing) {
    StaticMeshComponent required;
    required.meshAttributes.bitsUnion.flags.position = true;
    required.meshAttributes.bitsUnion.flags.normal = true;
    required.meshAttributes.bitsUnion.flags.uv0 = true;

    StaticMeshComponent provided;
    provided.meshAttributes.bitsUnion.flags.position = true;
    provided.meshAttributes.bitsUnion.flags.normal = true;
    // Missing uv0

    // Provided is missing a required attribute
    EXPECT_FALSE(required.meshAttributes.compatibleWith(provided.meshAttributes));
}

TEST_F(StaticMeshComponentTest, MeshAttributesCompatibilityEmpty) {
    StaticMeshComponent required;
    // No required attributes

    StaticMeshComponent provided;
    provided.meshAttributes.bitsUnion.flags.position = true;

    // Empty requirements are always satisfied
    EXPECT_TRUE(required.meshAttributes.compatibleWith(provided.meshAttributes));
}

TEST_F(StaticMeshComponentTest, MeshAttributesCompatibilityBothEmpty) {
    StaticMeshComponent required;
    StaticMeshComponent provided;

    EXPECT_TRUE(required.meshAttributes.compatibleWith(provided.meshAttributes));
}

// =============================================================================
// Edge Cases Tests
// =============================================================================

TEST_F(StaticMeshComponentTest, VaoSharedPointerSharing) {
    StaticMeshComponent mesh1;
    mesh1.vao = nullptr;

    StaticMeshComponent mesh2 = mesh1;

    // Both should share the same vao pointer
    EXPECT_EQ(mesh1.vao, mesh2.vao);
}

TEST_F(StaticMeshComponentTest, AttributesBitfieldPreservation) {
    StaticMeshComponent mesh;

    // Set specific bit pattern
    mesh.meshAttributes.bitsUnion.bits = 0b00101010;

    StaticMeshComponent copy = mesh;

    // Bit pattern should be preserved
    EXPECT_EQ(copy.meshAttributes.bitsUnion.bits, 0b00101010);
}

TEST_F(StaticMeshComponentTest, MementoIndependence) {
    StaticMeshComponent mesh;
    mesh.vao = nullptr;

    auto memento1 = mesh.save();
    auto memento2 = mesh.save();

    // Multiple mementos should be independent
    EXPECT_EQ(memento1.vao, memento2.vao);
}

TEST_F(StaticMeshComponentTest, RestoreDoesNotAffectOtherInstances) {
    StaticMeshComponent mesh1;
    mesh1.vao = nullptr;

    StaticMeshComponent mesh2;
    mesh2.vao = nullptr;

    auto memento = mesh1.save();

    mesh2.restore(memento);

    // Both should still have nullptr vao
    EXPECT_EQ(mesh1.vao, nullptr);
    EXPECT_EQ(mesh2.vao, nullptr);
}

TEST_F(StaticMeshComponentTest, AttributesCopyPreservesAllFlags) {
    StaticMeshComponent original;
    original.meshAttributes.bitsUnion.flags.position = true;
    original.meshAttributes.bitsUnion.flags.normal = false;
    original.meshAttributes.bitsUnion.flags.tangent = true;
    original.meshAttributes.bitsUnion.flags.bitangent = false;
    original.meshAttributes.bitsUnion.flags.uv0 = true;
    original.meshAttributes.bitsUnion.flags.lightmapUV = false;

    StaticMeshComponent copy = original;

    EXPECT_TRUE(copy.meshAttributes.bitsUnion.flags.position);
    EXPECT_FALSE(copy.meshAttributes.bitsUnion.flags.normal);
    EXPECT_TRUE(copy.meshAttributes.bitsUnion.flags.tangent);
    EXPECT_FALSE(copy.meshAttributes.bitsUnion.flags.bitangent);
    EXPECT_TRUE(copy.meshAttributes.bitsUnion.flags.uv0);
    EXPECT_FALSE(copy.meshAttributes.bitsUnion.flags.lightmapUV);
}

TEST_F(StaticMeshComponentTest, AttributesBitsClearOnReset) {
    StaticMeshComponent mesh;
    mesh.meshAttributes.bitsUnion.flags.position = true;
    mesh.meshAttributes.bitsUnion.flags.normal = true;

    EXPECT_NE(mesh.meshAttributes.bitsUnion.bits, 0);

    // Reset all bits
    mesh.meshAttributes.bitsUnion.bits = 0;

    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.position);
    EXPECT_FALSE(mesh.meshAttributes.bitsUnion.flags.normal);
}

}  // namespace nexo::components
