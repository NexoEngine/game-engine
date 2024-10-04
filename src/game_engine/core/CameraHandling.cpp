/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** CameraHandling
*/

#include "game_engine/GameEngine.hpp"

namespace engine {

    engine::core::EngineCamera Engine::createCamera(Vector3 pos, Vector3 target, Vector3 up, int mode, float fov)
    {
        engine::core::EngineCamera newCamera(_nextId, pos, target, up, mode, fov);
        _nextId++;
        return newCamera;
    }

    void Engine::attachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera)
    {
        _coordinator->attachCamera(sceneID, camera);
    }

    void Engine::detachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera)
    {
        _coordinator->detachCamera(sceneID, camera);
    }
}