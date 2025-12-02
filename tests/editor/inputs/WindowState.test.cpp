//// WindowState.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        02/12/2025
//  Description: Test file for WindowState class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "inputs/WindowState.hpp"

namespace nexo::editor {

    class WindowStateTest : public ::testing::Test {
    protected:
        // Helper to create a simple command for testing
        Command createTestCommand(const std::string& key, const std::string& desc = "Test") {
            return Command::create()
                .description(desc)
                .key(key)
                .build();
        }
    };

    // Constructor and ID Tests

    TEST_F(WindowStateTest, DefaultIdIsZero) {
        WindowState state;
        EXPECT_EQ(state.getId(), 0u);
    }

    TEST_F(WindowStateTest, ConstructorSetsId) {
        WindowState state(42);
        EXPECT_EQ(state.getId(), 42u);
    }

    TEST_F(WindowStateTest, GetIdReturnsCorrectValue) {
        WindowState state1(1);
        WindowState state2(100);
        WindowState state3(999);

        EXPECT_EQ(state1.getId(), 1u);
        EXPECT_EQ(state2.getId(), 100u);
        EXPECT_EQ(state3.getId(), 999u);
    }

    // Command Registration Tests

    TEST_F(WindowStateTest, GetCommandsReturnsEmptySpanInitially) {
        WindowState state;
        auto commands = state.getCommands();
        EXPECT_TRUE(commands.empty());
        EXPECT_EQ(commands.size(), 0u);
    }

    TEST_F(WindowStateTest, RegisterCommandAddsToList) {
        WindowState state;
        Command cmd = createTestCommand("a", "Test A");

        state.registerCommand(cmd);

        auto commands = state.getCommands();
        EXPECT_EQ(commands.size(), 1u);
        EXPECT_EQ(commands[0].getKey(), "a");
        EXPECT_EQ(commands[0].getDescription(), "Test A");
    }

    TEST_F(WindowStateTest, MultipleCommandsPreserveOrder) {
        WindowState state;

        state.registerCommand(createTestCommand("a", "First"));
        state.registerCommand(createTestCommand("b", "Second"));
        state.registerCommand(createTestCommand("c", "Third"));

        auto commands = state.getCommands();
        ASSERT_EQ(commands.size(), 3u);
        EXPECT_EQ(commands[0].getKey(), "a");
        EXPECT_EQ(commands[1].getKey(), "b");
        EXPECT_EQ(commands[2].getKey(), "c");
    }

    TEST_F(WindowStateTest, SpanSizeMatchesRegisteredCommands) {
        WindowState state;

        for (int i = 0; i < 10; ++i) {
            state.registerCommand(createTestCommand(std::to_string(i)));
            EXPECT_EQ(state.getCommands().size(), static_cast<size_t>(i + 1));
        }
    }

    // Edge Cases

    TEST_F(WindowStateTest, RegisterCommandWithModifier) {
        WindowState state;
        Command cmd = Command::create()
            .description("Modifier Command")
            .key("ctrl")
            .modifier(true)
            .build();

        state.registerCommand(cmd);

        auto commands = state.getCommands();
        ASSERT_EQ(commands.size(), 1u);
        EXPECT_TRUE(commands[0].isModifier());
    }

    TEST_F(WindowStateTest, RegisterCommandWithChildren) {
        WindowState state;

        Command child = Command::create()
            .description("Child")
            .key("s")
            .build();

        Command parent = Command::create()
            .description("Save")
            .key("ctrl")
            .modifier(true)
            .addChild(child)
            .build();

        state.registerCommand(parent);

        auto commands = state.getCommands();
        ASSERT_EQ(commands.size(), 1u);
        EXPECT_EQ(commands[0].getChildren().size(), 1u);
    }

    TEST_F(WindowStateTest, DifferentWindowStatesAreIndependent) {
        WindowState state1(1);
        WindowState state2(2);

        state1.registerCommand(createTestCommand("a"));
        state1.registerCommand(createTestCommand("b"));
        state2.registerCommand(createTestCommand("x"));

        EXPECT_EQ(state1.getCommands().size(), 2u);
        EXPECT_EQ(state2.getCommands().size(), 1u);
    }

}
