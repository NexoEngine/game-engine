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

            /**
             * @brief Shuts down all managed scenes.
             *
             * Iterates over each scene managed by the SceneViewManager and invokes its shutdown
             * method to ensure proper cleanup of resources.
             */
            void shutdown() override;
            void show() override;

            /**
             * @brief Updates all scenes and refreshes the list of open scenes.
             *
             * Clears the current list of open scenes and iterates over all managed scenes. For each scene that
             * is open (as determined by its isOpened() method), its scene ID and window ID are recorded. Finally,
             * the update() method of each scene is invoked to ensure its state is current.
             */
            void update() override;

            /**
             * @brief Adds a new scene with the provided name and configuration.
             *
             * This method first checks if a scene with the given name already exists. If a duplicate is found,
             * no new scene is added and the function returns false. Otherwise, a new scene is created and set up.
             * If there is an existing scene window, the new scene is docked to the same window.
             *
             * @param sceneName The unique name of the scene to add.
             * @param defaultScene Indicates whether the new scene should be treated as the default scene.
             * @return true if the scene was successfully created and added; false if a duplicate scene name exists.
             */
            bool addNewScene(const std::string &sceneName, bool defaultScene = false);

            /**
             * @brief Attempts to duplicate the scene view for the specified window ID.
             *
             * Checks if a scene corresponding to the given window ID exists. If not, logs an error and exits early.
             * The intended duplication logic is currently commented out, so no duplication is performed.
             *
             * @param uiId The unique identifier of the scene view window to duplicate.
             */
            void duplicateSceneView(WindowId uiId);

            /**
             * @brief Removes the scene associated with the specified window ID.
             *
             * If a scene matching the given UI ID exists, this method unselects any currently selected entity, deletes the scene from the application's scene manager, and removes it from the internal tracking container. It then clears the list of open scenes. If no matching scene is found, an error is logged.
             *
             * @param uiId The unique identifier of the scene's window to remove.
             */
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

            /**
             * @brief Retrieves the dock node associated with the specified window.
             *
             * This function searches for a window using ImGui's API by its name and returns the associated dock node if one exists.
             * If the window is not found or does not have a dock node assigned, it returns nullptr.
             *
             * @param windowName The name of the window to search for.
             * @return ImGuiDockNode* Pointer to the dock node if found; otherwise, nullptr.
             */
            static ImGuiDockNode* getDockNodeForWindow(const char* windowName);

            /**
             * @brief Checks if a scene with the specified name already exists.
             *
             * Iterates over the managed scenes to determine whether any scene's name matches the provided
             * scene name.
             *
             * @param sceneName The name to check for duplicate usage.
             * @return true if a duplicate scene name is found; false otherwise.
             */
            bool checkSceneNameDuplicate(std::string_view sceneName) const;
    };
}
