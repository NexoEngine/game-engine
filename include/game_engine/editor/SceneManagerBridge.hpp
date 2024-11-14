/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** SceneManagerBridge.hpp
*/

#pragma once
#include <ecs/Scene.hpp>

namespace engine::editor {

    class SceneManagerBridge {
        public:
            SceneManagerBridge(const SceneManagerBridge&) = delete;
            SceneManagerBridge& operator=(const SceneManagerBridge&) = delete;

            // Singleton
            static SceneManagerBridge& getInstance(ecs::SceneManager& sceneManager)
            {
                static SceneManagerBridge instance(sceneManager);
                return instance;
            }

            [[nodiscard]] std::vector<ecs::SceneID> getSceneIDs() const;
            [[nodiscard]] std::vector<ecs::Entity> getSceneEntities(ecs::SceneID sceneID) const;
            [[nodiscard]] std::set<ecs::Entity> getAllEntities() const;
            [[nodiscard]] ecs::Entity getSelectedEntity() const;
            [[nodiscard]] bool isEntitySelected() const;

            void deactivateAllScenes();



            void setSelectedEntity(ecs::Entity entity);
            void unselectEntity();

        private:
            explicit SceneManagerBridge(ecs::SceneManager& sceneManager) : _sceneManager(sceneManager) {}
            ~SceneManagerBridge() = default;

            ecs::SceneManager& _sceneManager;
            ecs::Entity _selectedEntity = 0;
            bool _isEntitySelected = false;
    };

}
