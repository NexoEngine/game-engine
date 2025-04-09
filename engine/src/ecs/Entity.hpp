//// Entity.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the entity definitions, helper functions and entity manager
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <deque>
#include <array>
#include <vector>
#include <span>

#include "Definitions.hpp"

namespace nexo::ecs {

    /**
    * @class EntityManager
    *
    * @brief Manages entities in an ECS (Entity-Component-System) architecture.
    *
    * This class is responsible for creating, managing, and destroying entities. It maintains
    * a record of active entities and their signatures, which define the components associated with each entity.
    */
    class EntityManager {
        public:
            /**
            * @brief Constructor for EntityManager.
            *
            * Initializes the pool of available entities. Each entity is represented by a unique ID.
            */
            EntityManager();

            /**
            * @brief Creates a new entity.
            *
            * Assigns a unique ID to the new entity and tracks it as an active entity.
            * @return Entity - The ID of the newly created entity.
            */
            Entity createEntity();

            /**
            * @brief Destroys an entity.
            *
            * Marks the entity as inactive and returns its ID to the pool of available IDs.
            * @param entity - The ID of the entity to be destroyed.
            */
            void destroyEntity(Entity entity);

            /**
            * @brief Sets the signature of an entity.
            *
            * The signature defines which components are associated with the entity.
            * @param entity - The ID of the entity.
            * @param signature - The signature to be set for the entity.
            */
            void setSignature(Entity entity, Signature signature);

            /**
            * @brief Retrieves the signature of an entity.
            *
            * @param entity - The ID of the entity.
            * @return Signature - The signature of the entity.
            */
            [[nodiscard]] Signature getSignature(Entity entity) const;

            /**
             * @brief Returns the number of currently active entities
             *
             * @return std::uint32_t The count of living entities
             */
            [[nodiscard]] std::uint32_t getLivingEntityCount() const;

            /**
             * @brief Retrieves a view of all currently active entities
             *
             * @return std::span<const Entity> A span containing all living entity IDs
             */
            [[nodiscard]] std::span<const Entity> getLivingEntities() const;

        private:
            std::deque<Entity> m_availableEntities{};
            std::vector<Entity> m_livingEntities{};

            std::array<Signature, MAX_ENTITIES> m_signatures{};
    };
}
