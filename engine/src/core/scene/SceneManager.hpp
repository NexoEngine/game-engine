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
    using LayerId = unsigned int;

    class SceneManager {
        public:
            void setCoordinator(std::shared_ptr<ecs::Coordinator> coordinator) { m_coordinator = std::move(coordinator); };
            [[nodiscard]] std::vector<SceneId> getSceneIDs() const;

            [[nodiscard]] std::vector<ecs::Entity> getAllEntities() const;
            [[nodiscard]] std::vector<ecs::Entity> getAllActiveEntities() const;
            [[nodiscard]] std::vector<ecs::Entity> getAllSceneEntities(SceneId sceneId) const;
            [[nodiscard]] std::vector<ecs::Entity> getAllSceneRenderedEntities(SceneId sceneId) const;
            std::set<ecs::Entity> getSceneGlobalEntities(SceneId sceneId) const;
            std::set<ecs::Entity> getLayerEntities(SceneId sceneId, LayerId id) const;

            Scene &getScene(SceneId sceneId);
            const std::string &getSceneName(SceneId sceneId) const;

            LayerId addLayer(SceneId sceneId, const std::string &layerName = "Default layer");
            void removeLayer(SceneId sceneId, LayerId id);
            LayerId addOverlay(SceneId sceneId, const std::string &overlayName = "Default overlay");
            void removeOverlay(SceneId sceneId, LayerId id);
            const layer::LayerStack &getSceneLayers(SceneId sceneId) const;
            void setLayerName(SceneId sceneId, LayerId id, const std::string_view &newName) const;

            void addEntityToLayer(ecs::Entity entity, SceneId sceneId, LayerId id);
            void addGlobalEntity(ecs::Entity entity, SceneId sceneId);

            void removeEntityFromLayer(ecs::Entity entity, SceneId sceneId, LayerId id);
            void removeGlobalEntity(ecs::Entity entity, SceneId sceneId);

            void entityDestroyed(ecs::Entity entity);

            SceneId createScene(const std::string& sceneName, bool active = true);
            void deleteScene(SceneId id);

            void setSceneRenderStatus(const SceneId sceneId, const bool status) { scenes.at(sceneId).isRendered = status;; };
            void setLayerRenderStatus(const SceneId sceneId, const LayerId id, const bool status) {scenes[sceneId].setLayerRenderStatus(status, id); };
            bool isSceneRendered(const SceneId sceneId) { return scenes[sceneId].isRendered; };
            bool isLayerRendered(const SceneId sceneId, const LayerId id) {return scenes[sceneId].getLayerRenderStatus(id); };

            void setSceneActiveStatus(const SceneId sceneId, const bool status) { scenes[sceneId].isActive = status; };
            void setLayerActiveStatus(const SceneId sceneId, const LayerId id, const bool status) { scenes[sceneId].setLayerActiveStatus(status, id); };
            bool isSceneActive(const SceneId sceneId) { return scenes[sceneId].isActive; };
            bool isLayerActive(const SceneId sceneId, const LayerId id) {return scenes[sceneId].getLayerActiveStatus(id); };

            void attachCameraToLayer(SceneId sceneId, const std::shared_ptr<camera::Camera> &camera, LayerId id);
            void detachCameraFromLayer(SceneId sceneId, LayerId id);
            std::shared_ptr<camera::Camera> getCameraLayer(SceneId sceneId, LayerId id);

            unsigned int addLightToScene(SceneId id, const std::shared_ptr<components::Light> &light);
            void removeLightFromScene(SceneId id, unsigned int index);
            void setSceneAmbientLightValue(SceneId id, float value);
            float getSceneAmbientLightValue(SceneId id);

            void setWindowOffset(SceneId id, glm::vec2 offset);
            glm::vec2 getWindowOffset(SceneId id);
        private:
            std::shared_ptr<ecs::Coordinator> m_coordinator;
            std::unordered_map<SceneId, Scene> scenes;
            std::set<SceneId> m_activeScenes;

            SceneId m_nextSceneId = 0;
            LayerId m_nextLayerId = 0;
    };

}

