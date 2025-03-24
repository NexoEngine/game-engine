//// CameraProperty.cpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Source file for the camera property class
//
///////////////////////////////////////////////////////////////////////////////

#include "CameraProperty.hpp"
#include "Components/EntityPropertiesComponents.hpp"
#include "Components/Widgets.hpp"
#include "components/Camera.hpp"
#include "IconsFontAwesome.h"
#include "Components/Components.hpp"

namespace nexo::editor {
	/**
	 * @brief Displays and updates the camera properties UI.
	 *
	 * Retrieves the camera component from the given ECS entity and renders a user interface for adjusting
	 * camera settings. The UI includes controls for modifying viewport dimensions (with a lock toggle), field of view,
	 * near plane, far plane, and the clear color using ImGui.
	 *
	 * @param entity The entity containing the camera component.
	 * @return true Always returns true to indicate the UI was rendered.
	 */
	bool CameraProperty::show(const ecs::Entity entity)
	{
        auto& cameraComponent = Application::getEntityComponent<components::CameraComponent>(entity);

		if (EntityPropertiesComponents::drawHeader("##CameraNode", "Camera"))
		{
			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
			if (ImGui::BeginTable("InspectorViewPortParams", 4,
				ImGuiTableFlags_SizingStretchProp))
			{
				ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
				ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
				ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
				ImGui::TableSetupColumn("##Lock", ImGuiTableColumnFlags_WidthFixed, 25.0f);
   				glm::vec2 viewPort = {cameraComponent.width, cameraComponent.height};
       			std::vector<ImU32> badgeColors;
          		badgeColors.reserve(2);
          		badgeColors.push_back(IM_COL32(50, 50, 50, 255));
          		badgeColors.push_back(IM_COL32(50, 50, 50, 255));
            	std::vector<ImU32> textBadgeColors;
             	textBadgeColors.reserve(2);
             	textBadgeColors.push_back(IM_COL32(200, 200, 200, 255));
             	textBadgeColors.push_back(IM_COL32(200, 200, 200, 255));


				if (const bool inactive = !cameraComponent.viewportLocked; EntityPropertiesComponents::drawRowDragFloat2("Viewport size", "W", "H", &viewPort.x, -FLT_MAX, FLT_MAX, 1.0f, badgeColors, textBadgeColors, inactive))
				{
					if (cameraComponent.viewportLocked)
						cameraComponent.resize(static_cast<unsigned int>(viewPort.x), static_cast<unsigned int>(viewPort.y));
				}

				ImGui::TableSetColumnIndex(3);

				constexpr float lockButtonWidth = 25;
				constexpr float lockButtonHeight = 25;
				const std::string icon = (cameraComponent.viewportLocked) ? std::string(ICON_FA_LOCK) : std::string(ICON_FA_UNLOCK);

				const float fontSize = ImGui::GetFontSize();
				const float verticalPadding = (lockButtonHeight - fontSize) * 0.5f;
				ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, verticalPadding + 2)); // Slightly add more textPaddingding to center the cog icon

				// Cog button
				if (const std::string viewportSettings = "##ViewPortSettings"; Components::drawButton(
					icon + viewportSettings,
					ImVec2(lockButtonWidth, lockButtonHeight),
					IM_COL32(60, 60, 60, 255), IM_COL32(80, 80, 80, 255),
					IM_COL32(100, 100, 100, 255), IM_COL32(255, 255, 255, 255)))
				{
					cameraComponent.viewportLocked = !cameraComponent.viewportLocked;
				}
				ImGui::PopStyleVar();


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
			ImGui::Text("Clear Color");
			ImGui::SameLine();
			Widgets::drawColorEditor("##ColorEditor Spot light", &cameraComponent.clearColor, &colorPickerMode, &showColorPicker);

        	ImGui::TreePop();
        }
        return true;
	}
}
