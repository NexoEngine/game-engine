//// Command.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        02/12/2025
//  Description: Test file for Command class
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "inputs/Command.hpp"

namespace nexo::editor {

    class CommandTest : public ::testing::Test {
    protected:
        // Helper to create a simple command
        Command createCommand(const std::string& key, const std::string& desc = "Test") {
            return Command::create()
                .description(desc)
                .key(key)
                .build();
        }
    };

    // Key Parsing Tests

    TEST_F(CommandTest, ParseSingleKey) {
        Command cmd = createCommand("s");
        EXPECT_FALSE(cmd.getSignature().none());
        EXPECT_EQ(cmd.getKey(), "s");
    }

    TEST_F(CommandTest, ParseSingleKeyUpperCase) {
        // Should be case-insensitive
        Command cmdLower = createCommand("s");
        Command cmdUpper = createCommand("S");
        EXPECT_EQ(cmdLower.getSignature(), cmdUpper.getSignature());
    }

    TEST_F(CommandTest, ParseModifierPlusKey) {
        Command cmd = createCommand("ctrl+s");
        EXPECT_FALSE(cmd.getSignature().none());
        EXPECT_EQ(cmd.getKey(), "ctrl+s");
    }

    TEST_F(CommandTest, ParseMultipleModifiers) {
        Command cmd = createCommand("ctrl+shift+s");
        EXPECT_FALSE(cmd.getSignature().none());
        EXPECT_EQ(cmd.getKey(), "ctrl+shift+s");
    }

    TEST_F(CommandTest, ParseWithWhitespace) {
        Command cmdNoSpace = createCommand("ctrl+s");
        Command cmdWithSpace = createCommand("ctrl + s");
        EXPECT_EQ(cmdNoSpace.getSignature(), cmdWithSpace.getSignature());
    }

    TEST_F(CommandTest, ParseFunctionKey) {
        Command cmd = createCommand("f1");
        EXPECT_FALSE(cmd.getSignature().none());
    }

    TEST_F(CommandTest, ParseSpecialKeys) {
        Command cmdSpace = createCommand("space");
        Command cmdEnter = createCommand("enter");
        Command cmdEsc = createCommand("escape");

        EXPECT_FALSE(cmdSpace.getSignature().none());
        EXPECT_FALSE(cmdEnter.getSignature().none());
        EXPECT_FALSE(cmdEsc.getSignature().none());

        // They should all be different
        EXPECT_NE(cmdSpace.getSignature(), cmdEnter.getSignature());
        EXPECT_NE(cmdSpace.getSignature(), cmdEsc.getSignature());
        EXPECT_NE(cmdEnter.getSignature(), cmdEsc.getSignature());
    }

    TEST_F(CommandTest, ParseAliases) {
        // ctrl and control should be the same
        Command cmdCtrl = createCommand("ctrl");
        Command cmdControl = createCommand("control");
        EXPECT_EQ(cmdCtrl.getSignature(), cmdControl.getSignature());

        // esc and escape should be the same
        Command cmdEsc = createCommand("esc");
        Command cmdEscape = createCommand("escape");
        EXPECT_EQ(cmdEsc.getSignature(), cmdEscape.getSignature());

        // enter and return should be the same
        Command cmdEnter = createCommand("enter");
        Command cmdReturn = createCommand("return");
        EXPECT_EQ(cmdEnter.getSignature(), cmdReturn.getSignature());
    }

    TEST_F(CommandTest, DifferentKeysHaveDifferentSignatures) {
        Command cmdA = createCommand("a");
        Command cmdB = createCommand("b");
        EXPECT_NE(cmdA.getSignature(), cmdB.getSignature());
    }

    // Matching Tests

    TEST_F(CommandTest, ExactMatchReturnsTrueForExactSignature) {
        Command cmd = createCommand("ctrl+s");
        EXPECT_TRUE(cmd.exactMatch(cmd.getSignature()));
    }

