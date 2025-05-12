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
            // Record a command after an operation is done
            void recordAction(std::unique_ptr<Action> action);
            // Record entity creation
            void recordEntityCreation(ecs::Entity entityId);
            // Record entity deletion (call before actually deleting)
            std::unique_ptr<Action> prepareEntityDeletion(ecs::Entity entityId);

            // For component changes using memento pattern
            template<typename MementoComponent>
            void recordComponentChange(ecs::Entity entityId,
                                    const typename MementoComponent::Memento& beforeState,
                                    const typename MementoComponent::Memento& afterState)
            {
                auto& app = nexo::getApp();
                auto& component = app.m_coordinator->getComponent<MementoComponent>(entityId);

                auto action = std::make_unique<ComponentChangeAction<MementoComponent>>(entityId, beforeState, afterState);

                recordAction(std::move(action));
            }

            // Action group for multiple operations
            std::unique_ptr<ActionGroup> createActionGroup();

            // Basic undo/redo operations
            void undo();
            void redo();
            bool canUndo() const;
            bool canRedo() const;
            void clearHistory();

            static ActionManager& get() {
                static ActionManager instance;
                return instance;
            }
        private:
            ActionHistory history;
    };

}
