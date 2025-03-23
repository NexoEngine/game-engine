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
#include "Components/EntityPropertiesComponents.hpp"
#include "Components/Widgets.hpp"
#include "Framebuffer.hpp"
#include "utils/ScenePreview.hpp"
#include "components/Camera.hpp"
#include "components/Render.hpp"
#include "components/Transform.hpp"
#include "CameraFactory.hpp"
#include "LightFactory.hpp"
#include "EntityFactory3D.hpp"

#include <imgui_internal.h>
#include <loguru/loguru.hpp>

namespace nexo::editor {
	bool RenderProperty::showMaterialInspector = false;
	components::Material *RenderProperty::selectedMaterial = nullptr;
	nexo::editor::PopupManager nexo::editor::RenderProperty::popupManager;

    RenderProperty::RenderProperty(const std::string& name)
        : AEntityProperty(name)
    {
    }

    RenderProperty::~RenderProperty()
    {
    }

    void RenderProperty::update()
    {
    }


    void RenderProperty::createMaterialPopup(ecs::Entity entity)
    {
    	auto &app = getApp();
        ImGui::Text("Create New Material");
        ImGui::Separator();

        ImVec2 availSize = ImGui::GetContentRegionAvail();
        float totalWidth = availSize.x;
        float totalHeight = availSize.y - 40; // Reserve space for bottom buttons

        // Define layout: 60% for inspector, 40% for preview
        float inspectorWidth = totalWidth * 0.4f;
        float previewWidth = totalWidth - inspectorWidth - 8; // Subtract spacing between panels

        static utils::ScenePreviewOut scenePreviewInfo;
        if (!scenePreviewInfo.sceneGenerated)
        {
        	utils::genScenePreview("New Material Preview", {previewWidth - 8, totalHeight}, entity, scenePreviewInfo);
        }
        auto renderable3D = std::dynamic_pointer_cast<components::Renderable3D>(app.m_coordinator->getComponent<components::RenderComponent>(scenePreviewInfo.entityCopy).renderable);

        ImGui::Columns(2, "MaterialPreviewColumns", false);

        ImGui::SetColumnWidth(0, inspectorWidth);
        // --- Left Side: Material Inspector ---
        {
            ImGui::BeginChild("MaterialInspector", ImVec2(inspectorWidth - 4, totalHeight), true);
            static char materialName[128] = "";
            ImGui::InputText("Name", materialName, IM_ARRAYSIZE(materialName));
            ImGui::Spacing();

            Widgets::drawMaterialInspector(&renderable3D->material);
            ImGui::EndChild();
        }
        ImGui::NextColumn();
        // --- Right Side: Material Preview ---
        {
            ImGui::BeginChild("MaterialPreview", ImVec2(previewWidth - 4, totalHeight), true);

            auto &app = getApp();
            app.run(scenePreviewInfo.sceneId, RenderingType::FRAMEBUFFER);
            auto &cameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(scenePreviewInfo.cameraId);
            unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);

            float aspectRatio = static_cast<float>(cameraComponent.width) /
                              static_cast<float>(cameraComponent.height);

            float displayHeight = totalHeight - 20;
            float displayWidth = displayHeight * aspectRatio;

            // Center the preview in the available space
            float offsetX = (previewWidth - 8 - displayWidth) * 0.5f;
            float offsetY = (totalHeight - displayHeight) * 0.5f;

            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 4, ImGui::GetCursorPosY() + 4));
            ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)),
                        ImVec2(displayWidth, displayHeight), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::EndChild();
        }

        ImGui::Columns(1);
        ImGui::Spacing();

        // Bottom buttons - centered
        float buttonWidth = 120.0f;
        float buttonsWidth = 2 * buttonWidth + ImGui::GetStyle().ItemSpacing.x;

        if (ImGui::Button("OK", ImVec2(buttonWidth, 0)))
        {
            // TODO: Insert logic to create the new material

            // Clean up preview scene
            if (scenePreviewInfo.sceneGenerated)
            {
                auto &app = getApp();
                auto &renderComponentBase = app.m_coordinator->getComponent<components::RenderComponent>(entity);
                renderComponentBase.renderable = renderable3D;
                app.getSceneManager().deleteScene(scenePreviewInfo.sceneId);
                scenePreviewInfo.sceneGenerated = false;
            }

            ImGui::CloseCurrentPopup();
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 0)))
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

    int RenderProperty::show(ecs::Entity entity)
    {
    	auto const& App = getApp();
        auto& renderComponent = App.getEntityComponent<components::RenderComponent>(entity);

        glm::vec4 *selectedEntityColor = nullptr;
        static components::Material *selectedMaterial = nullptr;
        static bool showMaterialInspector = false;
        if (renderComponent.type == components::RenderType::RENDER_3D)
        {
            auto renderable3D = std::dynamic_pointer_cast<components::Renderable3D>(renderComponent.renderable);
            if (renderable3D)
            {
            	selectedMaterial = &renderable3D->material;
            }
        }
        else if (renderComponent.type == components::RenderType::RENDER_2D)
        {
            auto renderable2D = std::dynamic_pointer_cast<components::Renderable2D>(renderComponent.renderable);
            if (renderable2D)
            {
                auto& [color, texture, sprite] = renderable2D->sprite;
                selectedEntityColor = &color;
            }
        }
        bool open = EntityPropertiesComponents::drawHeader("##RenderNode", "Render Component");
        static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
        static bool sectionOpen = true;

        if (open)
        {
            ImGui::SetWindowFontScale(1.15f);
        	ImGui::Text("Hide");
         	ImGui::SameLine(0, 12);
         	bool hidden = !renderComponent.isRendered;
            ImGui::Checkbox("##HideCheckBox", &hidden);
            renderComponent.isRendered = !hidden;

            EntityPropertiesComponents::drawToggleButtonWithSeparator("Material", &sectionOpen);
            static std::shared_ptr<renderer::Framebuffer> framebuffer = nullptr;
            static int entityBase = -1;
            if (sectionOpen)
            {
            	if (entityBase != entity)
				{
					//TODO: I guess all of this should be centralized in the assets
					utils::ScenePreviewOut previewParams;
					utils::genScenePreview("Modify material inspector", {64, 64}, entity, previewParams);
					auto &app = nexo::getApp();
					app.getSceneManager().getScene(previewParams.sceneId).setActiveStatus(false);
					auto &cameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(previewParams.cameraId);
					cameraComponent.clearColor = {0.05f, 0.05f, 0.05f, 0.0f};
					app.run(previewParams.sceneId, RenderingType::FRAMEBUFFER);
					framebuffer = cameraComponent.m_renderTarget;
					app.getSceneManager().deleteScene(previewParams.sceneId);
					entityBase = entity;
				}

                // --- Material Preview ---
                if (framebuffer->getColorAttachmentId(0) != 0)
                	ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(framebuffer->getColorAttachmentId(0))), ImVec2(64, 64), ImVec2(0, 1), ImVec2(1, 0));

                ImGui::SameLine();

                ImGui::BeginGroup();
                {
                    // --- Dropdown for Material Types ---
                    static int selectedMaterialIndex = 0;
                    const char* materialTypes[] = { "Default", "Metal", "Wood", "Plastic" };
                    ImGui::Combo("##MaterialType", &selectedMaterialIndex, materialTypes, IM_ARRAYSIZE(materialTypes));

                    // --- Material Action Buttons ---
                    if (ImGui::Button("Create new material"))
                    {
                    	popupManager.openPopup("Create new material");
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Modify Material"))
                    {
                    	RenderProperty::selectedMaterial = selectedMaterial;
                     	RenderProperty::showMaterialInspector = true;
                    }
                }
                ImGui::EndGroup();
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
            }

            ImGui::TreePop();
        }

        ImGui::SetNextWindowSize(ImVec2(1440,900));
        if (popupManager.showPopupModal("Create new material"))
        {
        	createMaterialPopup(entity);
         	popupManager.closePopup();
        }

        return true;
    }

    void RenderProperty::showEnd()
    {
        AEntityProperty::showEnd();
    }
}
