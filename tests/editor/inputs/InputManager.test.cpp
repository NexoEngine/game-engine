//// InputManager.test.cpp /////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        03/12/2025
//  Description: Test file for InputManager class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "inputs/InputManager.hpp"
#include "inputs/WindowState.hpp"
#include "inputs/Command.hpp"

namespace nexo::editor {

    class InputManagerTest : public ::testing::Test {
    protected:
        InputManager inputManager;

        // Helper to create a simple command
        Command createCommand(const std::string& key, const std::string& desc) {
            return Command::create()
                .description(desc)
                .key(key)
                .build();
        }

        // Helper to create a modifier command with children
        Command createModifierWithChildren(
            const std::string& modifierKey,
            const std::string& modifierDesc,
            const std::vector<std::pair<std::string, std::string>>& children
        ) {
            auto builder = Command::create()
                .description(modifierDesc)
                .key(modifierKey)
                .modifier(true);

            for (const auto& [childKey, childDesc] : children) {
                builder.addChild(createCommand(childKey, childDesc));
            }

            return builder.build();
        }
    };

    // ==========================================================================
    // CommandInfo Tests
    // ==========================================================================

    TEST_F(InputManagerTest, CommandInfoConstructor) {
        CommandInfo info("ctrl+s", "Save file");
        EXPECT_EQ(info.key, "ctrl+s");
        EXPECT_EQ(info.description, "Save file");
    }

    TEST_F(InputManagerTest, CommandInfoWithEmptyStrings) {
        CommandInfo info("", "");
        EXPECT_EQ(info.key, "");
        EXPECT_EQ(info.description, "");
    }

    // ==========================================================================
    // getAllPossibleCommands Tests - Empty WindowState
    // ==========================================================================

    TEST_F(InputManagerTest, GetAllPossibleCommandsEmptyWindowState) {
        WindowState state;
        auto commands = inputManager.getAllPossibleCommands(state);
        EXPECT_TRUE(commands.empty());
    }

    // ==========================================================================
    // getAllPossibleCommands Tests - Simple Commands
    // ==========================================================================

    TEST_F(InputManagerTest, GetAllPossibleCommandsSingleCommand) {
        WindowState state;
        state.registerCommand(createCommand("a", "Select all"));

        auto commands = inputManager.getAllPossibleCommands(state);
        ASSERT_EQ(commands.size(), 1u);
        EXPECT_EQ(commands[0].key, "a");
        EXPECT_EQ(commands[0].description, "Select all");
    }

    TEST_F(InputManagerTest, GetAllPossibleCommandsMultipleCommands) {
        WindowState state;
        state.registerCommand(createCommand("a", "Command A"));
        state.registerCommand(createCommand("b", "Command B"));
        state.registerCommand(createCommand("c", "Command C"));

        auto commands = inputManager.getAllPossibleCommands(state);
        ASSERT_EQ(commands.size(), 3u);

        // Verify all commands are present
        bool foundA = false, foundB = false, foundC = false;
        for (const auto& cmd : commands) {
            if (cmd.key == "a") foundA = true;
            if (cmd.key == "b") foundB = true;
            if (cmd.key == "c") foundC = true;
        }
        EXPECT_TRUE(foundA);
        EXPECT_TRUE(foundB);
        EXPECT_TRUE(foundC);
    }

    // ==========================================================================
    // getAllPossibleCommands Tests - Modifier Commands
    // ==========================================================================

