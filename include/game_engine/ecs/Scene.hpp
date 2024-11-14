/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Scene.hpp
*/

#pragma once

#include "Entity.hpp"
#include "game_engine/core/Camera.hpp"

#include <cstddef>
#include <set>
#include <unordered_map>
#include <map>
#include <iostream>

namespace ecs {
    using SceneID = std::size_t;

    class Scene {
        public:
            SceneID _id;
            std::set<ecs::Entity> entities;
            bool isPaused = false;

            Scene(SceneID id) : _id(id) {};

            void addEntity(ecs::Entity entity)
            {
                entities.insert(entity);
            }

            void removeEntity(ecs::Entity entity)
            {
                entities.erase(entity);
            }

            void attachCamera(std::shared_ptr<engine::core::EngineCamera> &camera)
            {
                _cameras.emplace(camera->getCameraID(), camera);
            }

            void detachCamera(std::shared_ptr<engine::core::EngineCamera> &camera)
            {
                _cameras.erase(camera->getCameraID());
            }

            std::shared_ptr<engine::core::EngineCamera> &getCamera(engine::core::CameraID id)
            {
                return _cameras.at(id);
            }

            void setWindowOffset(Vector2 offset) {_windowOffset = offset;};

            Vector2 getWindowOffset(void) const {return _windowOffset;};

        private:
            std::unordered_map<engine::core::CameraID, std::shared_ptr<engine::core::EngineCamera>> _cameras;
            Vector2 _windowOffset = { 0 };
    };

    class SceneManager {
        public:
            [[nodiscard]] std::vector<SceneID> getSceneIDs() const
            {
                std::vector<SceneID> ids;
                for (const auto &[id, scene] : scenes)
                    ids.push_back(id);
                return ids;
            }

            [[nodiscard]] std::vector<ecs::Entity> getSceneEntities(SceneID sceneID) const
            {
                std::vector<ecs::Entity> sceneEntities;
                for (const auto &entity : scenes.at(sceneID).entities)
                    sceneEntities.push_back(entity);
                return sceneEntities;
            }

            [[nodiscard]] std::set<ecs::Entity> getAllEntities() const
            {
                std::set<ecs::Entity> allEntities;
                for (const auto &[_, scene] : scenes)
					allEntities.insert(scene.entities.begin(), scene.entities.end());
                return allEntities;
            }

            void updateActiveEntities(void)
            {
                activeEntities.clear();
                for (const auto& sceneId : activeScenes) {
                    const auto &scene = scenes.at(sceneId);
                    activeEntities.insert(scene.entities.begin(), scene.entities.end());
                }
            }

            void addEntityToScene(ecs::Entity entity, SceneID sceneID)
            {
                scenes.at(sceneID).addEntity(entity);
                updateActiveEntities();
            }

            void removeEntityFromScene(ecs::Entity entity, SceneID sceneID)
            {
                scenes.at(sceneID).removeEntity(entity);
                updateActiveEntities();
            }

            void entityDestroyed(ecs::Entity entity)
            {
                for (auto &[id, scene] : scenes)
                    scene.removeEntity(entity);
                updateActiveEntities();
            }

            void createScene(SceneID id)
            {
                scenes.emplace(id, Scene{id});
            }

            void deleteScene(SceneID id)
            {
                scenes.erase(id);
            }

            void activateScene(SceneID sceneID)
            {
                activeScenes.insert(sceneID);
                updateActiveEntities();
            }

            void deactivateScene(SceneID sceneID)
            {
                activeScenes.erase(sceneID);
                updateActiveEntities();
            }

            bool isScenePaused(SceneID sceneID)
            {
                return scenes.at(sceneID).isPaused;
            }

            bool isSceneActive(SceneID sceneID)
            {
                return activeScenes.find(sceneID) != activeScenes.end();
            }

            void pauseScene(SceneID sceneID)
            {
                scenes.at(sceneID).isPaused = true;
            }

            void resumeScene(SceneID sceneID)
            {
                scenes.at(sceneID).isPaused = false;
            }

            const std::set<ecs::Entity> &getActiveEntities(void) const
            {
                return activeEntities;
            }

            void attachCamera(SceneID id, std::shared_ptr<engine::core::EngineCamera> &camera)
            {
                scenes.at(id).attachCamera(camera);
            }

            void detachCamera(SceneID id, std::shared_ptr<engine::core::EngineCamera> &camera)
            {
                scenes.at(id).detachCamera(camera);
            }

            std::shared_ptr<engine::core::EngineCamera> &getCamera(SceneID id, engine::core::CameraID cameraID)
            {
                return scenes.at(id).getCamera(cameraID);
            }

            void setWindowOffset(SceneID id, Vector2 offset)
            {
                scenes.at(id).setWindowOffset(offset);
            }

            Vector2 getWindowOffset(SceneID id) const
            {
                return scenes.at(id).getWindowOffset();
            }

        private:
            std::unordered_map<SceneID, Scene> scenes;
            std::set<SceneID> activeScenes;
            std::set<ecs::Entity> activeEntities;
    };
}