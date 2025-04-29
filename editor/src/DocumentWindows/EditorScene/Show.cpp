//// Show.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the editor scene rendering
//
///////////////////////////////////////////////////////////////////////////////

#include "EditorScene.hpp"
#include "context/Selector.hpp"
#include "EntityFactory3D.hpp"
#include "LightFactory.hpp"
#include "IconsFontAwesome.h"
#include "ImNexo/Panels.hpp"
#include "utils/EditorProps.hpp"

namespace nexo::editor {

    void EditorScene::renderNoActiveCamera()
    {
        // No active camera, render the text at the center of the screen
        ImVec2 textSize = ImGui::CalcTextSize("No active camera");
        auto textPos = ImVec2((m_viewSize.x - textSize.x) / 2, (m_viewSize.y - textSize.y) / 2);

        ImGui::SetCursorScreenPos(textPos);
        ImGui::Text("No active camera");
    }

    void EditorScene::renderNewEntityPopup()
    {
        auto &app = Application::getInstance();
        auto &sceneManager = app.getSceneManager();
        const auto sceneId = m_sceneId;

        // --- Primitives submenu ---
        if (ImGui::BeginMenu("Primitives")) {
            if (ImGui::MenuItem("Cube")) {
                const ecs::Entity newCube = EntityFactory3D::createCube({0.0f, 0.0f, -5.0f}, {1.0f, 1.0f, 1.0f},
                                                                       {0.0f, 0.0f, 0.0f}, {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f});
                sceneManager.getScene(sceneId).addEntity(newCube);
            }
            ImGui::EndMenu();
        }

        // --- Model item (with file‑dialog) ---
        if (ImGui::MenuItem("Model")) {
            //TODO: import model
        }

        // --- Lights submenu ---
        if (ImGui::BeginMenu("Lights")) {
            if (ImGui::MenuItem("Directional")) {
                const ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.0f, -1.0f, 0.0f});
                sceneManager.getScene(sceneId).addEntity(directionalLight);
            }
            if (ImGui::MenuItem("Point")) {
                const ecs::Entity pointLight = LightFactory::createPointLight({0.0f, 0.5f, 0.0f});
                utils::addPropsTo(pointLight, utils::PropsType::POINT_LIGHT);
                sceneManager.getScene(sceneId).addEntity(pointLight);
            }
            if (ImGui::MenuItem("Spot")) {
                const ecs::Entity spotLight = LightFactory::createSpotLight({0.0f, 0.5f, 0.0f}, {0.0f, -1.0f, 0.0f});
                utils::addPropsTo(spotLight, utils::PropsType::SPOT_LIGHT);
                sceneManager.getScene(sceneId).addEntity(spotLight);
            }
            ImGui::EndMenu();
        }

        // --- Camera item ---
        if (ImGui::MenuItem("Camera")) {
            m_popupManager.openPopupWithCallback("Popup camera inspector", [this]() {
                ImNexo::CameraInspector(this->m_sceneId, this->m_viewSize);
            }, ImVec2(1440,900));
        }
        m_popupManager.closePopup();
    }

    void EditorScene::renderView()
    {
        const auto viewPortOffset = ImGui::GetCursorPos();
       	auto &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);
        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        // Resize handling
        if ((viewportPanelSize.x > 0 && viewportPanelSize.y > 0) && (m_viewSize.x != viewportPanelSize.x || m_viewSize.y != viewportPanelSize.y))
        {
        	cameraComponent.resize(static_cast<unsigned int>(viewportPanelSize.x),
        							static_cast<unsigned int>(viewportPanelSize.y));

            m_viewSize.x = viewportPanelSize.x;
            m_viewSize.y = viewportPanelSize.y;
        }

        // Render framebuffer
        const unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);
        ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)), m_viewSize, ImVec2(0, 1), ImVec2(1, 0));

        const auto windowSize = ImGui::GetWindowSize();
        auto minBounds = ImGui::GetWindowPos();

        minBounds.x += viewPortOffset.x;
        minBounds.y += viewPortOffset.y;

        const ImVec2 maxBounds = {minBounds.x + windowSize.x, minBounds.y + windowSize.y};
        m_viewportBounds[0] = minBounds;
        m_viewportBounds[1] = maxBounds;
    }

    void EditorScene::show()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(480, 270), ImVec2(1920, 1080));
        auto &selector = Selector::get();
        m_windowName = selector.getUiHandle(m_sceneUuid, std::string(ICON_FA_GLOBE) + "   " + m_windowName);
        const std::string &sceneWindowName = m_windowName + std::string(NEXO_WND_USTRID_DEFAULT_SCENE) + std::to_string(m_sceneId);

        if (ImGui::Begin(sceneWindowName.c_str(), &m_opened, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
        {
            firstDockSetup(std::string(NEXO_WND_USTRID_DEFAULT_SCENE) + std::to_string(m_sceneId));
        	auto &app = getApp();

            // Add some spacing after the toolbar
            ImGui::Dummy(ImVec2(0, 5));
            m_viewPosition = ImGui::GetCursorScreenPos();


            m_focused = ImGui::IsWindowFocused();
            m_isVisibleInDock = ImGui::IsWindowFocused(ImGuiFocusedFlags_DockHierarchy);
            m_hovered = ImGui::IsWindowHovered();
            app.getSceneManager().getScene(m_sceneId).setActiveStatus(m_focused);
            if (m_focused && selector.getSelectedScene() != m_sceneId) {
                selector.setSelectedScene(m_sceneId);
                selector.clearSelection();
            }

            if (m_activeCamera == -1)
                renderNoActiveCamera();
            else {
	            renderView();
	            renderGizmo();
	            renderToolbar();
            }

            if (m_popupManager.showPopup("Add new entity popup"))
                renderNewEntityPopup();
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
}
