/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Render.cpp
*/

#include "game_engine/ecs/components/Render.hpp"
#include "game_engine/ecs/systems/Render.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Shader.hpp"

extern ecs::Coordinator gCoordinator;

namespace ecs {
    namespace system {
        void RenderSystem::render() {
            for (auto const &entity : _entities) {
                auto& transf = _coord->getComponent<components::physics::transform_t>(entity);
                auto& render = _coord->getComponent<components::render::render_t>(entity);
                auto& collision = _coord->getComponent<components::physics::collider_t>(entity);

                if (render.isRendered) {
                    auto model = render.data->getModel();
                    model.materials[0].shader = ecs::components::shader::defaultLightingShader;

                    render.data->draw(transf);

                }
            }
        }
    }
}
