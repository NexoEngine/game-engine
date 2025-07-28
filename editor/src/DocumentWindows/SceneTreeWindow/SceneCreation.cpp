//// SceneCreation.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the scene creation of the scene tree window
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "utils/Config.hpp"
#include <imgui_internal.h>

namespace nexo::editor {

    bool SceneTreeWindow::setupNewDockSpaceNode(const std::string &floatingWindowName, const std::string &newSceneName) const
    {
        const ImGuiWindow* floatingWindow = ImGui::FindWindowByName(floatingWindowName.c_str());
        if (!floatingWindow)
            return false;

        // Create a new docking node
        const auto newDockId = ImGui::GetID("##DockNode");

        // Configure the docking node
        ImGui::DockBuilderRemoveNode(newDockId);
        ImGui::DockBuilderAddNode(newDockId, ImGuiDockNodeFlags_None);

        // Set node size and position based on the floating window
        const ImVec2 windowPos = floatingWindow->Pos;
        const ImVec2 windowSize = floatingWindow->Size;
        ImGui::DockBuilderSetNodeSize(newDockId, windowSize);
        ImGui::DockBuilderSetNodePos(newDockId, windowPos);

        // Dock the windows to this node
        ImGui::DockBuilderDockWindow(floatingWindowName.c_str(), newDockId);
        ImGui::DockBuilderFinish(newDockId);

        // Update the registry with the new dock IDs
        m_windowRegistry.setDockId(floatingWindowName, newDockId);
        m_windowRegistry.setDockId(newSceneName, newDockId);
        return true;
    }

    bool SceneTreeWindow::handleSceneCreation(const std::string &newSceneName) const
    {
        if (newSceneName.empty()) {
            LOG(NEXO_WARN, "Scene name is empty !");
            return false;
        }

        const auto newScene = std::make_shared<EditorScene>(newSceneName, m_windowRegistry);
        newScene->setDefault();
        newScene->setup();
        m_windowRegistry.registerWindow<EditorScene>(newScene);

        auto currentEditorSceneWindow = m_windowRegistry.getWindows<EditorScene>();
        // If no editor scene is open, check the config file
        if (currentEditorSceneWindow.empty()) {
            const std::vector<std::string> &editorSceneInConfig = findAllEditorScenes();
            if (!editorSceneInConfig.empty()) {
                const auto dockId = m_windowRegistry.getDockId(editorSceneInConfig[0]);
                if (!dockId)
                    return false;
                m_windowRegistry.setDockId(std::format("{}{}", NEXO_WND_USTRID_DEFAULT_SCENE, newScene->getSceneId()), *dockId);
                return true;
            }
            // If nothing is present in config file, simply let it float
            return false;
        }

        // Else we retrieve the first active editor scene
        const std::string windowName = std::format("{}{}", NEXO_WND_USTRID_DEFAULT_SCENE, currentEditorSceneWindow[0]->getSceneId());
        const auto dockId = m_windowRegistry.getDockId(windowName);
        // If we dont find the dockId, it means the scene is floating, so we create a new dock space node
        if (!dockId) {
            setupNewDockSpaceNode(windowName, std::format("{}{}", NEXO_WND_USTRID_DEFAULT_SCENE, newScene->getSceneId()));
            return true;
        }
        m_windowRegistry.setDockId(std::format("{}{}", NEXO_WND_USTRID_DEFAULT_SCENE, newScene->getSceneId()), *dockId);
        return true;
    }
}
