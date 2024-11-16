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
            void duplicateSceneView(const std::string &sceneName, const std::string &fromScene, const std::shared_ptr<MainScene>& scene);
            void removeScene(const std::string &sceneName);

            std::vector<SceneProperties> &getOpenScenes() { return m_openScenes; };

            int getSelectedScene() const { return selectedScene; };
            void setSelectedScene(scene::SceneId scene) {selectedScene = scene;};
        private:
            std::vector<SceneProperties> m_openScenes;

            static std::shared_ptr<SceneViewManager> m_instance;
            std::map<std::string, std::shared_ptr<MainScene>> m_scenes;
            int selectedScene = -1;

            ImGuiDockNode* getDockNodeForWindow(const char* windowName);
    };
}