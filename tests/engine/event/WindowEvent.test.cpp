//// WindowEvent.test.cpp /////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
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
}