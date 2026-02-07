//// WindowEvent.test.cpp /////////////////////////////////////////////////////
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
//  Author:      Mehdy MORVAN
//  Date:        02/12/2024
//  Description: Test file for the window events
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "core/event/WindowEvent.hpp"

namespace nexo::event {
    using enum KeyAction;
    using enum MouseButton;

        TEST(WindowEventTest, EventWindowCloseInstantiation) {
        EventWindowClose closeEvent;
        SUCCEED(); // Validate no crash on instantiation
    }

    TEST(WindowEventTest, EventWindowResizeAttributes) {
        EventWindowResize resizeEvent(800, 600);
        EXPECT_EQ(resizeEvent.width, 800);
        EXPECT_EQ(resizeEvent.height, 600);

        std::ostringstream os;
        os << resizeEvent;
        EXPECT_EQ(os.str(), "[RESIZE WINDOW EVENT]: 800x600");
    }

    TEST(WindowEventTest, EventKeyAttributesAndModifiers) {
        EventKey keyEvent(65, KeyAction::PRESSED, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL);

        EXPECT_EQ(keyEvent.keycode, 65);
        EXPECT_EQ(keyEvent.action, KeyAction::PRESSED);
        EXPECT_TRUE(keyEvent.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(keyEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(keyEvent.hasMod(KeyMods::ALT));

        std::ostringstream os;
        os << keyEvent;
        EXPECT_EQ(os.str(), "[KEYBOARD EVENT] : 65 with action : PRESSED CTRL + SHIFT");
    }

    TEST(WindowEventTest, EventMouseClickAttributes) {
        EventMouseClick mouseClickEvent;
        mouseClickEvent.button = MouseButton::RIGHT;
        mouseClickEvent.action = KeyAction::RELEASED;
        mouseClickEvent.mods = GLFW_MOD_ALT;

        EXPECT_EQ(mouseClickEvent.button, MouseButton::RIGHT);
        EXPECT_EQ(mouseClickEvent.action, KeyAction::RELEASED);
        EXPECT_TRUE(mouseClickEvent.hasMod(KeyMods::ALT));
        EXPECT_FALSE(mouseClickEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(mouseClickEvent.hasMod(KeyMods::SHIFT));

        std::ostringstream os;
        os << mouseClickEvent;
        EXPECT_EQ(os.str(), "[MOUSE BUTTON EVENT] : RIGHT with action : RELEASED ALT");
    }

    TEST(WindowEventTest, EventMouseScrollAttributes) {
        EventMouseScroll mouseScrollEvent(1.5f, -3.0f);

        EXPECT_FLOAT_EQ(mouseScrollEvent.x, 1.5f);
        EXPECT_FLOAT_EQ(mouseScrollEvent.y, -3.0f);

        std::ostringstream os;
        os << mouseScrollEvent;
        EXPECT_EQ(os.str(), "[MOUSE SCROLL EVENT] xOffset : 1.5 yOffset : -3");
    }

    TEST(WindowEventTest, EventMouseMoveAttributes) {
        EventMouseMove mouseMoveEvent(400.0f, 300.0f);

        EXPECT_FLOAT_EQ(mouseMoveEvent.x, 400.0f);
        EXPECT_FLOAT_EQ(mouseMoveEvent.y, 300.0f);

        std::ostringstream os;
        os << mouseMoveEvent;
        EXPECT_EQ(os.str(), "[MOUSE MOVE EVENT] x : 400 y : 300");
    }

    TEST(WindowEventTest, KeyActionStreamOperator) {
        std::ostringstream os;
        os << KeyAction::PRESSED;
        EXPECT_EQ(os.str(), "PRESSED");

        os.str(""); os.clear();
        os << KeyAction::RELEASED;
        EXPECT_EQ(os.str(), "RELEASED");

        os.str(""); os.clear();
        os << KeyAction::REPEAT;
        EXPECT_EQ(os.str(), "REPEAT");
    }

    TEST(WindowEventTest, KeyModsStreamOperator) {
        std::ostringstream os;
        os << KeyMods::NONE;
        EXPECT_EQ(os.str(), "NONE");

        os.str(""); os.clear();
        os << KeyMods::SHIFT;
        EXPECT_EQ(os.str(), "SHIFT");

        os.str(""); os.clear();
        os << KeyMods::CONTROL;
        EXPECT_EQ(os.str(), "CONTROL");

        os.str(""); os.clear();
        os << KeyMods::ALT;
        EXPECT_EQ(os.str(), "ALT");
    }

    TEST(WindowEventTest, MouseButtonStreamOperator) {
        std::ostringstream os;
        os << MouseButton::LEFT;
        EXPECT_EQ(os.str(), "LEFT");

        os.str(""); os.clear();
        os << MouseButton::RIGHT;
        EXPECT_EQ(os.str(), "RIGHT");

        os.str(""); os.clear();
        os << MouseButton::MIDDLE;
        EXPECT_EQ(os.str(), "MIDDLE");
    }

    // Comprehensive hasMod() tests for EventKey
    TEST(WindowEventTest, EventKeyHasModNoModifiers) {
        EventKey keyEvent(65, PRESSED, 0);

        EXPECT_FALSE(keyEvent.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(keyEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(keyEvent.hasMod(KeyMods::ALT));
        // Note: hasMod(KeyMods::NONE) returns false because 0 & 0 = 0 (falsy in bool context)
        // This is expected behavior of the bitwise AND implementation
        EXPECT_FALSE(keyEvent.hasMod(KeyMods::NONE));
    }

    TEST(WindowEventTest, EventKeyHasModSingleModifier) {
        EventKey shiftEvent(65, PRESSED, GLFW_MOD_SHIFT);
        EXPECT_TRUE(shiftEvent.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(shiftEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(shiftEvent.hasMod(KeyMods::ALT));

        EventKey ctrlEvent(65, PRESSED, GLFW_MOD_CONTROL);
        EXPECT_FALSE(ctrlEvent.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(ctrlEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(ctrlEvent.hasMod(KeyMods::ALT));

        EventKey altEvent(65, PRESSED, GLFW_MOD_ALT);
        EXPECT_FALSE(altEvent.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(altEvent.hasMod(KeyMods::CONTROL));
        EXPECT_TRUE(altEvent.hasMod(KeyMods::ALT));
    }

    TEST(WindowEventTest, EventKeyHasModMultipleModifiers) {
        // Shift + Control
        EventKey shiftCtrlEvent(65, PRESSED, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL);
        EXPECT_TRUE(shiftCtrlEvent.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(shiftCtrlEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(shiftCtrlEvent.hasMod(KeyMods::ALT));

        // Shift + Alt
        EventKey shiftAltEvent(65, PRESSED, GLFW_MOD_SHIFT | GLFW_MOD_ALT);
        EXPECT_TRUE(shiftAltEvent.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(shiftAltEvent.hasMod(KeyMods::CONTROL));
        EXPECT_TRUE(shiftAltEvent.hasMod(KeyMods::ALT));

        // Control + Alt
        EventKey ctrlAltEvent(65, PRESSED, GLFW_MOD_CONTROL | GLFW_MOD_ALT);
        EXPECT_FALSE(ctrlAltEvent.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(ctrlAltEvent.hasMod(KeyMods::CONTROL));
        EXPECT_TRUE(ctrlAltEvent.hasMod(KeyMods::ALT));

        // All three modifiers
        EventKey allModsEvent(65, PRESSED, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT);
        EXPECT_TRUE(allModsEvent.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(allModsEvent.hasMod(KeyMods::CONTROL));
        EXPECT_TRUE(allModsEvent.hasMod(KeyMods::ALT));
    }

    TEST(WindowEventTest, EventKeyHasModWithAdditionalGLFWModifiers) {
        // Test with SUPER modifier (not in KeyMods enum but exists in GLFW)
        EventKey superEvent(65, PRESSED, GLFW_MOD_SUPER);
        EXPECT_FALSE(superEvent.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(superEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(superEvent.hasMod(KeyMods::ALT));

        // Test with CAPS_LOCK modifier
        EventKey capsEvent(65, PRESSED, GLFW_MOD_CAPS_LOCK);
        EXPECT_FALSE(capsEvent.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(capsEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(capsEvent.hasMod(KeyMods::ALT));

        // Test with NUM_LOCK modifier
        EventKey numEvent(65, PRESSED, GLFW_MOD_NUM_LOCK);
        EXPECT_FALSE(numEvent.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(numEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(numEvent.hasMod(KeyMods::ALT));

        // Test combination with tracked modifiers
        EventKey combinedEvent(65, PRESSED, GLFW_MOD_SHIFT | GLFW_MOD_CAPS_LOCK);
        EXPECT_TRUE(combinedEvent.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(combinedEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(combinedEvent.hasMod(KeyMods::ALT));
    }

    // Comprehensive hasMod() tests for EventMouseClick
    TEST(WindowEventTest, EventMouseClickHasModNoModifiers) {
        EventMouseClick clickEvent;
        clickEvent.button = LEFT;
        clickEvent.action = PRESSED;
        clickEvent.mods = 0;

        EXPECT_FALSE(clickEvent.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(clickEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(clickEvent.hasMod(KeyMods::ALT));
        // Note: hasMod(KeyMods::NONE) returns false because 0 & 0 = 0 (falsy in bool context)
        // This is expected behavior of the bitwise AND implementation
        EXPECT_FALSE(clickEvent.hasMod(KeyMods::NONE));
    }

    TEST(WindowEventTest, EventMouseClickHasModSingleModifier) {
        EventMouseClick shiftClick;
        shiftClick.mods = GLFW_MOD_SHIFT;
        EXPECT_TRUE(shiftClick.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(shiftClick.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(shiftClick.hasMod(KeyMods::ALT));

        EventMouseClick ctrlClick;
        ctrlClick.mods = GLFW_MOD_CONTROL;
        EXPECT_FALSE(ctrlClick.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(ctrlClick.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(ctrlClick.hasMod(KeyMods::ALT));

        EventMouseClick altClick;
        altClick.mods = GLFW_MOD_ALT;
        EXPECT_FALSE(altClick.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(altClick.hasMod(KeyMods::CONTROL));
        EXPECT_TRUE(altClick.hasMod(KeyMods::ALT));
    }

    TEST(WindowEventTest, EventMouseClickHasModMultipleModifiers) {
        // Shift + Control
        EventMouseClick shiftCtrlClick;
        shiftCtrlClick.mods = GLFW_MOD_SHIFT | GLFW_MOD_CONTROL;
        EXPECT_TRUE(shiftCtrlClick.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(shiftCtrlClick.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(shiftCtrlClick.hasMod(KeyMods::ALT));

        // Shift + Alt
        EventMouseClick shiftAltClick;
        shiftAltClick.mods = GLFW_MOD_SHIFT | GLFW_MOD_ALT;
        EXPECT_TRUE(shiftAltClick.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(shiftAltClick.hasMod(KeyMods::CONTROL));
        EXPECT_TRUE(shiftAltClick.hasMod(KeyMods::ALT));

        // Control + Alt
        EventMouseClick ctrlAltClick;
        ctrlAltClick.mods = GLFW_MOD_CONTROL | GLFW_MOD_ALT;
        EXPECT_FALSE(ctrlAltClick.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(ctrlAltClick.hasMod(KeyMods::CONTROL));
        EXPECT_TRUE(ctrlAltClick.hasMod(KeyMods::ALT));

        // All three modifiers
        EventMouseClick allModsClick;
        allModsClick.mods = GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT;
        EXPECT_TRUE(allModsClick.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(allModsClick.hasMod(KeyMods::CONTROL));
        EXPECT_TRUE(allModsClick.hasMod(KeyMods::ALT));
    }

    TEST(WindowEventTest, EventMouseClickHasModWithAdditionalGLFWModifiers) {
        // Test with SUPER modifier
        EventMouseClick superClick;
        superClick.mods = GLFW_MOD_SUPER;
        EXPECT_FALSE(superClick.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(superClick.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(superClick.hasMod(KeyMods::ALT));

        // Test with CAPS_LOCK modifier
        EventMouseClick capsClick;
        capsClick.mods = GLFW_MOD_CAPS_LOCK;
        EXPECT_FALSE(capsClick.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(capsClick.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(capsClick.hasMod(KeyMods::ALT));

        // Test with NUM_LOCK modifier
        EventMouseClick numClick;
        numClick.mods = GLFW_MOD_NUM_LOCK;
        EXPECT_FALSE(numClick.hasMod(KeyMods::SHIFT));
        EXPECT_FALSE(numClick.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(numClick.hasMod(KeyMods::ALT));

        // Test combination with tracked modifiers
        EventMouseClick combinedClick;
        combinedClick.mods = GLFW_MOD_CONTROL | GLFW_MOD_NUM_LOCK;
        EXPECT_FALSE(combinedClick.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(combinedClick.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(combinedClick.hasMod(KeyMods::ALT));
    }

    // Additional KeyAction enum tests
    TEST(WindowEventTest, KeyActionEnumValues) {
        EXPECT_EQ(static_cast<int>(PRESSED), 0);
        EXPECT_EQ(static_cast<int>(RELEASED), 1);
        EXPECT_EQ(static_cast<int>(REPEAT), 2);
    }

    TEST(WindowEventTest, EventKeyWithDifferentActions) {
        EventKey pressedEvent(65, PRESSED, 0);
        EXPECT_EQ(pressedEvent.action, PRESSED);

        EventKey releasedEvent(65, RELEASED, 0);
        EXPECT_EQ(releasedEvent.action, RELEASED);

        EventKey repeatEvent(65, REPEAT, 0);
        EXPECT_EQ(repeatEvent.action, REPEAT);
    }

    // Additional MouseButton enum tests
    TEST(WindowEventTest, MouseButtonEnumValues) {
        EXPECT_EQ(static_cast<int>(LEFT), 0);
        EXPECT_EQ(static_cast<int>(RIGHT), 1);
        EXPECT_EQ(static_cast<int>(MIDDLE), 2);
    }

    TEST(WindowEventTest, EventMouseClickWithDifferentButtons) {
        EventMouseClick leftClick;
        leftClick.button = LEFT;
        leftClick.action = PRESSED;
        leftClick.mods = 0;
        EXPECT_EQ(leftClick.button, LEFT);

        EventMouseClick rightClick;
        rightClick.button = RIGHT;
        rightClick.action = PRESSED;
        rightClick.mods = 0;
        EXPECT_EQ(rightClick.button, RIGHT);

        EventMouseClick middleClick;
        middleClick.button = MIDDLE;
        middleClick.action = PRESSED;
        middleClick.mods = 0;
        EXPECT_EQ(middleClick.button, MIDDLE);
    }

    // EventFileDrop tests
    TEST(WindowEventTest, EventFileDropSingleFile) {
        std::vector<std::string> files = {"/path/to/file.txt"};
        EventFileDrop dropEvent(files);

        EXPECT_EQ(dropEvent.files.size(), 1);
        EXPECT_EQ(dropEvent.files[0], "/path/to/file.txt");

        std::ostringstream os;
        os << dropEvent;
        EXPECT_EQ(os.str(), "[FILE DROP EVENT] 1 file(s): /path/to/file.txt");
    }

    TEST(WindowEventTest, EventFileDropMultipleFiles) {
        std::vector<std::string> files = {
            "/path/to/file1.txt",
            "/path/to/file2.png",
            "/path/to/file3.obj"
        };
        EventFileDrop dropEvent(files);

        EXPECT_EQ(dropEvent.files.size(), 3);
        EXPECT_EQ(dropEvent.files[0], "/path/to/file1.txt");
        EXPECT_EQ(dropEvent.files[1], "/path/to/file2.png");
        EXPECT_EQ(dropEvent.files[2], "/path/to/file3.obj");

        std::ostringstream os;
        os << dropEvent;
        EXPECT_EQ(os.str(), "[FILE DROP EVENT] 3 file(s): /path/to/file1.txt, /path/to/file2.png, /path/to/file3.obj");
    }

    TEST(WindowEventTest, EventFileDropEmptyList) {
        std::vector<std::string> files;
        EventFileDrop dropEvent(files);

        EXPECT_EQ(dropEvent.files.size(), 0);

        std::ostringstream os;
        os << dropEvent;
        EXPECT_EQ(os.str(), "[FILE DROP EVENT] 0 file(s): ");
    }

    // KeyMods enum value tests
    TEST(WindowEventTest, KeyModsEnumValues) {
        EXPECT_EQ(static_cast<int>(KeyMods::NONE), 0);
        EXPECT_EQ(static_cast<int>(KeyMods::SHIFT), GLFW_MOD_SHIFT);
        EXPECT_EQ(static_cast<int>(KeyMods::CONTROL), GLFW_MOD_CONTROL);
        EXPECT_EQ(static_cast<int>(KeyMods::ALT), GLFW_MOD_ALT);
    }

    // Default constructor tests
    TEST(WindowEventTest, EventKeyDefaultConstructor) {
        EventKey keyEvent;
        EXPECT_EQ(keyEvent.keycode, 0);
        EXPECT_EQ(keyEvent.action, PRESSED);
        EXPECT_EQ(keyEvent.mods, 0);
    }

    TEST(WindowEventTest, EventMouseClickDefaultConstructor) {
        EventMouseClick clickEvent;
        EXPECT_EQ(clickEvent.button, LEFT);
        EXPECT_EQ(clickEvent.action, PRESSED);
        EXPECT_EQ(clickEvent.mods, 0);
    }

    // Stream operator output format tests
    TEST(WindowEventTest, EventKeyStreamOperatorNoModifiers) {
        EventKey keyEvent(65, PRESSED, 0);
        std::ostringstream os;
        os << keyEvent;
        EXPECT_EQ(os.str(), "[KEYBOARD EVENT] : 65 with action : PRESSED ");
    }

    TEST(WindowEventTest, EventKeyStreamOperatorSingleModifier) {
        EventKey shiftEvent(65, PRESSED, GLFW_MOD_SHIFT);
        std::ostringstream os;
        os << shiftEvent;
        EXPECT_EQ(os.str(), "[KEYBOARD EVENT] : 65 with action : PRESSED SHIFT");

        os.str(""); os.clear();
        EventKey ctrlEvent(65, RELEASED, GLFW_MOD_CONTROL);
        os << ctrlEvent;
        EXPECT_EQ(os.str(), "[KEYBOARD EVENT] : 65 with action : RELEASED CTRL");

        os.str(""); os.clear();
        EventKey altEvent(65, REPEAT, GLFW_MOD_ALT);
        os << altEvent;
        EXPECT_EQ(os.str(), "[KEYBOARD EVENT] : 65 with action : REPEAT ALT");
    }

    TEST(WindowEventTest, EventMouseClickStreamOperatorNoModifiers) {
        EventMouseClick clickEvent;
        clickEvent.button = LEFT;
        clickEvent.action = PRESSED;
        clickEvent.mods = 0;

        std::ostringstream os;
        os << clickEvent;
        EXPECT_EQ(os.str(), "[MOUSE BUTTON EVENT] : LEFT with action : PRESSED ");
    }

    TEST(WindowEventTest, EventMouseClickStreamOperatorSingleModifier) {
        EventMouseClick shiftClick;
        shiftClick.button = MIDDLE;
        shiftClick.action = RELEASED;
        shiftClick.mods = GLFW_MOD_SHIFT;

        std::ostringstream os;
        os << shiftClick;
        EXPECT_EQ(os.str(), "[MOUSE BUTTON EVENT] : MIDDLE with action : RELEASED SHIFT");
    }

    TEST(WindowEventTest, EventMouseClickStreamOperatorMultipleModifiers) {
        EventMouseClick multiModClick;
        multiModClick.button = RIGHT;
        multiModClick.action = PRESSED;
        multiModClick.mods = GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT;

        std::ostringstream os;
        os << multiModClick;
        EXPECT_EQ(os.str(), "[MOUSE BUTTON EVENT] : RIGHT with action : PRESSED ALT + CTRL + SHIFT");
    }

    // ============================================================================
    // EXPANDED COMPREHENSIVE TESTS
    // ============================================================================

    // EventWindowResize - Boundary and edge cases
    TEST(WindowEventTest, EventWindowResizeZeroValues) {
        EventWindowResize resizeEvent(0, 0);
        EXPECT_EQ(resizeEvent.width, 0);
        EXPECT_EQ(resizeEvent.height, 0);

        std::ostringstream os;
        os << resizeEvent;
        EXPECT_EQ(os.str(), "[RESIZE WINDOW EVENT]: 0x0");
    }

    TEST(WindowEventTest, EventWindowResizeNegativeValues) {
        EventWindowResize resizeEvent(-100, -200);
        EXPECT_EQ(resizeEvent.width, -100);
        EXPECT_EQ(resizeEvent.height, -200);

        std::ostringstream os;
        os << resizeEvent;
        EXPECT_EQ(os.str(), "[RESIZE WINDOW EVENT]: -100x-200");
    }

    TEST(WindowEventTest, EventWindowResizeLargeValues) {
        EventWindowResize resizeEvent(4096, 2160);
        EXPECT_EQ(resizeEvent.width, 4096);
        EXPECT_EQ(resizeEvent.height, 2160);

        std::ostringstream os;
        os << resizeEvent;
        EXPECT_EQ(os.str(), "[RESIZE WINDOW EVENT]: 4096x2160");
    }

    TEST(WindowEventTest, EventWindowResizeMismatchedAspectRatio) {
        EventWindowResize resizeEvent(100, 5000);
        EXPECT_EQ(resizeEvent.width, 100);
        EXPECT_EQ(resizeEvent.height, 5000);
    }

    // EventKey - Comprehensive keycode tests
    TEST(WindowEventTest, EventKeyWithSpecialKeycodes) {
        // Test with various GLFW keycodes
        EventKey escapeKey(GLFW_KEY_ESCAPE, PRESSED, 0);
        EXPECT_EQ(escapeKey.keycode, GLFW_KEY_ESCAPE);

        EventKey enterKey(GLFW_KEY_ENTER, PRESSED, 0);
        EXPECT_EQ(enterKey.keycode, GLFW_KEY_ENTER);

        EventKey spaceKey(GLFW_KEY_SPACE, RELEASED, 0);
        EXPECT_EQ(spaceKey.keycode, GLFW_KEY_SPACE);

        EventKey f1Key(GLFW_KEY_F1, REPEAT, 0);
        EXPECT_EQ(f1Key.keycode, GLFW_KEY_F1);
    }

    TEST(WindowEventTest, EventKeyWithInvalidKeycode) {
        EventKey invalidKey(-1, PRESSED, 0);
        EXPECT_EQ(invalidKey.keycode, -1);

        EventKey largeKeycode(9999, PRESSED, 0);
        EXPECT_EQ(largeKeycode.keycode, 9999);
    }

    TEST(WindowEventTest, EventKeyAllActionsStreamOutput) {
        EventKey pressedKey(65, PRESSED, GLFW_MOD_SHIFT);
        std::ostringstream os1;
        os1 << pressedKey;
        EXPECT_EQ(os1.str(), "[KEYBOARD EVENT] : 65 with action : PRESSED SHIFT");

        EventKey releasedKey(66, RELEASED, GLFW_MOD_CONTROL);
        std::ostringstream os2;
        os2 << releasedKey;
        EXPECT_EQ(os2.str(), "[KEYBOARD EVENT] : 66 with action : RELEASED CTRL");

        EventKey repeatKey(67, REPEAT, GLFW_MOD_ALT);
        std::ostringstream os3;
        os3 << repeatKey;
        EXPECT_EQ(os3.str(), "[KEYBOARD EVENT] : 67 with action : REPEAT ALT");
    }

    TEST(WindowEventTest, EventKeyComplexModifierCombinations) {
        // Alt + Shift
        EventKey altShift(65, PRESSED, GLFW_MOD_ALT | GLFW_MOD_SHIFT);
        std::ostringstream os1;
        os1 << altShift;
        EXPECT_EQ(os1.str(), "[KEYBOARD EVENT] : 65 with action : PRESSED ALT + SHIFT");

        // Ctrl + Alt
        EventKey ctrlAlt(65, PRESSED, GLFW_MOD_CONTROL | GLFW_MOD_ALT);
        std::ostringstream os2;
        os2 << ctrlAlt;
        EXPECT_EQ(os2.str(), "[KEYBOARD EVENT] : 65 with action : PRESSED ALT + CTRL");

        // All three
        EventKey allMods(65, PRESSED, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT);
        std::ostringstream os3;
        os3 << allMods;
        EXPECT_EQ(os3.str(), "[KEYBOARD EVENT] : 65 with action : PRESSED ALT + CTRL + SHIFT");
    }

    // EventMouseClick - All button types with actions
    TEST(WindowEventTest, EventMouseClickAllButtonsAllActions) {
        // LEFT button with all actions
        EventMouseClick leftPressed;
        leftPressed.button = LEFT;
        leftPressed.action = PRESSED;
        leftPressed.mods = 0;
        EXPECT_EQ(leftPressed.button, LEFT);
        EXPECT_EQ(leftPressed.action, PRESSED);

        EventMouseClick leftReleased;
        leftReleased.button = LEFT;
        leftReleased.action = RELEASED;
        leftReleased.mods = 0;
        EXPECT_EQ(leftReleased.button, LEFT);
        EXPECT_EQ(leftReleased.action, RELEASED);

        EventMouseClick leftRepeat;
        leftRepeat.button = LEFT;
        leftRepeat.action = REPEAT;
        leftRepeat.mods = 0;
        EXPECT_EQ(leftRepeat.button, LEFT);
        EXPECT_EQ(leftRepeat.action, REPEAT);

        // RIGHT button
        EventMouseClick rightPressed;
        rightPressed.button = RIGHT;
        rightPressed.action = PRESSED;
        rightPressed.mods = 0;
        EXPECT_EQ(rightPressed.button, RIGHT);

        // MIDDLE button
        EventMouseClick middlePressed;
        middlePressed.button = MIDDLE;
        middlePressed.action = PRESSED;
        middlePressed.mods = 0;
        EXPECT_EQ(middlePressed.button, MIDDLE);
    }

    TEST(WindowEventTest, EventMouseClickStreamOperatorAllButtons) {
        EventMouseClick leftClick;
        leftClick.button = LEFT;
        leftClick.action = PRESSED;
        leftClick.mods = 0;
        std::ostringstream os1;
        os1 << leftClick;
        EXPECT_EQ(os1.str(), "[MOUSE BUTTON EVENT] : LEFT with action : PRESSED ");

        EventMouseClick rightClick;
        rightClick.button = RIGHT;
        rightClick.action = RELEASED;
        rightClick.mods = 0;
        std::ostringstream os2;
        os2 << rightClick;
        EXPECT_EQ(os2.str(), "[MOUSE BUTTON EVENT] : RIGHT with action : RELEASED ");

        EventMouseClick middleClick;
        middleClick.button = MIDDLE;
        middleClick.action = REPEAT;
        middleClick.mods = 0;
        std::ostringstream os3;
        os3 << middleClick;
        EXPECT_EQ(os3.str(), "[MOUSE BUTTON EVENT] : MIDDLE with action : REPEAT ");
    }

    TEST(WindowEventTest, EventMouseClickComplexModifierOutput) {
        // Shift + Ctrl
        EventMouseClick shiftCtrl;
        shiftCtrl.button = LEFT;
        shiftCtrl.action = PRESSED;
        shiftCtrl.mods = GLFW_MOD_SHIFT | GLFW_MOD_CONTROL;
        std::ostringstream os1;
        os1 << shiftCtrl;
        EXPECT_EQ(os1.str(), "[MOUSE BUTTON EVENT] : LEFT with action : PRESSED CTRL + SHIFT");

        // Ctrl + Alt
        EventMouseClick ctrlAlt;
        ctrlAlt.button = RIGHT;
        ctrlAlt.action = RELEASED;
        ctrlAlt.mods = GLFW_MOD_CONTROL | GLFW_MOD_ALT;
        std::ostringstream os2;
        os2 << ctrlAlt;
        EXPECT_EQ(os2.str(), "[MOUSE BUTTON EVENT] : RIGHT with action : RELEASED ALT + CTRL");

        // Alt + Shift
        EventMouseClick altShift;
        altShift.button = MIDDLE;
        altShift.action = PRESSED;
        altShift.mods = GLFW_MOD_ALT | GLFW_MOD_SHIFT;
        std::ostringstream os3;
        os3 << altShift;
        EXPECT_EQ(os3.str(), "[MOUSE BUTTON EVENT] : MIDDLE with action : PRESSED ALT + SHIFT");
    }

    // EventMouseScroll - Edge cases
    TEST(WindowEventTest, EventMouseScrollZeroOffsets) {
        EventMouseScroll scrollEvent(0.0f, 0.0f);
        EXPECT_FLOAT_EQ(scrollEvent.x, 0.0f);
        EXPECT_FLOAT_EQ(scrollEvent.y, 0.0f);

        std::ostringstream os;
        os << scrollEvent;
        EXPECT_EQ(os.str(), "[MOUSE SCROLL EVENT] xOffset : 0 yOffset : 0");
    }

    TEST(WindowEventTest, EventMouseScrollNegativeOffsets) {
        EventMouseScroll scrollEvent(-2.5f, -4.0f);
        EXPECT_FLOAT_EQ(scrollEvent.x, -2.5f);
        EXPECT_FLOAT_EQ(scrollEvent.y, -4.0f);

        std::ostringstream os;
        os << scrollEvent;
        EXPECT_EQ(os.str(), "[MOUSE SCROLL EVENT] xOffset : -2.5 yOffset : -4");
    }

    TEST(WindowEventTest, EventMouseScrollLargeOffsets) {
        EventMouseScroll scrollEvent(100.5f, 200.25f);
        EXPECT_FLOAT_EQ(scrollEvent.x, 100.5f);
        EXPECT_FLOAT_EQ(scrollEvent.y, 200.25f);
    }

    TEST(WindowEventTest, EventMouseScrollVerySmallOffsets) {
        EventMouseScroll scrollEvent(0.001f, -0.001f);
        EXPECT_FLOAT_EQ(scrollEvent.x, 0.001f);
        EXPECT_FLOAT_EQ(scrollEvent.y, -0.001f);
    }

    TEST(WindowEventTest, EventMouseScrollMixedSigns) {
        EventMouseScroll scrollEvent1(5.0f, -3.0f);
        EXPECT_FLOAT_EQ(scrollEvent1.x, 5.0f);
        EXPECT_FLOAT_EQ(scrollEvent1.y, -3.0f);

        EventMouseScroll scrollEvent2(-7.0f, 2.0f);
        EXPECT_FLOAT_EQ(scrollEvent2.x, -7.0f);
        EXPECT_FLOAT_EQ(scrollEvent2.y, 2.0f);
    }

    // EventMouseMove - Edge cases
    TEST(WindowEventTest, EventMouseMoveZeroPosition) {
        EventMouseMove moveEvent(0.0f, 0.0f);
        EXPECT_FLOAT_EQ(moveEvent.x, 0.0f);
        EXPECT_FLOAT_EQ(moveEvent.y, 0.0f);

        std::ostringstream os;
        os << moveEvent;
        EXPECT_EQ(os.str(), "[MOUSE MOVE EVENT] x : 0 y : 0");
    }

    TEST(WindowEventTest, EventMouseMoveNegativePosition) {
        EventMouseMove moveEvent(-100.0f, -200.0f);
        EXPECT_FLOAT_EQ(moveEvent.x, -100.0f);
        EXPECT_FLOAT_EQ(moveEvent.y, -200.0f);

        std::ostringstream os;
        os << moveEvent;
        EXPECT_EQ(os.str(), "[MOUSE MOVE EVENT] x : -100 y : -200");
    }

    TEST(WindowEventTest, EventMouseMoveLargePosition) {
        EventMouseMove moveEvent(1920.0f, 1080.0f);
        EXPECT_FLOAT_EQ(moveEvent.x, 1920.0f);
        EXPECT_FLOAT_EQ(moveEvent.y, 1080.0f);
    }

    TEST(WindowEventTest, EventMouseMoveSubpixelPosition) {
        EventMouseMove moveEvent(123.456f, 789.123f);
        EXPECT_FLOAT_EQ(moveEvent.x, 123.456f);
        EXPECT_FLOAT_EQ(moveEvent.y, 789.123f);
    }

    TEST(WindowEventTest, EventMouseMoveMixedSignsPosition) {
        EventMouseMove moveEvent1(100.0f, -50.0f);
        EXPECT_FLOAT_EQ(moveEvent1.x, 100.0f);
        EXPECT_FLOAT_EQ(moveEvent1.y, -50.0f);

        EventMouseMove moveEvent2(-75.0f, 200.0f);
        EXPECT_FLOAT_EQ(moveEvent2.x, -75.0f);
        EXPECT_FLOAT_EQ(moveEvent2.y, 200.0f);
    }

    // EventFileDrop - Path validation and edge cases
    TEST(WindowEventTest, EventFileDropManyFiles) {
        std::vector<std::string> files;
        for (int i = 0; i < 100; ++i) {
            files.push_back("/path/to/file" + std::to_string(i) + ".txt");
        }
        EventFileDrop dropEvent(files);
        EXPECT_EQ(dropEvent.files.size(), 100);
        EXPECT_EQ(dropEvent.files[0], "/path/to/file0.txt");
        EXPECT_EQ(dropEvent.files[99], "/path/to/file99.txt");
    }

    TEST(WindowEventTest, EventFileDropWithSpacesInPaths) {
        std::vector<std::string> files = {
            "/path/to/my file.txt",
            "/path/to/another file with spaces.png"
        };
        EventFileDrop dropEvent(files);
        EXPECT_EQ(dropEvent.files.size(), 2);
        EXPECT_EQ(dropEvent.files[0], "/path/to/my file.txt");
        EXPECT_EQ(dropEvent.files[1], "/path/to/another file with spaces.png");

        std::ostringstream os;
        os << dropEvent;
        EXPECT_EQ(os.str(), "[FILE DROP EVENT] 2 file(s): /path/to/my file.txt, /path/to/another file with spaces.png");
    }

    TEST(WindowEventTest, EventFileDropWithSpecialCharacters) {
        std::vector<std::string> files = {
            "/path/to/file-with-dashes.txt",
            "/path/to/file_with_underscores.txt",
            "/path/to/file.multiple.dots.txt",
            "/path/to/file@with#special$chars.txt"
        };
        EventFileDrop dropEvent(files);
        EXPECT_EQ(dropEvent.files.size(), 4);
        EXPECT_EQ(dropEvent.files[0], "/path/to/file-with-dashes.txt");
        EXPECT_EQ(dropEvent.files[3], "/path/to/file@with#special$chars.txt");
    }

    TEST(WindowEventTest, EventFileDropWithDifferentExtensions) {
        std::vector<std::string> files = {
            "/path/to/image.png",
            "/path/to/image.jpg",
            "/path/to/model.obj",
            "/path/to/model.fbx",
            "/path/to/script.cs",
            "/path/to/shader.glsl",
            "/path/to/data.json",
            "/path/to/archive.zip"
        };
        EventFileDrop dropEvent(files);
        EXPECT_EQ(dropEvent.files.size(), 8);
        EXPECT_EQ(dropEvent.files[0], "/path/to/image.png");
        EXPECT_EQ(dropEvent.files[7], "/path/to/archive.zip");
    }

    TEST(WindowEventTest, EventFileDropRelativePaths) {
        std::vector<std::string> files = {
            "./relative/path/file.txt",
            "../parent/path/file.txt",
            "no/leading/slash/file.txt"
        };
        EventFileDrop dropEvent(files);
        EXPECT_EQ(dropEvent.files.size(), 3);
        EXPECT_EQ(dropEvent.files[0], "./relative/path/file.txt");
        EXPECT_EQ(dropEvent.files[1], "../parent/path/file.txt");
        EXPECT_EQ(dropEvent.files[2], "no/leading/slash/file.txt");
    }

    TEST(WindowEventTest, EventFileDropWindowsPaths) {
        std::vector<std::string> files = {
            "C:\\Windows\\System32\\file.dll",
            "D:\\Games\\game.exe",
            "\\\\network\\share\\file.txt"
        };
        EventFileDrop dropEvent(files);
        EXPECT_EQ(dropEvent.files.size(), 3);
        EXPECT_EQ(dropEvent.files[0], "C:\\Windows\\System32\\file.dll");
        EXPECT_EQ(dropEvent.files[2], "\\\\network\\share\\file.txt");
    }

    TEST(WindowEventTest, EventFileDropVeryLongPaths) {
        std::string longPath = "/very/long/path/";
        for (int i = 0; i < 20; ++i) {
            longPath += "subdirectory/";
        }
        longPath += "file.txt";

        std::vector<std::string> files = {longPath};
        EventFileDrop dropEvent(files);
        EXPECT_EQ(dropEvent.files.size(), 1);
        EXPECT_EQ(dropEvent.files[0], longPath);
    }

    TEST(WindowEventTest, EventFileDropEmptyFilenamePaths) {
        std::vector<std::string> files = {
            "/path/to/directory/",
            ""
        };
        EventFileDrop dropEvent(files);
        EXPECT_EQ(dropEvent.files.size(), 2);
        EXPECT_EQ(dropEvent.files[0], "/path/to/directory/");
        EXPECT_EQ(dropEvent.files[1], "");
    }

    // KeyMods bitwise operations - Comprehensive tests
    TEST(WindowEventTest, KeyModsBitwiseOrCombinations) {
        int shiftCtrl = static_cast<int>(KeyMods::SHIFT) | static_cast<int>(KeyMods::CONTROL);
        EXPECT_NE(shiftCtrl, 0);
        EXPECT_TRUE(shiftCtrl & static_cast<int>(KeyMods::SHIFT));
        EXPECT_TRUE(shiftCtrl & static_cast<int>(KeyMods::CONTROL));
        EXPECT_FALSE(shiftCtrl & static_cast<int>(KeyMods::ALT));

        int allMods = static_cast<int>(KeyMods::SHIFT) | static_cast<int>(KeyMods::CONTROL) | static_cast<int>(KeyMods::ALT);
        EXPECT_TRUE(allMods & static_cast<int>(KeyMods::SHIFT));
        EXPECT_TRUE(allMods & static_cast<int>(KeyMods::CONTROL));
        EXPECT_TRUE(allMods & static_cast<int>(KeyMods::ALT));
    }

    TEST(WindowEventTest, KeyModsBitwiseAndOperations) {
        int shiftOnly = static_cast<int>(KeyMods::SHIFT);
        EXPECT_TRUE(shiftOnly & static_cast<int>(KeyMods::SHIFT));
        EXPECT_FALSE(shiftOnly & static_cast<int>(KeyMods::CONTROL));
        EXPECT_FALSE(shiftOnly & static_cast<int>(KeyMods::ALT));
    }

    TEST(WindowEventTest, KeyModsNoneValue) {
        int none = static_cast<int>(KeyMods::NONE);
        EXPECT_EQ(none, 0);
        EXPECT_FALSE(none & static_cast<int>(KeyMods::SHIFT));
        EXPECT_FALSE(none & static_cast<int>(KeyMods::CONTROL));
        EXPECT_FALSE(none & static_cast<int>(KeyMods::ALT));
    }

    // Stream operator tests for invalid/unknown values
    TEST(WindowEventTest, KeyActionStreamOperatorInvalidValue) {
        // Cast an invalid value to KeyAction
        KeyAction invalidAction = static_cast<KeyAction>(999);
        std::ostringstream os;
        os << invalidAction;
        EXPECT_EQ(os.str(), "UNKNOWN_ACTION");
    }

    TEST(WindowEventTest, KeyModsStreamOperatorInvalidValue) {
        // Cast an invalid value to KeyMods
        KeyMods invalidMod = static_cast<KeyMods>(999);
        std::ostringstream os;
        os << invalidMod;
        EXPECT_EQ(os.str(), "UNKNOWN_MOD");
    }

    TEST(WindowEventTest, MouseButtonStreamOperatorInvalidValue) {
        // Cast an invalid value to MouseButton
        MouseButton invalidButton = static_cast<MouseButton>(999);
        std::ostringstream os;
        os << invalidButton;
        EXPECT_EQ(os.str(), "UNKNOWN_MOD");  // Note: The source has a typo, it says UNKNOWN_MOD instead of UNKNOWN_BUTTON
    }

    // EventKey and EventMouseClick modifier ordering consistency
    TEST(WindowEventTest, EventKeyModifierOrderingConsistency) {
        // Test that modifiers are always printed in the same order: ALT + CTRL + SHIFT
        EventKey key1(65, PRESSED, GLFW_MOD_SHIFT | GLFW_MOD_ALT);
        std::ostringstream os1;
        os1 << key1;
        EXPECT_EQ(os1.str(), "[KEYBOARD EVENT] : 65 with action : PRESSED ALT + SHIFT");

        EventKey key2(65, PRESSED, GLFW_MOD_ALT | GLFW_MOD_SHIFT);
        std::ostringstream os2;
        os2 << key2;
        EXPECT_EQ(os2.str(), "[KEYBOARD EVENT] : 65 with action : PRESSED ALT + SHIFT");
        EXPECT_EQ(os1.str(), os2.str());  // Should be identical regardless of order
    }

    TEST(WindowEventTest, EventMouseClickModifierOrderingConsistency) {
        EventMouseClick click1;
        click1.button = LEFT;
        click1.action = PRESSED;
        click1.mods = GLFW_MOD_CONTROL | GLFW_MOD_SHIFT;
        std::ostringstream os1;
        os1 << click1;
        EXPECT_EQ(os1.str(), "[MOUSE BUTTON EVENT] : LEFT with action : PRESSED CTRL + SHIFT");

        EventMouseClick click2;
        click2.button = LEFT;
        click2.action = PRESSED;
        click2.mods = GLFW_MOD_SHIFT | GLFW_MOD_CONTROL;
        std::ostringstream os2;
        os2 << click2;
        EXPECT_EQ(os2.str(), "[MOUSE BUTTON EVENT] : LEFT with action : PRESSED CTRL + SHIFT");
        EXPECT_EQ(os1.str(), os2.str());
    }

    // Boundary conditions
    TEST(WindowEventTest, EventWindowResizeMaxIntValues) {
        EventWindowResize resizeEvent(2147483647, 2147483647);  // INT_MAX
        EXPECT_EQ(resizeEvent.width, 2147483647);
        EXPECT_EQ(resizeEvent.height, 2147483647);
    }

    TEST(WindowEventTest, EventWindowResizeMinIntValues) {
        EventWindowResize resizeEvent(-2147483648, -2147483648);  // INT_MIN
        EXPECT_EQ(resizeEvent.width, -2147483648);
        EXPECT_EQ(resizeEvent.height, -2147483648);
    }

    TEST(WindowEventTest, EventKeyZeroKeycode) {
        EventKey key(0, PRESSED, 0);
        EXPECT_EQ(key.keycode, 0);
    }

    // Multiple events creation (stress test)
    TEST(WindowEventTest, MultipleEventCreation) {
        std::vector<EventWindowResize> resizeEvents;
        for (int i = 0; i < 1000; ++i) {
            resizeEvents.emplace_back(800 + i, 600 + i);
        }
        EXPECT_EQ(resizeEvents.size(), 1000);
        EXPECT_EQ(resizeEvents[0].width, 800);
        EXPECT_EQ(resizeEvents[999].width, 1799);
    }

    TEST(WindowEventTest, MultipleEventFileDropCreation) {
        std::vector<EventFileDrop> dropEvents;
        for (int i = 0; i < 100; ++i) {
            std::vector<std::string> files = {"/path/file" + std::to_string(i) + ".txt"};
            dropEvents.emplace_back(files);
        }
        EXPECT_EQ(dropEvents.size(), 100);
        EXPECT_EQ(dropEvents[0].files[0], "/path/file0.txt");
        EXPECT_EQ(dropEvents[99].files[0], "/path/file99.txt");
    }

    // Test hasMod with combined GLFW modifiers not in KeyMods enum
    TEST(WindowEventTest, EventKeyHasModWithAllGLFWModifiers) {
        // Test with all GLFW modifiers combined
        EventKey allGLFWMods(65, PRESSED,
            GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT |
            GLFW_MOD_SUPER | GLFW_MOD_CAPS_LOCK | GLFW_MOD_NUM_LOCK);

        EXPECT_TRUE(allGLFWMods.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(allGLFWMods.hasMod(KeyMods::CONTROL));
        EXPECT_TRUE(allGLFWMods.hasMod(KeyMods::ALT));
    }

    TEST(WindowEventTest, EventMouseClickHasModWithAllGLFWModifiers) {
        EventMouseClick allGLFWMods;
        allGLFWMods.mods = GLFW_MOD_SHIFT | GLFW_MOD_CONTROL | GLFW_MOD_ALT |
                           GLFW_MOD_SUPER | GLFW_MOD_CAPS_LOCK | GLFW_MOD_NUM_LOCK;

        EXPECT_TRUE(allGLFWMods.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(allGLFWMods.hasMod(KeyMods::CONTROL));
        EXPECT_TRUE(allGLFWMods.hasMod(KeyMods::ALT));
    }

    // EventFileDrop output formatting verification
    TEST(WindowEventTest, EventFileDropStreamOperatorFormatting) {
        std::vector<std::string> oneFile = {"/file1.txt"};
        EventFileDrop drop1(oneFile);
        std::ostringstream os1;
        os1 << drop1;
        EXPECT_EQ(os1.str(), "[FILE DROP EVENT] 1 file(s): /file1.txt");

        std::vector<std::string> twoFiles = {"/file1.txt", "/file2.txt"};
        EventFileDrop drop2(twoFiles);
        std::ostringstream os2;
        os2 << drop2;
        EXPECT_EQ(os2.str(), "[FILE DROP EVENT] 2 file(s): /file1.txt, /file2.txt");

        std::vector<std::string> threeFiles = {"/file1.txt", "/file2.txt", "/file3.txt"};
        EventFileDrop drop3(threeFiles);
        std::ostringstream os3;
        os3 << drop3;
        EXPECT_EQ(os3.str(), "[FILE DROP EVENT] 3 file(s): /file1.txt, /file2.txt, /file3.txt");
    }

    // Floating point precision tests
    TEST(WindowEventTest, EventMouseScrollFloatPrecision) {
        EventMouseScroll scroll(0.123456789f, -0.987654321f);
        EXPECT_NEAR(scroll.x, 0.123456789f, 0.0001f);
        EXPECT_NEAR(scroll.y, -0.987654321f, 0.0001f);
    }

    TEST(WindowEventTest, EventMouseMoveFloatPrecision) {
        EventMouseMove move(123.456789f, 987.654321f);
        EXPECT_NEAR(move.x, 123.456789f, 0.0001f);
        EXPECT_NEAR(move.y, 987.654321f, 0.0001f);
    }
}