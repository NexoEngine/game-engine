//// PhysicsBodyComponent.test.cpp ////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for PhysicsBodyComponent (memento pattern)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include "components/PhysicsBodyComponent.hpp"
#include <type_traits>

namespace nexo::components {

class PhysicsBodyComponentTest : public ::testing::Test {
protected:
    PhysicsBodyComponent component;
};

// =============================================================================
// Type Enum Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, TypeEnumStaticExists) {
    PhysicsBodyComponent::Type type = PhysicsBodyComponent::Type::Static;
    EXPECT_EQ(type, PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsBodyComponentTest, TypeEnumDynamicExists) {
    PhysicsBodyComponent::Type type = PhysicsBodyComponent::Type::Dynamic;
    EXPECT_EQ(type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, TypeEnumValuesAreDistinct) {
    EXPECT_NE(PhysicsBodyComponent::Type::Static, PhysicsBodyComponent::Type::Dynamic);
}

// =============================================================================
// Default Initialization Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, DefaultTypeInitialization) {
    PhysicsBodyComponent comp;
    // Default initialization should use value initialization (Type{})
    // Without explicit initialization in the struct, the enum would be uninitialized
    // But the struct definition has type{} which value-initializes it to the first enum value
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsBodyComponentTest, DefaultBodyIDInitialization) {
    PhysicsBodyComponent comp;
    // JPH::BodyID default constructor creates an invalid ID
    EXPECT_TRUE(comp.bodyID.IsInvalid());
}

// =============================================================================
// Save Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, SaveCapturesBodyID) {
    JPH::BodyID testID(42);
    component.bodyID = testID;
    component.type = PhysicsBodyComponent::Type::Dynamic;

    auto memento = component.save();

    EXPECT_EQ(memento.bodyID.GetIndex(), testID.GetIndex());
}

TEST_F(PhysicsBodyComponentTest, SaveCapturesType) {
    component.bodyID = JPH::BodyID(100);
    component.type = PhysicsBodyComponent::Type::Dynamic;

    auto memento = component.save();

    EXPECT_EQ(memento.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, SaveCapturesStaticType) {
    component.bodyID = JPH::BodyID(200);
    component.type = PhysicsBodyComponent::Type::Static;

    auto memento = component.save();

    EXPECT_EQ(memento.type, PhysicsBodyComponent::Type::Static);
}

// =============================================================================
// Restore Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, RestoreAppliesBodyID) {
    JPH::BodyID testID(999);
    PhysicsBodyComponent::Memento memento;
    memento.bodyID = testID;
    memento.type = PhysicsBodyComponent::Type::Static;

    component.restore(memento);

    EXPECT_EQ(component.bodyID.GetIndex(), testID.GetIndex());
}

