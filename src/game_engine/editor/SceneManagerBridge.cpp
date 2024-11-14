/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** SceneManagerBridge.cpp
*/

#include "game_engine/editor/SceneManagerBridge.hpp"

std::vector<ecs::SceneID> engine::editor::SceneManagerBridge::getSceneIDs() const
{
    return std::move(_sceneManager.getSceneIDs());
}

std::vector<ecs::Entity> engine::editor::SceneManagerBridge::getSceneEntities(ecs::SceneID sceneID) const
{
    return std::move(_sceneManager.getSceneEntities(sceneID));
}

std::set<ecs::Entity> engine::editor::SceneManagerBridge::getAllEntities() const
{
    return std::move(_sceneManager.getAllEntities());
}

ecs::Entity engine::editor::SceneManagerBridge::getSelectedEntity() const
{
    return _selectedEntity;
}

bool engine::editor::SceneManagerBridge::isEntitySelected() const
{
    return _isEntitySelected;
}

void engine::editor::SceneManagerBridge::deactivateAllScenes()
{
    for (auto sceneID : _sceneManager.getSceneIDs()) {
        _sceneManager.deactivateScene(sceneID);
    }
}

void engine::editor::SceneManagerBridge::setSelectedEntity(ecs::Entity entity)
{
    _isEntitySelected = true;
    _selectedEntity = entity;
}

void engine::editor::SceneManagerBridge::unselectEntity()
{
    _isEntitySelected = false;
}
