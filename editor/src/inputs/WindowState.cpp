//// WindowState.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        23/05/2025
//  Description: Source file for the window state class
//
///////////////////////////////////////////////////////////////////////////////

#include "WindowState.hpp"

namespace nexo::editor {

    unsigned int WindowState::getId() const
    {
        return m_id;
    }

    void WindowState::registerCommand(const Command &command)
    {
        m_commands.push_back(command);
    }

    std::span<const Command> WindowState::getCommands() const
    {
        return std::span<const Command>(m_commands.data(), m_commands.size());
    }
}
