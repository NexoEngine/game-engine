//// EntityActions.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/06/2025
//  Description: Header file for the entity actions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Action.hpp"
#include "Nexo.hpp"

namespace nexo::editor {

    template<typename ComponentType>
    class ComponentRestoreAction final : public Action {
        public:
            explicit ComponentRestoreAction(const ecs::Entity entity) : m_entity(entity)
            {
                ComponentType &target = Application::m_coordinator->getComponent<ComponentType>(m_entity);
                m_memento = target.save();
            };

            void undo() override
            {
                ComponentType target;
                target.restore(m_memento);
                Application::m_coordinator->addComponent(m_entity, target);
            }

            void redo() override
            {
                //We have nothing to do here since we are simply redeleting the entity and its components
            }

        private:
            ecs::Entity m_entity;
            typename ComponentType::Memento m_memento;
    };

    template<typename ComponentType>
    class ComponentAddAction final : public Action {
        public:
            explicit ComponentAddAction(const ecs::Entity entity)
                : m_entity(entity) {}

            void undo() override
            {
                m_memento = Application::m_coordinator->getComponent<ComponentType>(m_entity).save();
                Application::m_coordinator->removeComponent<ComponentType>(m_entity);
            }

            void redo() override
            {
                //We have nothing to do here since we are simply redeleting the entity and its components
            }

        private:
            ecs::Entity m_entity;
            typename ComponentType::Memento m_memento;
    };

    template<typename ComponentType>
    class ComponentRemoveAction final : public Action {
        public:
            explicit ComponentRemoveAction(const ecs::Entity entity) : m_entity(entity)
            {
                m_memento = Application::m_coordinator->getComponent<ComponentType>(m_entity).save();
            }

            void undo() override
            {
                ComponentType target;
                target.restore(m_memento);
                Application::m_coordinator->addComponent(m_entity, target);
            }

            void redo() override
            {
                Application::m_coordinator->removeComponent<ComponentType>(m_entity);
            }

        private:
            ecs::Entity m_entity;
            typename ComponentType::Memento m_memento;
    };

    template<typename ComponentType>
    class ComponentChangeAction final : public Action {
        public:
            explicit ComponentChangeAction(
                const ecs::Entity entity,
                const typename ComponentType::Memento& before,
                const typename ComponentType::Memento& after
            ) : m_entity(entity), m_beforeState(before), m_afterState(after){}

            void redo() override
            {
                ComponentType &target = Application::m_coordinator->getComponent<ComponentType>(m_entity);
                target.restore(m_afterState);
            }

            void undo() override
            {
                ComponentType &target = Application::m_coordinator->getComponent<ComponentType>(m_entity);
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
    class EntityCreationAction final : public Action {
        public:
            explicit EntityCreationAction(const ecs::Entity entityId)
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
    class EntityDeletionAction final : public Action {
        public:
            explicit EntityDeletionAction(ecs::Entity entityId);

            void redo() override;
            void undo() override;
        private:
            ecs::Entity m_entityId;
            std::vector<std::unique_ptr<Action>> m_componentRestoreActions;
    };

    /**
    * Stores information needed to undo/redo entity parent changes
    * Handles hierarchy component updates
    */
    class EntityParentChangeAction final : public Action {
        public:
            EntityParentChangeAction(const ecs::Entity entity, const ecs::Entity oldParent, const ecs::Entity newParent)
                : m_entity(entity), m_oldParent(oldParent), m_newParent(newParent) {}

            void redo() override;
            void undo() override;
        private:
            ecs::Entity m_entity;
            ecs::Entity m_oldParent;
            ecs::Entity m_newParent;
    };

}
