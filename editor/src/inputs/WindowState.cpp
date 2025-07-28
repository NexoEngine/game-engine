//// WindowState.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
        return {m_commands.data(), m_commands.size()};
    }
}