    TEST_F(InputManagerTest, GetAllPossibleCommandsModifierWithChildren) {
        WindowState state;

        Command modifier = createModifierWithChildren("ctrl", "Control modifier", {
            {"s", "Save"},
            {"o", "Open"},
            {"n", "New"}
        });
        state.registerCommand(modifier);

        auto commands = inputManager.getAllPossibleCommands(state);

        // Should have 3 combinations: ctrl+s, ctrl+o, ctrl+n
        ASSERT_EQ(commands.size(), 3u);

        bool foundSave = false, foundOpen = false, foundNew = false;
        for (const auto& cmd : commands) {
            if (cmd.key == "ctrl+s" && cmd.description == "Save") foundSave = true;
            if (cmd.key == "ctrl+o" && cmd.description == "Open") foundOpen = true;
            if (cmd.key == "ctrl+n" && cmd.description == "New") foundNew = true;
        }
        EXPECT_TRUE(foundSave);
        EXPECT_TRUE(foundOpen);
        EXPECT_TRUE(foundNew);
    }

    TEST_F(InputManagerTest, GetAllPossibleCommandsNestedModifiers) {
        WindowState state;

        // Create: ctrl -> shift -> s (Save)
        Command innerModifier = createModifierWithChildren("shift", "Shift modifier", {
            {"s", "Save As"}
        });

        Command outerModifier = Command::create()
            .description("Control modifier")
            .key("ctrl")
            .modifier(true)
            .addChild(innerModifier)
            .build();

        state.registerCommand(outerModifier);

        auto commands = inputManager.getAllPossibleCommands(state);

        // Should build nested combinations: ctrl+shift+s
        ASSERT_GE(commands.size(), 1u);

        bool foundSaveAs = false;
        for (const auto& cmd : commands) {
            if (cmd.description == "Save As") {
                foundSaveAs = true;
                EXPECT_TRUE(cmd.key.find("ctrl") != std::string::npos);
                EXPECT_TRUE(cmd.key.find("shift") != std::string::npos);
                EXPECT_TRUE(cmd.key.find("s") != std::string::npos);
            }
        }
        EXPECT_TRUE(foundSaveAs);
    }

    TEST_F(InputManagerTest, GetAllPossibleCommandsMixedModifiersAndRegular) {
        WindowState state;

        // Add regular command
        state.registerCommand(createCommand("escape", "Cancel"));

        // Add modifier with children
        Command modifier = createModifierWithChildren("ctrl", "Control", {
            {"s", "Save"}
        });
        state.registerCommand(modifier);

        auto commands = inputManager.getAllPossibleCommands(state);

        // Should have both regular and modifier combinations
        ASSERT_GE(commands.size(), 2u);

        bool foundCancel = false, foundSave = false;
        for (const auto& cmd : commands) {
            if (cmd.key == "escape") foundCancel = true;
            if (cmd.key == "ctrl+s") foundSave = true;
        }
        EXPECT_TRUE(foundCancel);
        EXPECT_TRUE(foundSave);
    }

    // ==========================================================================
    // getAllPossibleCommands Tests - Modifier Without Children
    // ==========================================================================

    TEST_F(InputManagerTest, GetAllPossibleCommandsModifierWithoutChildren) {
        WindowState state;

        Command modifier = Command::create()
            .description("Just Ctrl")
            .key("ctrl")
            .modifier(true)
            .build();
        state.registerCommand(modifier);

        auto commands = inputManager.getAllPossibleCommands(state);

        // Modifier without children should still appear
        ASSERT_EQ(commands.size(), 1u);
        EXPECT_EQ(commands[0].key, "ctrl");
        EXPECT_EQ(commands[0].description, "Just Ctrl");
    }

    // ==========================================================================
    // getAllPossibleCommands Tests - Command With Children But Not Modifier
    // ==========================================================================

    TEST_F(InputManagerTest, GetAllPossibleCommandsNonModifierWithChildren) {
        WindowState state;

        Command child = createCommand("s", "Child");

        // Non-modifier with children - should show the parent, not children
        Command parent = Command::create()
            .description("Parent Command")
            .key("a")
            .modifier(false)  // Explicitly NOT a modifier
            .addChild(child)
            .build();
        state.registerCommand(parent);

        auto commands = inputManager.getAllPossibleCommands(state);

        // Non-modifier with children should show the parent key
        ASSERT_EQ(commands.size(), 1u);
        EXPECT_EQ(commands[0].key, "a");
        EXPECT_EQ(commands[0].description, "Parent Command");
    }

