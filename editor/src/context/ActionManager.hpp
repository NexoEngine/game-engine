//// ActionManager.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/04/2025
//  Description: Header file for the action manager class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ActionHistory.hpp"
#include "ActionGroup.hpp"
#include "context/actions/EntityActions.hpp"
#include <memory>

namespace nexo::editor {

    class ActionManager {
        public:
            void recordAction(std::unique_ptr<Action> action);
            void recordEntityCreation(ecs::Entity entityId);
            static std::unique_ptr<Action> prepareEntityDeletion(ecs::Entity entityId);
            static std::unique_ptr<Action> prepareEntityHierarchyDeletion(ecs::Entity entityId);

            template<typename MementoComponent>
            void recordComponentChange(ecs::Entity entityId,
                                    const typename MementoComponent::Memento& beforeState,
                                    const typename MementoComponent::Memento& afterState)
            {
                auto action = std::make_unique<ComponentChangeAction<MementoComponent>>(entityId, beforeState, afterState);
                recordAction(std::move(action));
            }

            static std::unique_ptr<ActionGroup> createActionGroup();

            void undo();
            void redo();
            [[nodiscard]] bool canUndo() const;
            [[nodiscard]] bool canRedo() const;
            void clearHistory(unsigned int count = 0);
            [[nodiscard]] unsigned int getUndoStackSize() const;

            static ActionManager& get() {
                static ActionManager instance;
                return instance;
            }
        private:
            ActionHistory history;
    };

}
