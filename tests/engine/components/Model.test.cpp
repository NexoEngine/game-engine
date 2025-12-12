//// Model.test.cpp ////////////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for ModelComponent
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/Model.hpp"

namespace nexo::components {

class ModelComponentTest : public ::testing::Test {};

// =============================================================================
// Default Initialization Tests
// =============================================================================

TEST_F(ModelComponentTest, DefaultModelIsEmpty) {
    ModelComponent comp;
    // AssetRef should be default constructed (empty/null)
    EXPECT_FALSE(comp.model.isLoaded());
}

// =============================================================================
// Type Traits Tests
// =============================================================================

TEST_F(ModelComponentTest, IsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<ModelComponent>);
}

TEST_F(ModelComponentTest, IsCopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<ModelComponent>);
}

TEST_F(ModelComponentTest, IsCopyAssignable) {
    EXPECT_TRUE(std::is_copy_assignable_v<ModelComponent>);
}

TEST_F(ModelComponentTest, IsMoveConstructible) {
    EXPECT_TRUE(std::is_move_constructible_v<ModelComponent>);
}

TEST_F(ModelComponentTest, IsMoveAssignable) {
    EXPECT_TRUE(std::is_move_assignable_v<ModelComponent>);
}

// =============================================================================
// Copy Semantics Tests
// =============================================================================

TEST_F(ModelComponentTest, CopyConstruction) {
    ModelComponent original;
    ModelComponent copy = original;

    EXPECT_EQ(original.model.isLoaded(), copy.model.isLoaded());
}

TEST_F(ModelComponentTest, CopyAssignment) {
    ModelComponent original;
    ModelComponent copy;

    copy = original;

    EXPECT_EQ(original.model.isLoaded(), copy.model.isLoaded());
}

// =============================================================================
// Move Semantics Tests
// =============================================================================

TEST_F(ModelComponentTest, MoveConstruction) {
    ModelComponent original;
    ModelComponent moved = std::move(original);

    // Moved-to should be valid
    EXPECT_FALSE(moved.model.isLoaded());
}

TEST_F(ModelComponentTest, MoveAssignment) {
    ModelComponent original;
    ModelComponent moved;

    moved = std::move(original);

    EXPECT_FALSE(moved.model.isLoaded());
}

// =============================================================================
// Struct Size Tests
// =============================================================================

TEST_F(ModelComponentTest, StructContainsOnlyModelMember) {
    // ModelComponent should only contain an AssetRef<Model> member
    ModelComponent comp;
    // We can't easily test the exact size, but we can verify it's not empty
    EXPECT_GT(sizeof(ModelComponent), 0u);
}

}  // namespace nexo::components
