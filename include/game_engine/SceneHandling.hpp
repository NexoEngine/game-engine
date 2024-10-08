/*
** EPITECH PROJECT, 2024
** Nexo
** File description:
** SceneHandling
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"

namespace engine {
    /**
     * @brief Create a Scene object
     *
     * @return ecs::SceneID
     */
    ecs::SceneID createScene();

    /**
     * @brief Deletes a scene
     *
     * @param id
     */
    void deleteScene(ecs::SceneID id);

    /**
     * @brief Activates a scene
     *
     * @param id
     */
    void activateScene(ecs::SceneID id);

    /**
     * @brief Deactivate a scene
     *
     * @param id
     */
    void deactivateScene(ecs::SceneID id);

    void pauseScene(ecs::SceneID id);
    void resumeScene(ecs::SceneID id);
    bool isScenePaused(ecs::SceneID id);


    [[nodiscard]] ecs::SceneManager& getSceneManager();
}