    // ==========================================================================
    // getAllPossibleCommands Tests - Multiple Modifiers
    // ==========================================================================

    TEST_F(InputManagerTest, GetAllPossibleCommandsMultipleModifiers) {
        WindowState state;

        Command ctrlMod = createModifierWithChildren("ctrl", "Ctrl", {
            {"s", "Save"},
            {"c", "Copy"}
        });
        state.registerCommand(ctrlMod);

        Command altMod = createModifierWithChildren("alt", "Alt", {
            {"f4", "Close"},
            {"tab", "Switch"}
        });
        state.registerCommand(altMod);

        auto commands = inputManager.getAllPossibleCommands(state);

        // Should have 4 combinations total
        ASSERT_EQ(commands.size(), 4u);

        bool foundSave = false, foundCopy = false, foundClose = false, foundSwitch = false;
        for (const auto& cmd : commands) {
            if (cmd.key == "ctrl+s") foundSave = true;
            if (cmd.key == "ctrl+c") foundCopy = true;
            if (cmd.key == "alt+f4") foundClose = true;
            if (cmd.key == "alt+tab") foundSwitch = true;
        }
        EXPECT_TRUE(foundSave);
        EXPECT_TRUE(foundCopy);
        EXPECT_TRUE(foundClose);
        EXPECT_TRUE(foundSwitch);
    }

    // ==========================================================================
    // getAllPossibleCommands Tests - Complex Hierarchy
    // ==========================================================================

    TEST_F(InputManagerTest, GetAllPossibleCommandsDeepHierarchy) {
        WindowState state;

        // Create: ctrl -> shift -> alt -> x
        Command altMod = createModifierWithChildren("alt", "Alt", {
            {"x", "Deep Command"}
        });

        Command shiftMod = Command::create()
            .description("Shift")
            .key("shift")
            .modifier(true)
            .addChild(altMod)
            .build();

        Command ctrlMod = Command::create()
            .description("Ctrl")
            .key("ctrl")
            .modifier(true)
            .addChild(shiftMod)
            .build();

        state.registerCommand(ctrlMod);

        auto commands = inputManager.getAllPossibleCommands(state);

        // Should build the deep combination
        ASSERT_GE(commands.size(), 1u);

        bool foundDeepCommand = false;
        for (const auto& cmd : commands) {
            if (cmd.description == "Deep Command") {
                foundDeepCommand = true;
                // Should contain all keys in the chain
                EXPECT_TRUE(cmd.key.find("ctrl") != std::string::npos);
                EXPECT_TRUE(cmd.key.find("shift") != std::string::npos);
                EXPECT_TRUE(cmd.key.find("alt") != std::string::npos);
                EXPECT_TRUE(cmd.key.find("x") != std::string::npos);
            }
        }
        EXPECT_TRUE(foundDeepCommand);
    }

    // ==========================================================================
    // getAllPossibleCommands Tests - Edge Cases
    // ==========================================================================

    TEST_F(InputManagerTest, GetAllPossibleCommandsEmptyDescription) {
        WindowState state;
        state.registerCommand(createCommand("a", ""));

        auto commands = inputManager.getAllPossibleCommands(state);
        ASSERT_EQ(commands.size(), 1u);
        EXPECT_EQ(commands[0].key, "a");
        EXPECT_EQ(commands[0].description, "");
    }

    TEST_F(InputManagerTest, GetAllPossibleCommandsSpecialKeys) {
        WindowState state;
        state.registerCommand(createCommand("space", "Jump"));
        state.registerCommand(createCommand("enter", "Confirm"));
        state.registerCommand(createCommand("f1", "Help"));

        auto commands = inputManager.getAllPossibleCommands(state);
        ASSERT_EQ(commands.size(), 3u);
    }

