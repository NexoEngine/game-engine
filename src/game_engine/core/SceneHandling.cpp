/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** SceneHandling
*/

#include "game_engine/GameEngine.hpp"

namespace engine {

    ecs::SceneID Engine::createScene()
    {
        static ecs::SceneID currentSceneId = 0;
        _coordinator->createScene(currentSceneId);
        return currentSceneId++;
    }

    void Engine::deleteScene(ecs::SceneID id)
    {
        _coordinator->deleteScene(id);
    }

    void Engine::activateScene(ecs::SceneID id)
    {
        _coordinator->activateScene(id);
    }

    void Engine::deactivateScene(ecs::SceneID id)
    {
        _coordinator->deactivateScene(id);
    }

    void Engine::pauseScene(ecs::SceneID id)
    {
        _coordinator->pauseScene(id);
    }

    bool Engine::isScenePaused(ecs::SceneID id)
    {
        return _coordinator->isScenePaused(id);
    }

    void Engine::resumeScene(ecs::SceneID id)
    {
        _coordinator->resumeScene(id);
    }

    ecs::SceneManager& getSceneManager()
    {
        return Engine::getInstance()->getSceneManager();
    }
}