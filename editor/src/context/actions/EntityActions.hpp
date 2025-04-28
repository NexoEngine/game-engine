// EntityActions.hpp
#pragma once

#include "Action.hpp"
#include "core/scene/Scene.hpp"
#include "Nexo.hpp"

namespace nexo::editor {

template<typename ComponentType>
auto getComponentLambda(ecs::Entity entity) {
    return [entity]() -> ComponentType& {
        return Application::getInstance().m_coordinator->getComponent<ComponentType>(entity);
    };
}

template<typename ComponentType>
class ComponentChangeAction : public Action {
    public:
        ComponentChangeAction(
            ecs::Entity entity,
            const typename ComponentType::Memento& before,
            const typename ComponentType::Memento& after
        ) : m_entity(entity), m_beforeState(before), m_afterState(after){}

        void redo() override
        {
            auto &app = getApp();
            ComponentType &target = app.m_coordinator->getComponent<ComponentType>(m_entity);
            target = m_afterState.restore();
        }

        void undo() override
        {
            auto &app = getApp();
            ComponentType &target = app.m_coordinator->getComponent<ComponentType>(m_entity);
            target = m_beforeState.restore();
        }

    private:
        ecs::Entity m_entity;
        typename ComponentType::Memento m_beforeState;
        typename ComponentType::Memento m_afterState;
};

inline std::vector<std::pair<std::type_index, std::any>> captureEntityState(ecs::Entity entity)
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

inline void restoreComponents(ecs::Entity entity, const std::vector<std::pair<std::type_index, std::any>>& mementos)
{
    auto &app = getApp();
    auto &coordinator = app.m_coordinator;
    for (const auto& [typeIndex, mementoAny] : mementos) {
        std::any component = coordinator->restoreComponent(mementoAny, typeIndex);
        coordinator->addComponentAny(entity, typeIndex, component);
    }
}

/**
 * Stores information needed to undo/redo entity creation
 * Relies on engine systems for actual creation/deletion logic
 */
class EntityCreationAction : public Action {
private:
    ecs::Entity m_entityId;
    std::vector<std::pair<std::type_index, std::any>> m_mementos;

public:
    EntityCreationAction(ecs::Entity entityId)
        : m_entityId(entityId) {}

    void redo() override {
        auto& coordinator = Application::getInstance().m_coordinator;
        m_entityId = coordinator->createEntity();

         restoreComponents(m_entityId, m_mementos);
    }

    void undo() override
    {
        m_mementos = captureEntityState(m_entityId);
        auto& coordinator = Application::getInstance().m_coordinator;
        coordinator->destroyEntity(m_entityId);
    }
};

/**
 * Stores information needed to undo/redo entity deletion
 * Relies on engine systems for actual deletion logic
 */
 class EntityDeletionAction : public Action {
 private:
     ecs::Entity m_entityId;
     // Store component mementos for those that support the pattern
     std::vector<std::pair<std::type_index, std::any>> m_mementos;

 public:
     EntityDeletionAction(ecs::Entity entityId) : m_entityId(entityId)
     {
        m_mementos = captureEntityState(m_entityId);
     }

     void redo() override
     {
         // Simply destroy the entity
         auto& coordinator = Application::getInstance().m_coordinator;
         coordinator->destroyEntity(m_entityId);
     }

     void undo() override
     {
         auto& coordinator = Application::getInstance().m_coordinator;

         // First recreate the entity with the same ID
         // This assumes you have a createEntityWithId method, otherwise
         // you'll need to adapt to your coordinator's API
        m_entityId = coordinator->createEntity();

         restoreComponents(m_entityId, m_mementos);
     }
 };

}
