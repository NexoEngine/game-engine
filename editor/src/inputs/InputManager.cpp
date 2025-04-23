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

        // -5 to avoid reserved mod key
        for (int key = ImGuiKey_NamedKey_BEGIN; key < ImGuiKey_NamedKey_COUNT + ImGuiKey_NamedKey_BEGIN - 5; key++) {
            if (ImGui::IsKeyPressed(static_cast<ImGuiKey>(key)) || ImGui::IsKeyDown(static_cast<ImGuiKey>(key)))
            {
                pressedSignature.set(static_cast<size_t>(key - ImGuiKey_NamedKey_BEGIN));
            }
            if (ImGui::IsKeyReleased(static_cast<ImGuiKey>(key)))
            {
                releasedSignature.set(static_cast<size_t>(key - ImGuiKey_NamedKey_BEGIN));
            }
        }

        // Process commands for the current window
        processCommands(windowState.getCommands(), pressedSignature, releasedSignature);
    }

    void InputManager::processCommands(
        const std::span<const Command>& commands,
        const std::bitset<ImGuiKey_NamedKey_COUNT>& pressedSignature,
        const std::bitset<ImGuiKey_NamedKey_COUNT>& releasedSignature
    ) {
        for (const auto& command : commands) {
            if (command.exactMatch(pressedSignature)) {
                // Execute the command on exact match
                command.executePressedCallback();
            }
            else if (command.partialMatch(pressedSignature) && !command.getChildren().empty()) {
                // If partial match and has children, check children commands
                auto remainingBits = pressedSignature ^ command.getSignature();
                processCommands(command.getChildren(), remainingBits, releasedSignature);
            }

            if (command.exactMatch(releasedSignature)) {
                // Execute the command on exact match
                command.executeReleasedCallback();
            }
            else if (command.partialMatch(releasedSignature) && !command.getChildren().empty()) {
                // If partial match and has children, check children commands
                auto remainingBits = releasedSignature ^ command.getSignature();
                processCommands(command.getChildren(), pressedSignature, remainingBits);
            }
        }
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
                // If it's a modifier and matches exactly
                if (command.isModifier() && (command.getSignature() & pressedSignature) == command.getSignature()) {
                    // Check if any child modifiers are pressed
                    bool hasActivatedChildModifier = false;

                    for (const auto& child : command.getChildren()) {
                        if (child.isModifier() && (child.getSignature() & pressedSignature) == child.getSignature()) {
                            hasActivatedChildModifier = true;
                            break;
                        }
                    }

                    // For each child command, add the appropriate representation
                    for (const auto& child : command.getChildren()) {
                        // If we have an activated child modifier, only process those
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
