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

std::shared_ptr<nexo::editor::SceneViewManager> nexo::editor::SceneViewManager::m_instance = nullptr;

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
        for (const auto &[_, scene]: m_scenes)
        {
            scene->setSceneManager(m_sceneManagerBridge);
            scene->setup();
        }
    }

    void SceneViewManager::shutdown()
    {
        for (const auto &[_, scene]: m_scenes)
            scene->shutdown();
    }

    void SceneViewManager::duplicateSceneView(const std::string &sceneName, const std::string &fromScene,
                                              const std::shared_ptr<MainScene> &scene)
    {

    }


    void SceneViewManager::addNewScene(const std::string &sceneName, const std::shared_ptr<MainScene> &scene)
    {
        std::shared_ptr<MainScene> newScene;
        if (scene != nullptr)
            newScene = scene;
        else
        {
            newScene = std::make_shared<MainScene>(sceneName);
            newScene->setup();
            newScene->setSceneManager(m_sceneManagerBridge);
        }
        std::string targetWindow = !m_scenes.empty() ? m_scenes.begin()->second->getName() : "";
        if (!targetWindow.empty())
        {
            ImGuiDockNode *dockNode = getDockNodeForWindow(targetWindow.c_str());
            if (dockNode)
            {
                ImGui::DockBuilderDockWindow(newScene->getName().c_str(), dockNode->ID);
            }
        }
        m_scenes[newScene->getName()] = newScene;
    }

    void SceneViewManager::removeScene(const std::string &sceneName)
    {
        if (!m_scenes.contains(sceneName))
        {
            LOG(NEXO_ERROR, "SceneViewManager::removeScene: scene {} not found", sceneName);
            return;
        }
        auto scene = m_scenes.at(sceneName);
        m_sceneManagerBridge->unselectEntity();
        auto &app = nexo::getApp();
        app.deleteScene(scene->getSceneId());
        m_scenes.erase(sceneName);
        m_openScenes.clear();
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
                SceneProperties openScene;
                openScene.sceneId = scene->getSceneId();
                openScene.uiName = scene->getName();
                m_openScenes.push_back(openScene);
            }
            scene->update();
        }
    }
}
