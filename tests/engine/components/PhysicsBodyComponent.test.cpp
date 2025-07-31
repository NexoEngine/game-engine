///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date: Today lol
//  Description: Test file for the PhysicsBodyComponent (simplified for testing)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "components/PhysicsBodyComponent.hpp"
#include <vector>

// NOTE: PhysicsBodyComponent tests are simplified due to complex Jolt Physics dependencies
// Full Jolt Physics integration requires complex setup and context initialization
// These tests focus on component structure and basic functionality

namespace nexo::components {

class PhysicsBodyComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        physicsBody = PhysicsBodyComponent{};
    }
    
    PhysicsBodyComponent physicsBody;
};

TEST_F(PhysicsBodyComponentTest, DefaultConstruction) {
    PhysicsBodyComponent comp;
    
    // Test default type initialization
    EXPECT_EQ(comp.type, PhysicsBodyComponent::Type{});
    
    // BodyID is default constructed (we can't easily test its internal state)
    // but we can verify the component has the bodyID member
    static_assert(sizeof(comp.bodyID) > 0, "bodyID should be present");
}

TEST_F(PhysicsBodyComponentTest, TypeEnumValues) {
    // Test Static type
    physicsBody.type = PhysicsBodyComponent::Type::Static;
    EXPECT_EQ(physicsBody.type, PhysicsBodyComponent::Type::Static);
    
    // Test Dynamic type
    physicsBody.type = PhysicsBodyComponent::Type::Dynamic;
    EXPECT_EQ(physicsBody.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, CopyConstruction) {
    // Set up original
    physicsBody.type = PhysicsBodyComponent::Type::Dynamic;
    
    // Copy construct
    PhysicsBodyComponent copy(physicsBody);
    
    // Verify copy has same type
    EXPECT_EQ(copy.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, MoveConstruction) {
    // Set up original
    physicsBody.type = PhysicsBodyComponent::Type::Static;
    auto originalType = physicsBody.type;
    
    // Move construct
    PhysicsBodyComponent moved(std::move(physicsBody));
    
    // Verify moved has the type
    EXPECT_EQ(moved.type, originalType);
}

TEST_F(PhysicsBodyComponentTest, CopyAssignment) {
    PhysicsBodyComponent source;
    source.type = PhysicsBodyComponent::Type::Dynamic;
    
    physicsBody = source;
    
    EXPECT_EQ(physicsBody.type, PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsBodyComponentTest, MoveAssignment) {
    PhysicsBodyComponent source;
    source.type = PhysicsBodyComponent::Type::Static;
    auto originalType = source.type;
    
    physicsBody = std::move(source);
    
    EXPECT_EQ(physicsBody.type, originalType);
}

TEST_F(PhysicsBodyComponentTest, MultipleComponents) {
    std::vector<PhysicsBodyComponent> components;
    
    // Create multiple components with different types
    for (int i = 0; i < 10; ++i) {
        PhysicsBodyComponent comp;
        comp.type = (i % 2 == 0) ? PhysicsBodyComponent::Type::Static : PhysicsBodyComponent::Type::Dynamic;
        components.push_back(comp);
    }
    
    // Verify all components have correct types
    for (size_t i = 0; i < components.size(); ++i) {
        PhysicsBodyComponent::Type expectedType = (i % 2 == 0) ? 
            PhysicsBodyComponent::Type::Static : PhysicsBodyComponent::Type::Dynamic;
        EXPECT_EQ(components[i].type, expectedType);
    }
}

TEST_F(PhysicsBodyComponentTest, TypeComparison) {
    PhysicsBodyComponent comp1;
    PhysicsBodyComponent comp2;
    
    // Same types should be equal
    comp1.type = PhysicsBodyComponent::Type::Static;
    comp2.type = PhysicsBodyComponent::Type::Static;
    EXPECT_EQ(comp1.type, comp2.type);
    
    // Different types should not be equal
    comp2.type = PhysicsBodyComponent::Type::Dynamic;
    EXPECT_NE(comp1.type, comp2.type);
}

TEST_F(PhysicsBodyComponentTest, StructureIntegrity) {
    // Verify the component has the expected members
    static_assert(std::is_same_v<decltype(physicsBody.type), PhysicsBodyComponent::Type>);
    static_assert(std::is_same_v<decltype(physicsBody.bodyID), JPH::BodyID>);
    
    // Verify component is lightweight (no virtual functions, etc.)
    EXPECT_TRUE(std::is_trivially_copyable_v<PhysicsBodyComponent::Type>);
    EXPECT_TRUE(std::is_standard_layout_v<PhysicsBodyComponent>);
}

} // namespace nexo::components