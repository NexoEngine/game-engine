//// DockingRegistry.test.cpp ///////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        02/12/2025
//  Description: Test file for DockingRegistry class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "DockingRegistry.hpp"

namespace nexo::editor {

    class DockingRegistryTest : public ::testing::Test {
    protected:
        DockingRegistry registry;
    };

    // SetDockId Tests

    TEST_F(DockingRegistryTest, SetDockIdStoresValue) {
        registry.setDockId("TestWindow", 123);
        auto result = registry.getDockId("TestWindow");
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), 123u);
    }

    TEST_F(DockingRegistryTest, SetDockIdOverwritesExisting) {
        registry.setDockId("TestWindow", 100);
        registry.setDockId("TestWindow", 200);
        auto result = registry.getDockId("TestWindow");
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), 200u);
    }

    TEST_F(DockingRegistryTest, SetDockIdWithZeroId) {
        registry.setDockId("ZeroWindow", 0);
        auto result = registry.getDockId("ZeroWindow");
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), 0u);
    }

    TEST_F(DockingRegistryTest, SetDockIdWithEmptyName) {
        registry.setDockId("", 42);
        auto result = registry.getDockId("");
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), 42u);
    }

    // GetDockId Tests

    TEST_F(DockingRegistryTest, GetDockIdReturnsNulloptForMissing) {
        auto result = registry.getDockId("NonExistent");
        EXPECT_FALSE(result.has_value());
    }

    TEST_F(DockingRegistryTest, GetDockIdReturnsCorrectValueForMultipleEntries) {
        registry.setDockId("Window1", 100);
        registry.setDockId("Window2", 200);
        registry.setDockId("Window3", 300);

        EXPECT_EQ(registry.getDockId("Window1").value(), 100u);
        EXPECT_EQ(registry.getDockId("Window2").value(), 200u);
        EXPECT_EQ(registry.getDockId("Window3").value(), 300u);
    }

    TEST_F(DockingRegistryTest, GetDockIdIsCaseSensitive) {
        registry.setDockId("Window", 100);

        EXPECT_TRUE(registry.getDockId("Window").has_value());
        EXPECT_FALSE(registry.getDockId("window").has_value());
        EXPECT_FALSE(registry.getDockId("WINDOW").has_value());
    }

    // ResetDockId Tests

    TEST_F(DockingRegistryTest, ResetDockIdRemovesEntry) {
        registry.setDockId("TestWindow", 123);
        EXPECT_TRUE(registry.getDockId("TestWindow").has_value());

        registry.resetDockId("TestWindow");
        EXPECT_FALSE(registry.getDockId("TestWindow").has_value());
    }

    TEST_F(DockingRegistryTest, ResetDockIdDoesNothingForMissing) {
        // Should not throw or cause issues
        EXPECT_NO_THROW(registry.resetDockId("NonExistent"));
    }

    TEST_F(DockingRegistryTest, ResetDockIdOnlyRemovesSpecifiedEntry) {
        registry.setDockId("Window1", 100);
        registry.setDockId("Window2", 200);
        registry.setDockId("Window3", 300);

        registry.resetDockId("Window2");

        EXPECT_TRUE(registry.getDockId("Window1").has_value());
        EXPECT_FALSE(registry.getDockId("Window2").has_value());
        EXPECT_TRUE(registry.getDockId("Window3").has_value());
    }

    TEST_F(DockingRegistryTest, ResetDockIdAllowsReaddingSameKey) {
        registry.setDockId("TestWindow", 100);
        registry.resetDockId("TestWindow");
        registry.setDockId("TestWindow", 200);

        auto result = registry.getDockId("TestWindow");
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), 200u);
    }

    // Edge Cases

    TEST_F(DockingRegistryTest, LargeNumberOfEntries) {
        constexpr int NUM_ENTRIES = 100;
        for (int i = 0; i < NUM_ENTRIES; ++i) {
            registry.setDockId("Window" + std::to_string(i), static_cast<ImGuiID>(i * 10));
        }

        for (int i = 0; i < NUM_ENTRIES; ++i) {
            auto result = registry.getDockId("Window" + std::to_string(i));
            ASSERT_TRUE(result.has_value()) << "Failed for Window" << i;
            EXPECT_EQ(result.value(), static_cast<ImGuiID>(i * 10));
        }
    }

    TEST_F(DockingRegistryTest, SetDockIdWithMaxImGuiID) {
        constexpr ImGuiID maxId = std::numeric_limits<ImGuiID>::max();
        registry.setDockId("MaxWindow", maxId);

        auto result = registry.getDockId("MaxWindow");
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), maxId);
    }

    TEST_F(DockingRegistryTest, LongWindowName) {
        std::string longName(1000, 'x');
        registry.setDockId(longName, 42);

        auto result = registry.getDockId(longName);
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), 42u);
    }

    TEST_F(DockingRegistryTest, WindowNameWithSpecialCharacters) {
        registry.setDockId("Window/With:Special*Chars?", 123);

        auto result = registry.getDockId("Window/With:Special*Chars?");
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), 123u);
    }

    TEST_F(DockingRegistryTest, WindowNameWithSpaces) {
        registry.setDockId("Window With Spaces", 456);

        auto result = registry.getDockId("Window With Spaces");
        ASSERT_TRUE(result.has_value());
        EXPECT_EQ(result.value(), 456u);
    }

}
