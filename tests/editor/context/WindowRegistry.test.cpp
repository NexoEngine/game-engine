//// WindowRegistry.test.cpp ////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        03/12/2025
//  Description: Test file for WindowRegistry class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "WindowRegistry.hpp"
#include "MockDocumentWindow.hpp"

namespace nexo::editor {

using namespace testing;
using ::testing::Return;

class WindowRegistryTest : public ::testing::Test {
protected:
    WindowRegistry registry;

    // Factory helpers
    std::shared_ptr<testing::MockDocumentWindow> createMockWindow(const std::string& name) {
        return std::make_shared<testing::MockDocumentWindow>(name);
    }

    std::shared_ptr<testing::MockDocumentWindow2> createMockWindow2(const std::string& name) {
        return std::make_shared<testing::MockDocumentWindow2>(name);
    }
};

// =============================================================================
// Registration Tests
// =============================================================================

TEST_F(WindowRegistryTest, RegisterWindow_StoresWindow) {
    auto window = createMockWindow("TestWindow");
    EXPECT_NO_THROW(registry.registerWindow(window));
    EXPECT_TRUE(registry.hasWindow("TestWindow"));
}

TEST_F(WindowRegistryTest, RegisterWindow_DuplicateThrows) {
    auto window1 = createMockWindow("TestWindow");
    auto window2 = createMockWindow("TestWindow");

    registry.registerWindow(window1);
    EXPECT_THROW(registry.registerWindow(window2), WindowAlreadyRegistered);
}

TEST_F(WindowRegistryTest, RegisterWindow_MultipleOfSameType) {
    auto window1 = createMockWindow("Window1");
    auto window2 = createMockWindow("Window2");
    auto window3 = createMockWindow("Window3");

    EXPECT_NO_THROW(registry.registerWindow(window1));
    EXPECT_NO_THROW(registry.registerWindow(window2));
    EXPECT_NO_THROW(registry.registerWindow(window3));

    EXPECT_TRUE(registry.hasWindow("Window1"));
    EXPECT_TRUE(registry.hasWindow("Window2"));
    EXPECT_TRUE(registry.hasWindow("Window3"));
}

TEST_F(WindowRegistryTest, RegisterWindow_DifferentTypes) {
    auto window1 = createMockWindow("MockWindow");
    auto window2 = createMockWindow2("Mock2Window");

    EXPECT_NO_THROW(registry.registerWindow(window1));
    EXPECT_NO_THROW(registry.registerWindow(window2));

    EXPECT_TRUE(registry.hasWindow("MockWindow"));
    EXPECT_TRUE(registry.hasWindow("Mock2Window"));
}

TEST_F(WindowRegistryTest, RegisterWindow_SameNameDifferentTypes) {
    auto window1 = createMockWindow("SharedName");
    auto window2 = createMockWindow2("SharedName");

    // Same name but different types should both be allowed
    EXPECT_NO_THROW(registry.registerWindow(window1));
    EXPECT_NO_THROW(registry.registerWindow(window2));
}

// =============================================================================
// Retrieval Tests
// =============================================================================

TEST_F(WindowRegistryTest, GetWindow_ReturnsValidWeakPtr) {
    auto window = createMockWindow("TestWindow");
    registry.registerWindow(window);

    auto retrieved = registry.getWindow<testing::MockDocumentWindow>("TestWindow");
    EXPECT_FALSE(retrieved.expired());

    auto locked = retrieved.lock();
    ASSERT_NE(locked, nullptr);
    EXPECT_EQ(locked->getWindowName(), "TestWindow");
}

TEST_F(WindowRegistryTest, GetWindow_ReturnsEmptyForMissingName) {
    auto window = createMockWindow("ExistingWindow");
    registry.registerWindow(window);

    auto retrieved = registry.getWindow<testing::MockDocumentWindow>("NonExistent");
    EXPECT_TRUE(retrieved.expired());
}

TEST_F(WindowRegistryTest, GetWindow_ReturnsEmptyForMissingType) {
    auto window = createMockWindow("TestWindow");
    registry.registerWindow(window);

    // Window exists but under different type
    auto retrieved = registry.getWindow<testing::MockDocumentWindow2>("TestWindow");
    EXPECT_TRUE(retrieved.expired());
}

TEST_F(WindowRegistryTest, GetWindows_ReturnsAllOfType) {
    auto window1 = createMockWindow("Window1");
    auto window2 = createMockWindow("Window2");
    auto window3 = createMockWindow("Window3");

    registry.registerWindow(window1);
    registry.registerWindow(window2);
    registry.registerWindow(window3);

    auto windows = registry.getWindows<testing::MockDocumentWindow>();
    int count = 0;
    for ([[maybe_unused]] const auto& w : windows) {
        count++;
    }
    EXPECT_EQ(count, 3);
}

TEST_F(WindowRegistryTest, GetWindows_EmptyForNoType) {
    // Register windows of one type
    auto window = createMockWindow("TestWindow");
    registry.registerWindow(window);

    // Query for different type
    auto windows = registry.getWindows<testing::MockDocumentWindow2>();
    int count = 0;
    for ([[maybe_unused]] const auto& w : windows) {
        count++;
    }
    EXPECT_EQ(count, 0);
}

TEST_F(WindowRegistryTest, GetWindows_OnlyReturnsRequestedType) {
    auto mock1 = createMockWindow("Mock1");
    auto mock2 = createMockWindow("Mock2");
    auto mock2Type = createMockWindow2("Mock2Type");

    registry.registerWindow(mock1);
    registry.registerWindow(mock2);
    registry.registerWindow(mock2Type);

    auto type1Windows = registry.getWindows<testing::MockDocumentWindow>();
    int type1Count = 0;
    for ([[maybe_unused]] const auto& w : type1Windows) {
        type1Count++;
    }
    EXPECT_EQ(type1Count, 2);

    auto type2Windows = registry.getWindows<testing::MockDocumentWindow2>();
    int type2Count = 0;
    for ([[maybe_unused]] const auto& w : type2Windows) {
        type2Count++;
    }
    EXPECT_EQ(type2Count, 1);
}

TEST_F(WindowRegistryTest, HasWindow_TrueForExisting) {
    auto window = createMockWindow("TestWindow");
    registry.registerWindow(window);

    EXPECT_TRUE(registry.hasWindow("TestWindow"));
}

TEST_F(WindowRegistryTest, HasWindow_FalseForMissing) {
    EXPECT_FALSE(registry.hasWindow("NonExistent"));
}

TEST_F(WindowRegistryTest, HasWindow_FindsAcrossTypes) {
    auto window1 = createMockWindow("Mock1Window");
    auto window2 = createMockWindow2("Mock2Window");

    registry.registerWindow(window1);
    registry.registerWindow(window2);

    // hasWindow should find windows regardless of type
    EXPECT_TRUE(registry.hasWindow("Mock1Window"));
    EXPECT_TRUE(registry.hasWindow("Mock2Window"));
}

// =============================================================================
// Unregistration Tests
// =============================================================================

TEST_F(WindowRegistryTest, UnregisterWindow_RemovesWindow) {
    auto window = createMockWindow("TestWindow");
    registry.registerWindow(window);
    EXPECT_TRUE(registry.hasWindow("TestWindow"));

    registry.unregisterWindow<testing::MockDocumentWindow>("TestWindow");
    EXPECT_FALSE(registry.hasWindow("TestWindow"));
}

TEST_F(WindowRegistryTest, UnregisterWindow_DoesNotThrowForMissing) {
    // Should log warning but not throw
    EXPECT_NO_THROW(registry.unregisterWindow<testing::MockDocumentWindow>("NonExistent"));
}

TEST_F(WindowRegistryTest, UnregisterWindow_LeavesOthersIntact) {
    auto window1 = createMockWindow("Window1");
    auto window2 = createMockWindow("Window2");
    auto window3 = createMockWindow("Window3");

    registry.registerWindow(window1);
    registry.registerWindow(window2);
    registry.registerWindow(window3);

    registry.unregisterWindow<testing::MockDocumentWindow>("Window2");

    EXPECT_TRUE(registry.hasWindow("Window1"));
    EXPECT_FALSE(registry.hasWindow("Window2"));
    EXPECT_TRUE(registry.hasWindow("Window3"));
}

TEST_F(WindowRegistryTest, UnregisterWindow_TypeSpecific) {
    auto mock1 = createMockWindow("SharedName");
    auto mock2 = createMockWindow2("SharedName");

    registry.registerWindow(mock1);
    registry.registerWindow(mock2);

    // Unregister only the first type
    registry.unregisterWindow<testing::MockDocumentWindow>("SharedName");

    // MockDocumentWindow2 with same name should still exist
    auto retrieved = registry.getWindow<testing::MockDocumentWindow2>("SharedName");
    EXPECT_FALSE(retrieved.expired());
}

// =============================================================================
// Lifecycle Delegation Tests
// =============================================================================

TEST_F(WindowRegistryTest, Setup_CallsSetupOnAllWindows) {
    auto window1 = createMockWindow("Window1");
    auto window2 = createMockWindow("Window2");

    EXPECT_CALL(*window1, setup()).Times(1);
    EXPECT_CALL(*window2, setup()).Times(1);

    registry.registerWindow(window1);
    registry.registerWindow(window2);

    registry.setup();
}

TEST_F(WindowRegistryTest, Shutdown_CallsShutdownOnAllWindows) {
    auto window1 = createMockWindow("Window1");
    auto window2 = createMockWindow("Window2");

    EXPECT_CALL(*window1, shutdown()).Times(1);
    EXPECT_CALL(*window2, shutdown()).Times(1);

    registry.registerWindow(window1);
    registry.registerWindow(window2);

    registry.shutdown();
}

TEST_F(WindowRegistryTest, Update_CallsUpdateOnAllWindows) {
    auto window1 = createMockWindow("Window1");
    auto window2 = createMockWindow("Window2");

    EXPECT_CALL(*window1, update()).Times(1);
    EXPECT_CALL(*window2, update()).Times(1);

    registry.registerWindow(window1);
    registry.registerWindow(window2);

    registry.update();
}

TEST_F(WindowRegistryTest, Render_CallsShowOnOpenWindows) {
    auto window = createMockWindow("OpenWindow");

    EXPECT_CALL(*window, isOpened()).WillOnce(Return(true));
    EXPECT_CALL(*window, show()).Times(1);

    registry.registerWindow(window);
    registry.render();
}

TEST_F(WindowRegistryTest, Render_SkipsClosedWindows) {
    auto window = createMockWindow("ClosedWindow");

    EXPECT_CALL(*window, isOpened()).WillOnce(Return(false));
    EXPECT_CALL(*window, show()).Times(0);  // Should NOT be called

    registry.registerWindow(window);
    registry.render();
}

TEST_F(WindowRegistryTest, Render_MixedOpenClosed) {
    auto openWindow = createMockWindow("OpenWindow");
    auto closedWindow = createMockWindow("ClosedWindow");

    EXPECT_CALL(*openWindow, isOpened()).WillOnce(Return(true));
    EXPECT_CALL(*openWindow, show()).Times(1);

    EXPECT_CALL(*closedWindow, isOpened()).WillOnce(Return(false));
    EXPECT_CALL(*closedWindow, show()).Times(0);

    registry.registerWindow(openWindow);
    registry.registerWindow(closedWindow);

    registry.render();
}

// =============================================================================
// Focus Tests
// =============================================================================

TEST_F(WindowRegistryTest, GetFocusedWindow_ReturnsFocused) {
    auto unfocused = createMockWindow("Unfocused");
    auto focused = createMockWindow("Focused");

    EXPECT_CALL(*unfocused, isFocused()).WillRepeatedly(Return(false));
    EXPECT_CALL(*focused, isFocused()).WillRepeatedly(Return(true));

    registry.registerWindow(unfocused);
    registry.registerWindow(focused);

    auto result = registry.getFocusedWindow();
    ASSERT_NE(result, nullptr);
    EXPECT_EQ(result->getWindowName(), "Focused");
}

TEST_F(WindowRegistryTest, GetFocusedWindow_ReturnsNullIfNone) {
    auto window1 = createMockWindow("Window1");
    auto window2 = createMockWindow("Window2");

    EXPECT_CALL(*window1, isFocused()).WillRepeatedly(Return(false));
    EXPECT_CALL(*window2, isFocused()).WillRepeatedly(Return(false));

    registry.registerWindow(window1);
    registry.registerWindow(window2);

    auto result = registry.getFocusedWindow();
    EXPECT_EQ(result, nullptr);
}

TEST_F(WindowRegistryTest, GetFocusedWindow_EmptyRegistry) {
    auto result = registry.getFocusedWindow();
    EXPECT_EQ(result, nullptr);
}

// =============================================================================
// Docking Delegation Tests
// =============================================================================

TEST_F(WindowRegistryTest, SetDockId_DelegatesToDockingRegistry) {
    registry.setDockId("TestWindow", 123);
    auto result = registry.getDockId("TestWindow");

    ASSERT_TRUE(result.has_value());
    EXPECT_EQ(result.value(), 123u);
}

TEST_F(WindowRegistryTest, GetDockId_ReturnsNulloptForMissing) {
    auto result = registry.getDockId("NonExistent");
    EXPECT_FALSE(result.has_value());
}

TEST_F(WindowRegistryTest, ResetDockId_RemovesDockId) {
    registry.setDockId("TestWindow", 456);
    EXPECT_TRUE(registry.getDockId("TestWindow").has_value());

    registry.resetDockId("TestWindow");
    EXPECT_FALSE(registry.getDockId("TestWindow").has_value());
}

// =============================================================================
// Edge Cases
// =============================================================================

TEST_F(WindowRegistryTest, EmptyWindowName) {
    auto window = createMockWindow("");
    EXPECT_NO_THROW(registry.registerWindow(window));
    EXPECT_TRUE(registry.hasWindow(""));
}

TEST_F(WindowRegistryTest, WindowNameWithSpecialCharacters) {
    auto window = createMockWindow("###Window/With:Special*Chars");
    EXPECT_NO_THROW(registry.registerWindow(window));
    EXPECT_TRUE(registry.hasWindow("###Window/With:Special*Chars"));
}

TEST_F(WindowRegistryTest, LargeNumberOfWindows) {
    constexpr int NUM_WINDOWS = 50;
    std::vector<std::shared_ptr<testing::MockDocumentWindow>> windows;

    for (int i = 0; i < NUM_WINDOWS; ++i) {
        auto window = createMockWindow("Window" + std::to_string(i));
        windows.push_back(window);
        registry.registerWindow(window);
    }

    for (int i = 0; i < NUM_WINDOWS; ++i) {
        EXPECT_TRUE(registry.hasWindow("Window" + std::to_string(i)));
    }

    auto allWindows = registry.getWindows<testing::MockDocumentWindow>();
    int count = 0;
    for ([[maybe_unused]] const auto& w : allWindows) {
        count++;
    }
    EXPECT_EQ(count, NUM_WINDOWS);
}

}  // namespace nexo::editor
