//// SceneViewManager.hpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        16/11/2024
//  Description: Header file for the scene view manager
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "MainScene.hpp"

#include <map>
#include <string>
#include <imgui_internal.h>

namespace nexo::editor {
    class SceneViewManager : public ADocumentWindow {
        public:
            static std::shared_ptr<SceneViewManager>& getInstance()
            {
                if (!m_instance)
                    m_instance = std::make_shared<SceneViewManager>();
                return m_instance;
            }

            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;
            void addNewScene(const std::string &sceneName, const std::shared_ptr<MainScene>& scene = nullptr);
            void duplicateSceneView(WindowId uiId);
            void removeScene(WindowId uiId);
            std::shared_ptr<MainScene> getScene(const WindowId uiId) {return m_scenes.at(uiId); };

            void hideLayer(WindowId uiId, scene::LayerId layerId) const;
            void showLayer(WindowId uiId, scene::LayerId layerId) const;
            [[nodiscard]] bool isLayerHidden(WindowId uiId, scene::LayerId layerId) const;
            void addDefaultCameraToLayer(WindowId uiId, scene::LayerId layerId) const;

            std::vector<SceneProperties> &getOpenScenes() { return m_openScenes; };

            [[nodiscard]] int getSelectedScene() const { return selectedScene; };
            void setSelectedScene(const scene::SceneId scene) {selectedScene = static_cast<int>(scene);};
            [[nodiscard]] const std::string &getSceneName(WindowId uiId) const;
        private:
            std::vector<SceneProperties> m_openScenes;

            static std::shared_ptr<SceneViewManager> m_instance;
            std::map<WindowId, std::shared_ptr<MainScene>> m_scenes;
            int selectedScene = -1;

            static ImGuiDockNode* getDockNodeForWindow(const char* windowName);
    };
}