    TEST_F(InputManagerTest, GetAllPossibleCommandsWithComplexKeystrings) {
        WindowState state;

        Command modifier = createModifierWithChildren("ctrl+shift", "Ctrl+Shift", {
            {"s", "Super Save"}
        });
        state.registerCommand(modifier);

        auto commands = inputManager.getAllPossibleCommands(state);
        ASSERT_GE(commands.size(), 1u);

        // The combination should include the full modifier key
        bool found = false;
        for (const auto& cmd : commands) {
            if (cmd.description == "Super Save") {
                found = true;
                EXPECT_TRUE(cmd.key.find("ctrl+shift") != std::string::npos);
            }
        }
        EXPECT_TRUE(found);
    }

    // ==========================================================================
    // InputManager Default Construction
    // ==========================================================================

    TEST_F(InputManagerTest, DefaultConstruction) {
        InputManager manager;
        // Should construct without issues
        WindowState emptyState;
        auto commands = manager.getAllPossibleCommands(emptyState);
        EXPECT_TRUE(commands.empty());
    }

    // ==========================================================================
    // Multiple Calls
    // ==========================================================================

    TEST_F(InputManagerTest, GetAllPossibleCommandsMultipleCalls) {
        WindowState state;
        state.registerCommand(createCommand("a", "Command A"));

        // Multiple calls should return consistent results
        auto commands1 = inputManager.getAllPossibleCommands(state);
        auto commands2 = inputManager.getAllPossibleCommands(state);

        EXPECT_EQ(commands1.size(), commands2.size());
        if (!commands1.empty() && !commands2.empty()) {
            EXPECT_EQ(commands1[0].key, commands2[0].key);
            EXPECT_EQ(commands1[0].description, commands2[0].description);
        }
    }

    TEST_F(InputManagerTest, GetAllPossibleCommandsDifferentStates) {
        WindowState state1;
        state1.registerCommand(createCommand("a", "State 1"));

        WindowState state2;
        state2.registerCommand(createCommand("b", "State 2"));
        state2.registerCommand(createCommand("c", "Also State 2"));

        auto commands1 = inputManager.getAllPossibleCommands(state1);
        auto commands2 = inputManager.getAllPossibleCommands(state2);

        EXPECT_EQ(commands1.size(), 1u);
        EXPECT_EQ(commands2.size(), 2u);
    }

}  // namespace nexo::editor

// =============================================================================
// ImGui Context-Based Tests
// =============================================================================
// These tests use a real ImGui context with input injection to test
// processInputs() and getPossibleCommands() which depend on ImGui state.
// =============================================================================

#include "ImGuiTestFixture.hpp"

namespace nexo::editor {

    class InputManagerImGuiTest : public testing::ImGuiTestFixture {
    protected:
        void SetUp() override {
            testing::ImGuiTestFixture::SetUp();
            inputManager = std::make_unique<InputManager>();
            windowState = std::make_unique<WindowState>(1);

            // Clear any lingering state from previous tests
            beginFrame();
            releaseAllKeys();
            nextFrame(0.5f);  // Large delta to exceed multi-press threshold
        }

        void TearDown() override {
            // Clean up key state
            releaseAllKeys();
            nextFrame();

            windowState.reset();
            inputManager.reset();
            testing::ImGuiTestFixture::TearDown();
        }

        // Helper to create a simple command
        Command createCommand(const std::string& key, const std::string& desc) {
            return Command::create()
                .description(desc)
                .key(key)
                .build();
        }

        // Helper to create command with callbacks
        Command createCommandWithCallbacks(
            const std::string& key,
            const std::string& desc,
            bool* pressedFlag = nullptr,
            bool* releasedFlag = nullptr,
            bool* repeatFlag = nullptr
        ) {
            auto builder = Command::create()
                .description(desc)
                .key(key);

            if (pressedFlag) {
                builder.onPressed([pressedFlag]() { *pressedFlag = true; });
            }
            if (releasedFlag) {
                builder.onReleased([releasedFlag]() { *releasedFlag = true; });
            }
            if (repeatFlag) {
                builder.onRepeat([repeatFlag]() { *repeatFlag = true; });
            }

            return builder.build();
        }

