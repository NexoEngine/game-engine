//// ActionGroup.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
    class ActionGroup final : public Action {
        public:
            ActionGroup() = default;

            void addAction(std::unique_ptr<Action> action);
            [[nodiscard]] bool hasActions() const;
            void redo() override;
            void undo() override;

        private:
            std::vector<std::unique_ptr<Action>> actions;
    };

}
