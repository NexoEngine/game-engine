//// EntityActions.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the entity actions
//
///////////////////////////////////////////////////////////////////////////////

#include "EntityActions.hpp"
#include "ComponentRestoreFactory.hpp"

namespace nexo::editor {

    void EntityCreationAction::redo()
    {
        const auto &coordinator = Application::m_coordinator;
        m_entityId = coordinator->createEntity();

        for (const auto &action : m_componentRestoreActions)
            action->undo();
    }

    void EntityCreationAction::undo()
    {
        const auto &coordinator = Application::m_coordinator;
        const std::vector<std::type_index>& componentsTypeIndex = coordinator->getAllComponentTypeIndices(m_entityId);
        for (const auto typeIndex : componentsTypeIndex) {
            if (!coordinator->supportsMementoPattern(typeIndex))
                continue;
            m_componentRestoreActions.push_back(ComponentRestoreFactory::createRestoreComponent(m_entityId, typeIndex));
        }
        coordinator->destroyEntity(m_entityId);
    }

    EntityDeletionAction::EntityDeletionAction(const ecs::Entity entityId) : m_entityId(entityId)
    {
        const auto &coordinator = Application::m_coordinator;
        std::vector<std::type_index> componentsTypeIndex = coordinator->getAllComponentTypeIndices(entityId);
        for (const auto typeIndex : componentsTypeIndex) {
             if (!coordinator->supportsMementoPattern(typeIndex))
                continue;
            m_componentRestoreActions.push_back(ComponentRestoreFactory::createRestoreComponent(entityId, typeIndex));
        }
    }

    void EntityDeletionAction::redo()
    {
        // Simply destroy the entity
        const auto& coordinator = Application::m_coordinator;
        coordinator->destroyEntity(m_entityId);
    }

    void EntityDeletionAction::undo()
    {
        const auto& coordinator = Application::m_coordinator;
        // This can cause problem is the entity is not the same, maybe in the future we would need another method
        m_entityId = coordinator->createEntity();
        for (const auto &action : m_componentRestoreActions)
            action->undo();
    }
}
