//// WindowProperty.test.cpp ///////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        12/12/2025
//  Description: Test file for NxWindowProperty struct
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "renderer/Window.hpp"

namespace nexo::renderer {

    // ==========================================================================
    // Constructor Tests
    // ==========================================================================

    TEST(WindowPropertyTest, ConstructorSetsWidthCorrectly) {
        NxWindowProperty props(1920, 1080, "Test Window");
        EXPECT_EQ(props.width, 1920);
    }

    TEST(WindowPropertyTest, ConstructorSetsHeightCorrectly) {
        NxWindowProperty props(1920, 1080, "Test Window");
        EXPECT_EQ(props.height, 1080);
    }

    TEST(WindowPropertyTest, ConstructorSetsTitleCorrectly) {
        NxWindowProperty props(1920, 1080, "Test Window");
        EXPECT_EQ(props.title, "Test Window");
    }

    TEST(WindowPropertyTest, DefaultVsyncIsTrue) {
        NxWindowProperty props(800, 600, "Window");
        EXPECT_TRUE(props.vsync);
    }

    TEST(WindowPropertyTest, DefaultIsDarkModeIsFalse) {
        NxWindowProperty props(800, 600, "Window");
        EXPECT_FALSE(props.isDarkMode);
    }

    TEST(WindowPropertyTest, ResizeCallbackIsEmptyByDefault) {
        NxWindowProperty props(1920, 1080, "Test Window");
        EXPECT_FALSE(props.resizeCallback);
    }

    TEST(WindowPropertyTest, CloseCallbackIsEmptyByDefault) {
        NxWindowProperty props(1920, 1080, "Test Window");
        EXPECT_FALSE(props.closeCallback);
    }

    TEST(WindowPropertyTest, KeyCallbackIsEmptyByDefault) {
        NxWindowProperty props(1920, 1080, "Test Window");
        EXPECT_FALSE(props.keyCallback);
    }

    TEST(WindowPropertyTest, MouseClickCallbackIsEmptyByDefault) {
        NxWindowProperty props(1920, 1080, "Test Window");
        EXPECT_FALSE(props.mouseClickCallback);
    }

    TEST(WindowPropertyTest, MouseScrollCallbackIsEmptyByDefault) {
        NxWindowProperty props(1920, 1080, "Test Window");
        EXPECT_FALSE(props.mouseScrollCallback);
    }

    TEST(WindowPropertyTest, MouseMoveCallbackIsEmptyByDefault) {
        NxWindowProperty props(1920, 1080, "Test Window");
        EXPECT_FALSE(props.mouseMoveCallback);
    }

    TEST(WindowPropertyTest, FileDropCallbackIsEmptyByDefault) {
        NxWindowProperty props(1920, 1080, "Test Window");
        EXPECT_FALSE(props.fileDropCallback);
    }

    // ==========================================================================
    // Value Tests
    // ==========================================================================

    TEST(WindowPropertyTest, ZeroDimensions) {
        NxWindowProperty props(0, 0, "Zero Size Window");
        EXPECT_EQ(props.width, 0);
        EXPECT_EQ(props.height, 0);
        EXPECT_EQ(props.title, "Zero Size Window");
    }

    TEST(WindowPropertyTest, LargeDimensions) {
        NxWindowProperty props(3840, 2160, "4K Window");
        EXPECT_EQ(props.width, 3840);
        EXPECT_EQ(props.height, 2160);
    }

    TEST(WindowPropertyTest, UltraWideDimensions) {
        NxWindowProperty props(5120, 1440, "Ultra Wide");
        EXPECT_EQ(props.width, 5120);
        EXPECT_EQ(props.height, 1440);
    }

    TEST(WindowPropertyTest, EmptyTitle) {
        NxWindowProperty props(800, 600, "");
        EXPECT_EQ(props.title, "");
        EXPECT_TRUE(props.title.empty());
    }

