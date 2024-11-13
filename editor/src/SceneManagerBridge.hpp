//// SceneManagerBridge.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the scene manager bridge class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include "core/scene/SceneManager.hpp"

namespace nexo::editor {
    class SceneManagerBridge {
        public:
            SceneManagerBridge(const SceneManagerBridge&) = delete;
            SceneManagerBridge& operator=(const SceneManagerBridge&) = delete;

            // Singleton
            static SceneManagerBridge& getInstance(scene::SceneManager& sceneManager)
            {
                static SceneManagerBridge instance(sceneManager);
                return instance;
            }

            [[nodiscard]] std::vector<scene::SceneId> getSceneIDs() const;
            [[nodiscard]] const layer::LayerStack &getSceneLayers(scene::SceneId sceneId) const;

            [[nodiscard]] const std::string &getSceneName(scene::SceneId sceneId) const;
            [[nodiscard]] std::set<ecs::Entity> getLayerEntities(scene::SceneId sceneId, const std::string &layerName) const;
            [[nodiscard]] std::vector<ecs::Entity> getSceneEntities(scene::SceneId sceneId) const;
            [[nodiscard]] std::vector<ecs::Entity> getAllEntities() const;
            [[nodiscard]] int getSelectedEntity() const;
            void setSceneActiveStatus(scene::SceneId sceneId, bool status) const;
            [[nodiscard]] bool isEntitySelected() const;

            void deactivateAllScenes() const;

            void setSelectedEntity(ecs::Entity entity);
            void unselectEntity();

            private:
            explicit SceneManagerBridge(scene::SceneManager& sceneManager) : m_sceneManager(sceneManager) {}
            ~SceneManagerBridge() = default;

            scene::SceneManager& m_sceneManager;
            int m_selectedEntity = -1;
            bool m_isEntitySelected = false;
    };
}