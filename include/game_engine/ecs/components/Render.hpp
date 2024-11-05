/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Render.hpp
*/

#pragma once

#include "my_raylib.h"
#include "Shapes.hpp"

#include <memory>

namespace ecs {
    namespace components {
        namespace render {

            /**
             * @struct render_t
             * @brief Represents the rendering information for an entity in the game.
             *
             * This struct is used to manage the rendering aspects of an entity within the ECS system. It contains
             * the shape type of the entity, a flag indicating whether it should be rendered, and a shared pointer
             * to the actual shape data used for rendering.
             */
            typedef struct render_s {
                ShapeType type; ///< The type of the shape to be rendered.
                bool isRendered; ///< Flag indicating if the entity should be rendered.
                std::shared_ptr<components::IShape> data; ///< Shared pointer to the shape data used for rendering.

                // TODO: this serialization is incomplete, just temporary for tests
                //NLOHMANN_DEFINE_TYPE_INTRUSIVE(render_s, type, isRendered)
            } render_t;

        }
    }
}
