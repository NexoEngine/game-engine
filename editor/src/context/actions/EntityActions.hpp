// EntityActions.hpp
#pragma once

#include "Action.hpp"
#include "core/scene/Scene.hpp"
#include "Nexo.hpp"

namespace nexo::editor {

    template<typename ComponentType>
    class ComponentRestoreAction : public Action {
        public:
            ComponentRestoreAction(ecs::Entity entity) : m_entity(entity)
            {
                auto &app = getApp();
                ComponentType &target = app.m_coordinator->getComponent<ComponentType>(m_entity);
                m_memento = target.save();
            };

            void undo() override
            {
                auto &app = getApp();
                ComponentType target;
                target.restore(m_memento);
                app.m_coordinator->addComponent(m_entity, target);
            }

            void redo() override {}

        private:
            ecs::Entity m_entity;
            typename ComponentType::Memento m_memento;
    };

    template<typename ComponentType>
    class ComponentAddAction : public Action {
        public:
            ComponentAddAction(ecs::Entity entity)
                : m_entity(entity) {}

            void undo() override
            {
                auto &app = getApp();
                m_memento = app.m_coordinator->getComponent<ComponentType>(m_entity).save();
                app.m_coordinator->removeComponent<ComponentType>(m_entity);
            }

            void redo() override
            {
                auto &app = getApp();
                ComponentType target;
                target.restore(m_memento);
                app.m_coordinator->addComponent(m_entity, target);
            }

        private:
            ecs::Entity m_entity;
            typename ComponentType::Memento m_memento;
    };

    template<typename ComponentType>
    class ComponentRemoveAction : public Action {
        public:
            ComponentRemoveAction(ecs::Entity entity) : m_entity(entity)
            {
                auto &app = getApp();
                m_memento = app.m_coordinator->getComponent<ComponentType>(m_entity).save();
            }

            void undo() override
            {
                auto &app = getApp();
                ComponentType target;
                target.restore(m_memento);
                app.m_coordinator->addComponent(m_entity, target);
            }

            void redo() override
            {
                auto &app = getApp();
                app.m_coordinator->removeComponent<ComponentType>(m_entity);
            }

        private:
            ecs::Entity m_entity;
            typename ComponentType::Memento m_memento;
    };

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
                target.restore(m_afterState);
            }

            void undo() override
            {
                auto &app = getApp();
                ComponentType &target = app.m_coordinator->getComponent<ComponentType>(m_entity);
                target.restore(m_beforeState);
            }

        private:
            ecs::Entity m_entity;
            typename ComponentType::Memento m_beforeState;
            typename ComponentType::Memento m_afterState;
    };

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
            std::vector<std::unique_ptr<Action>> m_componentRestoreActions;
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
            std::vector<std::unique_ptr<Action>> m_componentRestoreActions;
    };

}
