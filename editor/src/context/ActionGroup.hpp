//// ActionGroup.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/04/2025
//  Description: Header file for the action group class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "actions/Action.hpp"
#include <memory>
#include <vector>

namespace nexo::editor {

    /**
    * Groups multiple actions into a single undoable action
    */
    class ActionGroup : public Action {
        public:
            ActionGroup() = default;

            void addAction(std::unique_ptr<Action> action);
            bool hasActions() const;
            void redo() override;
            void undo() override;

        private:
            std::vector<std::unique_ptr<Action>> actions;
    };

}
