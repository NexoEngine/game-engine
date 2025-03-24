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
#include "DocumentWindows/InspectorWindow.hpp"
#include "DocumentWindows/MaterialInspector.hpp"

namespace nexo::editor {
    /**
     * @brief Renders a popup interface for creating a new material.
     *
     * This function draws a two-column popup using ImGui where the left panel serves as a material inspector
     * for entering and modifying material properties, and the right panel displays a live preview of the new material.
     * A static scene preview is generated on the first call if it has not already been created. The popup includes
     * bottom-centered "OK" and "Cancel" buttons; confirming applies the new material to the entity's render component
     * and cleans up the preview, while canceling simply deletes the preview scene.
     *
     * @param entity The entity for which the new material is being created.
     */
    void RenderProperty::createMaterialPopup(ecs::Entity entity) const
    {
        ImGui::Text("Create New Material");
        ImGui::Separator();

	    const ImVec2 availSize = ImGui::GetContentRegionAvail();
	    const float totalWidth = availSize.x;
        float totalHeight = availSize.y - 40; // Reserve space for bottom buttons

        // Define layout: 60% for inspector, 40% for preview
	    const float inspectorWidth = totalWidth * 0.4f;
	    const float previewWidth = totalWidth - inspectorWidth - 8; // Subtract spacing between panels

        static utils::ScenePreviewOut scenePreviewInfo;
        if (!scenePreviewInfo.sceneGenerated)
        {
        	utils::genScenePreview("New Material Preview", {previewWidth - 8, totalHeight}, entity, scenePreviewInfo);
        }
        auto renderable3D = std::dynamic_pointer_cast<components::Renderable3D>(nexo::Application::m_coordinator->getComponent<components::RenderComponent>(scenePreviewInfo.entityCopy).renderable);

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
            auto const &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(scenePreviewInfo.cameraId);
            const unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);

            const float aspectRatio = static_cast<float>(cameraComponent.width) /
                                      static_cast<float>(cameraComponent.height);

            const float displayHeight = totalHeight - 20;
            const float displayWidth = displayHeight * aspectRatio;

            ImGui::SetCursorPos(ImVec2(ImGui::GetCursorPosX() + 4, ImGui::GetCursorPosY() + 4));
            ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)),
                        ImVec2(displayWidth, displayHeight), ImVec2(0, 1), ImVec2(1, 0));

            ImGui::EndChild();
        }

        ImGui::Columns(1);
        ImGui::Spacing();

        // Bottom buttons - centered
	    constexpr float buttonWidth = 120.0f;

        if (ImGui::Button("OK", ImVec2(buttonWidth, 0)))
        {
            // TODO: Insert logic to create the new material

            // Clean up preview scene
            if (scenePreviewInfo.sceneGenerated)
            {
                auto &app = getApp();
                auto &renderComponentBase = nexo::Application::m_coordinator->getComponent<components::RenderComponent>(entity);
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

    /**
     * @brief Renders the UI for managing an entity's render properties.
     *
     * This function displays a properties panel for the render component of the specified entity.
     * For 3D render components, it updates the material inspector with the entity's material data.
     * The UI includes a header with a visibility toggle, a section for a material preview, a dropdown
     * for selecting material types, and buttons to create or modify the material. A popup is shown
     * for creating a new material, and material handling for 2D components is not yet implemented.
     *
     * @param entity The entity whose render properties are being managed.
     * @return true Always returns true after rendering the UI.
     *
     * @note The function uses static variables to maintain UI state (e.g., section visibility and preview data).
     */
    bool RenderProperty::show(ecs::Entity entity)
    {
        auto& renderComponent = Application::getEntityComponent<components::RenderComponent>(entity);

        if (renderComponent.type == components::RenderType::RENDER_3D)
        {
            auto renderable3D = std::dynamic_pointer_cast<components::Renderable3D>(renderComponent.renderable);
            if (renderable3D)
            {
            	m_inspector.setSubInspectorData<MaterialInspector>(&renderable3D->material);
            }
        }
        else if (renderComponent.type == components::RenderType::RENDER_2D)
        {
			//TODO: Implement sprite stuff
        }
    	static bool sectionOpen = true;

        if (EntityPropertiesComponents::drawHeader("##RenderNode", "Render Component"))
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
            	if (entityBase != static_cast<int>(entity))
				{
					//TODO: I guess all of this should be centralized in the assets
					utils::ScenePreviewOut previewParams;
					utils::genScenePreview("Modify material inspector", {64, 64}, entity, previewParams);
					auto &app = nexo::getApp();
					app.getSceneManager().getScene(previewParams.sceneId).setActiveStatus(false);
					auto &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(previewParams.cameraId);
					cameraComponent.clearColor = {0.05f, 0.05f, 0.05f, 0.0f};
					app.run(previewParams.sceneId, RenderingType::FRAMEBUFFER);
					framebuffer = cameraComponent.m_renderTarget;
					app.getSceneManager().deleteScene(previewParams.sceneId);
					entityBase = static_cast<int>(entity);
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
                    	m_popupManager.openPopup("Create new material");
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Modify Material"))
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

        ImGui::SetNextWindowSize(ImVec2(1440,900));
        if (m_popupManager.showPopupModal("Create new material"))
        {
        	createMaterialPopup(entity);
         	m_popupManager.closePopup();
        }

        return true;
    }
}
