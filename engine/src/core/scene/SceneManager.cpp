//// SceneManager.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the scene manager class
//
///////////////////////////////////////////////////////////////////////////////
#include "SceneManager.hpp"
#include "Logger.hpp"
#include "core/layer/Layer.hpp"

#include <numeric>
#include <vector>

namespace nexo::scene {

    Scene &SceneManager::getScene(SceneId sceneId)
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::getScene: id {} does not exist", sceneId);
            static Scene defaultScene;
            return defaultScene;
        }
        return scenes.at(sceneId);
    }

    const std::string &SceneManager::getSceneName(SceneId sceneId) const
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::getSceneName: id {} does not exist", sceneId);
            return "";
        }
        return scenes.at(sceneId).getName();
    }

    std::set<ecs::Entity> SceneManager::getLayerEntities(SceneId sceneId, const std::string &layerName) const
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::getLayerEntities: id {} does not exist", sceneId);
            return {};
        }
        auto layer = scenes.at(sceneId).getLayer(layerName);
        return layer->getEntities();
    }

    void SceneManager::addLayer(SceneId sceneId, const std::string &layerName)
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::addLayer: id {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).addLayer(layerName);
    }

    void SceneManager::removeLayer(SceneId sceneId, const std::string &layerName)
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::removeLayer: id {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).removeLayer(layerName);
    }

    void SceneManager::addOverlay(SceneId sceneId, const std::string &overlayName)
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::addOverlay: id {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).addOverlay(overlayName);
    }

    void SceneManager::removeOverlay(SceneId sceneId, const std::string &overlayName)
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::removeOverlay: id {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).removeOverlay(overlayName);
    }

    const layer::LayerStack &SceneManager::getSceneLayers(SceneId sceneId) const
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::getSceneLayers: id {} does not exist", sceneId);
            return {};
        }
        return scenes.at(sceneId).getLayerStack();
    }

    void SceneManager::setLayerName(SceneId sceneId, const std::string &layerName, const std::string &newName) const
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::setLayerName: id {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).getLayer(layerName)->name = newName;
    }

    std::vector<SceneId> SceneManager::getSceneIDs() const
    {
        const std::size_t totalScenes = scenes.size();

        std::vector<SceneId> ids;
        ids.reserve(totalScenes);

        for (const auto &[id, scene]: scenes)
            ids.push_back(id);
        return ids;
    }

    std::vector<ecs::Entity> SceneManager::getAllEntities() const
    {
        const std::size_t totalEntities = std::accumulate(scenes.begin(), scenes.end(), 0,
                                                          [](std::size_t sum, const auto &scenePair) {
                                                              return sum + scenePair.second.getEntities().size();
                                                          });

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &[id, scene]: scenes)
        {
            for (auto sceneEntities = scene.getEntities(); const auto &entity: sceneEntities)
            {
                entityIds.push_back(entity);
            }
        }
        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllActiveEntities() const
    {
        const std::size_t totalEntities = std::accumulate(m_activeScenes.begin(), m_activeScenes.end(), 0,
                                                          [this](std::size_t sum, const auto &sceneId) {
                                                              return sum + scenes.at(sceneId).getEntities().size();
                                                          });

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &sceneId : m_activeScenes) {
            const auto &sceneEntities = scenes.at(sceneId).getEntities();
            entityIds.insert(entityIds.end(), sceneEntities.begin(), sceneEntities.end());
        }
        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllSceneEntities(const SceneId sceneId) const
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::getAllSceneEntities: id {} does not exist", sceneId);
            return {};
        }
        const auto sceneEntities = scenes.at(sceneId).getEntities();
        const std::size_t totalEntities = sceneEntities.size();

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &entity: sceneEntities)
            entityIds.push_back(entity);

        return entityIds;
    }

    std::set<ecs::Entity> SceneManager::getSceneGlobalEntities(SceneId sceneId) const
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::getAllSceneGlobalEntity: id {} does not exist", sceneId);
            return {};
        }
        return scenes.at(sceneId).getGlobalEntities();
    }

    void SceneManager::addEntityToLayer(const ecs::Entity entity, SceneId sceneId, const std::string &name)
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::addEntityToLayerByName: id {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).addEntityToLayer(entity, name);
    }

    void SceneManager::addGlobalEntity(const ecs::Entity entity, SceneId sceneId)
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager:::addGlobalEntity: id {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).addGlobalEntity(entity);
    }

    void SceneManager::removeEntityFromLayer(const ecs::Entity entity, SceneId sceneId, const std::string &name)
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager::removeEntityFromLayerByName: id {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).removeEntityFromLayer(entity, name);
    }

    void SceneManager::removeGlobalEntity(const ecs::Entity entity, SceneId sceneId)
    {
        if (!scenes.contains(sceneId))
        {
            LOG(NEXO_ERROR, "SceneManager::removeEntityFromScene: id {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).removeGlobalEntity(entity);
    }

    void SceneManager::entityDestroyed(const ecs::Entity entity)
    {
        for (auto &[_, scene]: scenes)
            scene.entityDestroyed(entity);
    }

    SceneId SceneManager::createScene(const std::string &sceneName, bool active)
    {
        auto sceneId = m_nextSceneId++;
        Scene newScene(sceneId, sceneName);
        scenes.emplace(sceneId, newScene);
        if (active)
            m_activeScenes.insert(sceneId);
        return sceneId;
    }

    void SceneManager::deleteScene(const SceneId id)
    {
        if (scenes.erase(id) == 0)
        {
            LOG(NEXO_ERROR, "SceneManager::deleteScene: id {} does not exist, cannot delete", id);
            return;
        }
        if (const auto it = m_activeScenes.find(id); it != m_activeScenes.end())
            m_activeScenes.erase(it);
    }

    void SceneManager::attachCameraToLayer(SceneId id, const std::shared_ptr<camera::Camera> &camera,
                                           const std::string &layerName)
    {
        if (!scenes.contains(id))
        {
            LOG(NEXO_ERROR, "SceneManager::attachCameraToLayer: id {} does not exist", id);
            return;
        }
        scenes.at(id).attachCameraToLayer(camera, layerName);
    }

    void SceneManager::detachCameraFromLayer(SceneId id, const std::string &layerName)
    {
        if (!scenes.contains(id))
        {
            LOG(NEXO_ERROR, "SceneManager::detachCameraFromLayer: id {} does not exist", id);
            return;
        }
        scenes.at(id).detachCameraFromLayer(layerName);
    }

    std::shared_ptr<camera::Camera> SceneManager::getCameraLayer(SceneId id, const std::string &layerName)
    {
        if (!scenes.contains(id))
        {
            LOG(NEXO_ERROR, "SceneManager::getCameraLayer: id {} does not exist", id);
            return nullptr;
        }
        return scenes.at(id).getCameraLayer(layerName);
    }

    void SceneManager::setWindowOffset(const SceneId id, const glm::vec2 offset)
    {
        if (!scenes.contains(id))
        {
            LOG(NEXO_ERROR, "SceneManager::setWindowOffset: id {} does not exist", id);
            return;
        }
        scenes.at(id).setWindowOffset(offset);
    }

    glm::vec2 SceneManager::getWindowOffset(const SceneId id)
    {
        if (!scenes.contains(id))
        {
            LOG(NEXO_ERROR, "SceneManager::getWindowOffset: id {} does not exist", id);
            return {0, 0};
        }
        return scenes.at(id).getWindowOffset();
    }
}
