//// SceneManagerBridge.cpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Source file for the scene manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneManagerBridge.hpp"

namespace nexo::editor {

    const layer::LayerStack &SceneManagerBridge::getSceneLayers(const scene::SceneId sceneId) const
    {
        return getSceneManager().getSceneLayers(sceneId);
    }

    std::shared_ptr<camera::Camera> SceneManagerBridge::getCameraLayer(const scene::SceneId sceneId, const scene::LayerId id) const
    {
        return getSceneManager().getCameraLayer(sceneId, id);
    }

    std::string SceneManagerBridge::getSceneName(const scene::SceneId sceneId) const
    {
        return getSceneManager().getSceneName(sceneId);
    }

    std::set<ecs::Entity> SceneManagerBridge::getLayerEntities(const scene::SceneId sceneId, const scene::LayerId id) const
    {
        return getSceneManager().getLayerEntities(sceneId, id);
    }

    std::vector<ecs::Entity> SceneManagerBridge::getSceneEntities(const scene::SceneId sceneId) const
    {
        return getSceneManager().getAllSceneEntities(sceneId);
    }

    std::vector<ecs::Entity> SceneManagerBridge::getSceneRenderedEntities(const scene::SceneId sceneId) const
    {
        return getSceneManager().getAllSceneRenderedEntities(sceneId);
    }

    std::set<ecs::Entity> SceneManagerBridge::getSceneGlobalEntities(const scene::SceneId sceneId) const
    {
        return getSceneManager().getSceneGlobalEntities(sceneId);
    }

    std::vector<ecs::Entity> SceneManagerBridge::getAllEntities() const
    {
        return getSceneManager().getAllEntities();
    }

    int SceneManagerBridge::getSelectedEntity() const
    {
        return m_selectedEntity;
    }

    SelectionType SceneManagerBridge::getSelectionType() const
    {
        return m_selectionType;
    }

    void SceneManagerBridge::setSceneActiveStatus(const scene::SceneId sceneId, const bool status) const
    {
        getSceneManager().setSceneActiveStatus(sceneId, status);
    }

    void SceneManagerBridge::setLayerRenderStatus(const scene::SceneId sceneId, const scene::LayerId id, const bool status) const
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
        m_selectedEntity = static_cast<int>(entity);
    }

    void SceneManagerBridge::unselectEntity()
    {
        m_selectedEntity = -1;
        m_selectionData.emplace<std::monostate>();
        m_selectionType = SelectionType::NONE;
        m_isEntitySelected = false;
    }

    void SceneManagerBridge::renameObject(const SelectionType type, VariantData &data, const std::string &newName) const
    {
        if (type == SelectionType::SCENE && std::holds_alternative<SceneProperties>(data))
        {
            const auto &[sceneId, _] = std::get<SceneProperties>(data);
            getSceneManager().getScene(sceneId).name = newName;
        }
        else if (type == SelectionType::LAYER && std::holds_alternative<LayerProperties>(data))
        {
            auto [sceneProps, layerId] = std::get<LayerProperties>(data);
            getSceneManager().setLayerName(sceneProps.sceneId, layerId, newName);
        }
    }

}
