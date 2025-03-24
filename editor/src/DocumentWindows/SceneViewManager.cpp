//// SceneViewManager.cpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        16/11/2024
//  Description: Source file for the scene manager class
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneViewManager.hpp"
#include "context/Selector.hpp"

#include <algorithm>

namespace nexo::editor {
    /**
     * @brief Retrieves the dock node associated with the specified window.
     *
     * This function searches for a window using ImGui's API by its name and returns the associated dock node if one exists.
     * If the window is not found or does not have a dock node assigned, it returns nullptr.
     *
     * @param windowName The name of the window to search for.
     * @return ImGuiDockNode* Pointer to the dock node if found; otherwise, nullptr.
     */
    ImGuiDockNode *SceneViewManager::getDockNodeForWindow(const char *windowName)
    {
        ImGuiWindow *window = ImGui::FindWindowByName(windowName);
        if (window && window->DockNode)
        {
            return window->DockNode;
        }
        return nullptr;
    }

    /**
     * @brief No-op setup function.
     *
     * This function intentionally does nothing as individual scenes are initialized when they are added.
     */
    void SceneViewManager::setup()
    {
    	// No need to setup anything since the scene's setup are done when adding it
    }

    /**
     * @brief Shuts down all managed scenes.
     *
     * Iterates over each scene managed by the SceneViewManager and invokes its shutdown
     * method to ensure proper cleanup of resources.
     */
    void SceneViewManager::shutdown()
    {
        for (const auto &[_, scene]: m_scenes)
            scene->shutdown();
    }

    /**
     * @brief Attempts to duplicate the scene view for the specified window ID.
     *
     * Checks if a scene corresponding to the given window ID exists. If not, logs an error and exits early.
     * The intended duplication logic is currently commented out, so no duplication is performed.
     *
     * @param uiId The unique identifier of the scene view window to duplicate.
     */
    void SceneViewManager::duplicateSceneView(WindowId uiId)
    {
        if (!m_scenes.contains(uiId))
        {
            LOG(NEXO_ERROR, "SceneViewManager::duplicateSceneView: scene {} not found", uiId);
            return;
        }
        //const auto scene = m_scenes.at(uiId);
        //const std::string sceneName = scene->getName();
        //const auto newScene = std::make_shared<MainScene>(*scene);
        //const std::string newSceneName = std::format("{} - View {}", newScene->getName(), scene->idView++);
        //newScene->setName(newSceneName);
        //newScene->windowId = nextWindowId++;
        //if (const ImGuiDockNode *dockNode = getDockNodeForWindow(sceneName.c_str()))
        //    ImGui::DockBuilderDockWindow(newScene->getName().c_str(), dockNode->ID);
        //m_scenes[newScene->windowId] = newScene;
    }

    /**
     * @brief Checks if a scene with the specified name already exists.
     *
     * Iterates over the managed scenes to determine whether any scene's name matches the provided
     * scene name.
     *
     * @param sceneName The name to check for duplicate usage.
     * @return true if a duplicate scene name is found; false otherwise.
     */
    bool SceneViewManager::checkSceneNameDuplicate(std::string_view sceneName) const
    {
	    return std::ranges::any_of(m_scenes, [&sceneName](const auto &scene) {
	        return scene.second->getName() == sceneName;
	    });
    }

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
    bool SceneViewManager::addNewScene(const std::string &sceneName, bool defaultScene)
    {
    	if (checkSceneNameDuplicate(sceneName))
     		return false;
        auto newScene = std::make_shared<MainScene>(m_windowRegistry, sceneName, defaultScene);
        newScene->setup();
        const std::string targetWindow = !m_scenes.empty() ? m_scenes.begin()->second->getName() : "";
        if (!targetWindow.empty())
        {
            if (const ImGuiDockNode *dockNode = getDockNodeForWindow(targetWindow.c_str()))
                ImGui::DockBuilderDockWindow(newScene->getName().c_str(), dockNode->ID);
        }
        m_scenes[newScene->getWindowId()] = newScene;
        return true;
    }

    /**
     * @brief Removes the scene associated with the specified window ID.
     *
     * If a scene matching the given UI ID exists, this method unselects any currently selected entity, deletes the scene from the application's scene manager, and removes it from the internal tracking container. It then clears the list of open scenes. If no matching scene is found, an error is logged.
     *
     * @param uiId The unique identifier of the scene's window to remove.
     */
    void SceneViewManager::removeScene(WindowId uiId)
    {
        if (!m_scenes.contains(uiId))
        {
            LOG(NEXO_ERROR, "SceneViewManager::removeScene: scene {} not found", uiId);
            return;
        }
        const auto scene = m_scenes.at(uiId);
        Selector::get().unselectEntity();
        auto &app = nexo::getApp();
        app.getSceneManager().deleteScene(scene->getSceneId());
        m_scenes.erase(uiId);
        m_openScenes.clear();
    }

    const std::string &SceneViewManager::getSceneName(WindowId uiId) const
    {
        static const std::string emptyString;
        if (!m_scenes.contains(uiId))
        {
            LOG(NEXO_ERROR, "SceneViewManager::getSceneName: scene {} not found", uiId);
            return emptyString;
        }
        return m_scenes.at(uiId)->getName();
    }

    void SceneViewManager::show()
    {
        for (const auto &[_, scene]: m_scenes)
            scene->show();
    }

    /**
     * @brief Updates all scenes and refreshes the list of open scenes.
     *
     * Clears the current list of open scenes and iterates over all managed scenes. For each scene that
     * is open (as determined by its isOpened() method), its scene ID and window ID are recorded. Finally,
     * the update() method of each scene is invoked to ensure its state is current.
     */
    void SceneViewManager::update()
    {
        m_openScenes.clear();
        for (const auto &[_, scene]: m_scenes)
        {
            if (scene->isOpened())
            {
                SceneProperties openScene{};
                openScene.sceneId = scene->getSceneId();
                openScene.windowId = scene->getWindowId();

                m_openScenes.push_back(openScene);
            }
            scene->update();
        }
    }
}
