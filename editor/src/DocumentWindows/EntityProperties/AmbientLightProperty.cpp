//// AmbientLightProperty.cpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Source file for the ambient light property class
//
///////////////////////////////////////////////////////////////////////////////

#include "AmbientLightProperty.hpp"
#include "components/Light.hpp"
#include "Components/EntityPropertiesComponents.hpp"
#include "Components/Widgets.hpp"

namespace nexo::editor {
	/**
	 * @brief Renders the ambient light properties panel for the given entity.
	 *
	 * Retrieves the AmbientLightComponent associated with the specified entity and displays a header along with a color picker
	 * for editing the ambient light's color. If the header is successfully rendered, the function adds spacing and presents
	 * a color editor widget to update the component's color value.
	 *
	 * @param entity The entity whose ambient light component is being displayed and modified.
	 * @return bool Always returns true, indicating that the properties panel was processed.
	 */
	bool AmbientLightProperty::show(const ecs::Entity entity)
	{
        auto& ambientComponent = Application::getEntityComponent<components::AmbientLightComponent>(entity);

        if (EntityPropertiesComponents::drawHeader("##AmbientNode", "Ambient light"))
        {
        	ImGui::Spacing();
        	static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
			static bool showColorPicker = false;
			ImGui::Text("Color");
			ImGui::SameLine();
			glm::vec4 color = {ambientComponent.color, 1.0f};
			Widgets::drawColorEditor("##ColorEditor Ambient light", &color, &colorPickerMode, &showColorPicker);
			ambientComponent.color = color;
        	ImGui::TreePop();
        }
        return true;
	}
}
