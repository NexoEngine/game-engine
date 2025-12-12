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

}  // namespace nexo::components
