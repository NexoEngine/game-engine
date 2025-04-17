//// Widgets.cpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/02/2025
//  Description: Source file for the widgets components
//
///////////////////////////////////////////////////////////////////////////////

#include "Widgets.hpp"

#include <imgui.h>
#include <imgui_internal.h>
#include <Logger.hpp>
#include "Path.hpp"

#include "Components.hpp"
#include "Definitions.hpp"
#include "IconsFontAwesome.h"
#include "Nexo.hpp"
#include "Texture.hpp"
#include "components/Camera.hpp"
#include "EntityPropertiesComponents.hpp"
#include "CameraFactory.hpp"
#include "components/Render3D.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "tinyfiledialogs.h"
#include "context/Selector.hpp"

namespace nexo::editor {
	bool Widgets::drawColorEditor(
			const std::string &label,
			glm::vec4 *selectedEntityColor,
			ImGuiColorEditFlags *colorPickerMode,
			bool *showPicker,
			const ImGuiColorEditFlags colorButtonFlags
	) {
		const ImGuiStyle &style = ImGui::GetStyle();
        const ImVec2 contentAvailable = ImGui::GetContentRegionAvail();
        bool colorModified = false;

        const std::string colorButton = std::string("##ColorButton") + label;

		const ImVec2 cogIconSize = ImGui::CalcTextSize(ICON_FA_COG);
		const ImVec2 cogIconPadding = style.FramePadding;
		const ImVec2 itemSpacing = style.ItemSpacing;

        // Color button
        Components::drawColorButton(
           	colorButton,
           	ImVec2(contentAvailable.x - cogIconSize.x - cogIconPadding.x * 2 - itemSpacing.x, 0), // Make room for the cog button
           	ImVec4(selectedEntityColor->x, selectedEntityColor->y, selectedEntityColor->z, selectedEntityColor->w),
            showPicker,
            colorButtonFlags
        );

        ImGui::SameLine();

		const std::string pickerSettings = std::string("##PickerSettings") + label;
        const std::string colorPickerPopup = std::string("##ColorPickerPopup") + label;

		// Cog button
        if (Components::drawButton(std::string(ICON_FA_COG) + pickerSettings)) {
           	ImGui::OpenPopup(colorPickerPopup.c_str());
        }

        if (ImGui::BeginPopup(colorPickerPopup.c_str()))
        {
            ImGui::Text("Picker Mode:");
            if (ImGui::RadioButton("Hue Wheel", *colorPickerMode == ImGuiColorEditFlags_PickerHueWheel))
                *colorPickerMode = ImGuiColorEditFlags_PickerHueWheel;
            if (ImGui::RadioButton("Hue bar", *colorPickerMode == ImGuiColorEditFlags_PickerHueBar))
                *colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
            ImGui::EndPopup();
        }

        const std::string colorPickerInline = std::string("##ColorPickerInline") + label;
        if (*showPicker)
        {
            ImGui::Spacing();
            colorModified = ImGui::ColorPicker4(colorPickerInline.c_str(),
                                reinterpret_cast<float*>(selectedEntityColor), *colorPickerMode);
        }
        return colorModified;
	}

	bool Widgets::drawTextureButton(const std::string &label, std::shared_ptr<renderer::Texture2D> &texture)
	{
		bool textureModified = false;
		constexpr ImVec2 previewSize(32, 32);
        ImGui::PushID(label.c_str());

        const ImTextureID textureId = texture ? static_cast<ImTextureID>(static_cast<intptr_t>(texture->getId())) : 0;
        const std::string textureButton = std::string("##TextureButton") + label;

        if (ImGui::ImageButton(textureButton.c_str(), textureId, previewSize))
        {
            const char* filePath = tinyfd_openFileDialog(
                "Open Texture",
                "",
                0,
                nullptr,
                nullptr,
                0
            );

            if (filePath)
            {
	            const std::string path(filePath);
                std::shared_ptr<renderer::Texture2D> newTexture = renderer::Texture2D::create(path);
                if (newTexture)
                {
                    texture = newTexture;
                    textureModified = true;
                }
            }
        }
        Components::drawButtonBorder(IM_COL32(255,255,255,0), IM_COL32(255,255,255,255), IM_COL32(255,255,255,0), 0.0f, 0, 2.0f);
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::Text("%s", label.c_str());
		return textureModified;
	}

	bool Widgets::drawMaterialInspector(components::Material *material)
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
		modified = Widgets::drawTextureButton("Albedo texture", material->albedoTexture) || modified;
		ImGui::SameLine();
		modified = Widgets::drawColorEditor("##ColorEditor Albedo texture", &material->albedoColor, &colorPickerModeAlbedo, &showColorPickerAlbedo) || modified;

