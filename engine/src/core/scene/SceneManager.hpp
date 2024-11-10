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

#include "Scene.hpp"

namespace nexo::scene {

    class SceneManager {
        public:
            [[nodiscard]] std::vector<SceneId> getSceneIDs() const;

            [[nodiscard]] std::vector<ecs::Entity> getAllEntities() const;
            [[nodiscard]] std::vector<ecs::Entity> getAllActiveEntities() const;
            [[nodiscard]] std::vector<ecs::Entity> getAllUnactiveEntities() const;
            [[nodiscard]] std::vector<ecs::Entity> getAllRenderedEntities() const;
            [[nodiscard]] std::vector<ecs::Entity> getAllUnrenderedEntities() const;
            [[nodiscard]] std::vector<ecs::Entity> getAllEntities(SceneId sceneId) const;
            [[nodiscard]] std::vector<ecs::Entity> getAllActiveEntities(SceneId sceneId) const;
            [[nodiscard]] std::vector<ecs::Entity> getAllUnactiveEntities(SceneId sceneId) const;
            [[nodiscard]] std::vector<ecs::Entity> getAllRenderedEntities(SceneId sceneId) const;
            [[nodiscard]] std::vector<ecs::Entity> getAllUnrenderedEntities(SceneId sceneId) const;

            void addEntityToScene(ecs::Entity entity, SceneId sceneId, int layerIndex = -1, bool active = true, bool rendered = true);
            template<typename DerivedLayer = void>
            void addEntityToScene(const ecs::Entity entity, const SceneId sceneId, const bool active = true, const bool rendered = true)
            {
                ecs::Entity newEntityId = entity;
                ecs::setActive(newEntityId, active);
                ecs::setRendered(newEntityId, rendered);
                scenes[sceneId].addEntity<DerivedLayer>(entity);
            }

            template<typename DerivedLayer = void>
            void removeEntityFromScene(const ecs::Entity entity, const SceneId sceneId)
            {
                scenes[sceneId].removeEntity<DerivedLayer>(entity);
            }
            void removeEntityFromScene(ecs::Entity entity, SceneId sceneId, int layerIndex = -1);

            void entityDestroyed(ecs::Entity entity);

            void createScene(SceneId id, std::string sceneName = "scene");

            void deleteScene(SceneId id);

            void setSceneRenderStatus(const SceneId sceneId, const bool status) { scenes[sceneId].setRenderStatus(status); };
            void setLayerRenderStatus(const SceneId sceneId, const bool status, const unsigned int layerIndex) { scenes[sceneId].setLayerRenderStatus(status, layerIndex); };
            template<typename DerivedLayer = void>
            void setLayerRenderStatus(const SceneId sceneId, const bool status)
            {
                scenes[sceneId].setLayerRenderStatus<DerivedLayer>(status);
            }
            bool isSceneRendered(const SceneId sceneId) { return scenes[sceneId].isRendered(); };
            bool isLayerRendered(const SceneId sceneId, const unsigned int layerIndex) { return scenes[sceneId].getLayerRenderStatus(layerIndex); };
            template<typename DerivedLayer = void>
            bool isLayerRendered(const SceneId sceneId)
            {
                return scenes[sceneId].getLayerRenderStatus<DerivedLayer>();
            }

            void setSceneActiveStatus(const SceneId sceneId, const bool status) { scenes[sceneId].setActiveStatus(status); };
            void setLayerActiveStatus(const SceneId sceneId, const bool status, const unsigned int layerIndex) { scenes[sceneId].setLayerActiveStatus(status, layerIndex); };
            template<typename DerivedLayer = void>
            void setLayerActiveStatus(const SceneId sceneId, const bool status)
            {
                scenes[sceneId].setLayerActiveStatus<DerivedLayer>(status);
            }
            bool isSceneActive(const SceneId sceneId) { return scenes[sceneId].isActive(); };
            bool isLayerActive(const SceneId sceneId, const unsigned int layerIndex) { return scenes[sceneId].getLayerActiveStatus(layerIndex); };
            template<typename DerivedLayer = void>
            bool isLayerActive(const SceneId sceneId)
            {
                return scenes[sceneId].getLayerActiveStatus<DerivedLayer>();
            }

            void attachCamera(SceneId id, std::shared_ptr<camera::Camera> &camera, unsigned int layerIndex);
            template <typename DerivedLayer = void>
            void attachCamera(const SceneId id, std::shared_ptr<camera::Camera> &camera)
            {
                scenes.at(id).attachCamera<DerivedLayer>(camera);
            }

            void detachCamera(SceneId id, unsigned int layerIndex);
            template <typename DerivedLayer = void>
            void detachCamera(const SceneId id)
            {
                scenes.at(id).detachCamera<DerivedLayer>();
            }

            std::shared_ptr<camera::Camera> getCamera(SceneId id, unsigned int layerIndex);
            template <typename DerivedLayer = void>
            std::shared_ptr<camera::Camera> getCamera(const SceneId id)
            {
                return scenes.at(id).getCamera<DerivedLayer>();
            }

            void setWindowOffset(SceneId id, glm::vec2 offset);
            glm::vec2 getWindowOffset(SceneId id);
        private:
            std::unordered_map<SceneId, Scene> scenes;
            std::set<SceneId> m_activeScenes;
    };

}

