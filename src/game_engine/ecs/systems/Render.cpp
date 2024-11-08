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
#include "EventHandling.hpp"
#include "SceneHandling.hpp"
#include "Math.hpp"
#include "raylib.h"
#include "rlgl.h"

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

        void RenderSystemDebug::render(ecs::SceneID sceneId, std::shared_ptr<engine::core::EngineCamera> engineCamera) {
            for (auto const &entity : _entities) {
                auto &render = _coord->getComponent<components::render::render_t>(entity);
                if (render.isRendered) {
                    render.data->drawBoundingBox();
                }
            }
        }
    }
}
