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

#include "ADocumentWindow.hpp"
#include "MainScene.hpp"
#include "SceneTreeWindow.hpp"

#include <map>
#include <string>
#include <imgui_internal.h>

namespace nexo::editor {
	/**
	* @class SceneViewManager
	* @brief Singleton class managing scene views in the editor
	*
	* This class handles the creation, duplication, and management of scene views
	* in the editor interface. It extends ADocumentWindow to integrate with the
	* document window system.
	*/
    class SceneViewManager : public ADocumentWindow {
        public:
       		using ADocumentWindow::ADocumentWindow;
            void setup() override;
            void shutdown() override;
            void show() override;
            void update() override;
            bool addNewScene(const std::string &sceneName, bool defaultScene = false);
            void duplicateSceneView(WindowId uiId);
            void removeScene(WindowId uiId);
            /**
 * @brief Retrieves the MainScene associated with the given window identifier.
 *
 * This function accesses the internal scene mapping and returns the shared pointer
 * to the MainScene linked to the provided window identifier. Since it uses std::map::at,
 * a std::out_of_range exception is thrown if the identifier does not exist.
 *
 * @param uiId The unique identifier of the window whose scene is requested.
 * @return std::shared_ptr<MainScene> A shared pointer to the corresponding MainScene.
 *
 * @throws std::out_of_range If the window identifier is not found in the internal map.
 */
std::shared_ptr<MainScene> getScene(const WindowId uiId) {return m_scenes.at(uiId); };

            std::vector<SceneProperties> &getOpenScenes() { return m_openScenes; };

            [[nodiscard]] int getSelectedScene() const { return selectedScene; };
            void setSelectedScene(const scene::SceneId scene) {selectedScene = static_cast<int>(scene);};
            [[nodiscard]] const std::string &getSceneName(WindowId uiId) const;
        private:
            std::vector<SceneProperties> m_openScenes;

            std::map<WindowId, std::shared_ptr<MainScene>> m_scenes;
            int selectedScene = -1;

            static ImGuiDockNode* getDockNodeForWindow(const char* windowName);
            bool checkSceneNameDuplicate(std::string_view sceneName) const;
    };
}
