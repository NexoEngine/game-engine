//// AmbientLightProperty.cpp /////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
#include "ImNexo/EntityProperties.hpp"
#include "ImNexo/ImNexo.hpp"
#include <imgui.h>

namespace nexo::editor {

    void AmbientLightProperty::show(const ecs::Entity entity)
    {
        auto& ambientComponent = Application::getEntityComponent<components::AmbientLightComponent>(entity);
        static components::AmbientLightComponent::Memento beforeState;

        if (ImNexo::Header("##AmbientNode", "Ambient light"))
        {
            const auto ambientComponentCopy = ambientComponent;
            ImNexo::resetItemStates();
            ImNexo::Ambient(ambientComponent);
            if (ImNexo::isItemActivated()) {
                beforeState = ambientComponentCopy.save();
            } else if (ImNexo::isItemDeactivated()) {
                auto afterState = ambientComponent.save();
                auto action = std::make_unique<ComponentChangeAction<components::AmbientLightComponent>>(entity, beforeState, afterState);
                ActionManager::get().recordAction(std::move(action));
                beforeState = components::AmbientLightComponent::Memento{};
            }

            ImGui::TreePop();
        }
    }
}
