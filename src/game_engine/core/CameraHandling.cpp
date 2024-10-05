/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** CameraHandling
*/

#include "game_engine/GameEngine.hpp"

namespace engine {

    std::shared_ptr<engine::core::EngineCamera> Engine::createCamera(Vector3 pos, Vector3 target, Vector3 up, int mode, float fov)
    {
        auto newCamera = std::make_shared<engine::core::EngineCamera>(_nextId, pos, target, up, mode, fov);
        _nextId++;
        return newCamera;
    }

    std::shared_ptr<engine::core::EngineCamera> Engine::createMovableCamera(Vector3 pos = {0, 0, 0}, Vector3 target = {0, 0, 0}, Vector3 up = {0, 1, 0}, int mode = CAMERA_PERSPECTIVE, float fov = 90.0f)
    {
        auto newCamera = std::make_shared<engine::core::MovableCamera>(_nextId, pos, target, up, mode, fov);
        _nextId++;
        return newCamera;
    }

    void Engine::attachCamera(ecs::SceneID sceneID, std::shared_ptr<engine::core::EngineCamera> camera)
    {
        _coordinator->attachCamera(sceneID, camera);
    }

    void Engine::detachCamera(ecs::SceneID sceneID, std::shared_ptr<engine::core::EngineCamera> camera)
    {
        _coordinator->detachCamera(sceneID, camera);
    }
}