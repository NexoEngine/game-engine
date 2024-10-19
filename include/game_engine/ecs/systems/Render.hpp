/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Render.hpp
*/

#pragma once

#include "../Coordinator.hpp"
#include "../System.hpp"
#include "../components/Render.hpp"
#include "../components/Physics.hpp"

#include <unordered_map>

namespace ecs {
    namespace system {

        /**
         * @class RenderSystem
         * @brief System responsible for rendering entities in the ECS framework.
         *
         * Inherits from the base System class. This system handles the rendering process of entities
         * within the game. It utilizes render components associated with entities to determine how
         * each entity is visually represented in the game world.
         */
        class RenderSystem : public System {
            public:
                /**
                 * @brief Renders all entities with render components.
                 *
                 * This method goes through entities that have render components and calls their respective
                 * draw methods, effectively drawing them in the game window. It ensures that each entity is
                 * rendered according to its current state and transformation.
                 */
                void render();
        };

        class RenderSystemDebug : public System {
            public:
                void render(ecs::SceneID sceneId, std::shared_ptr<engine::core::EngineCamera> engineCamera);
        };
    }
}
