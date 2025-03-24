//// CameraController.cpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Source file for the camera controller
//
///////////////////////////////////////////////////////////////////////////////

#include "CameraController.hpp"
#include "components/Camera.hpp"
#include "Components/EntityPropertiesComponents.hpp"

namespace nexo::editor {
	/**
	 * @brief Renders the camera controller UI.
	 *
	 * Retrieves the PerspectiveCameraController component from the specified entity and displays its settings,
	 * including a control for adjusting mouse sensitivity, using an ImGui layout.
	 *
	 * @param entity The entity that holds the camera controller component.
	 * @return true after processing the UI elements.
	 */
	bool CameraController::show(const ecs::Entity entity)
	{
        auto& controllerComponent = Application::getEntityComponent<components::PerspectiveCameraController>(entity);

        if (EntityPropertiesComponents::drawHeader("##ControllerNode", "Camera Controller"))
        {
       		ImGui::Spacing();

			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
   			if (ImGui::BeginTable("InspectorControllerTable", 2,
                ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

                EntityPropertiesComponents::drawRowDragFloat1("Mouse sensitivity", "", &controllerComponent.mouseSensitivity);

                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        	ImGui::TreePop();
        }
        return true;
	}
}
