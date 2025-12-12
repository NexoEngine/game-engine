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
        EventKey keyEvent(65, PRESSED, GLFW_MOD_SHIFT | GLFW_MOD_CONTROL);

        EXPECT_EQ(keyEvent.keycode, 65);
        EXPECT_EQ(keyEvent.action, PRESSED);
        EXPECT_TRUE(keyEvent.hasMod(KeyMods::SHIFT));
        EXPECT_TRUE(keyEvent.hasMod(KeyMods::CONTROL));
        EXPECT_FALSE(keyEvent.hasMod(KeyMods::ALT));

        std::ostringstream os;
        os << keyEvent;
        EXPECT_EQ(os.str(), "[KEYBOARD EVENT] : 65 with action : PRESSED CTRL + SHIFT");
    }

    TEST(WindowEventTest, EventMouseClickAttributes) {
        EventMouseClick mouseClickEvent;
        mouseClickEvent.button = RIGHT;
        mouseClickEvent.action = RELEASED;
        mouseClickEvent.mods = GLFW_MOD_ALT;

        EXPECT_EQ(mouseClickEvent.button, RIGHT);
        EXPECT_EQ(mouseClickEvent.action, RELEASED);
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
        os << PRESSED;
        EXPECT_EQ(os.str(), "PRESSED");

        os.str(""); os.clear();
        os << RELEASED;
        EXPECT_EQ(os.str(), "RELEASED");

        os.str(""); os.clear();
        os << REPEAT;
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
        os << LEFT;
        EXPECT_EQ(os.str(), "LEFT");

        os.str(""); os.clear();
        os << RIGHT;
        EXPECT_EQ(os.str(), "RIGHT");

        os.str(""); os.clear();
        os << MIDDLE;
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
        EXPECT_EQ(PRESSED, 0);
        EXPECT_EQ(RELEASED, 1);
        EXPECT_EQ(REPEAT, 2);
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
        EXPECT_EQ(LEFT, 0);
        EXPECT_EQ(RIGHT, 1);
        EXPECT_EQ(MIDDLE, 2);
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
}