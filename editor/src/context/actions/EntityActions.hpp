// EntityActions.hpp
#pragma once

#include "Action.hpp"
#include "core/scene/Scene.hpp"
#include "Nexo.hpp"

namespace nexo::editor {

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

    std::vector<std::pair<std::type_index, std::any>> captureEntityState(ecs::Entity entity);
    void restoreComponents(ecs::Entity entity, const std::vector<std::pair<std::type_index, std::any>>& mementos);

    /**
    * Stores information needed to undo/redo entity creation
    * Relies on engine systems for actual creation/deletion logic
    */
    class EntityCreationAction : public Action {
        public:
            EntityCreationAction(ecs::Entity entityId)
                : m_entityId(entityId) {}

            void redo() override;
            void undo() override;

        private:
            ecs::Entity m_entityId;
            std::vector<std::pair<std::type_index, std::any>> m_mementos;
    };

    /**
    * Stores information needed to undo/redo entity deletion
    * Relies on engine systems for actual deletion logic
    */
    class EntityDeletionAction : public Action {
        public:
            EntityDeletionAction(ecs::Entity entityId);

            void redo() override;
            void undo() override;
        private:
            ecs::Entity m_entityId;
            // Store component mementos for those that support the pattern
            std::vector<std::pair<std::type_index, std::any>> m_mementos;
    };

}
