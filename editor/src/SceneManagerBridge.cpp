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

    const layer::LayerStack &SceneManagerBridge::getSceneLayers(scene::SceneId sceneId) const
    {
        return getSceneManager().getSceneLayers(sceneId);
    }

    std::shared_ptr<camera::Camera> SceneManagerBridge::getCameraLayer(scene::SceneId sceneId, scene::LayerId id) const
    {
        return getSceneManager().getCameraLayer(sceneId, id);
    }

    const std::string SceneManagerBridge::getSceneName(scene::SceneId sceneId) const
    {
        return getSceneManager().getSceneName(sceneId);
    }

    std::set<ecs::Entity> SceneManagerBridge::getLayerEntities(scene::SceneId sceneId, scene::LayerId id) const
    {
        return getSceneManager().getLayerEntities(sceneId, id);
    }

    std::vector<ecs::Entity> SceneManagerBridge::getSceneEntities(const scene::SceneId sceneId) const
    {
        return std::move(getSceneManager().getAllSceneEntities(sceneId));
    }

    std::vector<ecs::Entity> SceneManagerBridge::getSceneRenderedEntities(scene::SceneId sceneId) const
    {
        return std::move(getSceneManager().getAllSceneRenderedEntities(sceneId));
    }

    std::set<ecs::Entity> SceneManagerBridge::getSceneGlobalEntities(scene::SceneId sceneId) const
    {
        return getSceneManager().getSceneGlobalEntities(sceneId);
    }

    std::vector<ecs::Entity> SceneManagerBridge::getAllEntities() const
    {
        return std::move(getSceneManager().getAllEntities());
    }

    int SceneManagerBridge::getSelectedEntity() const
    {
        return m_selectedEntity;
    }

    SelectionType SceneManagerBridge::getSelectionType() const
    {
        return m_selectionType;
    }

    void SceneManagerBridge::setSceneActiveStatus(scene::SceneId sceneId, bool status) const
    {
        getSceneManager().setSceneActiveStatus(sceneId, status);
    }

    void SceneManagerBridge::setLayerRenderStatus(scene::SceneId sceneId, scene::LayerId id, bool status)
    {
        getSceneManager().setLayerRenderStatus(sceneId, id, status);
    }

    bool SceneManagerBridge::isEntitySelected() const
    {
        return m_isEntitySelected;
    }

    void SceneManagerBridge::deactivateAllScenes() const
    {
        for (const auto sceneId : getSceneManager().getSceneIDs()) {
            getSceneManager().setSceneActiveStatus(sceneId, false);
        }
    }

    void SceneManagerBridge::setSelectedEntity(const ecs::Entity entity)
    {
        m_isEntitySelected = true;
        m_selectedEntity = entity;
    }

    void SceneManagerBridge::unselectEntity()
    {
        m_selectedEntity = -1;
        m_selectionData.emplace<std::monostate>();
        m_selectionType = SelectionType::NONE;
        m_isEntitySelected = false;
    }

    void SceneManagerBridge::renameObject(SelectionType type, VariantData &data, const std::string &newName)
    {
        if (type == SelectionType::SCENE)
        {
            if (std::holds_alternative<SceneProperties>(data))
            {
                const auto &[sceneId, _] = std::get<SceneProperties>(data);
                getSceneManager().getScene(sceneId).name = newName;
            }
        }
        else if (type == SelectionType::LAYER)
        {
            if (std::holds_alternative<LayerProperties>(data))
            {
                auto [sceneProps, layerId] = std::get<LayerProperties>(data);
                getSceneManager().setLayerName(sceneProps.sceneId, layerId, newName);
            }
        }

    }

}