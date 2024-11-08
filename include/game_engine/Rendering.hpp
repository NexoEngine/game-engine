/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** Rendering
*/

#pragma once

#include "ecs/Scene.hpp"
#include "core/Camera.hpp"

namespace engine {

    void startRendering(ecs::SceneID sceneID, core::CameraID cameraID);
    void endRendering(ecs::SceneID sceneID);

    void renderAllEntities(ecs::SceneID sceneID, core::CameraID cameraID);
    void renderGrid(ecs::SceneID sceneID, core::CameraID cameraID);
}