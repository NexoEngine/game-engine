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
#include "context/actions/EntityActions.hpp"
#include "context/ActionManager.hpp"
#include <imgui_internal.h>

namespace nexo::editor
{
    void EditorScene::renderNoActiveCamera() const
    {
        // No active camera, render the text at the center of the screen
        const ImVec2 textSize = ImGui::CalcTextSize("No active camera");
        const auto textPos = ImVec2((m_contentSize.x - textSize.x) / 2, (m_contentSize.y - textSize.y) / 2);

        ImGui::SetCursorScreenPos(textPos);
        ImGui::Text("No active camera");
    }

    void EditorScene::renderNewEntityPopup()
    {
        auto& app = Application::getInstance();
        auto& sceneManager = app.getSceneManager();

        // --- Primitives submenu ---
        if (ImGui::BeginMenu("Primitives"))
        {
            if (ImGui::MenuItem("Cube"))
            {
                const ecs::Entity newCube = EntityFactory3D::createCube({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
                                                                        {0.0f, 0.0f, 0.0f}, {
                                                                            0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25,
                                                                            1.0f
                                                                        });
                sceneManager.getScene(m_sceneId).addEntity(newCube);
                auto createAction = std::make_unique<EntityCreationAction>(newCube);
                ActionManager::get().recordAction(std::move(createAction));
            }
            if (ImGui::MenuItem("Sphere"))
            {
                m_popupManager.openPopup("Sphere creation popup");
            }
            if (ImGui::MenuItem("Cylinder"))
            {
                m_popupManager.openPopup("Cylinder creation popup");
            }
            if (ImGui::MenuItem("Pyramid"))
            {
                const ecs::Entity newPyramid = EntityFactory3D::createPyramid({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
                                                                              {0.0f, 0.0f, 0.0f}, {
                                                                                  0.05f * 1.5, 0.09f * 1.15,
                                                                                  0.13f * 1.25, 1.0f
                                                                              });
                sceneManager.getScene(m_sceneId).addEntity(newPyramid);
                auto createAction = std::make_unique<EntityCreationAction>(newPyramid);
                ActionManager::get().recordAction(std::move(createAction));
            }
            if (ImGui::MenuItem("Tetrahedron"))
            {
                const ecs::Entity newTetrahedron = EntityFactory3D::createTetrahedron(
                    {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
                    {0.0f, 0.0f, 0.0f}, {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f});
                sceneManager.getScene(m_sceneId).addEntity(newTetrahedron);
                auto createAction = std::make_unique<EntityCreationAction>(newTetrahedron);
                ActionManager::get().recordAction(std::move(createAction));
            }
            ImGui::EndMenu();
        }

        // --- Model item (with file‑dialog) ---
        if (ImGui::MenuItem("Model"))
        {
            //TODO: import model
        }

        // --- Lights submenu ---
        if (ImGui::BeginMenu("Lights"))
        {
            if (ImGui::MenuItem("Directional"))
            {
                const ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.0f, -1.0f, 0.0f});
                sceneManager.getScene(m_sceneId).addEntity(directionalLight);
                auto createAction = std::make_unique<EntityCreationAction>(directionalLight);
                ActionManager::get().recordAction(std::move(createAction));
            }
            if (ImGui::MenuItem("Point"))
            {
                const ecs::Entity pointLight = LightFactory::createPointLight({0.0f, 0.5f, 0.0f});
                addPropsTo(pointLight, utils::PropsType::POINT_LIGHT);
                sceneManager.getScene(m_sceneId).addEntity(pointLight);
                auto createAction = std::make_unique<EntityCreationAction>(pointLight);
                ActionManager::get().recordAction(std::move(createAction));
            }
            if (ImGui::MenuItem("Spot"))
            {
                const ecs::Entity spotLight = LightFactory::createSpotLight({0.0f, 0.5f, 0.0f}, {0.0f, -1.0f, 0.0f});
                addPropsTo(spotLight, utils::PropsType::SPOT_LIGHT);
                sceneManager.getScene(m_sceneId).addEntity(spotLight);
                auto createAction = std::make_unique<EntityCreationAction>(spotLight);
                ActionManager::get().recordAction(std::move(createAction));
            }
            ImGui::EndMenu();
        }

        // --- Camera item ---
        if (ImGui::MenuItem("Camera"))
        {
            m_popupManager.openPopupWithCallback("Popup camera inspector", [this]()
            {
                ImNexo::CameraInspector(this->m_sceneId);
            }, ImVec2(1440, 900));
        }
        PopupManager::closePopup();
    }

    void EditorScene::renderView()
    {
        auto &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);
        if (!cameraComponent.m_renderTarget)
            return;

        // Resize handling
        if (const glm::vec2 renderTargetSize = cameraComponent.m_renderTarget->getSize(); !cameraComponent.
            viewportLocked && (m_contentSize.x > 0 && m_contentSize.y > 0)
            && (m_contentSize.x != renderTargetSize.x || m_contentSize.y != renderTargetSize.y))
        {
            cameraComponent.resize(static_cast<unsigned int>(m_contentSize.x),
                                   static_cast<unsigned int>(m_contentSize.y));
        }

        // Render framebuffer
        const unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);
        ImNexo::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)), m_contentSize);

        handleDropTarget();
        const ImVec2 viewportMin = ImGui::GetItemRectMin();
        const ImVec2 viewportMax = ImGui::GetItemRectMax();
        m_viewportBounds[0] = viewportMin;
        m_viewportBounds[1] = viewportMax;
    }

    void EditorScene::show()
    {
        // Handle deferred dock split before rendering
        if (m_shouldSplitDock && !m_gameWindowNameToSplit.empty())
        {
            const std::string currentWindowName = m_windowName;
            const ImGuiWindow *currentImGuiWindow = ImGui::FindWindowByName((currentWindowName + NEXO_WND_USTRID_DEFAULT_SCENE + std::to_string(m_sceneId)).c_str());

            if (currentImGuiWindow && currentImGuiWindow->DockId)
            {
                const ImGuiID editorDockId = currentImGuiWindow->DockId;
                ImGuiID rightNode, leftNode;

                if (ImGui::DockBuilderSplitNode(editorDockId, ImGuiDir_Right, 0.5f, &rightNode, &leftNode))
                {
                    // Dock the windows
                    ImGui::DockBuilderDockWindow((currentWindowName + NEXO_WND_USTRID_DEFAULT_SCENE + std::to_string(m_sceneId)).c_str(), leftNode);
                    ImGui::DockBuilderDockWindow(m_gameWindowNameToSplit.c_str(), rightNode);
                    ImGui::DockBuilderFinish(editorDockId);

                    // Update registry
                    m_windowRegistry.setDockId(currentWindowName + NEXO_WND_USTRID_DEFAULT_SCENE + std::to_string(m_sceneId), leftNode);
                    m_windowRegistry.setDockId(m_gameWindowNameToSplit, rightNode);
                }
            }

            m_shouldSplitDock = false;
            m_gameWindowNameToSplit.clear();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(480, 270), ImVec2(1920, 1080));
        auto& selector = Selector::get();
        m_windowName = selector.getUiHandle(m_sceneUuid, std::string(ICON_FA_GLOBE) + "   " + m_windowName);
        const std::string &sceneWindowName = std::format("{}{}{}",
                                                         m_windowName,
                                                         NEXO_WND_USTRID_DEFAULT_SCENE,
                                                         m_sceneId);
        m_wasVisibleLastFrame = m_isVisibleInDock;
        m_isVisibleInDock = false;
        if (ImGui::Begin(sceneWindowName.c_str(), &m_opened,
                         ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse |
                         ImGuiWindowFlags_NoScrollWithMouse))
        {
            const std::string renderName = std::format("{}{}",
                                                       NEXO_WND_USTRID_DEFAULT_SCENE,
                                                       m_sceneId
            );
            beginRender(renderName);
            auto& app = getApp();

            app.getSceneManager().getScene(m_sceneId).setActiveStatus(m_focused);

            if (m_focused && selector.getSelectedScene() != m_sceneId)
            {
                selector.setSelectedScene(m_sceneId);
                selector.clearSelection();
            }

            if (m_activeCamera == -1)
            {
                renderNoActiveCamera();
            }
            else
            {
                renderView();
                renderGizmo();
                renderToolbar();
            }

            if (m_popupManager.showPopup("Add new entity popup"))
            {
                renderNewEntityPopup();
            }
            if (m_popupManager.showPopup("Sphere creation popup"))
            {
                ImNexo::PrimitiveCustomizationMenu(m_sceneId, SPHERE);
            }
            if (m_popupManager.showPopup("Cylinder creation popup"))
            {
                ImNexo::PrimitiveCustomizationMenu(m_sceneId, CYLINDER);
            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }
}
