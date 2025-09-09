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
        const std::vector<std::any>& componentsTypeIndex = coordinator->getAllComponents(m_entityId);
        for (const auto &typeIndex : componentsTypeIndex) {
            if (!coordinator->supportsMementoPattern(typeIndex))
                continue;
            m_componentRestoreActions.push_back(ComponentRestoreFactory::createRestoreComponent(m_entityId, typeIndex));
        }
        coordinator->destroyEntity(m_entityId);
    }

    EntityDeletionAction::EntityDeletionAction(const ecs::Entity entityId) : m_entityId(entityId)
    {
        const auto &coordinator = Application::m_coordinator;
        const std::vector<std::any>& componentsTypeIndex = coordinator->getAllComponents(m_entityId);
        for (const auto &typeIndex : componentsTypeIndex) {
             if (!coordinator->supportsMementoPattern(typeIndex))
                continue;
            auto typeId = std::type_index(typeIndex.type());
            if (typeId == typeid(components::ParentComponent)) {
                auto parentOpt = coordinator->tryGetComponent<components::ParentComponent>(entityId);
                if (parentOpt.has_value()) {
                    ecs::Entity oldParent = parentOpt->get().parent;
                    m_componentRestoreActions.push_back(
                        std::make_unique<EntityParentChangeAction>(entityId, oldParent, ecs::INVALID_ENTITY)
                    );
                }
                continue;
            }
            m_componentRestoreActions.push_back(ComponentRestoreFactory::createRestoreComponent(entityId, typeIndex));
        }
    }

    void EntityDeletionAction::redo()
    {
        // Simply destroy the entity
        const auto& coordinator = Application::m_coordinator;
        auto parentOpt = coordinator->tryGetComponent<components::ParentComponent>(m_entityId);
        if (parentOpt.has_value()) {
            ecs::Entity oldParent = parentOpt->get().parent;
            auto parentTransformOpt = coordinator->tryGetComponent<components::TransformComponent>(oldParent);
            if (parentTransformOpt.has_value()) {
                parentTransformOpt->get().removeChild(m_entityId);
            }
        }
        coordinator->destroyEntity(m_entityId);
    }

    void EntityDeletionAction::undo()
    {
        const auto& coordinator = Application::m_coordinator;
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

    EntityHierarchyDeletionAction::EntityHierarchyDeletionAction(ecs::Entity rootEntity)
    : m_root(rootEntity), m_group(std::make_unique<ActionGroup>())
    {
        std::function<void(ecs::Entity)> collectActions = [&](ecs::Entity entity) {

            auto transformOpt = Application::m_coordinator->tryGetComponent<components::TransformComponent>(entity);
            if (transformOpt) {
                for (const auto& child : transformOpt->get().children) {
                    collectActions(child);
                    m_parentRelations.emplace_back(child, entity);
                    m_group->addAction(std::make_unique<EntityDeletionAction>(child));
                }
            }

            auto parentOpt = Application::m_coordinator->tryGetComponent<components::ParentComponent>(entity);
            if (parentOpt && parentOpt->get().parent != ecs::INVALID_ENTITY) {
                ecs::Entity parent = parentOpt->get().parent;
                m_parentRelations.emplace_back(entity, parent);
            }

            m_group->addAction(std::make_unique<EntityDeletionAction>(entity));
        };

        collectActions(rootEntity);
    }

    void EntityHierarchyDeletionAction::redo() {
        m_group->redo();
    }

    void EntityHierarchyDeletionAction::undo() {
        m_group->undo();
    }

    EntityHierarchyCreationAction::EntityHierarchyCreationAction(ecs::Entity rootEntity)
        : m_root(rootEntity), m_group(std::make_unique<ActionGroup>())
    {
        std::function<void(ecs::Entity)> collectActions = [&](ecs::Entity entity) {
            auto transformOpt = Application::m_coordinator->tryGetComponent<components::TransformComponent>(entity);
            if (transformOpt) {
                for (const auto& child : transformOpt->get().children) {
                    collectActions(child);
                    m_parentRelations.emplace_back(child, entity);
                    m_group->addAction(std::make_unique<EntityCreationAction>(child));
                }
            }

            auto parentOpt = Application::m_coordinator->tryGetComponent<components::ParentComponent>(entity);
            if (parentOpt && parentOpt->get().parent != ecs::INVALID_ENTITY) {
                ecs::Entity parent = parentOpt->get().parent;
                m_parentRelations.emplace_back(entity, parent);
            }

            m_group->addAction(std::make_unique<EntityCreationAction>(entity));
        };

        collectActions(rootEntity);

        for (const auto& [child, parent] : m_parentRelations) {
            if (parent != ecs::INVALID_ENTITY) {
                m_group->addAction(std::make_unique<EntityParentChangeAction>(
                    child, ecs::INVALID_ENTITY, parent));
            }
        }
    }

    void EntityHierarchyCreationAction::redo() {
        m_group->redo();
    }

    void EntityHierarchyCreationAction::undo() {
        m_group->undo();
    }



}
