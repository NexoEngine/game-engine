/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Entity.hpp
*/

#pragma once

#include <queue>
#include <array>
#include <cassert>

#include "Signature.hpp"

namespace ecs {

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
            std::queue<Entity> _availableEntities{};

            std::array<Signature, MAX_ENTITIES> _signatures{};

            std::uint32_t _livingEntityCount{};
    };
}
