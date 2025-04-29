//// Panels.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        18/04/2025
//  Description: Source file for the ui panels
//
///////////////////////////////////////////////////////////////////////////////

#include "Nexo.hpp"
#include "Panels.hpp"
#include "Elements.hpp"
#include "Widgets.hpp"
#include "EntityProperties.hpp"
#include "CameraFactory.hpp"
#include "Path.hpp"
#include "IconsFontAwesome.h"
#include "components/Uuid.hpp"
#include "context/Selector.hpp"
#include "utils/EditorProps.hpp"

namespace ImNexo {
    bool MaterialInspector(nexo::components::Material *material)
	{
		bool modified = false;
		// --- Shader Selection ---
		ImGui::BeginGroup();
		{
			ImGui::Text("Shader:");
			ImGui::SameLine();

			static int currentShaderIndex = 0;
			const char* shaderOptions[] = { "Standard", "Unlit", "CustomPBR" };
			const float availableWidth = ImGui::GetContentRegionAvail().x;
			ImGui::SetNextItemWidth(availableWidth);

			if (ImGui::Combo("##ShaderCombo", &currentShaderIndex, shaderOptions, IM_ARRAYSIZE(shaderOptions)))
			{
				//TODO: implement shader selection
			}
		}
		ImGui::EndGroup();
		ImGui::Spacing();

		// --- Rendering mode selection ---
		ImGui::Text("Rendering mode:");
		ImGui::SameLine();
		static int currentRenderingModeIndex = 0;
		const char* renderingModeOptions[] = { "Opaque", "Transparent", "Refraction" };
		float availableWidth = ImGui::GetContentRegionAvail().x;

		ImGui::SetNextItemWidth(availableWidth);
		if (ImGui::Combo("##RenderingModeCombo", &currentRenderingModeIndex, renderingModeOptions, IM_ARRAYSIZE(renderingModeOptions)))
		{
			//TODO: implement rendering mode
		}

	    // --- Albedo texture ---
	    static ImGuiColorEditFlags colorPickerModeAlbedo = ImGuiColorEditFlags_PickerHueBar;
	    static bool showColorPickerAlbedo = false;
		modified = TextureButton("Albedo texture", material->albedoTexture) || modified;
		ImGui::SameLine();
		modified = ColorEditor("##ColorEditor Albedo texture", &material->albedoColor, &colorPickerModeAlbedo, &showColorPickerAlbedo) || modified;

		// --- Specular texture ---
		static ImGuiColorEditFlags colorPickerModeSpecular = ImGuiColorEditFlags_PickerHueBar;
		static bool showColorPickerSpecular = false;
		modified = TextureButton("Specular texture", material->metallicMap) || modified;
		ImGui::SameLine();
		modified = ColorEditor("##ColorEditor Specular texture", &material->specularColor, &colorPickerModeSpecular, &showColorPickerSpecular) || modified;
		return modified;
	}

    /**
    * @brief Creates a default perspective camera for the camera inspector preview.
    *
    * Sets up a perspective camera with a framebuffer for rendering the preview view.
    * Also adds a billboard with a camera icon for visualization in the scene.
    *
    * @param sceneId The ID of the scene where the camera should be created
    * @param sceneViewportSize The dimensions to use for the camera's framebuffer
    * @return The entity ID of the created camera
    */
	static nexo::ecs::Entity createDefaultPerspectiveCamera(const nexo::scene::SceneId sceneId, ImVec2 sceneViewportSize)
	{
        auto &app = nexo::getApp();
        nexo::renderer::FramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            nexo::renderer::FrameBufferTextureFormats::RGBA8, nexo::renderer::FrameBufferTextureFormats::RED_INTEGER, nexo::renderer::FrameBufferTextureFormats::Depth
        };
        const ImVec2 availSize = ImGui::GetContentRegionAvail();
        const float totalWidth = availSize.x;
        float totalHeight = availSize.y - 40; // Reserve space for bottom buttons

        // Define layout: 60% for inspector, 40% for preview
        const float inspectorWidth = totalWidth * 0.4f;
        const float previewWidth = totalWidth - inspectorWidth - 8; // Subtract spacing between panel
        framebufferSpecs.width = static_cast<unsigned int>(sceneViewportSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(sceneViewportSize.y);
        const auto renderTarget = nexo::renderer::Framebuffer::create(framebufferSpecs);
        nexo::ecs::Entity defaultCamera = nexo::CameraFactory::createPerspectiveCamera({0.0f, 0.0f, -5.0f}, static_cast<unsigned int>(sceneViewportSize.x), static_cast<unsigned int>(sceneViewportSize.y), renderTarget);
        app.getSceneManager().getScene(sceneId).addEntity(static_cast<nexo::ecs::Entity>(defaultCamera));
        nexo::editor::utils::addPropsTo(defaultCamera, nexo::editor::utils::PropsType::CAMERA);
        return defaultCamera;
	}

