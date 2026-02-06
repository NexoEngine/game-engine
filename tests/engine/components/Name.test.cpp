//// Name.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for NameComponent (memento pattern)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/Name.hpp"

namespace nexo::components {

class NameComponentTest : public ::testing::Test {
protected:
    NameComponent component;
};

// =============================================================================
// Default State Tests
// =============================================================================

TEST_F(NameComponentTest, DefaultNameIsEmpty) {
    EXPECT_TRUE(component.name.empty());
}

// =============================================================================
// Save Tests
// =============================================================================

TEST_F(NameComponentTest, SaveCapturesName) {
    component.name = "TestEntity";
    auto memento = component.save();
    EXPECT_EQ(memento.name, "TestEntity");
}

TEST_F(NameComponentTest, SaveCapturesEmptyName) {
    component.name = "";
    auto memento = component.save();
    EXPECT_TRUE(memento.name.empty());
}

TEST_F(NameComponentTest, SaveCapturesLongName) {
    std::string longName(1000, 'x');
    component.name = longName;
    auto memento = component.save();
    EXPECT_EQ(memento.name.size(), 1000u);
    EXPECT_EQ(memento.name, longName);
}

TEST_F(NameComponentTest, SaveCapturesSpecialCharacters) {
    component.name = "Entity!@#$%^&*()_+-=[]{}|;':\",./<>?";
    auto memento = component.save();
    EXPECT_EQ(memento.name, "Entity!@#$%^&*()_+-=[]{}|;':\",./<>?");
}

TEST_F(NameComponentTest, SaveCapturesWhitespace) {
    component.name = "  Entity With Spaces  ";
    auto memento = component.save();
    EXPECT_EQ(memento.name, "  Entity With Spaces  ");
}

TEST_F(NameComponentTest, SaveCapturesNewlines) {
    component.name = "Line1\nLine2\nLine3";
    auto memento = component.save();
    EXPECT_EQ(memento.name, "Line1\nLine2\nLine3");
}

// =============================================================================
// Restore Tests
// =============================================================================

TEST_F(NameComponentTest, RestoreAppliesName) {
    NameComponent::Memento memento;
    memento.name = "RestoredEntity";

    component.restore(memento);
    EXPECT_EQ(component.name, "RestoredEntity");
}

TEST_F(NameComponentTest, RestoreAppliesEmptyName) {
    component.name = "NotEmpty";

    NameComponent::Memento memento;
    memento.name = "";

    component.restore(memento);
    EXPECT_TRUE(component.name.empty());
}

TEST_F(NameComponentTest, RestoreOverwritesExisting) {
    component.name = "OldName";

    NameComponent::Memento memento;
    memento.name = "NewName";

    component.restore(memento);
    EXPECT_EQ(component.name, "NewName");
}

// =============================================================================
// Round Trip Tests
// =============================================================================

TEST_F(NameComponentTest, SaveRestoreRoundTrip) {
    component.name = "OriginalName";
    auto memento = component.save();

    component.name = "ModifiedName";
    component.restore(memento);

    EXPECT_EQ(component.name, "OriginalName");
}

TEST_F(NameComponentTest, MultipleRoundTrips) {
    component.name = "First";
    auto memento1 = component.save();

    component.name = "Second";
    auto memento2 = component.save();

    component.name = "Third";

    // Restore to second state
    component.restore(memento2);
    EXPECT_EQ(component.name, "Second");

    // Restore to first state
    component.restore(memento1);
    EXPECT_EQ(component.name, "First");
}

TEST_F(NameComponentTest, MementoIndependence) {
    component.name = "State1";
    auto memento1 = component.save();

    component.name = "State2";
    auto memento2 = component.save();

    // Mementos should be independent
    EXPECT_EQ(memento1.name, "State1");
    EXPECT_EQ(memento2.name, "State2");
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST_F(NameComponentTest, UnicodeCharacters) {
    component.name = "Entity_日本語_émoji_🎮";
    auto memento = component.save();

    component.name = "";
    component.restore(memento);

    EXPECT_EQ(component.name, "Entity_日本語_émoji_🎮");
}

TEST_F(NameComponentTest, NullCharacterInName) {
    std::string nameWithNull = "Before";
    nameWithNull += '\0';
    nameWithNull += "After";

    component.name = nameWithNull;
    auto memento = component.save();

    component.name = "";
    component.restore(memento);

    // std::string preserves null characters
    EXPECT_EQ(component.name.size(), nameWithNull.size());
}

}  // namespace nexo::components
