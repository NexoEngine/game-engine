//// EntityActions.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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

    void EntityParentChangeAction::redo()
    {
        auto& coordinator = *Application::m_coordinator;

        // Handle old parent
        if (m_oldParent != ecs::INVALID_ENTITY) {
            const auto oldParentTransform = coordinator.tryGetComponent<components::TransformComponent>(m_oldParent);
            if (oldParentTransform.has_value()) {
                oldParentTransform->get().removeChild(m_entity);
            }
        }

        // Handle new parent
        if (m_newParent != ecs::INVALID_ENTITY) {
            // Add or update parent component on entity
            const auto parentComp = coordinator.tryGetComponent<components::ParentComponent>(m_entity);
            if (!parentComp.has_value()) {
                coordinator.addComponent(m_entity, components::ParentComponent{m_newParent});
            } else {
                parentComp->get().parent = m_newParent;
            }

            // Add to new parent's children
            auto newParentTransform = coordinator.tryGetComponent<components::TransformComponent>(m_newParent);
            if (!newParentTransform.has_value()) {
                coordinator.addComponent(m_newParent, components::TransformComponent{});
                newParentTransform = coordinator.tryGetComponent<components::TransformComponent>(m_newParent);
            }
            if (newParentTransform.has_value()) {
                newParentTransform->get().addChild(m_entity);
            }
        } else {
            // Remove parent component (make it a root entity)
            const auto parentComp = coordinator.tryGetComponent<components::ParentComponent>(m_entity);
            if (parentComp.has_value()) {
                coordinator.removeComponent<components::ParentComponent>(m_entity);
            }
        }
    }

    void EntityParentChangeAction::undo()
    {
        auto& coordinator = *Application::m_coordinator;

        // Handle new parent (undo by removing from it)
        if (m_newParent != ecs::INVALID_ENTITY) {
            const auto newParentTransform = coordinator.tryGetComponent<components::TransformComponent>(m_newParent);
            if (newParentTransform.has_value()) {
                newParentTransform->get().removeChild(m_entity);
            }
        }

        // Handle old parent (restore to it)
        if (m_oldParent != ecs::INVALID_ENTITY) {
            // Add or update parent component on entity
            const auto parentComp = coordinator.tryGetComponent<components::ParentComponent>(m_entity);
            if (!parentComp.has_value()) {
                coordinator.addComponent(m_entity, components::ParentComponent{m_oldParent});
            } else {
                parentComp->get().parent = m_oldParent;
            }

            // Add back to old parent's children
            auto oldParentTransform = coordinator.tryGetComponent<components::TransformComponent>(m_oldParent);
            if (!oldParentTransform.has_value()) {
                coordinator.addComponent(m_oldParent, components::TransformComponent{});
                oldParentTransform = coordinator.tryGetComponent<components::TransformComponent>(m_oldParent);
            }
            if (oldParentTransform.has_value()) {
                oldParentTransform->get().addChild(m_entity);
            }
        } else {
            // Remove parent component (restore to root entity)
            const auto parentComp = coordinator.tryGetComponent<components::ParentComponent>(m_entity);
            if (parentComp.has_value()) {
                coordinator.removeComponent<components::ParentComponent>(m_entity);
            }
        }
    }
}
