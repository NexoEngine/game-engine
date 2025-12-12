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

TEST_F(ModelComponentTest, DefaultModelIsNotValid) {
    ModelComponent comp;
    EXPECT_FALSE(comp.model.isValid());
}

TEST_F(ModelComponentTest, DefaultModelIsNull) {
    ModelComponent comp;
    EXPECT_EQ(comp.model, nullptr);
}

TEST_F(ModelComponentTest, DefaultModelLockReturnsNull) {
    ModelComponent comp;
    auto ptr = comp.model.lock();
    EXPECT_EQ(ptr, nullptr);
}

// =============================================================================
// Field Assignment Tests
// =============================================================================

TEST_F(ModelComponentTest, AssignNullAssetRef) {
    ModelComponent comp;
    comp.model = assets::AssetRef<assets::Model>::null();

    EXPECT_FALSE(comp.model.isValid());
    EXPECT_FALSE(comp.model.isLoaded());
    EXPECT_EQ(comp.model, nullptr);
}

TEST_F(ModelComponentTest, AssignNullptr) {
    ModelComponent comp;
    comp.model = nullptr;

    EXPECT_FALSE(comp.model.isValid());
    EXPECT_EQ(comp.model, nullptr);
}

TEST_F(ModelComponentTest, AssetRefCanBeReassigned) {
    ModelComponent comp;
    comp.model = assets::AssetRef<assets::Model>::null();
    comp.model = nullptr;

    EXPECT_FALSE(comp.model.isValid());
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

TEST_F(ModelComponentTest, IsTriviallyDestructible) {
    // AssetRef contains weak_ptr which is not trivially destructible
    EXPECT_FALSE(std::is_trivially_destructible_v<ModelComponent>);
}

TEST_F(ModelComponentTest, IsStandardLayout) {
    // AssetRef inherits from GenericAssetRef which has virtual destructor
    // so ModelComponent is not standard layout
    EXPECT_FALSE(std::is_standard_layout_v<ModelComponent>);
}

// =============================================================================
// Copy Semantics Tests
// =============================================================================

TEST_F(ModelComponentTest, CopyConstruction) {
    ModelComponent original;
    ModelComponent copy = original;

    EXPECT_EQ(original.model.isLoaded(), copy.model.isLoaded());
    EXPECT_EQ(original.model.isValid(), copy.model.isValid());
}

TEST_F(ModelComponentTest, CopyAssignment) {
    ModelComponent original;
    ModelComponent copy;

    copy = original;

    EXPECT_EQ(original.model.isLoaded(), copy.model.isLoaded());
    EXPECT_EQ(original.model.isValid(), copy.model.isValid());
}

TEST_F(ModelComponentTest, CopyPreservesNullState) {
    ModelComponent original;
    original.model = nullptr;

    ModelComponent copy = original;

    EXPECT_EQ(copy.model, nullptr);
    EXPECT_FALSE(copy.model.isValid());
}

TEST_F(ModelComponentTest, CopyAssignmentOverwritesExisting) {
    ModelComponent original;
    original.model = assets::AssetRef<assets::Model>::null();

    ModelComponent copy;
    copy.model = nullptr;

    copy = original;

    EXPECT_EQ(original.model.isValid(), copy.model.isValid());
    EXPECT_EQ(original.model, copy.model);
}

// =============================================================================
// Move Semantics Tests
// =============================================================================

TEST_F(ModelComponentTest, MoveConstruction) {
    ModelComponent original;
    ModelComponent moved = std::move(original);

    // Moved-to should be valid (even if null)
    EXPECT_FALSE(moved.model.isLoaded());
}

TEST_F(ModelComponentTest, MoveAssignment) {
    ModelComponent original;
    ModelComponent moved;

    moved = std::move(original);

    EXPECT_FALSE(moved.model.isLoaded());
}

TEST_F(ModelComponentTest, MoveFromNullAssetRef) {
    ModelComponent original;
    original.model = nullptr;

    ModelComponent moved = std::move(original);

    EXPECT_FALSE(moved.model.isValid());
    EXPECT_EQ(moved.model, nullptr);
}

TEST_F(ModelComponentTest, MoveAssignmentFromNull) {
    ModelComponent original;
    original.model = assets::AssetRef<assets::Model>::null();

    ModelComponent moved;
    moved = std::move(original);

    EXPECT_FALSE(moved.model.isValid());
}

// =============================================================================
// AssetRef Behavior Tests
// =============================================================================

TEST_F(ModelComponentTest, AssetRefBoolConversionOnDefault) {
    ModelComponent comp;
    EXPECT_FALSE(static_cast<bool>(comp.model));
}

TEST_F(ModelComponentTest, AssetRefBoolConversionOnNull) {
    ModelComponent comp;
    comp.model = nullptr;
    EXPECT_FALSE(static_cast<bool>(comp.model));
}

TEST_F(ModelComponentTest, AssetRefEqualityWithNull) {
    ModelComponent comp;
    EXPECT_TRUE(comp.model == nullptr);
    EXPECT_FALSE(comp.model != nullptr);
}

TEST_F(ModelComponentTest, TwoDefaultAssetRefsAreEqual) {
    ModelComponent comp1;
    ModelComponent comp2;

    EXPECT_TRUE(comp1.model == comp2.model);
    EXPECT_FALSE(comp1.model != comp2.model);
}

TEST_F(ModelComponentTest, NullAssetRefsAreEqual) {
    ModelComponent comp1;
    comp1.model = nullptr;

    ModelComponent comp2;
    comp2.model = assets::AssetRef<assets::Model>::null();

    EXPECT_TRUE(comp1.model == comp2.model);
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

TEST_F(ModelComponentTest, StructSizeMatchesAssetRef) {
    // ModelComponent should be the same size as AssetRef since it only contains one
    EXPECT_EQ(sizeof(ModelComponent), sizeof(assets::AssetRef<assets::Model>));
}

// =============================================================================
// Edge Cases and Boundary Tests
// =============================================================================

TEST_F(ModelComponentTest, SelfAssignment) {
    ModelComponent comp;
    comp.model = assets::AssetRef<assets::Model>::null();

    // Self-assignment should not cause issues
    comp = comp;

    EXPECT_FALSE(comp.model.isValid());
}

TEST_F(ModelComponentTest, MultipleNullAssignments) {
    ModelComponent comp;

    // Multiple null assignments should be safe
    comp.model = nullptr;
    comp.model = nullptr;
    comp.model = assets::AssetRef<assets::Model>::null();
    comp.model = nullptr;

    EXPECT_FALSE(comp.model.isValid());
    EXPECT_EQ(comp.model, nullptr);
}

TEST_F(ModelComponentTest, ChainedCopyAssignment) {
    ModelComponent comp1;
    ModelComponent comp2;
    ModelComponent comp3;

    comp1.model = nullptr;

    // Chained assignment
    comp3 = comp2 = comp1;

    EXPECT_EQ(comp1.model, comp2.model);
    EXPECT_EQ(comp2.model, comp3.model);
    EXPECT_FALSE(comp3.model.isValid());
}

TEST_F(ModelComponentTest, TemporaryAssetRefAssignment) {
    ModelComponent comp;

    // Assign a temporary AssetRef
    comp.model = assets::AssetRef<assets::Model>();

    EXPECT_FALSE(comp.model.isValid());
    EXPECT_FALSE(comp.model.isLoaded());
}

// =============================================================================
// Component Lifecycle Tests
// =============================================================================

TEST_F(ModelComponentTest, DefaultConstructionDoesNotThrow) {
    EXPECT_NO_THROW({
        ModelComponent comp;
    });
}

TEST_F(ModelComponentTest, CopyConstructionDoesNotThrow) {
    ModelComponent original;

    EXPECT_NO_THROW({
        ModelComponent copy = original;
    });
}

TEST_F(ModelComponentTest, MoveConstructionDoesNotThrow) {
    ModelComponent original;

    EXPECT_NO_THROW({
        ModelComponent moved = std::move(original);
    });
}

TEST_F(ModelComponentTest, AssignmentDoesNotThrow) {
    ModelComponent comp;

    EXPECT_NO_THROW({
        comp.model = nullptr;
        comp.model = assets::AssetRef<assets::Model>::null();
    });
}

TEST_F(ModelComponentTest, DestructionDoesNotThrow) {
    EXPECT_NO_THROW({
        ModelComponent comp;
        comp.model = nullptr;
        // Destructor called at end of scope
    });
}

// =============================================================================
// Integration Tests
// =============================================================================

TEST_F(ModelComponentTest, ComponentArrayUsage) {
    // Test that ModelComponent can be used in arrays
    ModelComponent components[10];

    for (auto& comp : components) {
        EXPECT_FALSE(comp.model.isValid());
    }
}

TEST_F(ModelComponentTest, VectorStorageAndResize) {
    // Test that ModelComponent works correctly in std::vector
    std::vector<ModelComponent> components;

    // Initial capacity
    components.reserve(100);

    // Add components
    for (int i = 0; i < 100; ++i) {
        components.emplace_back();
    }

    EXPECT_EQ(components.size(), 100u);

    for (const auto& comp : components) {
        EXPECT_FALSE(comp.model.isValid());
    }
}

TEST_F(ModelComponentTest, SwapComponents) {
    ModelComponent comp1;
    comp1.model = nullptr;

    ModelComponent comp2;
    comp2.model = assets::AssetRef<assets::Model>::null();

    using std::swap;
    swap(comp1, comp2);

    // Both should still be null/invalid after swap
    EXPECT_FALSE(comp1.model.isValid());
    EXPECT_FALSE(comp2.model.isValid());
}

}  // namespace nexo::components
