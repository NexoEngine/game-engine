//// RenderProperty.cpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/03/2025
//  Description: Source file for the render property inspector
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>

#include "RenderProperty.hpp"
#include "AEntityProperty.hpp"
#include "Application.hpp"
#include "DocumentWindows/PopupManager.hpp"
#include "Framebuffer.hpp"
#include "components/Light.hpp"
#include "context/actions/EntityActions.hpp"
#include "utils/ScenePreview.hpp"
#include "components/Camera.hpp"
#include "components/Render.hpp"
#include "DocumentWindows/InspectorWindow/InspectorWindow.hpp"
#include "DocumentWindows/MaterialInspector/MaterialInspector.hpp"
#include "context/ActionManager.hpp"
#include "ImNexo/Panels.hpp"
#include "ImNexo/Elements.hpp"
#include "ImNexo/Components.hpp"

namespace nexo::editor {

    void RenderProperty::createMaterialPopup(const ecs::Entity entity)
    {
        ImGui::Text("Create New Material");
        ImGui::Separator();

        const ImVec2 availSize = ImGui::GetContentRegionAvail();
        const float totalWidth = availSize.x;
        float totalHeight = availSize.y - 40; // Reserve space for bottom buttons

        // Define layout: 40% for inspector, 60% for preview
        const float inspectorWidth = totalWidth * 0.4f;
        const float previewWidth = totalWidth - inspectorWidth - 8; // Subtract spacing between panels

        static utils::ScenePreviewOut scenePreviewInfo;
        if (!scenePreviewInfo.sceneGenerated)
        {
            utils::genScenePreview("New Material Preview", {previewWidth - 8, totalHeight}, entity, scenePreviewInfo);
            auto &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(scenePreviewInfo.cameraId);
            cameraComponent.clearColor =  {67.0f/255.0f, 65.0f/255.0f, 80.0f/255.0f, 111.0f/255.0f};
            cameraComponent.render = true;
        }

        ImGui::Columns(2, "MaterialPreviewColumns", false);

        ImGui::SetColumnWidth(0, inspectorWidth);
        // --- Left Side: Material Inspector ---
        {
            ImGui::BeginChild("MaterialInspector", ImVec2(inspectorWidth - 4, totalHeight), true);
            static char materialName[128] = "";
            ImGui::InputText("Name", materialName, IM_ARRAYSIZE(materialName));
            ImGui::Spacing();

            ImGui::EndChild();
        }
        ImGui::NextColumn();
        // --- Right Side: Material Preview ---
        {
            ImGui::BeginChild("MaterialPreview", ImVec2(previewWidth - 4, totalHeight), true);

            auto &app = getApp();
            const Application::SceneInfo sceneInfo{scenePreviewInfo.sceneId, nexo::RenderingType::FRAMEBUFFER};
            app.run(sceneInfo);
            auto const &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(scenePreviewInfo.cameraId);
            const unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);

            const float aspectRatio = static_cast<float>(cameraComponent.width) /
                                      static_cast<float>(cameraComponent.height);

            const float displayHeight = totalHeight - 20;
            const float displayWidth = displayHeight * aspectRatio;

            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 4, ImGui::GetCursorPosY() + 4));
            ImNexo::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)),
                        ImVec2(displayWidth, displayHeight));

            ImGui::EndChild();
        }

        ImGui::Columns(1);
        ImGui::Spacing();

        // Bottom buttons - centered
        constexpr float buttonWidth = 120.0f;

        if (ImNexo::Button("OK", ImVec2(buttonWidth, 0)))
        {
            // TODO: Insert logic to create the new material

            // Clean up preview scene
            if (scenePreviewInfo.sceneGenerated)
            {
                auto &app = getApp();
                app.getSceneManager().deleteScene(scenePreviewInfo.sceneId);
                scenePreviewInfo.sceneGenerated = false;
            }

            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImNexo::Button("Cancel", ImVec2(buttonWidth, 0)))
        {
            if (scenePreviewInfo.sceneGenerated)
            {
                auto &app = getApp();
                app.getSceneManager().deleteScene(scenePreviewInfo.sceneId);
                scenePreviewInfo.sceneGenerated = false;
            }

            ImGui::CloseCurrentPopup();
        }
    }

    void RenderProperty::show(ecs::Entity entity)
    {
        if (Application::m_coordinator->entityHasComponent<components::CameraComponent>(entity) ||
            Application::m_coordinator->entityHasComponent<components::PointLightComponent>(entity) ||
            Application::m_coordinator->entityHasComponent<components::SpotLightComponent>(entity))
            return;
        auto& renderComponent = Application::getEntityComponent<components::RenderComponent>(entity);

        static bool sectionOpen = true;

        if (ImNexo::Header("##RenderNode", "Render Component"))
        {
            ImGui::Text("Hide");
            ImGui::SameLine(0, 12);
            bool hidden = !renderComponent.isRendered;
            if (ImGui::Checkbox("##HideCheckBox", &hidden)) {
                auto beforeState = renderComponent.save();
                renderComponent.isRendered = !hidden;
                auto afterState = renderComponent.save();
                auto action = std::make_unique<ComponentChangeAction<components::RenderComponent>>(entity, beforeState, afterState);
                ActionManager::get().recordAction(std::move(action));
            }

            ImNexo::ToggleButtonWithSeparator("Material", &sectionOpen);
            static std::shared_ptr<renderer::NxFramebuffer> framebuffer = nullptr;
            static int entityBase = -1;
            if (sectionOpen)
            {
            	if (entityBase != static_cast<int>(entity))
				{
					//TODO: I guess all of this should be centralized in the assets
					utils::ScenePreviewOut previewParams;
					utils::genScenePreview("Modify material inspector", {64, 64}, entity, previewParams);
					auto &app = nexo::getApp();
					app.getSceneManager().getScene(previewParams.sceneId).setActiveStatus(false);
					const Application::SceneInfo sceneInfo{previewParams.sceneId, nexo::RenderingType::FRAMEBUFFER};
					app.run(sceneInfo);
					const auto &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(previewParams.cameraId);
					framebuffer = cameraComponent.m_renderTarget;
					app.getSceneManager().deleteScene(previewParams.sceneId);
					entityBase = static_cast<int>(entity);
				}

                // --- Material Preview ---
                if (framebuffer && framebuffer->getColorAttachmentId(0) != 0)
                	ImNexo::Image(static_cast<ImTextureID>(static_cast<intptr_t>(framebuffer->getColorAttachmentId(0))), ImVec2(64, 64));
                ImGui::SameLine();

                ImGui::BeginGroup();
                {
                    // --- Dropdown for Material Types ---
                    static int selectedMaterialIndex = 0;
                    const char* materialTypes[] = { "Default", "Metal", "Wood", "Plastic" };
                    ImGui::Combo("##MaterialType", &selectedMaterialIndex, materialTypes, IM_ARRAYSIZE(materialTypes));

                    // --- Material Action Buttons ---
                    if (ImNexo::Button("Create new material"))
                    {
                        m_popupManager.openPopup("Create new material", ImVec2(1440,900));
                    }
                    ImGui::SameLine();
                    if (ImNexo::Button("Modify Material"))
                    {
                        m_inspector.setSubInspectorVisibility<MaterialInspector>(true);
                    }
                }
                ImGui::EndGroup();
                const ImVec2 center = ImGui::GetMainViewport()->GetCenter();
                ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            }

            ImGui::TreePop();
        }

        if (m_popupManager.showPopupModal("Create new material"))
        {
            createMaterialPopup(entity);
            PopupManager::closePopup();
        }
    }
}
