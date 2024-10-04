/*
** EPITECH PROJECT, 2024
** Nexo
** File description:
** CameraHandling
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"

namespace engine {
    /**
     * @brief Create a Camera object
     *
     * @param pos
     * @param target
     * @param up
     * @param mode
     * @param fov
     * @return engine::core::EngineCamera
     */
    engine::core::EngineCamera createCamera(Vector3 pos = {0, 0, 0}, Vector3 target = {0, 0, 0}, Vector3 up = {0, 1, 0}, int mode = CAMERA_PERSPECTIVE, float fov = 90.0f);

    /**
     * @brief Attach a camera to a scene
     *
     * @param sceneID
     * @param camera
     */
    void attachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera);

    /**
     * @brief Detach a camera from a scene
     *
     * @param sceneID
     * @param camera
     */
    void detachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera);
}