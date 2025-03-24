//// PointLightProperty.cpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Source file for the point light property
//
///////////////////////////////////////////////////////////////////////////////

#include "PointLightProperty.hpp"
#include "Components/EntityPropertiesComponents.hpp"
#include "Components/Widgets.hpp"
#include "components/Light.hpp"
#include "math/Light.hpp"

namespace nexo::editor {


	bool PointLightProperty::show(const ecs::Entity entity)
	{
        auto& pointComponent = nexo::Application::getEntityComponent<components::PointLightComponent>(entity);

        if (EntityPropertiesComponents::drawHeader("##PointNode", "Point light"))
        {
       		ImGui::Spacing();
        	static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
			static bool showColorPicker = false;
			ImGui::Text("Color");
			ImGui::SameLine();
			glm::vec4 color = {pointComponent.color, 1.0f};
			Widgets::drawColorEditor("##ColorEditor Point light", &color, &colorPickerMode, &showColorPicker);
			pointComponent.color = color;

			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
   			if (ImGui::BeginTable("InspectorPointTable", 4,
                ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

                EntityPropertiesComponents::drawRowDragFloat3("Position", "X", "Y", "Z", &pointComponent.pos.x);

                ImGui::EndTable();
            }

            ImGui::Spacing();
            ImGui::Text("Distance");
            ImGui::SameLine();
            if (ImGui::DragFloat("##DistanceSlider", &pointComponent.maxDistance, 1.0f, 1.0f, 3250.0f))
            {
                // Recompute the attenuation from the distance
                auto [lin, quad] = math::computeAttenuationFromDistance(pointComponent.maxDistance);
                pointComponent.constant = 1.0f;
                pointComponent.linear = lin;
                pointComponent.quadratic = quad;
            }
            ImGui::PopStyleVar();
        	ImGui::TreePop();
        }
        return true;
	}
}
