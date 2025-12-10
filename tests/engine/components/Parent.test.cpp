//// Parent.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for ParentComponent and RootComponent (memento pattern)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/Parent.hpp"

namespace nexo::components {

// =============================================================================
// ParentComponent Tests
// =============================================================================

class ParentComponentTest : public ::testing::Test {
protected:
    ParentComponent component;
};

TEST_F(ParentComponentTest, DefaultParentIsZero) {
    // Default Entity value
    EXPECT_EQ(component.parent, 0u);
}

TEST_F(ParentComponentTest, SaveCapturesParent) {
    component.parent = 42;
    auto memento = component.save();
    EXPECT_EQ(memento.parent, 42u);
}

TEST_F(ParentComponentTest, RestoreAppliesParent) {
    ParentComponent::Memento memento;
    memento.parent = 123;

    component.restore(memento);
    EXPECT_EQ(component.parent, 123u);
}

TEST_F(ParentComponentTest, SaveRestoreRoundTrip) {
    component.parent = 999;
    auto memento = component.save();

    component.parent = 0;
    component.restore(memento);

    EXPECT_EQ(component.parent, 999u);
}

TEST_F(ParentComponentTest, MementoIndependence) {
    component.parent = 100;
    auto memento1 = component.save();

    component.parent = 200;
    auto memento2 = component.save();

    EXPECT_EQ(memento1.parent, 100u);
    EXPECT_EQ(memento2.parent, 200u);
}

TEST_F(ParentComponentTest, LargeEntityValue) {
    component.parent = 4999;  // MAX_ENTITIES - 1
    auto memento = component.save();

    component.parent = 0;
    component.restore(memento);

    EXPECT_EQ(component.parent, 4999u);
}

// =============================================================================
// RootComponent Tests
// =============================================================================

class RootComponentTest : public ::testing::Test {
protected:
    RootComponent component;
};

TEST_F(RootComponentTest, DefaultName) {
    EXPECT_EQ(component.name, "Root");
}

TEST_F(RootComponentTest, DefaultChildCount) {
    EXPECT_EQ(component.childCount, 0);
}

TEST_F(RootComponentTest, DefaultModelRefIsNull) {
    // Default AssetRef should be null/invalid
    EXPECT_FALSE(component.modelRef.isValid());
}

TEST_F(RootComponentTest, SaveCapturesName) {
    component.name = "CustomRoot";
    auto memento = component.save();
    EXPECT_EQ(memento.name, "CustomRoot");
}

TEST_F(RootComponentTest, SaveCapturesChildCount) {
    component.childCount = 5;
    auto memento = component.save();
    EXPECT_EQ(memento.childCount, 5);
}

TEST_F(RootComponentTest, RestoreAppliesName) {
    RootComponent::Memento memento;
    memento.name = "RestoredRoot";
    memento.childCount = 0;
    // modelRef left as default (null)

    component.restore(memento);
    EXPECT_EQ(component.name, "RestoredRoot");
}

TEST_F(RootComponentTest, RestoreAppliesChildCount) {
    RootComponent::Memento memento;
    memento.name = "Root";
    memento.childCount = 10;

    component.restore(memento);
    EXPECT_EQ(component.childCount, 10);
}

TEST_F(RootComponentTest, SaveRestoreRoundTripName) {
    component.name = "OriginalName";
    auto memento = component.save();

    component.name = "ModifiedName";
    component.restore(memento);

    EXPECT_EQ(component.name, "OriginalName");
}

TEST_F(RootComponentTest, SaveRestoreRoundTripChildCount) {
    component.childCount = 7;
    auto memento = component.save();

    component.childCount = 0;
    component.restore(memento);

    EXPECT_EQ(component.childCount, 7);
}

TEST_F(RootComponentTest, SaveRestoreRoundTripAll) {
    component.name = "TestRoot";
    component.childCount = 15;
    auto memento = component.save();

    component.name = "Changed";
    component.childCount = 0;
    component.restore(memento);

    EXPECT_EQ(component.name, "TestRoot");
    EXPECT_EQ(component.childCount, 15);
}

TEST_F(RootComponentTest, MementoIndependence) {
    component.name = "First";
    component.childCount = 1;
    auto memento1 = component.save();

    component.name = "Second";
    component.childCount = 2;
    auto memento2 = component.save();

    EXPECT_EQ(memento1.name, "First");
    EXPECT_EQ(memento1.childCount, 1);
    EXPECT_EQ(memento2.name, "Second");
    EXPECT_EQ(memento2.childCount, 2);
}

TEST_F(RootComponentTest, NegativeChildCount) {
    component.childCount = -1;
    auto memento = component.save();

    component.childCount = 0;
    component.restore(memento);

    EXPECT_EQ(component.childCount, -1);
}

TEST_F(RootComponentTest, EmptyName) {
    component.name = "";
    auto memento = component.save();

    component.name = "NotEmpty";
    component.restore(memento);

    EXPECT_TRUE(component.name.empty());
}

TEST_F(RootComponentTest, LongName) {
    std::string longName(500, 'x');
    component.name = longName;
    auto memento = component.save();

    component.name = "";
    component.restore(memento);

    EXPECT_EQ(component.name.size(), 500u);
    EXPECT_EQ(component.name, longName);
}

TEST_F(RootComponentTest, SpecialCharactersInName) {
    component.name = "Root/子节点/émoji_🌳";
    auto memento = component.save();

    component.name = "";
    component.restore(memento);

    EXPECT_EQ(component.name, "Root/子节点/émoji_🌳");
}

}  // namespace nexo::components
