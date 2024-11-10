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
                bool isRendered; ///< Flag indicating if the entity should be rendered.
                std::shared_ptr<components::AShape> data; ///< Shared pointer to the shape data used for rendering.

                friend void to_json(nlohmann::json &j, const render_s &render)
                {
                    j["shapeType"] = render.data->getShapeType();
                    j["isRendered"] = render.isRendered;
                    switch (render.data->getShapeType()) {
                        case ShapeType::CUBE:
                            j["data"] = *std::dynamic_pointer_cast<Cube>(render.data);
                            break;
                        case ShapeType::HEMISPHERE:
                            j["data"] = *std::dynamic_pointer_cast<Hemisphere>(render.data);
                            break;
                        case ShapeType::CONE:
                            j["data"] = *std::dynamic_pointer_cast<Cone>(render.data);
                            break;
                        case ShapeType::TORUS:
                            j["data"] = *std::dynamic_pointer_cast<Torus>(render.data);
                            break;
                        case ShapeType::KNOT:
                            j["data"] = *std::dynamic_pointer_cast<Knot>(render.data);
                            break;
                        case ShapeType::SPHERE:
                            j["data"] = *std::dynamic_pointer_cast<Sphere>(render.data);
                            break;
                        case ShapeType::CYLINDER:
                            j["data"] = *std::dynamic_pointer_cast<Cylinder>(render.data);
                            break;
                        case ShapeType::PLANE:
                            j["data"] = *std::dynamic_pointer_cast<Plane>(render.data);
                            break;
                        case ShapeType::MODEL3D:
                            j["data"] = *std::dynamic_pointer_cast<Model3D>(render.data);
                            break;
                        default:
                            break;
                    }

                }

                friend void from_json(const nlohmann::json &j, render_s &render)
                {
                    render.isRendered = j.at("isRendered").get<bool>();
                    switch (j.at("shapeType").get<ShapeType>()) {
                        case ShapeType::CUBE:
                            render.data = j.at("data").get<std::shared_ptr<Cube>>();
                        break;
                        case ShapeType::HEMISPHERE:
                            render.data = j.at("data").get<std::shared_ptr<Hemisphere>>();
                        break;
                        case ShapeType::CONE:
                            render.data = j.at("data").get<std::shared_ptr<Cone>>();
                        break;
                        case ShapeType::TORUS:
                            render.data = j.at("data").get<std::shared_ptr<Torus>>();
                        break;
                        case ShapeType::KNOT:
                            render.data = j.at("data").get<std::shared_ptr<Knot>>();
                        break;
                        case ShapeType::SPHERE:
                            render.data = j.at("data").get<std::shared_ptr<Sphere>>();
                        break;
                        case ShapeType::CYLINDER:
                            render.data = j.at("data").get<std::shared_ptr<Cylinder>>();
                        break;
                        case ShapeType::PLANE:
                            render.data = j.at("data").get<std::shared_ptr<Plane>>();
                        break;
                        case ShapeType::MODEL3D:
                            render.data = j.at("data").get<std::shared_ptr<Model3D>>();
                        break;
                        case ShapeType::SKYBOX:
                            render.data = j.at("data").get<std::shared_ptr<Skybox>>();
                        break;
                        default:
                            break;
                    }
                }
            } render_t;
        }
    }
}