	bool CameraInspector(const nexo::scene::SceneId sceneId, ImVec2 sceneViewportSize)
	{
	    auto &app = nexo::getApp();

        const ImVec2 availSize = ImGui::GetContentRegionAvail();
        const float totalWidth = availSize.x;
        float totalHeight = availSize.y - 40; // Reserve space for bottom buttons

        // Define layout: 60% for inspector, 40% for preview
        const float inspectorWidth = totalWidth * 0.4f;
        const float previewWidth = totalWidth - inspectorWidth - 8; // Subtract spacing between panels
        static nexo::ecs::Entity camera = nexo::ecs::MAX_ENTITIES;
        if (camera == nexo::ecs::MAX_ENTITIES)
        {
            camera = createDefaultPerspectiveCamera(sceneId, ImVec2(previewWidth, totalHeight));
        }

        static char cameraName[128] = "";
        static bool nameIsEmpty = false;
        static bool closingPopup = false;
        static bool result = false;

        // We do this since imgui seems to render once more the popup, so we need to wait one frame before deleting
        // the render target
        if (closingPopup) {
            // Now it's safe to delete the entity
            auto &app = nexo::getApp();
            app.deleteEntity(camera);

            camera = nexo::ecs::MAX_ENTITIES;
            cameraName[0] = '\0';
            nameIsEmpty = false;
            closingPopup = false;

            return true;
        }
        ImGui::Columns(2, "CameraCreatorColumns", false);

        ImGui::SetColumnWidth(0, inspectorWidth);
        // --- Left Side: Camera Inspector ---
        {
            ImGui::BeginChild("CameraInspector", ImVec2(inspectorWidth - 4, totalHeight), true);
            ImGui::AlignTextToFramePadding();
            ImGui::Text("Name");
            ImGui::SameLine();
            if (nameIsEmpty) {
                ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
                ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.0f);
            }
            ImGui::InputText("##CameraName", cameraName, IM_ARRAYSIZE(cameraName));
            if (nameIsEmpty) {
                ImGui::PopStyleVar();
                ImGui::PopStyleColor();

                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
                ImGui::TextWrapped("Name is empty");
                ImGui::PopStyleColor();
                ImGui::Spacing();
            } else {
                ImGui::Spacing();
            }
            if (nameIsEmpty && cameraName[0] != '\0')
                nameIsEmpty = false;
            ImGui::Spacing();

            if (Header("##CameraNode", "Camera"))
            {
                auto &cameraComponent = app.m_coordinator->getComponent<nexo::components::CameraComponent>(camera);
                cameraComponent.render = true;
                Camera(cameraComponent);
                ImGui::TreePop();
            }

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();

            if (Header("##TransformNode", "Transform Component"))
            {
                static glm::vec3 lastDisplayedEuler(0.0f);
                auto &transformComponent = app.m_coordinator->getComponent<nexo::components::TransformComponent>(camera);
                Transform(transformComponent, lastDisplayedEuler);
                ImGui::TreePop();
            }

            if (app.m_coordinator->entityHasComponent<nexo::components::PerspectiveCameraTarget>(camera) &&
                Header("##PerspectiveCameraTarget", "Camera Target Component"))
            {
                auto &cameraTargetComponent = app.m_coordinator->getComponent<nexo::components::PerspectiveCameraTarget>(camera);
                CameraTarget(cameraTargetComponent);
                ImGui::TreePop();
            }

            if (app.m_coordinator->entityHasComponent<nexo::components::PerspectiveCameraController>(camera) &&
                Header("##PerspectiveCameraController", "Camera Controller Component"))
            {
                auto &cameraControllerComponent = app.m_coordinator->getComponent<nexo::components::PerspectiveCameraController>(camera);
                CameraController(cameraControllerComponent);
                ImGui::TreePop();
            }

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();
            // Add Component button
            const float buttonWidth = inspectorWidth - 16;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(4, 4));
            float centeredX = (inspectorWidth - buttonWidth) * 0.5f;
            ImGui::SetCursorPosX(centeredX);