        std::unique_ptr<InputManager> inputManager;
        std::unique_ptr<WindowState> windowState;
    };

    // ==========================================================================
    // processInputs - Basic Key Press Tests
    // ==========================================================================

    TEST_F(InputManagerImGuiTest, ProcessInputs_SingleKeyPress_ExecutesPressedCallback) {
        bool pressed = false;
        auto cmd = createCommandWithCallbacks("a", "Test A", &pressed);
        windowState->registerCommand(cmd);

        // Frame 1: Press key A
        pressKey(ImGuiKey_A);
        nextFrame();
        inputManager->processInputs(*windowState);

        EXPECT_TRUE(pressed) << "Pressed callback should have been executed";
    }

    TEST_F(InputManagerImGuiTest, ProcessInputs_KeyHeld_DoesNotRepeatPressedCallback) {
        int pressCount = 0;
        auto cmd = Command::create()
            .description("Test A")
            .key("a")
            .onPressed([&pressCount]() { pressCount++; })
            .build();
        windowState->registerCommand(cmd);

        // Clear static state - run several empty frames with key released
        runEmptyFrames(3);
        inputManager->processInputs(*windowState);

        // Frame 1: Press key A (should be detected as new press)
        pressKey(ImGuiKey_A);
        nextFrame();
        inputManager->processInputs(*windowState);
        EXPECT_GE(pressCount, 1) << "First press should trigger callback";

        int countAfterFirstPress = pressCount;

        // Frame 2: Key still held (no release) - should NOT increment
        nextFrame();
        inputManager->processInputs(*windowState);
        EXPECT_EQ(pressCount, countAfterFirstPress) << "Callback should NOT repeat while key is held";

        // Frame 3: Key still held - still should NOT increment
        nextFrame();
        inputManager->processInputs(*windowState);
        EXPECT_EQ(pressCount, countAfterFirstPress) << "Callback should NOT repeat while key is held";
    }

    TEST_F(InputManagerImGuiTest, ProcessInputs_KeyRelease_ExecutesReleasedCallback) {
        bool released = false;
        auto cmd = createCommandWithCallbacks("a", "Test A", nullptr, &released);
        windowState->registerCommand(cmd);

        // Frame 1: Press key A
        pressKey(ImGuiKey_A);
        nextFrame();
        inputManager->processInputs(*windowState);
        EXPECT_FALSE(released);

        // Frame 2: Release key A
        releaseKey(ImGuiKey_A);
        nextFrame();
        inputManager->processInputs(*windowState);

        EXPECT_TRUE(released) << "Released callback should have been executed";
    }

    TEST_F(InputManagerImGuiTest, ProcessInputs_NoCommands_NoErrors) {
        // Empty window state - should not crash
        pressKey(ImGuiKey_A);
        nextFrame();
        EXPECT_NO_THROW(inputManager->processInputs(*windowState));
    }

    TEST_F(InputManagerImGuiTest, ProcessInputs_UnregisteredKey_NoCallback) {
        bool pressed = false;
        auto cmd = createCommandWithCallbacks("a", "Test A", &pressed);
        windowState->registerCommand(cmd);

        // Press B instead of A
        pressKey(ImGuiKey_B);
        nextFrame();
        inputManager->processInputs(*windowState);

        EXPECT_FALSE(pressed) << "Callback should NOT execute for unregistered key";
    }

    // ==========================================================================
    // processInputs - Modifier Combination Tests
    // ==========================================================================

