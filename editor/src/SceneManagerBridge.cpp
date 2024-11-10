//// SceneManagerBridge.cpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source file for the scene manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneManagerBridge.hpp"

namespace nexo::editor {
    std::vector<scene::SceneId> SceneManagerBridge::getSceneIDs() const
    {
        return std::move(m_sceneManager.getSceneIDs());
    }

    std::vector<ecs::Entity> SceneManagerBridge::getSceneEntities(const scene::SceneId sceneId) const
    {
        return std::move(m_sceneManager.getAllEntities(sceneId));
    }

    std::vector<ecs::Entity> SceneManagerBridge::getAllEntities() const
    {
        return std::move(m_sceneManager.getAllEntities());
    }

    ecs::Entity SceneManagerBridge::getSelectedEntity() const
    {
        return m_selectedEntity;
    }

    bool SceneManagerBridge::isEntitySelected() const
    {
        return m_isEntitySelected;
    }

    void SceneManagerBridge::deactivateAllScenes() const
    {
        for (const auto sceneId : m_sceneManager.getSceneIDs()) {
            m_sceneManager.setSceneActiveStatus(sceneId, false);
        }
    }

    void SceneManagerBridge::setSelectedEntity(const ecs::Entity entity)
    {
        m_isEntitySelected = true;
        m_selectedEntity = entity;
    }

    void SceneManagerBridge::unselectEntity()
    {
        m_isEntitySelected = false;
    }
}