    TEST_F(CommandTest, ExactMatchReturnsFalseForDifferentSignature) {
        Command cmd1 = createCommand("ctrl+s");
        Command cmd2 = createCommand("ctrl+a");
        EXPECT_FALSE(cmd1.exactMatch(cmd2.getSignature()));
    }

    TEST_F(CommandTest, ExactMatchReturnsFalseForPartialSignature) {
        Command cmdFull = createCommand("ctrl+shift+s");
        Command cmdPartial = createCommand("ctrl+s");
        EXPECT_FALSE(cmdFull.exactMatch(cmdPartial.getSignature()));
    }

    TEST_F(CommandTest, PartialMatchReturnsTrueForExactSignature) {
        Command cmd = createCommand("ctrl+s");
        EXPECT_TRUE(cmd.partialMatch(cmd.getSignature()));
    }

    TEST_F(CommandTest, PartialMatchReturnsTrueForSuperset) {
        Command cmdSmall = createCommand("ctrl+s");
        Command cmdLarge = createCommand("ctrl+shift+s");
        // cmdSmall should partially match cmdLarge's signature (superset contains subset)
        EXPECT_TRUE(cmdSmall.partialMatch(cmdLarge.getSignature()));
    }

    TEST_F(CommandTest, PartialMatchReturnsFalseWhenMissingKey) {
        Command cmd = createCommand("ctrl+s");
        Command other = createCommand("ctrl+a");
        // ctrl+s doesn't match ctrl+a because 's' is not in ctrl+a
        EXPECT_FALSE(cmd.partialMatch(other.getSignature()));
    }

    // Builder Tests

    TEST_F(CommandTest, BuilderCreatesCorrectCommand) {
        Command cmd = Command::create()
            .description("Test Description")
            .key("ctrl+z")
            .modifier(true)
            .build();

        EXPECT_EQ(cmd.getDescription(), "Test Description");
        EXPECT_EQ(cmd.getKey(), "ctrl+z");
        EXPECT_TRUE(cmd.isModifier());
    }

    TEST_F(CommandTest, BuilderDefaultsToNonModifier) {
        Command cmd = Command::create()
            .description("Test")
            .key("a")
            .build();

        EXPECT_FALSE(cmd.isModifier());
    }

    TEST_F(CommandTest, BuilderCanAddChildren) {
        Command child = Command::create()
            .description("Child")
            .key("s")
            .build();

        Command parent = Command::create()
            .description("Parent")
            .key("ctrl")
            .modifier(true)
            .addChild(child)
            .build();

        auto children = parent.getChildren();
        EXPECT_EQ(children.size(), 1u);
        EXPECT_EQ(children[0].getDescription(), "Child");
    }

    // Callback Tests

    TEST_F(CommandTest, PressedCallbackExecutes) {
        bool called = false;
        Command cmd = Command::create()
            .description("Test")
            .key("a")
            .onPressed([&called]() { called = true; })
            .build();

        cmd.executePressedCallback();
        EXPECT_TRUE(called);
    }

    TEST_F(CommandTest, ReleasedCallbackExecutes) {
        bool called = false;
        Command cmd = Command::create()
            .description("Test")
            .key("a")
            .onReleased([&called]() { called = true; })
            .build();

        cmd.executeReleasedCallback();
        EXPECT_TRUE(called);
    }

    TEST_F(CommandTest, RepeatCallbackExecutes) {
        bool called = false;
        Command cmd = Command::create()
            .description("Test")
            .key("a")
            .onRepeat([&called]() { called = true; })
            .build();

        cmd.executeRepeatCallback();
        EXPECT_TRUE(called);
    }

    TEST_F(CommandTest, NullCallbacksDoNotCrash) {
        Command cmd = Command::create()
            .description("Test")
            .key("a")
            .build();

        EXPECT_NO_THROW(cmd.executePressedCallback());
        EXPECT_NO_THROW(cmd.executeReleasedCallback());
        EXPECT_NO_THROW(cmd.executeRepeatCallback());
    }

    // Accessor Tests

