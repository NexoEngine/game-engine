//// WindowState.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        23/05/2025
//  Description: Header file for the window state, a class wrapping a set of input commands
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <span>
#include "Command.hpp"

namespace nexo::editor {

    /**
     * @class WindowState
     * @brief Represents the state of a window, including its unique identifier and associated commands.
     *
     * The WindowState class encapsulates a unique identifier for the window and manages a collection of
     * Command objects that define the input commands available within that window. It provides methods to
     * register new commands and retrieve the list of registered commands.
     */
    class WindowState {
       public:
        /**
         * @brief Default constructor for the WindowState class.
         */
        WindowState() = default;

        /**
         * @brief Constructs a WindowState with a unique identifier.
         * @param id The unique identifier for the window state.
         */
        WindowState(const unsigned int id) : m_id(id)
        {}
        /**
         * @brief Destructor for the WindowState class.
         */
        ~WindowState() = default;

        /**
         * @brief Retrieves the unique identifier of the window state.
         * @return The unique identifier as an unsigned integer.
         */
        [[nodiscard]] unsigned int getId() const;

        /**
         * @brief Registers a new command to the window state.
         * @param command The Command object to be registered.
         */
        void registerCommand(const Command &command);

        /**
         * @brief Retrieves a span of all registered commands.
         * @return A std::span containing all registered Command objects.
         */
        [[nodiscard]] std::span<const Command> getCommands() const;

       private:
        unsigned int m_id{};
        std::vector<Command> m_commands;
    };
} // namespace nexo::editor
