//// Render.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for RenderComponent (memento pattern)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/Render.hpp"

namespace nexo::components {

// =============================================================================
// PrimitiveType Enum Tests
// =============================================================================

TEST(PrimitiveTypeTest, EnumValuesAreDefined) {
    EXPECT_EQ(static_cast<int>(PrimitiveType::UNKNOWN), 0);
    EXPECT_EQ(static_cast<int>(PrimitiveType::CUBE), 1);
    EXPECT_EQ(static_cast<int>(PrimitiveType::MESH), 2);
    EXPECT_EQ(static_cast<int>(PrimitiveType::BILLBOARD), 3);
    EXPECT_EQ(static_cast<int>(PrimitiveType::_COUNT), 4);
}

// =============================================================================
// RenderComponent Default State Tests
// =============================================================================

class RenderComponentTest : public ::testing::Test {
protected:
    RenderComponent component;
};

TEST_F(RenderComponentTest, DefaultIsRenderedIsTrue) {
    EXPECT_TRUE(component.isRendered);
}

TEST_F(RenderComponentTest, DefaultTypeIsMesh) {
    EXPECT_EQ(component.type, PrimitiveType::MESH);
}

// =============================================================================
// Save Tests
// =============================================================================

TEST_F(RenderComponentTest, SaveCapturesIsRenderedTrue) {
    component.isRendered = true;
    auto memento = component.save();
    EXPECT_TRUE(memento.isRendered);
}

TEST_F(RenderComponentTest, SaveCapturesIsRenderedFalse) {
    component.isRendered = false;
    auto memento = component.save();
    EXPECT_FALSE(memento.isRendered);
}

TEST_F(RenderComponentTest, SaveCapturesTypeUnknown) {
    component.type = PrimitiveType::UNKNOWN;
    auto memento = component.save();
    EXPECT_EQ(memento.type, PrimitiveType::UNKNOWN);
}

TEST_F(RenderComponentTest, SaveCapturesTypeCube) {
    component.type = PrimitiveType::CUBE;
    auto memento = component.save();
    EXPECT_EQ(memento.type, PrimitiveType::CUBE);
}

TEST_F(RenderComponentTest, SaveCapturesTypeMesh) {
    component.type = PrimitiveType::MESH;
    auto memento = component.save();
    EXPECT_EQ(memento.type, PrimitiveType::MESH);
}

TEST_F(RenderComponentTest, SaveCapturesTypeBillboard) {
    component.type = PrimitiveType::BILLBOARD;
    auto memento = component.save();
    EXPECT_EQ(memento.type, PrimitiveType::BILLBOARD);
}

TEST_F(RenderComponentTest, SaveCapturesBothFields) {
    component.isRendered = false;
    component.type = PrimitiveType::CUBE;
    auto memento = component.save();

    EXPECT_FALSE(memento.isRendered);
    EXPECT_EQ(memento.type, PrimitiveType::CUBE);
}

// =============================================================================
// Restore Tests
// =============================================================================

TEST_F(RenderComponentTest, RestoreAppliesIsRenderedTrue) {
    component.isRendered = false;

    RenderComponent::Memento memento;
    memento.isRendered = true;
    memento.type = PrimitiveType::MESH;

    component.restore(memento);
    EXPECT_TRUE(component.isRendered);
}

TEST_F(RenderComponentTest, RestoreAppliesIsRenderedFalse) {
    component.isRendered = true;

    RenderComponent::Memento memento;
    memento.isRendered = false;
    memento.type = PrimitiveType::MESH;

    component.restore(memento);
    EXPECT_FALSE(component.isRendered);
}

TEST_F(RenderComponentTest, RestoreAppliesType) {
    component.type = PrimitiveType::MESH;

    RenderComponent::Memento memento;
    memento.isRendered = true;
    memento.type = PrimitiveType::BILLBOARD;

    component.restore(memento);
    EXPECT_EQ(component.type, PrimitiveType::BILLBOARD);
}

TEST_F(RenderComponentTest, RestoreAppliesBothFields) {
    component.isRendered = true;
    component.type = PrimitiveType::MESH;

    RenderComponent::Memento memento;
    memento.isRendered = false;
    memento.type = PrimitiveType::CUBE;

    component.restore(memento);

    EXPECT_FALSE(component.isRendered);
    EXPECT_EQ(component.type, PrimitiveType::CUBE);
}

// =============================================================================
// Round Trip Tests
// =============================================================================

TEST_F(RenderComponentTest, SaveRestoreRoundTrip) {
    component.isRendered = false;
    component.type = PrimitiveType::BILLBOARD;

    auto memento = component.save();

    component.isRendered = true;
    component.type = PrimitiveType::UNKNOWN;

    component.restore(memento);

    EXPECT_FALSE(component.isRendered);
    EXPECT_EQ(component.type, PrimitiveType::BILLBOARD);
}

TEST_F(RenderComponentTest, MultipleRoundTrips) {
    component.isRendered = true;
    component.type = PrimitiveType::CUBE;
    auto memento1 = component.save();

    component.isRendered = false;
    component.type = PrimitiveType::BILLBOARD;
    auto memento2 = component.save();

    component.restore(memento1);
    EXPECT_TRUE(component.isRendered);
    EXPECT_EQ(component.type, PrimitiveType::CUBE);

    component.restore(memento2);
    EXPECT_FALSE(component.isRendered);
    EXPECT_EQ(component.type, PrimitiveType::BILLBOARD);
}

TEST_F(RenderComponentTest, MementoIndependence) {
    component.isRendered = true;
    component.type = PrimitiveType::MESH;
    auto memento1 = component.save();

    component.isRendered = false;
    component.type = PrimitiveType::CUBE;
    auto memento2 = component.save();

    // Mementos should be independent
    EXPECT_TRUE(memento1.isRendered);
    EXPECT_EQ(memento1.type, PrimitiveType::MESH);
    EXPECT_FALSE(memento2.isRendered);
    EXPECT_EQ(memento2.type, PrimitiveType::CUBE);
}

// =============================================================================
// All Primitive Types Round Trip
// =============================================================================

TEST_F(RenderComponentTest, AllPrimitiveTypesRoundTrip) {
    std::vector<PrimitiveType> types = {
        PrimitiveType::UNKNOWN,
        PrimitiveType::CUBE,
        PrimitiveType::MESH,
        PrimitiveType::BILLBOARD
    };

    for (auto type : types) {
        component.type = type;
        auto memento = component.save();

        component.type = PrimitiveType::UNKNOWN;
        component.restore(memento);

        EXPECT_EQ(component.type, type);
    }
}

}  // namespace nexo::components