TEST_F(PhysicsBodyComponentTest, RestoreAppliesType) {
    PhysicsBodyComponent::Memento memento;
    memento.bodyID = JPH::BodyID(123);
    memento.type = PhysicsBodyComponent::Type::Dynamic;

    component.restore(memento);

    EXPECT_EQ(component.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, RestoreAppliesStaticType) {
    PhysicsBodyComponent::Memento memento;
    memento.bodyID = JPH::BodyID(456);
    memento.type = PhysicsBodyComponent::Type::Static;

    component.restore(memento);

    EXPECT_EQ(component.type, PhysicsBodyComponent::Type::Static);
}

// =============================================================================
// Round-Trip Save/Restore Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, SaveRestoreRoundTripDynamic) {
    // Set up initial state
    component.bodyID = JPH::BodyID(777);
    component.type = PhysicsBodyComponent::Type::Dynamic;

    // Save state
    auto memento = component.save();

    // Modify component
    component.bodyID = JPH::BodyID(0);
    component.type = PhysicsBodyComponent::Type::Static;

    // Restore from memento
    component.restore(memento);

    // Verify restoration
    EXPECT_EQ(component.bodyID.GetIndex(), 777u);
    EXPECT_EQ(component.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, SaveRestoreRoundTripStatic) {
    // Set up initial state
    component.bodyID = JPH::BodyID(888);
    component.type = PhysicsBodyComponent::Type::Static;

    // Save state
    auto memento = component.save();

    // Modify component
    component.bodyID = JPH::BodyID(1);
    component.type = PhysicsBodyComponent::Type::Dynamic;

    // Restore from memento
    component.restore(memento);

    // Verify restoration
    EXPECT_EQ(component.bodyID.GetIndex(), 888u);
    EXPECT_EQ(component.type, PhysicsBodyComponent::Type::Static);
}

// =============================================================================
// Multiple Memento Independence Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, MultipleMementosAreIndependent) {
    // Create first memento
    component.bodyID = JPH::BodyID(100);
    component.type = PhysicsBodyComponent::Type::Static;
    auto memento1 = component.save();

    // Modify and create second memento
    component.bodyID = JPH::BodyID(200);
    component.type = PhysicsBodyComponent::Type::Dynamic;
    auto memento2 = component.save();

    // Modify and create third memento
    component.bodyID = JPH::BodyID(300);
    component.type = PhysicsBodyComponent::Type::Static;
    auto memento3 = component.save();

    // Verify all mementos are independent
    EXPECT_EQ(memento1.bodyID.GetIndex(), 100u);
    EXPECT_EQ(memento1.type, PhysicsBodyComponent::Type::Static);

    EXPECT_EQ(memento2.bodyID.GetIndex(), 200u);
    EXPECT_EQ(memento2.type, PhysicsBodyComponent::Type::Dynamic);

    EXPECT_EQ(memento3.bodyID.GetIndex(), 300u);
    EXPECT_EQ(memento3.type, PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsBodyComponentTest, RestoreFromMultipleMementos) {
    // Create multiple mementos
    component.bodyID = JPH::BodyID(10);
    component.type = PhysicsBodyComponent::Type::Static;
    auto memento1 = component.save();

    component.bodyID = JPH::BodyID(20);
    component.type = PhysicsBodyComponent::Type::Dynamic;
    auto memento2 = component.save();

    // Clear component
    component.bodyID = JPH::BodyID();
    component.type = PhysicsBodyComponent::Type::Static;

    // Restore from first memento
    component.restore(memento1);
    EXPECT_EQ(component.bodyID.GetIndex(), 10u);
    EXPECT_EQ(component.type, PhysicsBodyComponent::Type::Static);

    // Restore from second memento
    component.restore(memento2);
    EXPECT_EQ(component.bodyID.GetIndex(), 20u);
    EXPECT_EQ(component.type, PhysicsBodyComponent::Type::Dynamic);

    // Restore from first memento again
    component.restore(memento1);
    EXPECT_EQ(component.bodyID.GetIndex(), 10u);
    EXPECT_EQ(component.type, PhysicsBodyComponent::Type::Static);
}

// =============================================================================
// Type Trait Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, ComponentIsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<PhysicsBodyComponent>);
}

TEST_F(PhysicsBodyComponentTest, ComponentIsCopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<PhysicsBodyComponent>);
}

TEST_F(PhysicsBodyComponentTest, ComponentIsCopyAssignable) {
    EXPECT_TRUE(std::is_copy_assignable_v<PhysicsBodyComponent>);
}

TEST_F(PhysicsBodyComponentTest, ComponentIsMoveConstructible) {
    EXPECT_TRUE(std::is_move_constructible_v<PhysicsBodyComponent>);
}

TEST_F(PhysicsBodyComponentTest, ComponentIsMoveAssignable) {
    EXPECT_TRUE(std::is_move_assignable_v<PhysicsBodyComponent>);
}

TEST_F(PhysicsBodyComponentTest, MementoIsDefaultConstructible) {
    EXPECT_TRUE(std::is_default_constructible_v<PhysicsBodyComponent::Memento>);
}

TEST_F(PhysicsBodyComponentTest, MementoIsCopyConstructible) {
    EXPECT_TRUE(std::is_copy_constructible_v<PhysicsBodyComponent::Memento>);
}

TEST_F(PhysicsBodyComponentTest, MementoIsCopyAssignable) {
    EXPECT_TRUE(std::is_copy_assignable_v<PhysicsBodyComponent::Memento>);
}

TEST_F(PhysicsBodyComponentTest, MementoIsMoveConstructible) {
    EXPECT_TRUE(std::is_move_constructible_v<PhysicsBodyComponent::Memento>);
}

