//// SceneManager.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the scene manager class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <utility>

#include "Scene.hpp"
#include "ecs/Coordinator.hpp"

namespace nexo::scene {
    using SceneId = unsigned int;

    class SceneManager {
        public:
            void setCoordinator(std::shared_ptr<ecs::Coordinator> coordinator) { m_coordinator = std::move(coordinator); };
            [[nodiscard]] std::vector<SceneId> getSceneIDs() const;

            [[nodiscard]] std::vector<ecs::Entity> getAllEntities() const;
            [[nodiscard]] std::vector<ecs::Entity> getAllActiveEntities() const;
            [[nodiscard]] std::vector<ecs::Entity> getAllSceneEntities(SceneId sceneId) const;
            std::set<ecs::Entity> getSceneGlobalEntities(SceneId sceneId) const;
            std::set<ecs::Entity> getLayerEntities(SceneId sceneId, const std::string &layerName) const;

            Scene &getScene(SceneId sceneId);
            const std::string &getSceneName(SceneId sceneId) const;

            void addLayer(SceneId sceneId, const std::string &layerName);
            void removeLayer(SceneId sceneId, const std::string &layerName);
            void addOverlay(SceneId sceneId, const std::string &overlayName);
            void removeOverlay(SceneId sceneId, const std::string &overlayName);
            const layer::LayerStack &getSceneLayers(SceneId sceneId) const;
            void setLayerName(SceneId sceneId, const std::string &layerName, const std::string &newName) const;

            void addEntityToLayer(ecs::Entity entity, SceneId sceneId, const std::string& name);
            void addGlobalEntity(ecs::Entity entity, SceneId sceneId);

            void removeEntityFromLayer(ecs::Entity entity, SceneId sceneId, const std::string& name);
            void removeGlobalEntity(ecs::Entity entity, SceneId sceneId);

            void entityDestroyed(ecs::Entity entity);

            SceneId createScene(const std::string& sceneName, bool active = true);
            void deleteScene(SceneId id);

            void setSceneRenderStatus(const SceneId sceneId, const bool status) { scenes.at(sceneId).isRendered = status;; };
            void setLayerRenderStatus(const SceneId sceneId, const std::string &layerName, const bool status) {scenes[sceneId].setLayerRenderStatus(status, layerName); };
            bool isSceneRendered(const SceneId sceneId) { return scenes[sceneId].isRendered; };
            bool isLayerRendered(const SceneId sceneId, const std::string &layerName) {return scenes[sceneId].getLayerRenderStatus(layerName); };

            void setSceneActiveStatus(const SceneId sceneId, const bool status) { scenes[sceneId].isActive = status; };
            void setLayerActiveStatus(const SceneId sceneId, const std::string &layerName, const bool status) { scenes[sceneId].setLayerActiveStatus(status, layerName); };
            bool isSceneActive(const SceneId sceneId) { return scenes[sceneId].isActive; };
            bool isLayerActive(const SceneId sceneId, const std::string &layerName) {return scenes[sceneId].getLayerActiveStatus(layerName); };

            void attachCameraToLayer(SceneId id, const std::shared_ptr<camera::Camera> &camera, const std::string &layerName);
            void detachCameraFromLayer(SceneId id, const std::string &layerName);
            std::shared_ptr<camera::Camera> getCameraLayer(SceneId id, const std::string &layerName);

            void setWindowOffset(SceneId id, glm::vec2 offset);
            glm::vec2 getWindowOffset(SceneId id);
        private:
            std::shared_ptr<ecs::Coordinator> m_coordinator;
            std::unordered_map<SceneId, Scene> scenes;
            std::set<SceneId> m_activeScenes;

            SceneId m_nextSceneId = 0;
    };

}

