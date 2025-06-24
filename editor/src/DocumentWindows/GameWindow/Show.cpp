//// Show.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Cardonne
//  Date:        2025-06-24
//  Description: Show/rendering implementation for GameWindow
//
///////////////////////////////////////////////////////////////////////////////

#include "GameWindow.hpp"
#include "IconsFontAwesome.h"
#include "ImNexo/ImNexo.hpp"
#include "ImNexo/Elements.hpp"
#include "ImNexo/Widgets.hpp"
#include "Application.hpp"
#include "ecs/Coordinator.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"
#include "components/RenderContext.hpp"
#include "core/scene/SceneManager.hpp"
#include <imgui.h>
#include <glm/gtc/type_ptr.hpp>

namespace nexo::editor
{

    void GameWindow::show()
    {
        // Set initial window size
        if (m_firstFrame)
        {
            ImGui::SetNextWindowSize(ImVec2(1280, 720), ImGuiCond_FirstUseEver);
            m_firstFrame = false;
        }

        // Begin the window
        std::string windowTitle = "Game View - " + m_sceneUuid + "###GameWindow" + std::to_string(m_sceneId);
        ImGui::Begin(windowTitle.c_str(), &m_opened, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_MenuBar);

        // Call beginRender to handle docking and state tracking
        beginRender("###GameWindow" + std::to_string(m_sceneId));

        // Store window position and size for toolbar rendering
        m_windowPos = ImGui::GetWindowPos();
        m_contentSize = ImGui::GetContentRegionAvail();

        // Render the toolbar
        renderToolbar();

        // Render the viewport
        renderViewport();

        ImGui::End();
    }

    void GameWindow::renderToolbar()
    {
        constexpr float buttonWidth = 35.0f;
        constexpr float buttonHeight = 35.0f;

        ImVec2 toolbarPos = m_windowPos;
        toolbarPos.x += 10.0f;
        toolbarPos.y += 50.0f;

        ImGui::SetCursorScreenPos(toolbarPos);

        const auto toolbarSize = ImVec2(200.0f, 50.0f);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.0f));
        ImGui::BeginChild("##GameToolbarOverlay", toolbarSize, 0,
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
                              ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                              ImGuiWindowFlags_NoSavedSettings);

        ImGui::SetCursorPosY((ImGui::GetWindowHeight() - ImGui::GetFrameHeight()) * 0.5f);

        // Stop button
        static const std::vector<ImNexo::GradientStop> stopButtonGradient = {
            {0.0f, IM_COL32(204, 51, 51, 255)},
            {1.0f, IM_COL32(153, 26, 26, 255)}};

        if (ImNexo::IconGradientButton("stop_game", ICON_FA_STOP, ImVec2(buttonWidth, buttonHeight), stopButtonGradient))
        {
            m_opened = false;
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Stop game");

        ImGui::SameLine();

        // Pause button
        static const std::vector<ImNexo::GradientStop> pauseButtonGradient = {
            {0.0f, IM_COL32(51, 51, 204, 255)},
            {1.0f, IM_COL32(26, 26, 153, 255)}};

        if (ImNexo::IconGradientButton("pause_game", ICON_FA_PAUSE, ImVec2(buttonWidth, buttonHeight), pauseButtonGradient))
        {
            // TODO: Implement pause functionality
        }
        if (ImGui::IsItemHovered())
            ImGui::SetTooltip("Pause game (not implemented)");

        ImGui::EndChild();
        ImGui::PopStyleColor();
    }

    void GameWindow::renderViewport()
    {
        auto &coordinator = *Application::m_coordinator;
        auto &app = getApp();
        auto &sceneManager = app.getSceneManager();
        auto &scene = sceneManager.getScene(m_sceneId);

        if (m_gameCamera == 0)
        {
            // No game camera, render a message
            const ImVec2 textSize = ImGui::CalcTextSize("No game camera");
            const auto textPos = ImVec2((m_contentSize.x - textSize.x) / 2, (m_contentSize.y - textSize.y) / 2);
            ImGui::SetCursorPos(ImVec2((m_contentSize.x - textSize.x) / 2, (m_contentSize.y - textSize.y) / 2));
            ImGui::Text("No game camera");
            return;
        }

        // Get camera component
        auto &cameraComponent = coordinator.getComponent<components::CameraComponent>(m_gameCamera);
        if (!cameraComponent.m_renderTarget)
            return;

        const glm::vec2 renderTargetSize = cameraComponent.m_renderTarget->getSize();

        // Resize handling
        if (!cameraComponent.viewportLocked && (m_contentSize.x > 0 && m_contentSize.y > 0) && (m_contentSize.x != renderTargetSize.x || m_contentSize.y != renderTargetSize.y))
        {
            cameraComponent.resize(static_cast<unsigned int>(m_contentSize.x),
                                   static_cast<unsigned int>(m_contentSize.y));
        }

        // Store viewport bounds for potential future use
        auto viewportMinRegion = ImGui::GetWindowContentRegionMin();
        auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();
        auto viewportOffset = ImGui::GetWindowPos();
        m_viewportBounds[0] = {viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y};
        m_viewportBounds[1] = {viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y};

        // Get render context
        auto &renderContext = coordinator.getSingletonComponent<components::RenderContext>();

        // Disable grid for game view
        bool gridWasEnabled = renderContext.gridParams.enabled;
        renderContext.gridParams.enabled = false;

        // Restore grid state
        renderContext.gridParams.enabled = gridWasEnabled;

        const unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);
        ImNexo::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)), m_contentSize);
    }
}
