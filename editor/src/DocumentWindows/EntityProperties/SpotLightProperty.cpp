//// SpotLightProperty.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Source file for the spot light property class
//
///////////////////////////////////////////////////////////////////////////////

#include "SpotLightProperty.hpp"
#include "Components/EntityPropertiesComponents.hpp"
#include "Components/Widgets.hpp"
#include "components/Light.hpp"
#include "math/Light.hpp"

namespace nexo::editor {
	int SpotLightProperty::show(ecs::Entity entity)
	{
		auto const& App = getApp();
        auto& spotComponent = App.getEntityComponent<components::SpotLightComponent>(entity);

        bool open = EntityPropertiesComponents::drawHeader("##SpotNode", "Spot light");

        if (open)
        {
       		ImGui::Spacing();
        	static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
			static bool showColorPicker = false;
			ImGui::Text("Color");
			ImGui::SameLine();
			glm::vec4 color = {spotComponent.color, 1.0f};
			Widgets::drawColorEditor("##ColorEditor Spot light", &color, &colorPickerMode, &showColorPicker);
			spotComponent.color = color;

			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
   			if (ImGui::BeginTable("InspectorSpotTable", 4,
                ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

                EntityPropertiesComponents::drawRowDragFloat3("Direction", "X", "Y", "Z", &spotComponent.direction.x);
                EntityPropertiesComponents::drawRowDragFloat3("Position", "X", "Y", "Z", &spotComponent.pos.x);

                ImGui::EndTable();
            }

            ImGui::Spacing();
            ImGui::Text("Distance");
            ImGui::SameLine();
            if (ImGui::DragFloat("##DistanceSlider", &spotComponent.maxDistance, 1.0f, 1.0f, 3250.0f))
            {
                auto [lin, quad] = math::computeAttenuationFromDistance(spotComponent.maxDistance);
                spotComponent.constant = 1.0f;
                spotComponent.linear = lin;
                spotComponent.quadratic = quad;
            }

            ImGui::Spacing();
            ImGui::Text("Inner cutoff");
            ImGui::SameLine();
            float innerCutOffDegrees = glm::degrees(glm::acos(spotComponent.cutOff));
            float outerCutOffDegrees = glm::degrees(glm::acos(spotComponent.outerCutoff));
            ImGui::DragFloat("##InnerCutOffSlider", &innerCutOffDegrees, 0.5f, 0.0f, outerCutOffDegrees);
            spotComponent.cutOff = glm::cos(glm::radians(innerCutOffDegrees));

            ImGui::Spacing();
            ImGui::Text("Outer cutoff");
            ImGui::SameLine();
            ImGui::DragFloat("##OuterCutOffSlider", &outerCutOffDegrees, 0.5f, innerCutOffDegrees, 90.0f);
            spotComponent.outerCutoff = glm::cos(glm::radians(outerCutOffDegrees));
            ImGui::PopStyleVar();
        	ImGui::TreePop();
        }
        return true;
	}
}
