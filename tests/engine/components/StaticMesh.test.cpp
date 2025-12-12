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

}  // namespace nexo::components
