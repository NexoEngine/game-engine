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
#include "ImNexo/Widgets.hpp"
#include "context/actions/EntityActions.hpp"
#include "context/ActionManager.hpp"
#include <imgui.h>

namespace nexo::editor {

    void AmbientLightProperty::show(const ecs::Entity entity)
    {
        auto& ambientComponent = Application::getEntityComponent<components::AmbientLightComponent>(entity);

        if (ImNexo::Header("##AmbientNode", "Ambient light"))
        {
            ImGui::Spacing();
            static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
            static bool showColorPicker = false;
            static bool wasUsingLastFrame = false;
            static components::AmbientLightComponent::Memento beforeState;

            ImGui::Text("Color");
            ImGui::SameLine();
            glm::vec4 color = {ambientComponent.color, 1.0f};
            if (ImNexo::ColorEditor("##ColorEditor Ambient light", &color, &colorPickerMode, &showColorPicker)) {
                if (!wasUsingLastFrame) {
                    beforeState = ambientComponent.save();
                    wasUsingLastFrame = true;
                }
                ambientComponent.color = color;
            }

            // Only record the action when the user is done with the color picker
            if (wasUsingLastFrame && ImGui::IsMouseReleased(ImGuiMouseButton_Left)) {
                auto afterState = ambientComponent.save();
                auto action = std::make_unique<ComponentChangeAction<components::AmbientLightComponent>>(
                    entity, beforeState, afterState);
                ActionManager::get().recordAction(std::move(action));
                wasUsingLastFrame = false;
                beforeState = components::AmbientLightComponent::Memento{};
            }

            ImGui::TreePop();
        }
    }
}