    TEST_F(CommandTest, GetDescriptionReturnsDescription) {
        Command cmd = Command::create()
            .description("My Description")
            .key("a")
            .build();

        EXPECT_EQ(cmd.getDescription(), "My Description");
    }

    TEST_F(CommandTest, GetKeyReturnsKeyString) {
        Command cmd = Command::create()
            .description("Test")
            .key("ctrl+alt+delete")
            .build();

        EXPECT_EQ(cmd.getKey(), "ctrl+alt+delete");
    }

    TEST_F(CommandTest, IsModifierReturnsCorrectValue) {
        Command modifier = Command::create()
            .description("Modifier")
            .key("ctrl")
            .modifier(true)
            .build();

        Command nonModifier = Command::create()
            .description("Non-Modifier")
            .key("a")
            .modifier(false)
            .build();

        EXPECT_TRUE(modifier.isModifier());
        EXPECT_FALSE(nonModifier.isModifier());
    }

    TEST_F(CommandTest, GetSignatureReturnsValidBitset) {
        Command cmd = createCommand("ctrl+s");
        const auto& sig = cmd.getSignature();

        // Should have at least 2 bits set (ctrl and s)
        EXPECT_GE(sig.count(), 2u);
    }

    // Function Keys Tests

    TEST_F(CommandTest, ParseAllFunctionKeys) {
        // Test all F1-F12 keys
        for (int i = 1; i <= 12; ++i) {
            std::string key = "f" + std::to_string(i);
            Command cmd = createCommand(key);
            EXPECT_FALSE(cmd.getSignature().none()) << "Failed for key: " << key;
        }
    }

    TEST_F(CommandTest, FunctionKeysAreDifferent) {
        Command f1 = createCommand("f1");
        Command f2 = createCommand("f2");
        Command f12 = createCommand("f12");

        EXPECT_NE(f1.getSignature(), f2.getSignature());
        EXPECT_NE(f1.getSignature(), f12.getSignature());
        EXPECT_NE(f2.getSignature(), f12.getSignature());
    }

    // Arrow Keys Tests

    TEST_F(CommandTest, ParseArrowKeys) {
        Command up = createCommand("up");
        Command down = createCommand("down");
        Command left = createCommand("left");
        Command right = createCommand("right");

        EXPECT_FALSE(up.getSignature().none());
        EXPECT_FALSE(down.getSignature().none());
        EXPECT_FALSE(left.getSignature().none());
        EXPECT_FALSE(right.getSignature().none());

        // All should be different
        EXPECT_NE(up.getSignature(), down.getSignature());
        EXPECT_NE(left.getSignature(), right.getSignature());
        EXPECT_NE(up.getSignature(), left.getSignature());
    }

    // Navigation Keys Tests

    TEST_F(CommandTest, ParseNavigationKeys) {
        Command home = createCommand("home");
        Command end = createCommand("end");
        Command pageUp = createCommand("pageup");
        Command pageDown = createCommand("pagedown");

        EXPECT_FALSE(home.getSignature().none());
        EXPECT_FALSE(end.getSignature().none());
        EXPECT_FALSE(pageUp.getSignature().none());
        EXPECT_FALSE(pageDown.getSignature().none());

        // All should be different
        EXPECT_NE(home.getSignature(), end.getSignature());
        EXPECT_NE(pageUp.getSignature(), pageDown.getSignature());
    }

    // Edit Keys Tests

    TEST_F(CommandTest, ParseEditKeys) {
        Command tab = createCommand("tab");
        Command backspace = createCommand("backspace");
        Command del = createCommand("delete");
        Command insert = createCommand("insert");

        EXPECT_FALSE(tab.getSignature().none());
        EXPECT_FALSE(backspace.getSignature().none());
        EXPECT_FALSE(del.getSignature().none());
        EXPECT_FALSE(insert.getSignature().none());
    }

    // Special Keys Tests

