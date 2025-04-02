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

	void CameraProperty::show(const ecs::Entity entity)
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
            	std::vector<ImU32> textBadgeColors;


				if (const bool inactive = !cameraComponent.viewportLocked; EntityPropertiesComponents::drawRowDragFloat2("Viewport size", "W", "H", &viewPort.x, -FLT_MAX, FLT_MAX, 1.0f, badgeColors, textBadgeColors, inactive))
				{
					if (cameraComponent.viewportLocked)
						cameraComponent.resize(static_cast<unsigned int>(viewPort.x), static_cast<unsigned int>(viewPort.y));
				}

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
			ImGui::Text("Clear Color");
			ImGui::SameLine();
			Widgets::drawColorEditor("##ColorEditor Spot light", &cameraComponent.clearColor, &colorPickerMode, &showColorPicker);

        	ImGui::TreePop();
        }
	}
}
