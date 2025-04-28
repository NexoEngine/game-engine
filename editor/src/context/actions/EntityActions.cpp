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

namespace nexo::editor {

    std::vector<std::pair<std::type_index, std::any>> captureEntityState(ecs::Entity entity)
    {
        auto& coordinator = Application::getInstance().m_coordinator;
        const auto &componentData = coordinator->getAllComponents(entity);
        std::vector<std::pair<std::type_index, std::any>> result;

        // For components that support the Memento pattern, create mementos
        for (const auto& [typeIndex, componentAny] : componentData) {
            if (coordinator->supportsMementoPattern(componentAny)) {
                std::any memento = coordinator->saveComponent(componentAny);
                if (memento.has_value()) {
                    result.emplace_back(typeIndex, std::move(memento));
                }
            }
        }
        return result;
    }

    void restoreComponents(ecs::Entity entity, const std::vector<std::pair<std::type_index, std::any>>& mementos)
    {
        auto &app = getApp();
        auto &coordinator = app.m_coordinator;
        for (const auto& [typeIndex, mementoAny] : mementos) {
            std::any component = coordinator->restoreComponent(mementoAny, typeIndex);
            coordinator->addComponentAny(entity, typeIndex, component);
        }
    }

    void EntityCreationAction::redo()
    {
        auto& coordinator = Application::getInstance().m_coordinator;
        m_entityId = coordinator->createEntity();

        restoreComponents(m_entityId, m_mementos);
    }

    void EntityCreationAction::undo()
    {
        m_mementos = captureEntityState(m_entityId);
        auto& coordinator = Application::getInstance().m_coordinator;
        coordinator->destroyEntity(m_entityId);
    }

    EntityDeletionAction::EntityDeletionAction(ecs::Entity entityId) : m_entityId(entityId)
    {
        m_mementos = captureEntityState(m_entityId);
    }

    void EntityDeletionAction::redo()
    {
        // Simply destroy the entity
        auto& coordinator = Application::getInstance().m_coordinator;
        coordinator->destroyEntity(m_entityId);
    }

    void EntityDeletionAction::undo()
    {
        auto& coordinator = Application::getInstance().m_coordinator;

        // First recreate the entity with the same ID
        // This assumes you have a createEntityWithId method, otherwise
        // you'll need to adapt to your coordinator's API
        m_entityId = coordinator->createEntity();

        restoreComponents(m_entityId, m_mementos);
    }
}