    TEST_F(CommandTest, ParseCapsLockAndNumLock) {
        Command capslock = createCommand("capslock");
        Command numlock = createCommand("numlock");

        EXPECT_FALSE(capslock.getSignature().none());
        EXPECT_FALSE(numlock.getSignature().none());
        EXPECT_NE(capslock.getSignature(), numlock.getSignature());
    }

    TEST_F(CommandTest, ParsePrintScreenAndPause) {
        Command printscreen = createCommand("printscreen");
        Command pause = createCommand("pause");

        EXPECT_FALSE(printscreen.getSignature().none());
        EXPECT_FALSE(pause.getSignature().none());
    }

    // Keypad Tests

    TEST_F(CommandTest, ParseKeypadNumbers) {
        for (int i = 0; i <= 9; ++i) {
            std::string key = "keypad" + std::to_string(i);
            Command cmd = createCommand(key);
            EXPECT_FALSE(cmd.getSignature().none()) << "Failed for key: " << key;
        }
    }

    TEST_F(CommandTest, ParseKeypadOperators) {
        // Note: keypad+ cannot be parsed because '+' is the key combination delimiter
        // The parser splits "keypad+" into "keypad" and "" which don't match
        // Only keypad operators without '+' in the name can be parsed
        Command sub = createCommand("keypad-");
        Command mul = createCommand("keypad*");
        Command div = createCommand("keypad/");
        Command dec = createCommand("keypad.");

        EXPECT_FALSE(sub.getSignature().none());
        EXPECT_FALSE(mul.getSignature().none());
        EXPECT_FALSE(div.getSignature().none());
        EXPECT_FALSE(dec.getSignature().none());
    }

    TEST_F(CommandTest, KeypadPlusCannotBeParsedDueToDelimiter) {
        // This is a known limitation - "keypad+" gets split on '+'
        // resulting in "keypad" and "" which don't match any keys
        Command add = createCommand("keypad+");
        EXPECT_TRUE(add.getSignature().none());
    }

    // Edge Cases

    TEST_F(CommandTest, ParseUnknownKeyReturnsEmptySignature) {
        Command cmd = createCommand("unknownkey123");
        EXPECT_TRUE(cmd.getSignature().none());
    }

    TEST_F(CommandTest, ParseEmptyStringReturnsEmptySignature) {
        Command cmd = createCommand("");
        EXPECT_TRUE(cmd.getSignature().none());
    }

    TEST_F(CommandTest, ParseOnlyPlusSignsHandledGracefully) {
        Command cmd = createCommand("+++");
        EXPECT_TRUE(cmd.getSignature().none());
    }

    TEST_F(CommandTest, ParseMixedValidAndInvalidKeys) {
        // ctrl is valid, unknownkey is not
        Command cmd = createCommand("ctrl+unknownkey");
        // Should have at least ctrl bit set
        EXPECT_FALSE(cmd.getSignature().none());
    }

    // Modifier Aliases

    TEST_F(CommandTest, SuperCmdWinAreEquivalent) {
        Command cmdSuper = createCommand("super");
        Command cmdCmd = createCommand("cmd");
        Command cmdWin = createCommand("win");

        EXPECT_EQ(cmdSuper.getSignature(), cmdCmd.getSignature());
        EXPECT_EQ(cmdSuper.getSignature(), cmdWin.getSignature());
    }

    // Complex Combinations

    TEST_F(CommandTest, TripleModifierCombination) {
        Command cmd = createCommand("ctrl+alt+shift+s");
        EXPECT_FALSE(cmd.getSignature().none());
        // Should have 4 bits set (ctrl, alt, shift, s)
        EXPECT_GE(cmd.getSignature().count(), 4u);
    }

    TEST_F(CommandTest, ModifierWithFunctionKey) {
        Command ctrlF5 = createCommand("ctrl+f5");
        Command altF1 = createCommand("alt+f1");
        Command shiftF12 = createCommand("shift+f12");

        EXPECT_GE(ctrlF5.getSignature().count(), 2u);
        EXPECT_GE(altF1.getSignature().count(), 2u);
        EXPECT_GE(shiftF12.getSignature().count(), 2u);
    }

