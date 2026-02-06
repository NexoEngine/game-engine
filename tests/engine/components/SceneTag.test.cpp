//// SceneTag.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        10/12/2025
//  Description: Test file for SceneTag component (memento pattern)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "components/SceneComponents.hpp"

namespace nexo::components {

class SceneTagTest : public ::testing::Test {
protected:
    SceneTag tag;
};

// =============================================================================
// Default State Tests
// =============================================================================

TEST_F(SceneTagTest, DefaultIdIsZero) {
    EXPECT_EQ(tag.id, 0u);
}

TEST_F(SceneTagTest, DefaultIsActiveIsTrue) {
    EXPECT_TRUE(tag.isActive);
}

TEST_F(SceneTagTest, DefaultIsRenderedIsTrue) {
    EXPECT_TRUE(tag.isRendered);
}

// =============================================================================
// Save Tests
// =============================================================================

TEST_F(SceneTagTest, SaveCapturesId) {
    tag.id = 42;
    auto memento = tag.save();
    EXPECT_EQ(memento.id, 42u);
}

TEST_F(SceneTagTest, SaveCapturesIsActiveTrue) {
    tag.isActive = true;
    auto memento = tag.save();
    EXPECT_TRUE(memento.isActive);
}

TEST_F(SceneTagTest, SaveCapturesIsActiveFalse) {
    tag.isActive = false;
    auto memento = tag.save();
    EXPECT_FALSE(memento.isActive);
}

TEST_F(SceneTagTest, SaveCapturesIsRenderedTrue) {
    tag.isRendered = true;
    auto memento = tag.save();
    EXPECT_TRUE(memento.isRendered);
}

TEST_F(SceneTagTest, SaveCapturesIsRenderedFalse) {
    tag.isRendered = false;
    auto memento = tag.save();
    EXPECT_FALSE(memento.isRendered);
}

TEST_F(SceneTagTest, SaveCapturesAllFields) {
    tag.id = 123;
    tag.isActive = false;
    tag.isRendered = false;

    auto memento = tag.save();

    EXPECT_EQ(memento.id, 123u);
    EXPECT_FALSE(memento.isActive);
    EXPECT_FALSE(memento.isRendered);
}

// =============================================================================
// Restore Tests
// =============================================================================

TEST_F(SceneTagTest, RestoreAppliesId) {
    SceneTag::Memento memento;
    memento.id = 999;
    memento.isActive = true;
    memento.isRendered = true;

    tag.restore(memento);
    EXPECT_EQ(tag.id, 999u);
}

TEST_F(SceneTagTest, RestoreAppliesIsActive) {
    tag.isActive = true;

    SceneTag::Memento memento;
    memento.id = 0;
    memento.isActive = false;
    memento.isRendered = true;

    tag.restore(memento);
    EXPECT_FALSE(tag.isActive);
}

TEST_F(SceneTagTest, RestoreAppliesIsRendered) {
    tag.isRendered = true;

    SceneTag::Memento memento;
    memento.id = 0;
    memento.isActive = true;
    memento.isRendered = false;

    tag.restore(memento);
    EXPECT_FALSE(tag.isRendered);
}

TEST_F(SceneTagTest, RestoreAppliesAllFields) {
    tag.id = 0;
    tag.isActive = true;
    tag.isRendered = true;

    SceneTag::Memento memento;
    memento.id = 500;
    memento.isActive = false;
    memento.isRendered = false;

    tag.restore(memento);

    EXPECT_EQ(tag.id, 500u);
    EXPECT_FALSE(tag.isActive);
    EXPECT_FALSE(tag.isRendered);
}

// =============================================================================
// Round Trip Tests
// =============================================================================

TEST_F(SceneTagTest, SaveRestoreRoundTrip) {
    tag.id = 42;
    tag.isActive = false;
    tag.isRendered = true;

    auto memento = tag.save();

    tag.id = 0;
    tag.isActive = true;
    tag.isRendered = false;

    tag.restore(memento);

    EXPECT_EQ(tag.id, 42u);
    EXPECT_FALSE(tag.isActive);
    EXPECT_TRUE(tag.isRendered);
}

TEST_F(SceneTagTest, MultipleRoundTrips) {
    tag.id = 1;
    tag.isActive = true;
    tag.isRendered = true;
    auto memento1 = tag.save();

    tag.id = 2;
    tag.isActive = false;
    tag.isRendered = false;
    auto memento2 = tag.save();

    tag.restore(memento1);
    EXPECT_EQ(tag.id, 1u);
    EXPECT_TRUE(tag.isActive);
    EXPECT_TRUE(tag.isRendered);

    tag.restore(memento2);
    EXPECT_EQ(tag.id, 2u);
    EXPECT_FALSE(tag.isActive);
    EXPECT_FALSE(tag.isRendered);
}

TEST_F(SceneTagTest, MementoIndependence) {
    tag.id = 100;
    tag.isActive = true;
    tag.isRendered = false;
    auto memento1 = tag.save();

    tag.id = 200;
    tag.isActive = false;
    tag.isRendered = true;
    auto memento2 = tag.save();

    EXPECT_EQ(memento1.id, 100u);
    EXPECT_TRUE(memento1.isActive);
    EXPECT_FALSE(memento1.isRendered);

    EXPECT_EQ(memento2.id, 200u);
    EXPECT_FALSE(memento2.isActive);
    EXPECT_TRUE(memento2.isRendered);
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST_F(SceneTagTest, MaxId) {
    tag.id = std::numeric_limits<unsigned int>::max();
    auto memento = tag.save();

    tag.id = 0;
    tag.restore(memento);

    EXPECT_EQ(tag.id, std::numeric_limits<unsigned int>::max());
}

TEST_F(SceneTagTest, ZeroId) {
    tag.id = 0;
    auto memento = tag.save();

    tag.id = 999;
    tag.restore(memento);

    EXPECT_EQ(tag.id, 0u);
}

TEST_F(SceneTagTest, AllBoolCombinations) {
    std::vector<std::pair<bool, bool>> combinations = {
        {false, false},
        {false, true},
        {true, false},
        {true, true}
    };

    for (const auto& [active, rendered] : combinations) {
        tag.isActive = active;
        tag.isRendered = rendered;
        auto memento = tag.save();

        tag.isActive = !active;
        tag.isRendered = !rendered;
        tag.restore(memento);

        EXPECT_EQ(tag.isActive, active);
        EXPECT_EQ(tag.isRendered, rendered);
    }
}

}  // namespace nexo::components
