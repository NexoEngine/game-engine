//// WindowState.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        23/05/2025
//  Description: Header file for the window state, a class wrapping a set of input commands
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Command.hpp"
#include <span>

namespace nexo::editor {

    class WindowState {
        public:
            WindowState() = default;

            WindowState(const unsigned int id) : m_id(id) {}
            ~WindowState() = default;

            [[nodiscard]] unsigned int getId() const;
            void registerCommand(const Command &command);
            [[nodiscard]] std::span<const Command> getCommands() const;
        private:
            unsigned int m_id{};
            std::vector<Command> m_commands;
    };
}
