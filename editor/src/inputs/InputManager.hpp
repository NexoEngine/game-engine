//// Input.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:
//  Description:
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "WindowState.hpp"
#include "Command.hpp"
#include <bitset>
#include <utility>

namespace nexo::editor {

    struct CommandInfo {
        std::string key;
        std::string description;

        CommandInfo(std::string  k, std::string  d) : key(std::move(k)), description(std::move(d)) {}
    };

    class InputManager {
        public:
            InputManager() = default;
            ~InputManager() = default;

            // Process inputs based on current window state
            void processInputs(const WindowState& windowState);

            // Update these method signatures:
            [[nodiscard]] std::vector<CommandInfo> getPossibleCommands(const WindowState& windowState) const;
            [[nodiscard]] std::vector<CommandInfo> getAllPossibleCommands(const WindowState& windowState) const;

        private:
            // Current and previous key states for detecting changes
            std::bitset<ImGuiKey_NamedKey_COUNT> m_currentKeyState;
            std::bitset<ImGuiKey_NamedKey_COUNT> m_keyWasDownLastFrame;

            void processRepeatCommands(
                const std::span<const Command>& commands,
                const std::bitset<ImGuiKey_NamedKey_COUNT>& repeatSignature,
                const std::bitset<ImGuiKey_NamedKey_COUNT>& currentlyHeldKeys
            );
            void collectPossibleCommands(
                const std::span<const Command>& commands,
                const std::bitset<ImGuiKey_NamedKey_COUNT>& pressedSignature,
                std::vector<CommandInfo>& possibleCommands) const;

            void buildCommandCombinations(
                const Command& command,
                const std::string& prefix,
                std::vector<CommandInfo>& combinations) const;
    };
}
