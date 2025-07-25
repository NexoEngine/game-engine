//// ActionGroup.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
