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
//  Description: Test file for the Render component
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "components/Render.hpp"
#include <vector>

namespace nexo::components {

class RenderComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        renderComp = RenderComponent{};
    }
    
    RenderComponent renderComp;
};

TEST_F(RenderComponentTest, DefaultConstruction) {
    RenderComponent comp;
    
    EXPECT_TRUE(comp.isRendered);
    EXPECT_EQ(comp.type, PrimitiveType::MESH);
}

TEST_F(RenderComponentTest, IsRenderedFlag) {
    EXPECT_TRUE(renderComp.isRendered);
    
    renderComp.isRendered = false;
    EXPECT_FALSE(renderComp.isRendered);
    
    renderComp.isRendered = true;
    EXPECT_TRUE(renderComp.isRendered);
}

TEST_F(RenderComponentTest, PrimitiveTypes) {
    // Test all primitive types
    std::vector<PrimitiveType> types = {
        PrimitiveType::UNKNOWN,
        PrimitiveType::CUBE,
        PrimitiveType::MESH,
        PrimitiveType::BILLBOARD
    };
    
    for (auto type : types) {
        renderComp.type = type;
        EXPECT_EQ(renderComp.type, type);
    }
}

TEST_F(RenderComponentTest, PrimitiveTypeCount) {
    // Verify _COUNT is after all valid types
    int count = static_cast<int>(PrimitiveType::_COUNT);
    EXPECT_EQ(count, 4); // UNKNOWN, CUBE, MESH, BILLBOARD
}

TEST_F(RenderComponentTest, MementoSave) {
    renderComp.isRendered = false;
    renderComp.type = PrimitiveType::CUBE;
    
    auto memento = renderComp.save();
    
    EXPECT_FALSE(memento.isRendered);
    EXPECT_EQ(memento.type, PrimitiveType::CUBE);
}

TEST_F(RenderComponentTest, MementoRestore) {
    RenderComponent::Memento memento{true, PrimitiveType::BILLBOARD};
    
    renderComp.restore(memento);
    
    EXPECT_TRUE(renderComp.isRendered);
    EXPECT_EQ(renderComp.type, PrimitiveType::BILLBOARD);
}

TEST_F(RenderComponentTest, MementoRoundTrip) {
    renderComp.isRendered = false;
    renderComp.type = PrimitiveType::UNKNOWN;
    
    auto memento = renderComp.save();
    
    // Modify values
    renderComp.isRendered = true;
    renderComp.type = PrimitiveType::MESH;
    
    // Restore
    renderComp.restore(memento);
    
    EXPECT_FALSE(renderComp.isRendered);
    EXPECT_EQ(renderComp.type, PrimitiveType::UNKNOWN);
}

TEST_F(RenderComponentTest, CopyConstruction) {
    renderComp.isRendered = false;
    renderComp.type = PrimitiveType::CUBE;
    
    RenderComponent copy(renderComp);
    
    EXPECT_FALSE(copy.isRendered);
    EXPECT_EQ(copy.type, PrimitiveType::CUBE);
    
    // Verify independence
    copy.isRendered = true;
    copy.type = PrimitiveType::MESH;
    
    EXPECT_FALSE(renderComp.isRendered);
    EXPECT_EQ(renderComp.type, PrimitiveType::CUBE);
}

TEST_F(RenderComponentTest, CopyAssignment) {
    RenderComponent first;
    first.isRendered = false;
    first.type = PrimitiveType::BILLBOARD;
    
    RenderComponent second;
    second.isRendered = true;
    second.type = PrimitiveType::MESH;
    
    second = first;
    
    EXPECT_FALSE(second.isRendered);
    EXPECT_EQ(second.type, PrimitiveType::BILLBOARD);
}

TEST_F(RenderComponentTest, MoveSemantics) {
    renderComp.isRendered = false;
    renderComp.type = PrimitiveType::CUBE;
    
    // Move construction
    RenderComponent moved(std::move(renderComp));
    EXPECT_FALSE(moved.isRendered);
    EXPECT_EQ(moved.type, PrimitiveType::CUBE);
    
    // Move assignment
    RenderComponent assigned;
    assigned = std::move(moved);
    EXPECT_FALSE(assigned.isRendered);
    EXPECT_EQ(assigned.type, PrimitiveType::CUBE);
}

TEST_F(RenderComponentTest, MultipleComponents) {
    std::vector<RenderComponent> components;
    
    // Create components with different configurations
    for (int i = 0; i < 10; ++i) {
        RenderComponent comp;
        comp.isRendered = (i % 2 == 0);
        comp.type = static_cast<PrimitiveType>(i % static_cast<int>(PrimitiveType::_COUNT));
        components.push_back(comp);
    }
    
    // Verify all maintain their values
    for (size_t i = 0; i < components.size(); ++i) {
        EXPECT_EQ(components[i].isRendered, (i % 2 == 0));
        EXPECT_EQ(components[i].type, 
                  static_cast<PrimitiveType>(i % static_cast<int>(PrimitiveType::_COUNT)));
    }
}

TEST_F(RenderComponentTest, PrimitiveTypeSwitch) {
    // Simulate runtime type switching
    renderComp.type = PrimitiveType::MESH;
    EXPECT_EQ(renderComp.type, PrimitiveType::MESH);
    
    // Switch to cube
    renderComp.type = PrimitiveType::CUBE;
    EXPECT_EQ(renderComp.type, PrimitiveType::CUBE);
    
    // Switch to billboard
    renderComp.type = PrimitiveType::BILLBOARD;
    EXPECT_EQ(renderComp.type, PrimitiveType::BILLBOARD);
    
    // Switch to unknown
    renderComp.type = PrimitiveType::UNKNOWN;
    EXPECT_EQ(renderComp.type, PrimitiveType::UNKNOWN);
}

TEST_F(RenderComponentTest, RenderStateToggle) {
    // Simulate enabling/disabling rendering
    EXPECT_TRUE(renderComp.isRendered);
    
    // Disable rendering
    renderComp.isRendered = false;
    EXPECT_FALSE(renderComp.isRendered);
    
    // Save state while disabled
    auto disabledMemento = renderComp.save();
    
    // Enable rendering
    renderComp.isRendered = true;
    EXPECT_TRUE(renderComp.isRendered);
    
    // Restore disabled state
    renderComp.restore(disabledMemento);
    EXPECT_FALSE(renderComp.isRendered);
}

TEST_F(RenderComponentTest, TypeSafety) {
    // Test that primitive type is strongly typed
    renderComp.type = PrimitiveType::CUBE;
    
    // This should not compile if uncommented:
    // renderComp.type = 99; // Invalid assignment
    
    // Valid comparison
    EXPECT_TRUE(renderComp.type == PrimitiveType::CUBE);
    EXPECT_FALSE(renderComp.type == PrimitiveType::MESH);
}

TEST_F(RenderComponentTest, AggregateInitialization) {
    // Test aggregate initialization patterns
    RenderComponent comp1{};
    EXPECT_TRUE(comp1.isRendered);
    EXPECT_EQ(comp1.type, PrimitiveType::MESH);
    
    RenderComponent comp2{};
    comp2.isRendered = false;
    EXPECT_FALSE(comp2.isRendered);
    EXPECT_EQ(comp2.type, PrimitiveType::MESH);
    
    RenderComponent comp3{};
    comp3.isRendered = true;
    comp3.type = PrimitiveType::BILLBOARD;
    EXPECT_TRUE(comp3.isRendered);
    EXPECT_EQ(comp3.type, PrimitiveType::BILLBOARD);
}

} // namespace nexo::components