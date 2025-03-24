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

namespace nexo::editor {
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
     * @brief Placeholder for scene setup.
     *
     * This method is intentionally left empty since each scene's setup is performed when it is added.
     */
    void SceneViewManager::setup()
    {
    	// No need to setup anything since the scene's setup are done when adding it
    }

    /**
     * @brief Shuts down all managed scenes.
     *
     * Iterates through the collection of scenes and invokes each scene's shutdown method,
     * ensuring that all scenes are properly cleaned up.
     */
    void SceneViewManager::shutdown()
    {
        for (const auto &[_, scene]: m_scenes)
            scene->shutdown();
    }

    /**
     * @brief Duplicates an existing scene view.
     *
     * Checks whether a scene with the specified window ID exists. If the scene is not found,
     * an error is logged and the function exits. The duplication logic is currently disabled.
     *
     * @param uiId The unique identifier of the scene view to duplicate.
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
     * @brief Creates and registers a new scene.
     *
     * This function instantiates a new scene using the provided name and default flag, sets it up,
     * and docks its window to an existing scene's dock node if available. The new scene is then added to the
     * internal scene collection keyed by its window ID.
     *
     * @param sceneName The name to assign to the new scene.
     * @param defaultScene Indicates whether the new scene is the default scene.
     */
    void SceneViewManager::addNewScene(const std::string &sceneName, bool defaultScene)
    {
        auto newScene = std::make_shared<MainScene>(m_windowRegistry, sceneName, defaultScene);
        newScene->setup();
        const std::string targetWindow = !m_scenes.empty() ? m_scenes.begin()->second->getName() : "";
        if (!targetWindow.empty())
        {
            if (const ImGuiDockNode *dockNode = getDockNodeForWindow(targetWindow.c_str()))
                ImGui::DockBuilderDockWindow(newScene->getName().c_str(), dockNode->ID);
        }
        m_scenes[newScene->getWindowId()] = newScene;
    }

    /**
     * @brief Removes the scene associated with the specified window identifier.
     *
     * Checks if a scene with the given identifier exists. If it does, the function unselects
     * any currently selected entity, deletes the scene from the application's scene manager,
     * removes it from the internal scene map, and clears the list of open scenes. If no
     * matching scene is found, an error is logged and no further action is taken.
     *
     * @param uiId Unique window identifier of the scene to remove.
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
     * @brief Updates the state of each scene and refreshes the list of open scenes.
     *
     * This method clears the current list of open scenes and iterates over all managed scenes. For each scene,
     * if it is open, the scene's identifier and window identifier are recorded in a SceneProperties structure, which
     * is then added to the open scenes list. Regardless of its open state, the scene's update method is invoked to
     * refresh its state.
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
