//// MaterialComponent.test.cpp ////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for MaterialComponent (wrapper for Material asset)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/MaterialComponent.hpp"

namespace nexo::components {

class MaterialComponentTest : public ::testing::Test {};

// =============================================================================
// Default Initialization Tests
// =============================================================================

TEST_F(MaterialComponentTest, DefaultMaterialIsEmpty) {
    MaterialComponent comp;
    // AssetRef should be default constructed (empty/null)
    EXPECT_FALSE(comp.material.isLoaded());
}

// =============================================================================
// Memento Pattern Tests
// =============================================================================

TEST_F(MaterialComponentTest, SaveCreatesMemento) {
    MaterialComponent comp;
    auto memento = comp.save();
    EXPECT_FALSE(memento.material.isLoaded());
}

TEST_F(MaterialComponentTest, RestoreFromMemento) {
    MaterialComponent comp1;
    auto memento = comp1.save();

    MaterialComponent comp2;
    comp2.restore(memento);

    // Both should have same material state
    EXPECT_EQ(comp1.material.isLoaded(), comp2.material.isLoaded());
}

TEST_F(MaterialComponentTest, MementoPreservesMaterial) {
    MaterialComponent comp;
    auto memento = comp.save();

    // Modify original
    MaterialComponent comp2;
    comp2.restore(memento);

    // Restored should match saved state
    EXPECT_FALSE(comp2.material.isLoaded());
}

// =============================================================================
// Type Traits Tests
// =============================================================================

TEST_F(MaterialComponentTest, IsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<MaterialComponent>);
}

TEST_F(MaterialComponentTest, IsCopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<MaterialComponent>);
}

TEST_F(MaterialComponentTest, IsCopyAssignable) {
    EXPECT_TRUE(std::is_copy_assignable_v<MaterialComponent>);
}

TEST_F(MaterialComponentTest, IsMoveConstructible) {
    EXPECT_TRUE(std::is_move_constructible_v<MaterialComponent>);
}

TEST_F(MaterialComponentTest, IsMoveAssignable) {
    EXPECT_TRUE(std::is_move_assignable_v<MaterialComponent>);
}

// =============================================================================
// Memento Type Traits Tests
// =============================================================================

TEST_F(MaterialComponentTest, MementoIsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<MaterialComponent::Memento>);
}

TEST_F(MaterialComponentTest, MementoIsCopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<MaterialComponent::Memento>);
}

TEST_F(MaterialComponentTest, MementoIsCopyAssignable) {
    EXPECT_TRUE(std::is_copy_assignable_v<MaterialComponent::Memento>);
}

// =============================================================================
// Copy Semantics Tests
// =============================================================================

TEST_F(MaterialComponentTest, CopyConstruction) {
    MaterialComponent original;
    MaterialComponent copy = original;

    EXPECT_EQ(original.material.isLoaded(), copy.material.isLoaded());
}

TEST_F(MaterialComponentTest, CopyAssignment) {
    MaterialComponent original;
    MaterialComponent copy;

    copy = original;

    EXPECT_EQ(original.material.isLoaded(), copy.material.isLoaded());
}

// =============================================================================
// Move Semantics Tests
// =============================================================================

TEST_F(MaterialComponentTest, MoveConstruction) {
    MaterialComponent original;
    MaterialComponent moved = std::move(original);

    // Moved-to should be valid
    EXPECT_FALSE(moved.material.isLoaded());
}

TEST_F(MaterialComponentTest, MoveAssignment) {
    MaterialComponent original;
    MaterialComponent moved;

    moved = std::move(original);

    EXPECT_FALSE(moved.material.isLoaded());
}

}  // namespace nexo::components
