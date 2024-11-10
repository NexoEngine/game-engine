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

#include <numeric>
#include <vector>

namespace nexo::scene {
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
        ecs::Entity newEntityId = entity;
        ecs::setActive(newEntityId, active);
        ecs::setRendered(newEntityId, rendered);
        scenes.at(sceneId).addEntity(newEntityId, layerIndex);
    }

    void SceneManager::removeEntityFromScene(const ecs::Entity entity, const SceneId sceneId, const int layerIndex)
    {
        scenes.at(sceneId).removeEntity(entity, layerIndex);
    }
    void SceneManager::entityDestroyed(const ecs::Entity entity)
    {
        for (auto &[_, scene] : scenes)
            scene.removeEntity(entity);
    }
    void SceneManager::createScene(SceneId id, const std::string sceneName)
    {
        scenes.emplace(id, Scene{id, sceneName});
    }
    void SceneManager::deleteScene(const SceneId id)
    {
        scenes.erase(id);
    }
    void SceneManager::attachCamera(SceneId id, std::shared_ptr<nexo::camera::Camera> &camera, unsigned int layerIndex)
    {
        scenes.at(id).attachCamera(camera, layerIndex);
    }
    void SceneManager::detachCamera(const SceneId id, const unsigned int layerIndex)
    {
        scenes.at(id).detachCamera(layerIndex);
    }

    std::shared_ptr<camera::Camera> SceneManager::getCamera(const SceneId id, const unsigned int layerIndex)
    {
        return scenes.at(id).getCamera(layerIndex);
    }
    void SceneManager::setWindowOffset(const SceneId id, const glm::vec2 offset)
    {
        scenes.at(id).setWindowOffset(offset);
    }
    glm::vec2 SceneManager::getWindowOffset(const SceneId id)
    {
        return scenes.at(id).getWindowOffset();
    }
}