    TEST_F(CommandTest, ModifierWithArrowKey) {
        Command ctrlUp = createCommand("ctrl+up");
        Command shiftDown = createCommand("shift+down");

        EXPECT_GE(ctrlUp.getSignature().count(), 2u);
        EXPECT_GE(shiftDown.getSignature().count(), 2u);
    }

    // Number Keys

    TEST_F(CommandTest, ParseAllNumberKeys) {
        for (int i = 0; i <= 9; ++i) {
            Command cmd = createCommand(std::to_string(i));
            EXPECT_FALSE(cmd.getSignature().none()) << "Failed for number: " << i;
        }
    }

    TEST_F(CommandTest, NumberKeysAreDifferentFromKeypad) {
        Command num1 = createCommand("1");
        Command keypad1 = createCommand("keypad1");

        EXPECT_NE(num1.getSignature(), keypad1.getSignature());
    }

    // Builder Edge Cases

    TEST_F(CommandTest, BuilderWithMultipleChildren) {
        Command child1 = Command::create()
            .description("Child 1")
            .key("a")
            .build();

        Command child2 = Command::create()
            .description("Child 2")
            .key("b")
            .build();

        Command child3 = Command::create()
            .description("Child 3")
            .key("c")
            .build();

        Command parent = Command::create()
            .description("Parent")
            .key("ctrl")
            .modifier(true)
            .addChild(child1)
            .addChild(child2)
            .addChild(child3)
            .build();

        auto children = parent.getChildren();
        EXPECT_EQ(children.size(), 3u);
        EXPECT_EQ(children[0].getKey(), "a");
        EXPECT_EQ(children[1].getKey(), "b");
        EXPECT_EQ(children[2].getKey(), "c");
    }

    TEST_F(CommandTest, GetChildrenReturnsEmptySpanWhenNoChildren) {
        Command cmd = createCommand("a");
        auto children = cmd.getChildren();
        EXPECT_TRUE(children.empty());
    }

    TEST_F(CommandTest, NestedModifiers) {
        Command leaf = Command::create()
            .description("Leaf")
            .key("s")
            .build();

        Command inner = Command::create()
            .description("Inner Modifier")
            .key("shift")
            .modifier(true)
            .addChild(leaf)
            .build();

        Command outer = Command::create()
            .description("Outer Modifier")
            .key("ctrl")
            .modifier(true)
            .addChild(inner)
            .build();

        auto outerChildren = outer.getChildren();
        ASSERT_EQ(outerChildren.size(), 1u);
        EXPECT_TRUE(outerChildren[0].isModifier());

        auto innerChildren = outerChildren[0].getChildren();
        ASSERT_EQ(innerChildren.size(), 1u);
        EXPECT_EQ(innerChildren[0].getKey(), "s");
    }

    // All Modifier Keys

    TEST_F(CommandTest, AllModifierKeysWork) {
        Command ctrl = createCommand("ctrl");
        Command shift = createCommand("shift");
        Command alt = createCommand("alt");
        Command super = createCommand("super");

        EXPECT_FALSE(ctrl.getSignature().none());
        EXPECT_FALSE(shift.getSignature().none());
        EXPECT_FALSE(alt.getSignature().none());
        EXPECT_FALSE(super.getSignature().none());

        // All should be different
        EXPECT_NE(ctrl.getSignature(), shift.getSignature());
        EXPECT_NE(ctrl.getSignature(), alt.getSignature());
        EXPECT_NE(ctrl.getSignature(), super.getSignature());
    }

    // All Alphabet Keys

    TEST_F(CommandTest, ParseAllAlphabetKeys) {
        for (char c = 'a'; c <= 'z'; ++c) {
            std::string key(1, c);
            Command cmd = createCommand(key);
            EXPECT_FALSE(cmd.getSignature().none()) << "Failed for key: " << key;
        }
    }

    // Additional Coverage Tests - Documenting Supported Keys