    TEST(WindowPropertyTest, LongTitleString) {
        std::string longTitle = "This is a very long window title that contains many characters and should test the string handling capabilities of the NxWindowProperty struct";
        NxWindowProperty props(1920, 1080, longTitle);
        EXPECT_EQ(props.title, longTitle);
        EXPECT_EQ(props.title.length(), longTitle.length());
    }

    TEST(WindowPropertyTest, TitleWithSpecialCharacters) {
        NxWindowProperty props(800, 600, "Window™ - Test © 2025 [Build #1234]");
        EXPECT_EQ(props.title, "Window™ - Test © 2025 [Build #1234]");
    }

    // ==========================================================================
    // Boolean Flag Modification Tests
    // ==========================================================================

    TEST(WindowPropertyTest, CanModifyVsyncFlag) {
        NxWindowProperty props(1920, 1080, "Window");
        EXPECT_TRUE(props.vsync);
        props.vsync = false;
        EXPECT_FALSE(props.vsync);
        props.vsync = true;
        EXPECT_TRUE(props.vsync);
    }

    TEST(WindowPropertyTest, CanModifyDarkModeFlag) {
        NxWindowProperty props(1920, 1080, "Window");
        EXPECT_FALSE(props.isDarkMode);
        props.isDarkMode = true;
        EXPECT_TRUE(props.isDarkMode);
        props.isDarkMode = false;
        EXPECT_FALSE(props.isDarkMode);
    }

    // ==========================================================================
    // Callback Assignment Tests
    // ==========================================================================

    TEST(WindowPropertyTest, CanSetResizeCallback) {
        NxWindowProperty props(1920, 1080, "Test Window");
        props.resizeCallback = [](int w, int h) { /* no-op */ };
        EXPECT_TRUE(props.resizeCallback);
    }

    TEST(WindowPropertyTest, CanSetCloseCallback) {
        NxWindowProperty props(1920, 1080, "Test Window");
        props.closeCallback = []() { /* no-op */ };
        EXPECT_TRUE(props.closeCallback);
    }

    TEST(WindowPropertyTest, CanSetKeyCallback) {
        NxWindowProperty props(1920, 1080, "Test Window");
        props.keyCallback = [](int key, int scancode, int action) { /* no-op */ };
        EXPECT_TRUE(props.keyCallback);
    }

    TEST(WindowPropertyTest, CanSetMouseClickCallback) {
        NxWindowProperty props(1920, 1080, "Test Window");
        props.mouseClickCallback = [](int button, int action, int mods) { /* no-op */ };
        EXPECT_TRUE(props.mouseClickCallback);
    }

    TEST(WindowPropertyTest, CanSetMouseScrollCallback) {
        NxWindowProperty props(1920, 1080, "Test Window");
        props.mouseScrollCallback = [](double xoffset, double yoffset) { /* no-op */ };
        EXPECT_TRUE(props.mouseScrollCallback);
    }

    TEST(WindowPropertyTest, CanSetMouseMoveCallback) {
        NxWindowProperty props(1920, 1080, "Test Window");
        props.mouseMoveCallback = [](double xpos, double ypos) { /* no-op */ };
        EXPECT_TRUE(props.mouseMoveCallback);
    }

    TEST(WindowPropertyTest, CanSetFileDropCallback) {
        NxWindowProperty props(1920, 1080, "Test Window");
        props.fileDropCallback = [](int count, const char** paths) { /* no-op */ };
        EXPECT_TRUE(props.fileDropCallback);
    }

    // ==========================================================================
    // Callback Invocation Tests
    // ==========================================================================

    TEST(WindowPropertyTest, ResizeCallbackIsCallable) {
        NxWindowProperty props(1920, 1080, "Test Window");
        int capturedWidth = 0;
        int capturedHeight = 0;

        props.resizeCallback = [&capturedWidth, &capturedHeight](int w, int h) {
            capturedWidth = w;
            capturedHeight = h;
        };

        props.resizeCallback(800, 600);
        EXPECT_EQ(capturedWidth, 800);
        EXPECT_EQ(capturedHeight, 600);
    }