    TEST_F(InputManagerImGuiTest, ProcessInputs_ModifierPlusKey_ExecutesChildCallback) {
        bool childPressed = false;

        Command childCmd = Command::create()
            .description("Save")
            .key("s")
            .onPressed([&childPressed]() { childPressed = true; })
            .build();

        Command ctrlMod = Command::create()
            .description("Ctrl")
            .key("ctrl")
            .modifier(true)
            .addChild(childCmd)
            .build();

        windowState->registerCommand(ctrlMod);

        // Frame 1: Press Ctrl
        pressKey(ImGuiKey_LeftCtrl);
        nextFrame();
        inputManager->processInputs(*windowState);
        EXPECT_FALSE(childPressed) << "Child should not trigger with just modifier";

        // Frame 2: Press S while Ctrl is held
        pressKey(ImGuiKey_S);
        nextFrame();
        inputManager->processInputs(*windowState);

        EXPECT_TRUE(childPressed) << "Child callback should execute when modifier+key pressed";
    }

    TEST_F(InputManagerImGuiTest, ProcessInputs_ModifierPlusKey_ReleasesChildCallback) {
        bool childReleased = false;

        Command childCmd = Command::create()
            .description("Save")
            .key("s")
            .onReleased([&childReleased]() { childReleased = true; })
            .build();

        Command ctrlMod = Command::create()
            .description("Ctrl")
            .key("ctrl")
            .modifier(true)
            .addChild(childCmd)
            .build();

        windowState->registerCommand(ctrlMod);

        // Frame 1: Press Ctrl + S
        pressKey(ImGuiKey_LeftCtrl);
        pressKey(ImGuiKey_S);
        nextFrame();
        inputManager->processInputs(*windowState);

        // Frame 2: Release S while Ctrl is held
        releaseKey(ImGuiKey_S);
        nextFrame();
        inputManager->processInputs(*windowState);

        EXPECT_TRUE(childReleased) << "Child release callback should execute";
    }

    TEST_F(InputManagerImGuiTest, ProcessInputs_ModifierBlocksRegularCommand) {
        bool regularPressed = false;
        bool childPressed = false;

        // Regular 'S' command
        Command regularS = Command::create()
            .description("Regular S")
            .key("s")
            .onPressed([&regularPressed]() { regularPressed = true; })
            .build();

        // Ctrl+S command
        Command childS = Command::create()
            .description("Ctrl+S")
            .key("s")
            .onPressed([&childPressed]() { childPressed = true; })
            .build();

        Command ctrlMod = Command::create()
            .description("Ctrl")
            .key("ctrl")
            .modifier(true)
            .addChild(childS)
            .build();

        windowState->registerCommand(regularS);
        windowState->registerCommand(ctrlMod);

        // Press Ctrl + S
        pressKey(ImGuiKey_LeftCtrl);
        pressKey(ImGuiKey_S);
        nextFrame();
        inputManager->processInputs(*windowState);

        EXPECT_TRUE(childPressed) << "Modifier combo should trigger";
        EXPECT_FALSE(regularPressed) << "Regular command should be blocked by modifier";
    }

    // ==========================================================================
    // processInputs - Multiple Keys
    // ==========================================================================

    TEST_F(InputManagerImGuiTest, ProcessInputs_MultipleIndependentKeys) {
        bool aPressed = false;
        bool bPressed = false;

        auto cmdA = createCommandWithCallbacks("a", "A", &aPressed);
        auto cmdB = createCommandWithCallbacks("b", "B", &bPressed);

        windowState->registerCommand(cmdA);
        windowState->registerCommand(cmdB);

        // Press both A and B in same frame
        // Note: Due to exact matching, only exact single-key signatures match
        pressKey(ImGuiKey_A);
        nextFrame();
        inputManager->processInputs(*windowState);

        // A should match its exact signature
        EXPECT_TRUE(aPressed);
        EXPECT_FALSE(bPressed);

        // Reset and press B
        aPressed = false;
        releaseKey(ImGuiKey_A);
        pressKey(ImGuiKey_B);
        nextFrame();
        inputManager->processInputs(*windowState);

        EXPECT_TRUE(bPressed);
    }

    // ==========================================================================
    // getPossibleCommands - With ImGui Key State
    // ==========================================================================

