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

#include <bitset>
#include <utility>
#include "Command.hpp"
#include "WindowState.hpp"

namespace nexo::editor {

    /**
     * @Struct CommandInfo
     * @brief Simple struct to hold command key and description for display purposes.
     */
    struct CommandInfo {
        std::string key;
        std::string description;

        /**
         * @brief Constructor to initialize CommandInfo with key and description.
         * @param k The key combination string.
         * @param d The command description.
         */
        CommandInfo(std::string k, std::string d) : key(std::move(k)), description(std::move(d))
        {}
    };

    /**
     * @class InputManager
     * @brief Manages input processing and command execution based on key states.
     *
     * The InputManager class tracks the current and previous states of keys,
     * processes input events, and executes associated command callbacks.
     * It supports detecting key presses, releases, and repeats, as well as
     * handling modifier keys and nested command structures.
     */
    class InputManager {
       public:
        InputManager()  = default;
        ~InputManager() = default;

        // Process inputs based on current window state
        /**
         * @brief Processes input events and executes associated command callbacks.
         * @param windowState The current state of the window, including available commands.
         *
         * This method checks the current state of keys, detects changes from the previous frame,
         * and executes the appropriate callbacks for commands that match the detected key combinations.
         * It handles key presses, releases, and repeats, as well as modifier keys and nested commands.
         */
        static void processInputs(const WindowState& windowState);

        // Update these method signatures:
        /**
         * @brief Retrieves a list of possible commands based on the current key state.
         * @param windowState The current state of the window, including available commands.
         * @return A vector of CommandInfo objects representing possible commands.
         *
         * This method analyzes the current key state and matches it against the available commands
         * in the provided WindowState. It returns a list of commands that can be activated based on
         * the currently pressed keys, including handling modifier keys and nested command structures.
         */
        [[nodiscard]] std::vector<CommandInfo> getPossibleCommands(const WindowState& windowState) const;

        /** @brief Retrieves all possible commands regardless of current key state.
         * @param windowState The current state of the window, including available commands.
         * @return A vector of CommandInfo objects representing all possible commands.
         *
         * This method returns a comprehensive list of all commands available in the provided
         * WindowState, without filtering based on the current key state. It is useful for displaying
         * all commands to the user, such as in a help menu or command palette.
         */
        [[nodiscard]] std::vector<CommandInfo> getAllPossibleCommands(const WindowState& windowState) const;

       private:
        // Current and previous key states for detecting changes
        std::bitset<ImGuiKey_NamedKey_COUNT> m_currentKeyState;
        std::bitset<ImGuiKey_NamedKey_COUNT> m_keyWasDownLastFrame;

        /**
         * @brief Bitset tracking keys held down in the last frame.
         *
         * This bitset is used to determine which keys were held down in the previous frame,
         * allowing for the detection of key releases and repeats.
         */
        static void processRepeatCommands(const std::span<const Command>& commands,
                                          const std::bitset<ImGuiKey_NamedKey_COUNT>& repeatSignature,
                                          const std::bitset<ImGuiKey_NamedKey_COUNT>& currentlyHeldKeys);

        /**
         * @brief Recursively collects possible commands based on the pressed key signature.
         * @param commands A span of available Command objects to check against.
         * @param pressedSignature A bitset representing the currently pressed keys.
         * @param possibleCommands A vector to store the collected CommandInfo objects.
         *
         * This method traverses the provided commands and checks if they match the current
         * pressed key signature. It collects commands that can be activated based on the
         * pressed keys, including handling modifier keys and nested command structures.
         */
        static void collectPossibleCommands(const std::span<const Command>& commands,
                                     const std::bitset<ImGuiKey_NamedKey_COUNT>& pressedSignature,
                                     std::vector<CommandInfo>& possibleCommands);

        /**
         * @brief Recursively builds command combinations for modifier commands.
         * @param command The current Command object to process.
         * @param prefix The accumulated key prefix for nested commands.
         * @param combinations A vector to store the resulting CommandInfo combinations.
         *
         * This method is used to generate all possible key combinations for commands
         * that are modifiers and have child commands. It constructs the full key
         * combination strings by appending child command keys to the prefix.
         */
        static void buildCommandCombinations(const Command& command, const std::string& prefix,
                                             std::vector<CommandInfo>& combinations);
    };
} // namespace nexo::editor
