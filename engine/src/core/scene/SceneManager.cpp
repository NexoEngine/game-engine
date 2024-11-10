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
#include "core/Logger.hpp"

#include <numeric>
#include <vector>

namespace nexo::scene {

    Scene &SceneManager::getScene(SceneId sceneId)
    {
        if (!scenes.contains(sceneId)) {
            LOG(NEXO_ERROR, "SceneManager(getScene): SceneId {} does not exist", sceneId);
        }
        return scenes.at(sceneId);
    }

    void SceneManager::addLayer(SceneId sceneId, const std::shared_ptr<layer::Layer>& layer)
    {
        if (!scenes.contains(sceneId)) {
            LOG(NEXO_ERROR, "SceneManager(addLayer): SceneId {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).addLayer(layer);
    }

    void SceneManager::removeLayer(SceneId sceneId, const std::shared_ptr<layer::Layer>& layer)
    {
        if (!scenes.contains(sceneId)) {
            LOG(NEXO_ERROR, "SceneManager(removeLayer): SceneId {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).removeLayer(layer);
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

        for (const auto &[id, scene] : scenes) {
            auto sceneEntities = scene.getEntities();
            for (const auto &entity : sceneEntities) {
                entityIds.push_back(ecs::getEntityId(entity));
            }
        }
        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllActiveEntities() const
    {
        const std::size_t totalEntities = std::accumulate(scenes.begin(), scenes.end(), 0,
                                                  [](std::size_t sum, const auto &scenePair) {
                                                      return sum + scenePair.second.getEntities().size();
                                                  });

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &[id, scene] : scenes) {
            auto sceneEntities = scene.getEntities();
            for (const auto &entity : sceneEntities) {
                if (ecs::isActive(entity))
                    entityIds.push_back(ecs::getEntityId(entity));
            }
        }
        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllUnactiveEntities() const
    {
        const std::size_t totalEntities = std::accumulate(scenes.begin(), scenes.end(), 0,
                                                  [](std::size_t sum, const auto &scenePair) {
                                                      return sum + scenePair.second.getEntities().size();
                                                  });

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &[id, scene] : scenes) {
            auto sceneEntities = scene.getEntities();
            for (const auto &entity : sceneEntities) {
                if (!ecs::isActive(entity))
                    entityIds.push_back(ecs::getEntityId(entity));
            }
        }
        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllRenderedEntities() const
    {
        const std::size_t totalEntities = std::accumulate(scenes.begin(), scenes.end(), 0,
                                          [](std::size_t sum, const auto &scenePair) {
                                              return sum + scenePair.second.getEntities().size();
                                          });

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &[id, scene] : scenes) {
            auto sceneEntities = scene.getEntities();
            for (const auto &entity : sceneEntities) {
                if (ecs::isRendered(entity))
                    entityIds.push_back(ecs::getEntityId(entity));
            }
        }
        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllUnrenderedEntities() const
    {
        const std::size_t totalEntities = std::accumulate(scenes.begin(), scenes.end(), 0,
                                          [](std::size_t sum, const auto &scenePair) {
                                              return sum + scenePair.second.getEntities().size();
                                          });

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &[id, scene] : scenes) {
            auto sceneEntities = scene.getEntities();
            for (const auto &entity : sceneEntities) {
                if (!ecs::isRendered(entity))
                    entityIds.push_back(ecs::getEntityId(entity));
            }
        }
        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllEntities(const SceneId sceneId) const
    {
        if (!scenes.contains(sceneId)) {
            LOG(NEXO_ERROR, "SceneManager(getAllEntities): SceneId {} does not exist", sceneId);
            return {};
        }
        const auto sceneEntities = scenes.at(sceneId).getEntities();
        const std::size_t totalEntities = sceneEntities.size();

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &entity : sceneEntities) {
            entityIds.push_back(ecs::getEntityId(entity));
        }

        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllActiveEntities(const SceneId sceneId) const
    {
        if (!scenes.contains(sceneId)) {
            LOG(NEXO_ERROR, "SceneManager(getAllActiveEntities): SceneId {} does not exist", sceneId);
            return {};
        }
        const auto sceneEntities = scenes.at(sceneId).getEntities();
        const std::size_t totalEntities = sceneEntities.size();

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &entity : sceneEntities) {
            if (ecs::isActive(entity))
                entityIds.push_back(ecs::getEntityId(entity));
        }

        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllUnactiveEntities(const SceneId sceneId) const
    {
        if (!scenes.contains(sceneId)) {
            LOG(NEXO_ERROR, "SceneManager(getAllUnactiveEntities): SceneId {} does not exist", sceneId);
            return {};
        }
        const auto sceneEntities = scenes.at(sceneId).getEntities();
        const std::size_t totalEntities = sceneEntities.size();

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &entity : sceneEntities) {
            if (!ecs::isActive(entity))
                entityIds.push_back(ecs::getEntityId(entity));
        }

        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllRenderedEntities(const SceneId sceneId) const
    {
        if (!scenes.contains(sceneId)) {
            LOG(NEXO_ERROR, "SceneManager(getAllRenderedEntities): SceneId {} does not exist", sceneId);
            return {};
        }
        const auto sceneEntities = scenes.at(sceneId).getEntities();
        const std::size_t totalEntities = sceneEntities.size();

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &entity : sceneEntities) {
            if (ecs::isRendered(entity))
                entityIds.push_back(ecs::getEntityId(entity));
        }

        return entityIds;
    }

    std::vector<ecs::Entity> SceneManager::getAllUnrenderedEntities(const SceneId sceneId) const
    {
        if (!scenes.contains(sceneId)) {
            LOG(NEXO_ERROR, "SceneManager(getAllUnrenderedEntities): SceneId {} does not exist", sceneId);
            return {};
        }
        const auto sceneEntities = scenes.at(sceneId).getEntities();
        const std::size_t totalEntities = sceneEntities.size();

        std::vector<ecs::Entity> entityIds;
        entityIds.reserve(totalEntities);

        for (const auto &entity : sceneEntities) {
            if (!ecs::isRendered(entity))
                entityIds.push_back(ecs::getEntityId(entity));
        }

        return entityIds;
    }

    void SceneManager::addEntityToScene(const ecs::Entity entity, const SceneId sceneId, const int layerIndex, const bool active, const bool rendered)
    {
        if (!scenes.contains(sceneId)) {
            LOG(NEXO_ERROR, "SceneManager(addEntityToScene): SceneId {} does not exist", sceneId);
            return;
        }
        ecs::Entity newEntityId = entity;
        ecs::setActive(newEntityId, active);
        ecs::setRendered(newEntityId, rendered);
        scenes.at(sceneId).addEntity(newEntityId, layerIndex);
    }

    void SceneManager::removeEntityFromScene(const ecs::Entity entity, const SceneId sceneId, const int layerIndex)
    {
        if (!scenes.contains(sceneId)) {
            LOG(NEXO_ERROR, "SceneManager(removeEntityFromScene): SceneId {} does not exist", sceneId);
            return;
        }
        scenes.at(sceneId).removeEntity(entity, layerIndex);
    }
    void SceneManager::entityDestroyed(const ecs::Entity entity)
    {
        for (auto &[_, scene] : scenes)
            scene.removeEntity(entity);
    }
    void SceneManager::createScene(SceneId id, const std::string sceneName)
    {
        if (scenes.contains(id)) {
            LOG(NEXO_WARN, "SceneManager: SceneId {} already exists, skipping creation", id);
            return;
        }
        scenes.emplace(id, Scene{id, sceneName});
    }
    void SceneManager::deleteScene(const SceneId id)
    {
        if (scenes.erase(id) == 0) {
            LOG(NEXO_ERROR, "SceneManager(deleteScene): SceneId {} does not exist, cannot delete", id);
        }
    }
    void SceneManager::attachCamera(SceneId id, std::shared_ptr<nexo::camera::Camera> &camera, unsigned int layerIndex)
    {
        if (!scenes.contains(id)) {
            LOG(NEXO_ERROR, "SceneManager(attachCamera): SceneId {} does not exist", id);
            return;
        }
        scenes.at(id).attachCamera(camera, layerIndex);
    }
    void SceneManager::detachCamera(const SceneId id, const unsigned int layerIndex)
    {
        if (!scenes.contains(id)) {
            LOG(NEXO_ERROR, "SceneManager(detachCamera): SceneId {} does not exist", id);
            return;
        }
        scenes.at(id).detachCamera(layerIndex);
    }

    std::shared_ptr<camera::Camera> SceneManager::getCamera(const SceneId id, const unsigned int layerIndex)
    {
        if (!scenes.contains(id)) {
            LOG(NEXO_ERROR, "SceneManager(getCamera): SceneId {} does not exist", id);
            return nullptr;
        }
        return scenes.at(id).getCamera(layerIndex);
    }
    void SceneManager::setWindowOffset(const SceneId id, const glm::vec2 offset)
    {
        if (!scenes.contains(id)) {
            LOG(NEXO_ERROR, "SceneManager(setWindowOffset): SceneId {} does not exist", id);
            return;
        }
        scenes.at(id).setWindowOffset(offset);
    }
    glm::vec2 SceneManager::getWindowOffset(const SceneId id)
    {
        if (!scenes.contains(id)) {
            LOG(NEXO_ERROR, "SceneManager(getWindowOffset): SceneId {} does not exist", id);
            return {0, 0};
        }
        return scenes.at(id).getWindowOffset();
    }
}
