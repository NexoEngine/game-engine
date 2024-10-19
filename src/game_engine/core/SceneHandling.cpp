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

    ecs::SceneID createScene()
    {
        return Engine::getInstance()->createScene();
    }

    void Engine::deleteScene(ecs::SceneID id)
    {
        _coordinator->deleteScene(id);
    }

    void deleteScene(ecs::SceneID id)
    {
        Engine::getInstance()->deleteScene(id);
    }

    void Engine::activateScene(ecs::SceneID id)
    {
        _coordinator->activateScene(id);
    }

    void activateScene(ecs::SceneID id)
    {
        Engine::getInstance()->activateScene(id);
    }

    void Engine::deactivateScene(ecs::SceneID id)
    {
        _coordinator->deactivateScene(id);
    }

    void deactivateScene(ecs::SceneID id)
    {
        Engine::getInstance()->deactivateScene(id);
    }

    void Engine::pauseScene(ecs::SceneID id)
    {
        _coordinator->pauseScene(id);
    }

    void pauseScene(ecs::SceneID id)
    {
        Engine::getInstance()->pauseScene(id);
    }

    bool Engine::isScenePaused(ecs::SceneID id)
    {
        return _coordinator->isScenePaused(id);
    }

    bool isScenePaused(ecs::SceneID id)
    {
        return Engine::getInstance()->isScenePaused(id);
    }

    void Engine::resumeScene(ecs::SceneID id)
    {
        _coordinator->resumeScene(id);
    }

    void resumeScene(ecs::SceneID id)
    {
        Engine::getInstance()->resumeScene(id);
    }

    ecs::SceneManager& getSceneManager()
    {
        return Engine::getInstance()->getSceneManager();
    }

    void Engine::setSceneWindowOffset(ecs::SceneID id, Vector2 offset)
    {
        _coordinator->setSceneWindowOffset(id, offset);
    }

    [[nodiscard]] Vector2 Engine::getSceneWindowOffset(ecs::SceneID id) const
    {
        return _coordinator->getSceneWindowOffset(id);
    }

    void setSceneWindowOffset(ecs::SceneID id, Vector2 offset)
    {
        Engine::getInstance()->setSceneWindowOffset(id, offset);
    }

    [[nodiscard]] Vector2 getSceneWindowOffset(ecs::SceneID id)
    {
        return Engine::getInstance()->getSceneWindowOffset(id);
    }
}