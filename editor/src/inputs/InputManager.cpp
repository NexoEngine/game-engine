//// InputManager.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        23/05/2025
//  Description: Source file for the input manager
//
///////////////////////////////////////////////////////////////////////////////

#include "InputManager.hpp"
#include <imgui.h>
#include <iostream>
#include <set>

// Implementation sketch for InputManager

namespace nexo::editor {

    void InputManager::processInputs(const WindowState& windowState) {
        std::bitset<ImGuiKey_NamedKey_COUNT> pressedSignature;
        std::bitset<ImGuiKey_NamedKey_COUNT> releasedSignature;
        std::bitset<ImGuiKey_NamedKey_COUNT> repeatSignature;
        std::bitset<ImGuiKey_NamedKey_COUNT> currentlyHeldKeys;

        static const std::set<ImGuiKey> excludedKeys = {
            ImGuiKey_MouseLeft,
            ImGuiKey_MouseRight,
            ImGuiKey_MouseMiddle,
            ImGuiKey_MouseX1,
            ImGuiKey_MouseX2,
            ImGuiKey_MouseWheelX,
            ImGuiKey_MouseWheelY
        };

        // Track keys that were held down last frame
        static std::bitset<ImGuiKey_NamedKey_COUNT> lastFrameHeldKeys;

        // Track multiple-press detection
        static std::vector<float> keyLastPressTime(ImGuiKey_NamedKey_COUNT, -1.0f);
        static std::vector<int> keyPressCount(ImGuiKey_NamedKey_COUNT, 0);
        const float multiPressThreshold = 0.3f; // Time threshold for multiple press detection (seconds)

        float currentTime = ImGui::GetTime();

        for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_COUNT + ImGuiKey_NamedKey_BEGIN - 5; key++) {
            if (excludedKeys.contains(static_cast<ImGuiKey>(key)))
                continue;

            ImGuiKey imKey = static_cast<ImGuiKey>(key);
            size_t idx = static_cast<size_t>(key - ImGuiKey_NamedKey_BEGIN);

            bool keyDown = ImGui::IsKeyDown(imKey);
            bool keyPressed = ImGui::IsKeyPressed(imKey, false);

            // Update currently held keys
            if (keyDown) {
                currentlyHeldKeys.set(idx);

                if (!lastFrameHeldKeys[idx]) {  // Key was just pressed this frame
                    pressedSignature.set(idx);

                    // Handle multiple press detection
                    if (currentTime - keyLastPressTime[idx] < multiPressThreshold) {
                        keyPressCount[idx]++;
                        if (keyPressCount[idx] > 1) {
                            repeatSignature.set(idx);
                        }
                    } else {
                        keyPressCount[idx] = 1;
                    }
                    keyLastPressTime[idx] = currentTime;
                }
            } else {
                if (lastFrameHeldKeys[idx]) {
                    // Key was just released
                    releasedSignature.set(idx);
                } else if (keyLastPressTime[idx] > 0 && currentTime - keyLastPressTime[idx] > multiPressThreshold) {
                    // Too much time has passed since last press, reset the counter
                    keyPressCount[idx] = 0;
                }
            }
        }

        // Get all commands to process
        const auto& commands = windowState.getCommands();

        // STEP 1: Find and process modifier/key combinations first
        bool modifierCombinationProcessed = false;

        for (const auto& command : commands) {
            if (command.isModifier()) {
                std::bitset<ImGuiKey_NamedKey_COUNT> modifierSignature = command.getSignature();
                if ((modifierSignature & currentlyHeldKeys) == modifierSignature) {
                    // This modifier is held down, now check its children
                    for (const auto& childCmd : command.getChildren()) {
                        std::bitset<ImGuiKey_NamedKey_COUNT> childSignature = childCmd.getSignature();
                        if ((childSignature & pressedSignature).any()) {
                            // We found a modifier+key combination! Execute it
                            childCmd.executePressedCallback();
                            modifierCombinationProcessed = true;
                            break; // Process only one modifier combination at a time
                        }

                        // Check for key releases while modifier is held
                        if ((childSignature & releasedSignature).any()) {
                            childCmd.executeReleasedCallback();
                        }
                    }

                    if (modifierCombinationProcessed) {
                        break;
                    }
                }
            }
        }

        // STEP 2: Only process regular commands if no modifier combination was processed
        if (!modifierCombinationProcessed) {
            for (const auto& command : commands) {
                // Skip modifiers, we already handled them
                if (command.isModifier()) continue;

                if (command.exactMatch(pressedSignature)) {
                    command.executePressedCallback();
                }

                if (command.exactMatch(releasedSignature)) {
                    command.executeReleasedCallback();
                }
            }
        }

        if (repeatSignature.any()) {
            processRepeatCommands(windowState.getCommands(), repeatSignature, currentlyHeldKeys);
        }