    TEST_F(CommandTest, KeypadDecimalAndOperators) {
        // These are the supported keypad operators from the key map
        Command decimal = createCommand("keypad.");
        Command subtract = createCommand("keypad-");
        Command multiply = createCommand("keypad*");
        Command divide = createCommand("keypad/");

        EXPECT_FALSE(decimal.getSignature().none());
        EXPECT_FALSE(subtract.getSignature().none());
        EXPECT_FALSE(multiply.getSignature().none());
        EXPECT_FALSE(divide.getSignature().none());
    }

    TEST_F(CommandTest, ModifierAliasesMapping) {
        // Document that ctrl/control, cmd/win/super are aliases
        Command ctrl = createCommand("ctrl");
        Command control = createCommand("control");
        EXPECT_EQ(ctrl.getSignature(), control.getSignature());

        Command cmd = createCommand("cmd");
        Command win = createCommand("win");
        Command super = createCommand("super");
        EXPECT_EQ(cmd.getSignature(), win.getSignature());
        EXPECT_EQ(win.getSignature(), super.getSignature());
    }

    TEST_F(CommandTest, UnsupportedKeysReturnEmptySignature) {
        // These keys are NOT in the key map and should return empty signatures
        EXPECT_TRUE(createCommand("scrolllock").getSignature().none());
        EXPECT_TRUE(createCommand("menu").getSignature().none());
        EXPECT_TRUE(createCommand("keypadenter").getSignature().none());
        EXPECT_TRUE(createCommand("leftctrl").getSignature().none());
        EXPECT_TRUE(createCommand(";").getSignature().none());
        EXPECT_TRUE(createCommand(",").getSignature().none());
    }

    // Callback Edge Cases

    TEST_F(CommandTest, MultipleCallbacksCanBeChained) {
        int pressCount = 0;
        int releaseCount = 0;
        int repeatCount = 0;

        Command cmd = Command::create()
            .description("Test")
            .key("a")
            .onPressed([&pressCount]() { pressCount++; })
            .onReleased([&releaseCount]() { releaseCount++; })
            .onRepeat([&repeatCount]() { repeatCount++; })
            .build();

        cmd.executePressedCallback();
        cmd.executePressedCallback();
        cmd.executeReleasedCallback();
        cmd.executeRepeatCallback();
        cmd.executeRepeatCallback();
        cmd.executeRepeatCallback();

        EXPECT_EQ(pressCount, 2);
        EXPECT_EQ(releaseCount, 1);
        EXPECT_EQ(repeatCount, 3);
    }

    TEST_F(CommandTest, CallbackCanModifyExternalState) {
        std::string state = "initial";

        Command cmd = Command::create()
            .description("Test")
            .key("a")
            .onPressed([&state]() { state = "pressed"; })
            .build();

        EXPECT_EQ(state, "initial");
        cmd.executePressedCallback();
        EXPECT_EQ(state, "pressed");
    }

    // Matching Edge Cases

    TEST_F(CommandTest, EmptySignatureMatchingBehavior) {
        Command cmd = createCommand("unknownkey"); // Will have empty signature
        Command other = createCommand("a");

        // Empty signature doesn't exactly match anything
        EXPECT_FALSE(cmd.exactMatch(other.getSignature()));
        // Empty signature partially matches everything (empty set is subset of all sets)
        // This is the expected behavior: (empty & X) == empty is always true
        EXPECT_TRUE(cmd.partialMatch(other.getSignature()));
    }

    TEST_F(CommandTest, ExactMatchWithEmptyInputSignature) {
        Command cmd = createCommand("a");
        std::bitset<ImGuiKey_NamedKey_COUNT> emptySignature;

        EXPECT_FALSE(cmd.exactMatch(emptySignature));
    }

    TEST_F(CommandTest, PartialMatchWithEmptyInputSignature) {
        Command cmd = createCommand("a");
        std::bitset<ImGuiKey_NamedKey_COUNT> emptySignature;

        EXPECT_FALSE(cmd.partialMatch(emptySignature));
    }