    TEST(WindowPropertyTest, CloseCallbackIsCallable) {
        NxWindowProperty props(1920, 1080, "Test Window");
        bool wasCalled = false;

        props.closeCallback = [&wasCalled]() {
            wasCalled = true;
        };

        props.closeCallback();
        EXPECT_TRUE(wasCalled);
    }

    TEST(WindowPropertyTest, KeyCallbackIsCallable) {
        NxWindowProperty props(1920, 1080, "Test Window");
        int capturedKey = 0;
        int capturedScancode = 0;
        int capturedAction = 0;

        props.keyCallback = [&capturedKey, &capturedScancode, &capturedAction](int key, int scancode, int action) {
            capturedKey = key;
            capturedScancode = scancode;
            capturedAction = action;
        };

        props.keyCallback(65, 38, 1); // 'A' key press example
        EXPECT_EQ(capturedKey, 65);
        EXPECT_EQ(capturedScancode, 38);
        EXPECT_EQ(capturedAction, 1);
    }

    TEST(WindowPropertyTest, MouseClickCallbackIsCallable) {
        NxWindowProperty props(1920, 1080, "Test Window");
        int capturedButton = 0;
        int capturedAction = 0;
        int capturedMods = 0;

        props.mouseClickCallback = [&capturedButton, &capturedAction, &capturedMods](int button, int action, int mods) {
            capturedButton = button;
            capturedAction = action;
            capturedMods = mods;
        };

        props.mouseClickCallback(0, 1, 0); // Left button press
        EXPECT_EQ(capturedButton, 0);
        EXPECT_EQ(capturedAction, 1);
        EXPECT_EQ(capturedMods, 0);
    }

    TEST(WindowPropertyTest, MouseScrollCallbackIsCallable) {
        NxWindowProperty props(1920, 1080, "Test Window");
        double capturedXOffset = 0.0;
        double capturedYOffset = 0.0;

        props.mouseScrollCallback = [&capturedXOffset, &capturedYOffset](double xoffset, double yoffset) {
            capturedXOffset = xoffset;
            capturedYOffset = yoffset;
        };

        props.mouseScrollCallback(1.5, -2.5);
        EXPECT_DOUBLE_EQ(capturedXOffset, 1.5);
        EXPECT_DOUBLE_EQ(capturedYOffset, -2.5);
    }

    TEST(WindowPropertyTest, MouseMoveCallbackIsCallable) {
        NxWindowProperty props(1920, 1080, "Test Window");
        double capturedXPos = 0.0;
        double capturedYPos = 0.0;

        props.mouseMoveCallback = [&capturedXPos, &capturedYPos](double xpos, double ypos) {
            capturedXPos = xpos;
            capturedYPos = ypos;
        };

        props.mouseMoveCallback(123.45, 678.90);
        EXPECT_DOUBLE_EQ(capturedXPos, 123.45);
        EXPECT_DOUBLE_EQ(capturedYPos, 678.90);
    }

    TEST(WindowPropertyTest, FileDropCallbackIsCallable) {
        NxWindowProperty props(1920, 1080, "Test Window");
        int capturedCount = 0;
        const char* capturedPath = nullptr;

        props.fileDropCallback = [&capturedCount, &capturedPath](int count, const char** paths) {
            capturedCount = count;
            if (count > 0 && paths != nullptr) {
                capturedPath = paths[0];
            }
        };

        const char* testPaths[] = { "/path/to/file.txt" };
        props.fileDropCallback(1, testPaths);
        EXPECT_EQ(capturedCount, 1);
        EXPECT_STREQ(capturedPath, "/path/to/file.txt");
    }

    // ==========================================================================
    // Multiple Callback Invocation Tests
    // ==========================================================================

