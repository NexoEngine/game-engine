//// ActionGroup.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/04/2025
//  Description: Source file for the action group class
//
///////////////////////////////////////////////////////////////////////////////

#include "ActionGroup.hpp"

#include <ranges>

namespace nexo::editor {

    void ActionGroup::addAction(std::unique_ptr<Action> action)
    {
        actions.push_back(std::move(action));
    }

    bool ActionGroup::hasActions() const
    {
        return !actions.empty();
    }

    void ActionGroup::redo()
    {
        for (const auto &action : actions)
            action->redo();
    }

    void ActionGroup::undo()
    {
        for (const auto &action : std::ranges::reverse_view(actions))
            action->undo();
    }
}