    TEST_F(CommandTest, PartialMatchWithSameSignature) {
        Command cmd = createCommand("ctrl+s");
        EXPECT_TRUE(cmd.partialMatch(cmd.getSignature()));
    }

    TEST_F(CommandTest, PartialMatchWithSubsetSignature) {
        Command larger = createCommand("ctrl+shift+s");
        Command smaller = createCommand("ctrl+s");

        // smaller should partially match larger (all bits in smaller are in larger)
        EXPECT_TRUE(smaller.partialMatch(larger.getSignature()));
        // larger should NOT partially match smaller
        EXPECT_FALSE(larger.partialMatch(smaller.getSignature()));
    }

    // Builder Edge Cases

    TEST_F(CommandTest, BuilderCanOverwriteKey) {
        Command cmd = Command::create()
            .description("Test")
            .key("a")
            .key("b")  // Overwrite
            .build();

        EXPECT_EQ(cmd.getKey(), "b");
    }

    TEST_F(CommandTest, BuilderCanOverwriteDescription) {
        Command cmd = Command::create()
            .description("First")
            .key("a")
            .description("Second")  // Overwrite
            .build();

        EXPECT_EQ(cmd.getDescription(), "Second");
    }

    TEST_F(CommandTest, BuilderPreservesOrderOfChildren) {
        Command c1 = Command::create().description("c1").key("1").build();
        Command c2 = Command::create().description("c2").key("2").build();
        Command c3 = Command::create().description("c3").key("3").build();

        Command parent = Command::create()
            .description("Parent")
            .key("ctrl")
            .addChild(c1)
            .addChild(c2)
            .addChild(c3)
            .build();

        auto children = parent.getChildren();
        ASSERT_EQ(children.size(), 3u);
        EXPECT_EQ(children[0].getDescription(), "c1");
        EXPECT_EQ(children[1].getDescription(), "c2");
        EXPECT_EQ(children[2].getDescription(), "c3");
    }

    // Description Edge Cases

    TEST_F(CommandTest, EmptyDescription) {
        Command cmd = Command::create()
            .description("")
            .key("a")
            .build();

        EXPECT_EQ(cmd.getDescription(), "");
    }

    TEST_F(CommandTest, LongDescription) {
        std::string longDesc(1000, 'x');
        Command cmd = Command::create()
            .description(longDesc)
            .key("a")
            .build();

        EXPECT_EQ(cmd.getDescription(), longDesc);
    }

    TEST_F(CommandTest, DescriptionWithSpecialCharacters) {
        Command cmd = Command::create()
            .description("Test: (Ctrl+S) - Save & Exit!")
            .key("a")
            .build();

        EXPECT_EQ(cmd.getDescription(), "Test: (Ctrl+S) - Save & Exit!");
    }

    // Signature Consistency

    TEST_F(CommandTest, SameKeyProducesSameSignatureMultipleTimes) {
        Command cmd1 = createCommand("ctrl+s");
        Command cmd2 = createCommand("ctrl+s");
        Command cmd3 = createCommand("ctrl+s");

        EXPECT_EQ(cmd1.getSignature(), cmd2.getSignature());
        EXPECT_EQ(cmd2.getSignature(), cmd3.getSignature());
    }

    TEST_F(CommandTest, DifferentOrderSameResult) {
        // ctrl+shift+s and shift+ctrl+s should produce the same signature
        Command cmd1 = createCommand("ctrl+shift+s");
        Command cmd2 = createCommand("shift+ctrl+s");

        EXPECT_EQ(cmd1.getSignature(), cmd2.getSignature());
    }

    TEST_F(CommandTest, WhitespaceVariationsProduceSameSignature) {
        Command cmd1 = createCommand("ctrl+s");
        Command cmd2 = createCommand("ctrl + s");
        Command cmd3 = createCommand(" ctrl + s ");

        EXPECT_EQ(cmd1.getSignature(), cmd2.getSignature());
        EXPECT_EQ(cmd2.getSignature(), cmd3.getSignature());
    }

}