            // Static variables for state tracking
            static bool showComponentSelector = false;
            static float animProgress = 0.0f;
            static float lastClickTime = 0.0f;

            // Button with arrow indicating state
            std::string buttonText = "Add Component " + std::string(showComponentSelector ? ICON_FA_CHEVRON_UP : ICON_FA_CHEVRON_DOWN);

            if (Button(buttonText.c_str(), ImVec2(buttonWidth, 0)))
            {
                showComponentSelector = !showComponentSelector;
                if (showComponentSelector) {
                    lastClickTime = ImGui::GetTime();
                    animProgress = 0.0f;
                }
            }
            ImGui::PopStyleVar();

            // Component selector with just two options
            if (showComponentSelector)
            {
                // Animation calculation
                const float animDuration = 0.25f;
                float timeSinceClick = ImGui::GetTime() - lastClickTime;
                animProgress = std::min(timeSinceClick / animDuration, 1.0f);

                // Simplified component grid with compact layout
                const float maxGridHeight = 90.0f;
                const float currentHeight = maxGridHeight * animProgress;

                // Create child window for components with animated height
                ImGui::PushStyleVar(ImGuiStyleVar_ChildRounding, 3.0f);
                ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 4)); // Reduce spacing between items
                ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(8, 8)); // Better padding inside items

                ImGui::BeginChild("ComponentSelector", ImVec2(buttonWidth, currentHeight), 0, ImGuiWindowFlags_NoScrollbar);

                if (animProgress > 0.5f)
                {
                    // Center elements horizontally with proper spacing
                    const float itemSize = 75.0f;

                    // Draw component buttons side-by-side with controlled spacing
                    ImGui::BeginGroup();

                    if (!app.m_coordinator->entityHasComponent<nexo::components::PerspectiveCameraTarget>(camera) &&
                        !app.m_coordinator->entityHasComponent<nexo::components::PerspectiveCameraController>(camera) &&
                        ButtonWithIconAndText("camera_target", ICON_FA_CAMERA, "Camera target", ImVec2(75.0f, 75.0f)))
                    {
                        nexo::components::PerspectiveCameraTarget cameraTarget{};
                        app.m_coordinator->addComponent(camera, cameraTarget);
                        showComponentSelector = false;
                    }
                    ImGui::SameLine();
                    if (!app.m_coordinator->entityHasComponent<nexo::components::PerspectiveCameraTarget>(camera) &&
                        !app.m_coordinator->entityHasComponent<nexo::components::PerspectiveCameraController>(camera) &&
                        ButtonWithIconAndText("camera_controller", ICON_FA_GAMEPAD, "Camera Controller", ImVec2(75.0f, 75.0f)))
                    {
                        nexo::components::PerspectiveCameraController cameraController{};
                        app.m_coordinator->addComponent(camera, cameraController);
                        showComponentSelector = false;
                    }
                    ImGui::EndGroup();
                }

                ImGui::EndChild();
                ImGui::PopStyleVar(3);

                // Reset animation if needed
                if (!showComponentSelector && animProgress >= 1.0f) {
                    animProgress = 0.0f;
                }
            }

            ImGui::EndChild(); // End CameraInspector
        }
        ImGui::NextColumn();
        // --- Right Side: Camera Preview ---
        {
            ImGui::BeginChild("CameraPreview", ImVec2(previewWidth - 4, totalHeight), true);

            auto &app = nexo::getApp();
            app.run(sceneId, nexo::RenderingType::FRAMEBUFFER);
            auto const &cameraComponent = nexo::Application::m_coordinator->getComponent<nexo::components::CameraComponent>(camera);
            const unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);

            const float displayHeight = totalHeight - 20;
            const float displayWidth = displayHeight;

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
            if (cameraName[0] == '\0') {
                nameIsEmpty = true;
                return false;
            }
            nameIsEmpty = false;
            auto &selector = nexo::editor::Selector::get();
            auto &uuid = app.m_coordinator->getComponent<nexo::components::UuidComponent>(camera);
            auto &cameraComponent = app.m_coordinator->getComponent<nexo::components::CameraComponent>(camera);
            cameraComponent.active = false;
            selector.setUiHandle(uuid.uuid, std::string(ICON_FA_CAMERA "  ") + cameraName);
            camera = nexo::ecs::MAX_ENTITIES;
            cameraName[0] = '\0';
            ImGui::CloseCurrentPopup();
            return true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 0)))
        {
            ImGui::CloseCurrentPopup();
            closingPopup = true;
            return false;
        }
        return false;
	}
}
