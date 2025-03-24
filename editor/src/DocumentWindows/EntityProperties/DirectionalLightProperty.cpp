//// DirectionalLightProperty.cpp /////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Source file for the directional light property
//
///////////////////////////////////////////////////////////////////////////////

#include "DirectionalLightProperty.hpp"
#include "Components/EntityPropertiesComponents.hpp"
#include "Components/Widgets.hpp"
#include "components/Light.hpp"

namespace nexo::editor {
	int DirectionalLightProperty::show(const ecs::Entity entity)
	{
        auto& directionalComponent = Application::getEntityComponent<components::DirectionalLightComponent>(entity);


        if (EntityPropertiesComponents::drawHeader("##DirectionalNode", "Directional light"))
        {
       		ImGui::Spacing();
        	static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
			static bool showColorPicker = false;
			ImGui::Text("Color");
			ImGui::SameLine();
			glm::vec4 color = {directionalComponent.color, 1.0f};
			Widgets::drawColorEditor("##ColorEditor Directional light", &color, &colorPickerMode, &showColorPicker);
			directionalComponent.color = color;

			ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
   			if (ImGui::BeginTable("InspectorDirectionTable", 4,
                ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

                EntityPropertiesComponents::drawRowDragFloat3("Direction", "X", "Y", "Z", &directionalComponent.direction.x);

                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
        	ImGui::TreePop();
        }
        return true;
	}
}
