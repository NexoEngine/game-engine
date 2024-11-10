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

#include <queue>
#include <array>

#include "Signature.hpp"

namespace nexo::ecs {

    inline Entity getEntityId(const Entity entity)
    {
        return entity & ENTITY_ID_MASK;
    }

    // Set the active status flag
    inline void setActive(Entity &entity, const bool isActive)
    {
        if (isActive)
        {
            entity |= ACTIVE_FLAG_MASK;
        } else
        {
            entity &= ~ACTIVE_FLAG_MASK;
        }
    }

    // Set the rendered status flag
    inline void setRendered(Entity &entity, const bool isRendered)
    {
        if (isRendered)
        {
            entity |= RENDERED_FLAG_MASK;
        } else
        {
            entity &= ~RENDERED_FLAG_MASK;
        }
    }

    // Check if the entity is active
    inline bool isActive(const Entity entity)
    {
        return (entity & ACTIVE_FLAG_MASK) != 0;
    }

    // Check if the entity is rendered
    inline bool isRendered(const Entity entity)
    {
        return (entity & RENDERED_FLAG_MASK) != 0;
    }

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

        private:
            std::queue<Entity> m_availableEntities{};

            std::array<Signature, MAX_ENTITIES> m_signatures{};

            std::uint32_t m_livingEntityCount{};
    };
}
