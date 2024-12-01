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

    void SceneViewManager::duplicateSceneView(WindowId uiId)
    {
        if (!m_scenes.contains(uiId))
        {
            LOG(NEXO_ERROR, "SceneViewManager::duplicateSceneView: scene {} not found", uiId);
            return;
        }
        const auto scene = m_scenes.at(uiId);
        const std::string sceneName = scene->getName();
        const auto newScene = std::make_shared<MainScene>(*scene);
        const std::string newSceneName = newScene->getName() + " - View " + std::to_string(scene->idView++);
        newScene->setName(newSceneName);
        newScene->setupFramebuffer();
        newScene->windowId = nextWindowId++;
        if (const ImGuiDockNode *dockNode = getDockNodeForWindow(sceneName.c_str()))
            ImGui::DockBuilderDockWindow(newScene->getName().c_str(), dockNode->ID);
        m_scenes[newScene->windowId] = newScene;
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
        const std::string targetWindow = !m_scenes.empty() ? m_scenes.begin()->second->getName() : "";
        if (!targetWindow.empty())
        {
            if (const ImGuiDockNode *dockNode = getDockNodeForWindow(targetWindow.c_str()))
                ImGui::DockBuilderDockWindow(newScene->getName().c_str(), dockNode->ID);
        }
        m_scenes[newScene->windowId] = newScene;
    }

    void SceneViewManager::removeScene(WindowId uiId)
    {
        if (!m_scenes.contains(uiId))
        {
            LOG(NEXO_ERROR, "SceneViewManager::removeScene: scene {} not found", uiId);
            return;
        }
        const auto scene = m_scenes.at(uiId);
        m_sceneManagerBridge->unselectEntity();
        auto &app = nexo::getApp();
        app.deleteScene(scene->getSceneId());
        m_scenes.erase(uiId);
        m_openScenes.clear();
    }

    void SceneViewManager::hideLayer(WindowId uiId, const scene::LayerId layerId) const
    {
        if (!m_scenes.contains(uiId))
        {
            LOG(NEXO_ERROR, "SceneViewManager::hideLayer: scene {} not found", uiId);
            return;
        }
        m_scenes.at(uiId)->hideLayer(layerId);
    }

    void SceneViewManager::showLayer(WindowId uiId, const scene::LayerId layerId) const
    {
        if (!m_scenes.contains(uiId))
        {
            LOG(NEXO_ERROR, "SceneViewManager::showLayer: scene {} not found", uiId);
            return;
        }
        m_scenes.at(uiId)->showLayer(layerId);
    }

    bool SceneViewManager::isLayerHidden(WindowId uiId, const scene::LayerId layerId) const
    {
        if (!m_scenes.contains(uiId))
        {
            LOG(NEXO_ERROR, "SceneViewManager::isLayerHidden: scene {} not found", uiId);
            return false;
        }
        return m_scenes.at(uiId)->isLayerHidden(layerId);
    }

    void SceneViewManager::addDefaultCameraToLayer(WindowId uiId, const scene::LayerId layerId) const
    {
        if (!m_scenes.contains(uiId))
        {
            LOG(NEXO_ERROR, "SceneViewManager::addDefaultCameraToLayer: scene {} not found", uiId);
            return;
        }
        m_scenes.at(uiId)->addDefaultCameraToLayer(layerId);
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
                openScene.windowId = scene->windowId;

                m_openScenes.push_back(openScene);
            }
            scene->update();
        }
    }
}