TEST_F(PhysicsBodyComponentTest, MementoIsMoveAssignable) {
    EXPECT_TRUE(std::is_move_assignable_v<PhysicsBodyComponent::Memento>);
}

// =============================================================================
// BodyID Handling Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, BodyIDCanBeSet) {
    PhysicsBodyComponent comp;
    JPH::BodyID testID(500);
    comp.bodyID = testID;

    EXPECT_EQ(comp.bodyID.GetIndex(), 500u);
}

TEST_F(PhysicsBodyComponentTest, BodyIDCanBeModified) {
    PhysicsBodyComponent comp;
    comp.bodyID = JPH::BodyID(100);
    EXPECT_EQ(comp.bodyID.GetIndex(), 100u);

    comp.bodyID = JPH::BodyID(200);
    EXPECT_EQ(comp.bodyID.GetIndex(), 200u);
}

TEST_F(PhysicsBodyComponentTest, BodyIDInvalidByDefault) {
    PhysicsBodyComponent comp;
    EXPECT_TRUE(comp.bodyID.IsInvalid());
}

TEST_F(PhysicsBodyComponentTest, BodyIDValidAfterSet) {
    PhysicsBodyComponent comp;
    comp.bodyID = JPH::BodyID(1);
    EXPECT_FALSE(comp.bodyID.IsInvalid());
}

TEST_F(PhysicsBodyComponentTest, BodyIDWithMaxIndex) {
    PhysicsBodyComponent comp;
    JPH::BodyID maxID(0x7FFFFFFF);  // Maximum valid index
    comp.bodyID = maxID;

    EXPECT_EQ(comp.bodyID.GetIndex(), maxID.GetIndex());
}

TEST_F(PhysicsBodyComponentTest, BodyIDWithZeroIndex) {
    PhysicsBodyComponent comp;
    JPH::BodyID zeroID(0);
    comp.bodyID = zeroID;

    EXPECT_EQ(comp.bodyID.GetIndex(), 0u);
}

// =============================================================================
// Body Type Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, TypeCanBeSetToStatic) {
    PhysicsBodyComponent comp;
    comp.type = PhysicsBodyComponent::Type::Static;
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsBodyComponentTest, TypeCanBeSetToDynamic) {
    PhysicsBodyComponent comp;
    comp.type = PhysicsBodyComponent::Type::Dynamic;
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, TypeCanBeChangedFromStaticToDynamic) {
    PhysicsBodyComponent comp;
    comp.type = PhysicsBodyComponent::Type::Static;
    comp.type = PhysicsBodyComponent::Type::Dynamic;
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, TypeCanBeChangedFromDynamicToStatic) {
    PhysicsBodyComponent comp;
    comp.type = PhysicsBodyComponent::Type::Dynamic;
    comp.type = PhysicsBodyComponent::Type::Static;
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Static);
}

