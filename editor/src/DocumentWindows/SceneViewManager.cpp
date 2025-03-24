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

    ImGuiDockNode *SceneViewManager::getDockNodeForWindow(const char *windowName)
    {
        ImGuiWindow *window = ImGui::FindWindowByName(windowName);
        if (window && window->DockNode)
        {
            return window->DockNode;
        }
        return nullptr;
    }

    void SceneViewManager::setup()
    {
    	// No need to setup anything since the scene's setup are done when adding it
    }

    void SceneViewManager::shutdown()
    {
        for (const auto &[_, scene]: m_scenes)
            scene->shutdown();
    }

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

    bool SceneViewManager::checkSceneNameDuplicate(std::string_view sceneName) const
    {
	    return std::ranges::any_of(m_scenes, [&sceneName](const auto &scene) {
	        return scene.second->getName() == sceneName;
	    });
    }

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