    TEST(WindowPropertyTest, ResizeCallbackCanBeCalledMultipleTimes) {
        NxWindowProperty props(1920, 1080, "Test Window");
        int callCount = 0;

        props.resizeCallback = [&callCount](int w, int h) {
            callCount++;
        };

        props.resizeCallback(800, 600);
        props.resizeCallback(1024, 768);
        props.resizeCallback(1920, 1080);

        EXPECT_EQ(callCount, 3);
    }

    TEST(WindowPropertyTest, CallbackCanBeReassigned) {
        NxWindowProperty props(1920, 1080, "Test Window");
        bool firstCalled = false;
        bool secondCalled = false;

        props.closeCallback = [&firstCalled]() {
            firstCalled = true;
        };

        props.closeCallback();
        EXPECT_TRUE(firstCalled);
        EXPECT_FALSE(secondCalled);

        // Reassign callback
        props.closeCallback = [&secondCalled]() {
            secondCalled = true;
        };

        props.closeCallback();
        EXPECT_TRUE(secondCalled);
    }

    TEST(WindowPropertyTest, CallbackCanBeCleared) {
        NxWindowProperty props(1920, 1080, "Test Window");
        props.closeCallback = []() { /* no-op */ };
        EXPECT_TRUE(props.closeCallback);

        props.closeCallback = nullptr;
        EXPECT_FALSE(props.closeCallback);
    }

    // ==========================================================================
    // Edge Case Tests
    // ==========================================================================

    TEST(WindowPropertyTest, MaxUnsignedIntDimensions) {
        unsigned int maxValue = std::numeric_limits<unsigned int>::max();
        NxWindowProperty props(maxValue, maxValue, "Max Size");
        EXPECT_EQ(props.width, maxValue);
        EXPECT_EQ(props.height, maxValue);
    }

    TEST(WindowPropertyTest, AsymmetricDimensions) {
        NxWindowProperty props(1, 4096, "Very Tall");
        EXPECT_EQ(props.width, 1);
        EXPECT_EQ(props.height, 4096);

        NxWindowProperty props2(4096, 1, "Very Wide");
        EXPECT_EQ(props2.width, 4096);
        EXPECT_EQ(props2.height, 1);
    }

    TEST(WindowPropertyTest, TitleWithUnicodeCharacters) {
        NxWindowProperty props(800, 600, "游戏引擎 - ゲームエンジン - محرك اللعبة");
        EXPECT_FALSE(props.title.empty());
    }

    TEST(WindowPropertyTest, MultiplePropertiesIndependent) {
        NxWindowProperty props1(800, 600, "Window 1");
        NxWindowProperty props2(1920, 1080, "Window 2");

        props1.vsync = false;
        props2.vsync = true;

        EXPECT_FALSE(props1.vsync);
        EXPECT_TRUE(props2.vsync);
        EXPECT_EQ(props1.title, "Window 1");
        EXPECT_EQ(props2.title, "Window 2");
    }

    // ==========================================================================
    // Struct Copy and Assignment Tests
    // ==========================================================================

    TEST(WindowPropertyTest, StructCanBeCopied) {
        NxWindowProperty props1(1920, 1080, "Original");
        props1.vsync = false;
        props1.isDarkMode = true;

        NxWindowProperty props2 = props1;

        EXPECT_EQ(props2.width, 1920);
        EXPECT_EQ(props2.height, 1080);
        EXPECT_EQ(props2.title, "Original");
        EXPECT_FALSE(props2.vsync);
        EXPECT_TRUE(props2.isDarkMode);
    }

    TEST(WindowPropertyTest, CallbacksCanBeCopied) {
        NxWindowProperty props1(800, 600, "Window");
        bool wasCalled = false;

        props1.closeCallback = [&wasCalled]() {
            wasCalled = true;
        };

        NxWindowProperty props2 = props1;
        EXPECT_TRUE(props2.closeCallback);

        props2.closeCallback();
        EXPECT_TRUE(wasCalled);
    }

}