// =============================================================================
// Copy Semantics Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, CopyConstruction) {
    PhysicsBodyComponent original;
    original.bodyID = JPH::BodyID(123);
    original.type = PhysicsBodyComponent::Type::Dynamic;

    PhysicsBodyComponent copy = original;

    EXPECT_EQ(copy.bodyID.GetIndex(), 123u);
    EXPECT_EQ(copy.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, CopyAssignment) {
    PhysicsBodyComponent original;
    original.bodyID = JPH::BodyID(456);
    original.type = PhysicsBodyComponent::Type::Static;

    PhysicsBodyComponent copy;
    copy = original;

    EXPECT_EQ(copy.bodyID.GetIndex(), 456u);
    EXPECT_EQ(copy.type, PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsBodyComponentTest, CopyConstructionPreservesOriginal) {
    PhysicsBodyComponent original;
    original.bodyID = JPH::BodyID(789);
    original.type = PhysicsBodyComponent::Type::Dynamic;

    PhysicsBodyComponent copy = original;

    // Original should be unchanged
    EXPECT_EQ(original.bodyID.GetIndex(), 789u);
    EXPECT_EQ(original.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, CopyAssignmentPreservesOriginal) {
    PhysicsBodyComponent original;
    original.bodyID = JPH::BodyID(321);
    original.type = PhysicsBodyComponent::Type::Static;

    PhysicsBodyComponent copy;
    copy = original;

    // Original should be unchanged
    EXPECT_EQ(original.bodyID.GetIndex(), 321u);
    EXPECT_EQ(original.type, PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsBodyComponentTest, CopyIsIndependent) {
    PhysicsBodyComponent original;
    original.bodyID = JPH::BodyID(111);
    original.type = PhysicsBodyComponent::Type::Dynamic;

    PhysicsBodyComponent copy = original;

    // Modify copy
    copy.bodyID = JPH::BodyID(222);
    copy.type = PhysicsBodyComponent::Type::Static;

    // Original should be unchanged
    EXPECT_EQ(original.bodyID.GetIndex(), 111u);
    EXPECT_EQ(original.type, PhysicsBodyComponent::Type::Dynamic);
}

// =============================================================================
// Move Semantics Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, MoveConstruction) {
    PhysicsBodyComponent original;
    original.bodyID = JPH::BodyID(999);
    original.type = PhysicsBodyComponent::Type::Dynamic;

    PhysicsBodyComponent moved = std::move(original);

    EXPECT_EQ(moved.bodyID.GetIndex(), 999u);
    EXPECT_EQ(moved.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, MoveAssignment) {
    PhysicsBodyComponent original;
    original.bodyID = JPH::BodyID(888);
    original.type = PhysicsBodyComponent::Type::Static;

    PhysicsBodyComponent moved;
    moved = std::move(original);

    EXPECT_EQ(moved.bodyID.GetIndex(), 888u);
    EXPECT_EQ(moved.type, PhysicsBodyComponent::Type::Static);
}

// =============================================================================
// Memento Copy Semantics Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, MementoCopyConstruction) {
    PhysicsBodyComponent::Memento original;
    original.bodyID = JPH::BodyID(555);
    original.type = PhysicsBodyComponent::Type::Dynamic;

    PhysicsBodyComponent::Memento copy = original;

    EXPECT_EQ(copy.bodyID.GetIndex(), 555u);
    EXPECT_EQ(copy.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, MementoCopyAssignment) {
    PhysicsBodyComponent::Memento original;
    original.bodyID = JPH::BodyID(666);
    original.type = PhysicsBodyComponent::Type::Static;

    PhysicsBodyComponent::Memento copy;
    copy = original;

    EXPECT_EQ(copy.bodyID.GetIndex(), 666u);
    EXPECT_EQ(copy.type, PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsBodyComponentTest, MementoCopyIsIndependent) {
    PhysicsBodyComponent::Memento original;
    original.bodyID = JPH::BodyID(333);
    original.type = PhysicsBodyComponent::Type::Dynamic;

    PhysicsBodyComponent::Memento copy = original;

    // Modify copy
    copy.bodyID = JPH::BodyID(444);
    copy.type = PhysicsBodyComponent::Type::Static;

    // Original should be unchanged
    EXPECT_EQ(original.bodyID.GetIndex(), 333u);
    EXPECT_EQ(original.type, PhysicsBodyComponent::Type::Dynamic);
}

// =============================================================================
// Edge Cases Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, SaveWithInvalidBodyID) {
    PhysicsBodyComponent comp;
    comp.bodyID = JPH::BodyID();  // Invalid
    comp.type = PhysicsBodyComponent::Type::Static;

    auto memento = comp.save();

    EXPECT_TRUE(memento.bodyID.IsInvalid());
    EXPECT_EQ(memento.type, PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsBodyComponentTest, RestoreWithInvalidBodyID) {
    PhysicsBodyComponent::Memento memento;
    memento.bodyID = JPH::BodyID();  // Invalid
    memento.type = PhysicsBodyComponent::Type::Dynamic;

    PhysicsBodyComponent comp;
    comp.restore(memento);

    EXPECT_TRUE(comp.bodyID.IsInvalid());
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, MultipleRestoresOverwriteState) {
    PhysicsBodyComponent comp;

    // First restore
    PhysicsBodyComponent::Memento memento1;
    memento1.bodyID = JPH::BodyID(100);
    memento1.type = PhysicsBodyComponent::Type::Static;
    comp.restore(memento1);

    EXPECT_EQ(comp.bodyID.GetIndex(), 100u);
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Static);

    // Second restore overwrites
    PhysicsBodyComponent::Memento memento2;
    memento2.bodyID = JPH::BodyID(200);
    memento2.type = PhysicsBodyComponent::Type::Dynamic;
    comp.restore(memento2);

    EXPECT_EQ(comp.bodyID.GetIndex(), 200u);
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, SaveDoesNotModifyComponent) {
    PhysicsBodyComponent comp;
    comp.bodyID = JPH::BodyID(777);
    comp.type = PhysicsBodyComponent::Type::Dynamic;

    auto memento = comp.save();

    // Component should be unchanged
    EXPECT_EQ(comp.bodyID.GetIndex(), 777u);
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, SaveMultipleTimesCreatesIndependentMementos) {
    PhysicsBodyComponent comp;
    comp.bodyID = JPH::BodyID(100);
    comp.type = PhysicsBodyComponent::Type::Static;

    auto memento1 = comp.save();

    comp.bodyID = JPH::BodyID(200);
    comp.type = PhysicsBodyComponent::Type::Dynamic;

    auto memento2 = comp.save();

    // First memento should still have original values
    EXPECT_EQ(memento1.bodyID.GetIndex(), 100u);
    EXPECT_EQ(memento1.type, PhysicsBodyComponent::Type::Static);

    // Second memento should have new values
    EXPECT_EQ(memento2.bodyID.GetIndex(), 200u);
    EXPECT_EQ(memento2.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, RestoreSameStateMultipleTimes) {
    PhysicsBodyComponent::Memento memento;
    memento.bodyID = JPH::BodyID(500);
    memento.type = PhysicsBodyComponent::Type::Static;

    PhysicsBodyComponent comp;

    // Restore multiple times
    comp.restore(memento);
    comp.restore(memento);
    comp.restore(memento);

    EXPECT_EQ(comp.bodyID.GetIndex(), 500u);
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsBodyComponentTest, SelfAssignment) {
    PhysicsBodyComponent comp;
    comp.bodyID = JPH::BodyID(123);
    comp.type = PhysicsBodyComponent::Type::Dynamic;

    comp = comp;  // Self-assignment

    EXPECT_EQ(comp.bodyID.GetIndex(), 123u);
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, ComponentWithBothFieldsModified) {
    PhysicsBodyComponent comp;

    // Set both fields
    comp.bodyID = JPH::BodyID(999);
    comp.type = PhysicsBodyComponent::Type::Dynamic;

    // Verify both are set correctly
    EXPECT_EQ(comp.bodyID.GetIndex(), 999u);
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Dynamic);

    // Save and verify memento
    auto memento = comp.save();
    EXPECT_EQ(memento.bodyID.GetIndex(), 999u);
    EXPECT_EQ(memento.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, DefaultConstructedMemento) {
    PhysicsBodyComponent::Memento memento;

    // Default constructed memento should have invalid bodyID and default type
    EXPECT_TRUE(memento.bodyID.IsInvalid());
    // Type will be value-initialized (first enum value)
}

// =============================================================================
// Combined State Tests
// =============================================================================

TEST_F(PhysicsBodyComponentTest, StaticBodyWithValidID) {
    PhysicsBodyComponent comp;
    comp.bodyID = JPH::BodyID(100);
    comp.type = PhysicsBodyComponent::Type::Static;

    EXPECT_FALSE(comp.bodyID.IsInvalid());
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsBodyComponentTest, DynamicBodyWithValidID) {
    PhysicsBodyComponent comp;
    comp.bodyID = JPH::BodyID(200);
    comp.type = PhysicsBodyComponent::Type::Dynamic;

    EXPECT_FALSE(comp.bodyID.IsInvalid());
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, ChangeBothFieldsSimultaneously) {
    PhysicsBodyComponent comp;
    comp.bodyID = JPH::BodyID(100);
    comp.type = PhysicsBodyComponent::Type::Static;

    // Change both fields
    comp.bodyID = JPH::BodyID(200);
    comp.type = PhysicsBodyComponent::Type::Dynamic;

    EXPECT_EQ(comp.bodyID.GetIndex(), 200u);
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type::Dynamic);
}

}  // namespace nexo::components