    TEST_F(InputManagerImGuiTest, GetPossibleCommands_NoKeysPressed_DoesNotCrash) {
        auto cmd = createCommand("a", "Test A");
        windowState->registerCommand(cmd);

        // Clear state
        runEmptyFrames(3);

        // getPossibleCommands should not crash even with no keys pressed
        // Note: Due to static state, the exact return value depends on prior tests
        EXPECT_NO_THROW(inputManager->getPossibleCommands(*windowState));
    }

    TEST_F(InputManagerImGuiTest, GetPossibleCommands_ModifierHeld_ShowsChildren) {
        Command childCmd = createCommand("s", "Save");

        Command ctrlMod = Command::create()
            .description("Ctrl")
            .key("ctrl")
            .modifier(true)
            .addChild(childCmd)
            .build();

        windowState->registerCommand(ctrlMod);

        // Press and hold Ctrl
        pressKey(ImGuiKey_LeftCtrl);
        nextFrame();

        auto commands = inputManager->getPossibleCommands(*windowState);

        // Should show child commands when modifier is held
        bool foundSave = false;
        for (const auto& cmd : commands) {
            if (cmd.description == "Save") {
                foundSave = true;
            }
        }
        EXPECT_TRUE(foundSave) << "Should show child commands when modifier is held";
    }

    // ==========================================================================
    // Edge Cases
    // ==========================================================================

    TEST_F(InputManagerImGuiTest, ProcessInputs_RapidPressRelease) {
        int pressCount = 0;
        int releaseCount = 0;

        auto cmd = Command::create()
            .description("Test")
            .key("a")
            .onPressed([&pressCount]() { pressCount++; })
            .onReleased([&releaseCount]() { releaseCount++; })
            .build();

        windowState->registerCommand(cmd);

        // Rapid press/release cycle
        for (int i = 0; i < 3; ++i) {
            pressKey(ImGuiKey_A);
            nextFrame();
            inputManager->processInputs(*windowState);

            releaseKey(ImGuiKey_A);
            nextFrame();
            inputManager->processInputs(*windowState);
        }

        EXPECT_EQ(pressCount, 3);
        EXPECT_EQ(releaseCount, 3);
    }

    TEST_F(InputManagerImGuiTest, ProcessInputs_FunctionKey) {
        bool pressed = false;
        auto cmd = createCommandWithCallbacks("f1", "Help", &pressed);
        windowState->registerCommand(cmd);

        pressKey(ImGuiKey_F1);
        nextFrame();
        inputManager->processInputs(*windowState);

        EXPECT_TRUE(pressed) << "Function key should work";
    }

    TEST_F(InputManagerImGuiTest, ProcessInputs_SpecialKeys) {
        bool spacePressed = false;
        bool enterPressed = false;
        bool escapePressed = false;

        auto cmdSpace = createCommandWithCallbacks("space", "Space", &spacePressed);
        auto cmdEnter = createCommandWithCallbacks("enter", "Enter", &enterPressed);
        auto cmdEscape = createCommandWithCallbacks("escape", "Escape", &escapePressed);

        windowState->registerCommand(cmdSpace);
        windowState->registerCommand(cmdEnter);
        windowState->registerCommand(cmdEscape);

        // Test space
        pressKey(ImGuiKey_Space);
        nextFrame();
        inputManager->processInputs(*windowState);
        EXPECT_TRUE(spacePressed);

        releaseKey(ImGuiKey_Space);

        // Test enter
        pressKey(ImGuiKey_Enter);
        nextFrame();
        inputManager->processInputs(*windowState);
        EXPECT_TRUE(enterPressed);

        releaseKey(ImGuiKey_Enter);

        // Test escape
        pressKey(ImGuiKey_Escape);
        nextFrame();
        inputManager->processInputs(*windowState);
        EXPECT_TRUE(escapePressed);
    }

}  // namespace nexo::editor
