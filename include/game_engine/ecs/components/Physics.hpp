/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.hpp
*/

#pragma once

#include "my_raylib.h"
#include "raylib_json.hpp"
#include "./Shapes.hpp"
#include "game_engine/save/ASerializableMemento.hpp"

#include <memory>
#include <save/IOriginator.hpp>

namespace ecs {
    namespace components {
        namespace physics {

            /**
             * @struct transform_t
             * @brief Represents the transformation data of an entity in the game world.
             *
             * This struct stores the position, rotation, and scale of an entity, allowing for
             * manipulation of its physical attributes in the game world.
             */
            typedef struct transform_s {
                Vector3 pos{}; ///< Position of the entity in 3D space.
                Vector3 rotation{}; ///< Rotation of the entity around the x, y, and z axes.
                Vector3 scale = {1, 1 ,1}; ///< Scale of the entity in 3D space.

                NLOHMANN_DEFINE_TYPE_INTRUSIVE(transform_s, pos, rotation, scale)
            } transform_t;

            /**
             * @struct rigidBody_t
             * @brief Represents the physical properties of an entity, allowing for dynamic movement and interaction.
             *
             * This struct stores the mass, velocity, and forces acting on an entity, along with a timestamp
             * of the last velocity update, facilitating physics-based movement and interactions.
             */
            typedef struct rigidBody_s {
                float mass; ///< Mass of the entity.
                Vector3 velocity; ///< Current velocity of the entity.
                Vector3 force; ///< Current force acting on the entity.
                double velocityLastUpdate; ///< Timestamp of the last velocity update.

                NLOHMANN_DEFINE_TYPE_INTRUSIVE(rigidBody_s, mass, velocity, force, velocityLastUpdate)
            } rigidBody_t;

            /**
             * @enum CollisionType
             * @brief Enumerates the types of collision interactions an entity can have.
             */
            enum class CollisionType {
                COLLIDE, ///< Standard collision.
                LOOTABLE, ///< Item can be collected or interacted with.
                NON_COLLIDE ///< Item does not collide with other entities.
            };

            NLOHMANN_JSON_SERIALIZE_ENUM(CollisionType,
            {
                {CollisionType::COLLIDE, "COLLIDE"},
                {CollisionType::LOOTABLE, "LOOTABLE"},
                {CollisionType::NON_COLLIDE, "NON_COLLIDE"}
            })

            /**
             * @struct collider_t
             * @brief Represents the collision boundaries and properties of an entity.
             *
             * This struct stores information about an entity's collision shape, type, and the actual
             * data describing the shape. It also includes matrices for rotation, scaling, and translation,
             * along with a bounding box and global vertices for collision detection.
             */
            typedef struct  collider_s {
                ShapeType shapeType; ///< Type of the shape used for collision.
                CollisionType collisionType; ///< Type of collision interaction.
                std::shared_ptr<IShape> data; ///< Shared pointer to the shape data.
                BoundingBox box; ///< Bounding box for collision detection.
                Vector3 vertsGlobal[8]; ///< Global vertices for the collider.
                Matrix matRotate; ///< Matrix for rotation transformations.
                Matrix matScale; ///< Matrix for scaling transformations.
                Matrix matTranslate; ///< Matrix for translation transformations.

                NLOHMANN_DEFINE_TYPE_INTRUSIVE(collider_s, shapeType, collisionType, box, vertsGlobal, matRotate, matScale, matTranslate)
            } collider_t;



        }
    }
}