        // Store current key state for next frame
        lastFrameHeldKeys = currentlyHeldKeys;
    }

    void InputManager::processRepeatCommands(
        const std::span<const Command>& commands,
        const std::bitset<ImGuiKey_NamedKey_COUNT>& repeatSignature,
        const std::bitset<ImGuiKey_NamedKey_COUNT>& currentlyHeldKeys
    ) {
        for (const auto& command : commands) {
            // If this is a non-modifier command that has a repeat key
            if (command.exactMatch(repeatSignature)) {
                command.executeRepeatCallback();
            }

            // Handle cases where a modifier is held and another key is repeating
            if (!command.getChildren().empty()) {
                // Special case for modifiers: if the modifier key is held and a child key is repeating
                if (command.isModifier() && (command.getSignature() & currentlyHeldKeys) == command.getSignature()) {
                    // Check if any child key is in the repeat signature
                    for (const auto& child : command.getChildren()) {
                        // If this child is directly repeating
                        if ((child.getSignature() & repeatSignature) == child.getSignature() &&
                            child.getSignature() != command.getSignature()) {
                            child.executeRepeatCallback();
                        }
                    }

                    // Also check deeper in the hierarchy
                    auto remainingBits = repeatSignature;
                    processRepeatCommands(command.getChildren(), remainingBits, currentlyHeldKeys);
                }
                // Standard partial match handling
                else if (command.partialMatch(repeatSignature)) {
                    auto remainingBits = repeatSignature ^ command.getSignature();
                    if (remainingBits.any()) {
                        processRepeatCommands(command.getChildren(), remainingBits, currentlyHeldKeys);
                    }
                }
            }
        }
    }

    // Add this method implementation
    std::vector<CommandInfo> InputManager::getAllPossibleCommands(const WindowState& windowState) const {
        std::vector<CommandInfo> allCommands;
        // Use an empty signature to get all commands
        std::bitset<ImGuiKey_NamedKey_COUNT> emptySignature;
        collectPossibleCommands(windowState.getCommands(), emptySignature, allCommands);
        return allCommands;
    }

    std::vector<CommandInfo> InputManager::getPossibleCommands(const WindowState& windowState) const {
        std::bitset<ImGuiKey_NamedKey_COUNT> pressedSignature;

        static const std::set<ImGuiKey> excludedKeys = {
            ImGuiKey_MouseLeft,
            ImGuiKey_MouseRight,
            ImGuiKey_MouseMiddle,
            ImGuiKey_MouseX1,
            ImGuiKey_MouseX2,
            ImGuiKey_MouseWheelX,
            ImGuiKey_MouseWheelY
        };

        // Get currently pressed keys
        for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_COUNT + ImGuiKey_NamedKey_BEGIN - 5; key++) {
            if (excludedKeys.contains(static_cast<ImGuiKey>(key)))
                continue;
            if (ImGui::IsKeyDown(static_cast<ImGuiKey>(key)))
            {
                pressedSignature.set(static_cast<size_t>(key - ImGuiKey_NamedKey_BEGIN));
            }
        }

        std::vector<CommandInfo> possibleCommands;
        collectPossibleCommands(windowState.getCommands(), pressedSignature, possibleCommands);
        return possibleCommands;
    }

    void InputManager::collectPossibleCommands(
        const std::span<const Command>& commands,
        const std::bitset<ImGuiKey_NamedKey_COUNT>& pressedSignature,
        std::vector<CommandInfo>& possibleCommands) const {

        for (const auto& command : commands) {
            // If no keys are pressed, show all possible command chains
            if (pressedSignature.none()) {
                if (command.getChildren().empty() || !command.isModifier()) {
                    possibleCommands.push_back({command.getKey(), command.getDescription()});
                } else {
                    // For modifiers with children, build combinations recursively
                    std::vector<CommandInfo> childCombinations;
                    buildCommandCombinations(command, "", childCombinations);
                    possibleCommands.insert(possibleCommands.end(), childCombinations.begin(), childCombinations.end());
                }
                continue;
            }

            // If this command matches the pressed signature exactly or partially
            if (command.partialMatch(pressedSignature)) {
                if (command.isModifier() && (command.getSignature() & pressedSignature) == command.getSignature()) {
                    bool hasActivatedChildModifier = false;

                    for (const auto& child : command.getChildren()) {
                        if (child.isModifier() && (child.getSignature() & pressedSignature) == child.getSignature()) {
                            hasActivatedChildModifier = true;
                            break;
                        }
                    }

                    // For each child command, add the appropriate representation
                    for (const auto& child : command.getChildren()) {
                        if (hasActivatedChildModifier) {
                            // Skip non-modifier children or modifiers that aren't pressed
                            if (!child.isModifier() || !((child.getSignature() & pressedSignature) == child.getSignature())) {
                                continue;
                            }

                            // Child modifier is pressed, show only its children's keys
                            for (const auto& grandchild : child.getChildren()) {
                                possibleCommands.push_back({grandchild.getKey(), grandchild.getDescription()});
                            }
                        } else {
                            // No child modifiers are pressed, show all children
                            if (child.isModifier() && !child.getChildren().empty()) {
                                // Build combinations for this child modifier
                                for (const auto& grandchild : child.getChildren()) {
                                    possibleCommands.push_back({
                                        child.getKey() + "+" + grandchild.getKey(),
                                        grandchild.getDescription()
                                    });
                                }
                            } else {
                                // Child is not a modifier
                                possibleCommands.push_back({child.getKey(), child.getDescription()});
                            }
                        }
                    }

                    // Skip recursive processing if we've handled modifiers
                    continue;
                }

                // Recursively check child commands if this is not an exact match
                if (!command.getChildren().empty() && !command.exactMatch(pressedSignature)) {
                    auto remainingBits = pressedSignature ^ command.getSignature();
                    if (remainingBits.any()) {  // Only recurse if there are remaining bits
                        collectPossibleCommands(command.getChildren(), remainingBits, possibleCommands);
                    }
                }
            }
        }
    }

    // Helper method to recursively build all possible command combinations
    void InputManager::buildCommandCombinations(
        const Command& command,
        const std::string& prefix,
        std::vector<CommandInfo>& combinations) const {

        std::string currentPrefix = prefix.empty() ? command.getKey() : prefix + "+" + command.getKey();

        // If this is a leaf command or not a modifier, add the combination
        if (command.getChildren().empty() || !command.isModifier()) {
            combinations.push_back({currentPrefix, command.getDescription()});
            return;
        }

        // For modifiers with children, recursively build combinations
        for (const auto& child : command.getChildren()) {
            buildCommandCombinations(child, currentPrefix, combinations);
        }
    }
}
