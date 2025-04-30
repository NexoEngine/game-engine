//// ActionManager.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/04/2025
//  Description: Source file for the action manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "ActionManager.hpp"
#include "context/actions/EntityActions.hpp"

namespace nexo::editor {
    void ActionManager::recordAction(std::unique_ptr<Action> action)
    {
        history.addAction(std::move(action));
    }

    void ActionManager::recordEntityCreation(ecs::Entity entityId)
    {
        recordAction(std::make_unique<EntityCreationAction>(entityId));
    }

    std::unique_ptr<Action> ActionManager::prepareEntityDeletion(ecs::Entity entityId)
    {
        return std::make_unique<EntityDeletionAction>(entityId);
    }

    std::unique_ptr<ActionGroup> ActionManager::createActionGroup()
    {
        return std::make_unique<ActionGroup>();
    }

    void ActionManager::undo()
    {
        history.undo();
    }

    void ActionManager::redo()
    {
        history.redo();
    }

    bool ActionManager::canUndo() const
    {
        return history.canUndo();
    }

    bool ActionManager::canRedo() const
    {
        return history.canRedo();
    }

    void ActionManager::clearHistory(unsigned int count)
    {
        history.clear();
    }
}
