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

#include "Command.hpp"
#include <span>

namespace nexo::editor {

    class WindowState {
        public:
            WindowState() = default;
            WindowState(unsigned int id) : m_id(id) {}
            ~WindowState() = default;

            const unsigned int getId() const;
            void registerCommand(const Command &command);
            const std::span<const Command> getCommands() const;
        private:
            unsigned int m_id;
            std::vector<Command> m_commands;
    };
}