		// --- Specular texture ---
		static ImGuiColorEditFlags colorPickerModeSpecular = ImGuiColorEditFlags_PickerHueBar;
		static bool showColorPickerSpecular = false;
		modified = Widgets::drawTextureButton("Specular texture", material->metallicMap) || modified;
		ImGui::SameLine();
		modified = Widgets::drawColorEditor("##ColorEditor Specular texture", &material->specularColor, &colorPickerModeSpecular, &showColorPickerSpecular) || modified;
		return modified;
	}

	static ecs::Entity createDefaultPerspectiveCamera(const scene::SceneId sceneId, ImVec2 sceneViewportSize)
	{
        auto &app = getApp();
        renderer::FramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            renderer::FrameBufferTextureFormats::RGBA8, renderer::FrameBufferTextureFormats::RED_INTEGER, renderer::FrameBufferTextureFormats::Depth
        };
        const ImVec2 availSize = ImGui::GetContentRegionAvail();
        const float totalWidth = availSize.x;
        float totalHeight = availSize.y - 40; // Reserve space for bottom buttons

        // Define layout: 60% for inspector, 40% for preview
        const float inspectorWidth = totalWidth * 0.4f;
        const float previewWidth = totalWidth - inspectorWidth - 8; // Subtract spacing between panel
        framebufferSpecs.width = static_cast<unsigned int>(sceneViewportSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(sceneViewportSize.y);
        const auto renderTarget = renderer::Framebuffer::create(framebufferSpecs);
        ecs::Entity defaultCamera = CameraFactory::createPerspectiveCamera({0.0f, 0.0f, -5.0f}, static_cast<unsigned int>(sceneViewportSize.x), static_cast<unsigned int>(sceneViewportSize.y), renderTarget);
        app.getSceneManager().getScene(sceneId).addEntity(static_cast<ecs::Entity>(defaultCamera));

        components::Material billboardMat{};
        std::shared_ptr<renderer::Texture2D> cameraIconTexture = renderer::Texture2D::create(Path::resolvePathRelativeToExe("../resources/textures/cameraIcon.png").string());
        billboardMat.albedoTexture = cameraIconTexture;
        auto billboard = std::make_shared<components::BillBoard>();
        auto renderable = std::make_shared<components::Renderable3D>(billboardMat, billboard);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);
        app.m_coordinator->addComponent(defaultCamera, renderComponent);
        return defaultCamera;
	}

	void Widgets::drawTransformProperties(components::TransformComponent &transformComponent, glm::vec3 &lastDisplayedEuler)
	{
	    // Increase cell padding so rows have more space:
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
        auto& [pos, size, quat] = transformComponent;

        if (ImGui::BeginTable("InspectorTransformTable", 4,
            ImGuiTableFlags_SizingStretchProp))
        {
            // Only the first column has a fixed width
            ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
            ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
            ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
            ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

            EntityPropertiesComponents::drawRowDragFloat3("Position", "X", "Y", "Z", &pos.x);

            const glm::vec3 computedEuler = math::customQuatToEuler(quat);

            lastDisplayedEuler = computedEuler;
            glm::vec3 rotation = lastDisplayedEuler;

            // Draw the Rotation row.
            // When the user edits the rotation, we compute the delta from the last displayed Euler,
            // convert that delta into an incremental quaternion, and update the master quaternion.
            if (EntityPropertiesComponents::drawRowDragFloat3("Rotation", "X", "Y", "Z", &rotation.x)) {
                const glm::vec3 deltaEuler = rotation - lastDisplayedEuler;
                const glm::quat deltaQuat = glm::radians(deltaEuler);
                quat = glm::normalize(deltaQuat * quat);
                lastDisplayedEuler = math::customQuatToEuler(quat);
                rotation = lastDisplayedEuler;
            }
            EntityPropertiesComponents::drawRowDragFloat3("Scale", "X", "Y", "Z", &size.x);

            ImGui::EndTable();
        }
        ImGui::PopStyleVar();
	}

	void Widgets::drawCameraProperties(components::CameraComponent &cameraComponent)
	{
	    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
        if (ImGui::BeginTable("CameraInspectorViewPortParams", 4,
     			ImGuiTableFlags_SizingStretchProp))
        {
     			ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
     			ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
     			ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
     			ImGui::TableSetupColumn("##Lock", ImGuiTableColumnFlags_WidthStretch);
                    				glm::vec2 viewPort = {cameraComponent.width, cameraComponent.height};
                     			std::vector<ImU32> badgeColors;
                           	std::vector<ImU32> textBadgeColors;

     			const bool disabled = cameraComponent.viewportLocked;
     			if (disabled)
        				ImGui::BeginDisabled();
     			if (EntityPropertiesComponents::drawRowDragFloat2("Viewport size", "W", "H", &viewPort.x, -FLT_MAX, FLT_MAX, 1.0f, badgeColors, textBadgeColors, disabled))
     			{
        				if (!cameraComponent.viewportLocked)
        				cameraComponent.resize(static_cast<unsigned int>(viewPort.x), static_cast<unsigned int>(viewPort.y));
     			}
     			if (disabled)
        				ImGui::EndDisabled();

     			ImGui::TableSetColumnIndex(3);

     			// Lock button
     			const std::string lockBtnLabel = cameraComponent.viewportLocked ? ICON_FA_LOCK "##ViewPortSettings" : ICON_FA_UNLOCK "##ViewPortSettings";
     			if (Components::drawButton(lockBtnLabel)) {
        				cameraComponent.viewportLocked = !cameraComponent.viewportLocked;
     			}


     			ImGui::EndTable();
        }

        if (ImGui::BeginTable("InspectorCameraVariables", 2, ImGuiTableFlags_SizingStretchProp))
                            {
     			ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
     			ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

     			EntityPropertiesComponents::drawRowDragFloat1("FOV", "", &cameraComponent.fov, 30.0f, 120.0f, 0.3f);
     			EntityPropertiesComponents::drawRowDragFloat1("Near plane", "", &cameraComponent.nearPlane, 0.01f, 1.0f, 0.001f);
     			EntityPropertiesComponents::drawRowDragFloat1("Far plane", "", &cameraComponent.farPlane, 100.0f, 10000.0f, 1.0f);

                                ImGui::EndTable();
                            }


        ImGui::PopStyleVar();

        ImGui::Spacing();
        static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
        static bool showColorPicker = false;
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Clear Color");
        ImGui::SameLine();
        Widgets::drawColorEditor("##ColorEditor Spot light", &cameraComponent.clearColor, &colorPickerMode, &showColorPicker);
	}

	bool Widgets::drawCameraCreator(const scene::SceneId sceneId, ImVec2 sceneViewportSize)
	{
	    auto &app = getApp();

        const ImVec2 availSize = ImGui::GetContentRegionAvail();
        const float totalWidth = availSize.x;
        float totalHeight = availSize.y - 40; // Reserve space for bottom buttons

        // Define layout: 60% for inspector, 40% for preview
        const float inspectorWidth = totalWidth * 0.4f;
        const float previewWidth = totalWidth - inspectorWidth - 8; // Subtract spacing between panels
        static ecs::Entity camera = ecs::MAX_ENTITIES;
        if (camera == ecs::MAX_ENTITIES)
        {
            camera = createDefaultPerspectiveCamera(sceneId, ImVec2(previewWidth, totalHeight));
        }

        static char cameraName[128] = "";
        static bool nameIsEmpty = false;
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

            if (EntityPropertiesComponents::drawHeader("##CameraNode", "Camera"))
            {
                auto &cameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(camera);
                cameraComponent.render = true;
                Widgets::drawCameraProperties(cameraComponent);
                ImGui::TreePop();
            }

            ImGui::Spacing();
            ImGui::Spacing();
            ImGui::Spacing();

            if (EntityPropertiesComponents::drawHeader("##TransformNode", "Transform Component"))
            {
                static glm::vec3 lastDisplayedEuler(0.0f);
                auto &transformComponent = app.m_coordinator->getComponent<components::TransformComponent>(camera);
                Widgets::drawTransformProperties(transformComponent, lastDisplayedEuler);
                ImGui::TreePop();
            }

            ImGui::EndChild();
        }
        ImGui::NextColumn();
        // --- Right Side: Camera Preview ---
        {
            ImGui::BeginChild("CameraPreview", ImVec2(previewWidth - 4, totalHeight), true);

            auto &app = getApp();
            app.run(sceneId, RenderingType::FRAMEBUFFER);
            auto const &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(camera);
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
            auto &selector = Selector::get();
            auto &uuid = app.m_coordinator->getComponent<components::UuidComponent>(camera);
            auto &cameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(camera);
            cameraComponent.active = false;
            selector.setUiHandle(uuid.uuid, std::string(ICON_FA_CAMERA "  ") + cameraName);
            camera = ecs::MAX_ENTITIES;
            cameraName[0] = '\0';
            ImGui::CloseCurrentPopup();
            return true;
        }
        ImGui::SameLine();
        if (ImGui::Button("Cancel", ImVec2(buttonWidth, 0)))
        {
            nameIsEmpty = false;
            app.deleteEntity(camera);
            camera = ecs::MAX_ENTITIES;
            cameraName[0] = '\0';
            ImGui::CloseCurrentPopup();
            return true;
        }
        return false;
	}
}
