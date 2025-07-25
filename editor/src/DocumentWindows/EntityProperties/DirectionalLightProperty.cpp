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
#include "ImNexo/EntityProperties.hpp"
#include "ImNexo/ImNexo.hpp"
#include "components/Light.hpp"
#include "ImNexo/Widgets.hpp"
#include "context/ActionManager.hpp"

namespace nexo::editor {

    void DirectionalLightProperty::show(const ecs::Entity entity)
    {
        auto& directionalComponent = Application::getEntityComponent<components::DirectionalLightComponent>(entity);
        static components::DirectionalLightComponent::Memento beforeState;

        if (ImNexo::Header("##DirectionalNode", "Directional light"))
        {
            const auto directionalComponentCopy = directionalComponent;
            ImNexo::resetItemStates();
            ImNexo::DirectionalLight(directionalComponent);
            if (ImNexo::isItemActivated()) {
                beforeState = directionalComponentCopy.save();
            } else if (ImNexo::isItemDeactivated()) {
                auto afterState = directionalComponent.save();
                auto action = std::make_unique<ComponentChangeAction<components::DirectionalLightComponent>>(entity, beforeState, afterState);
                ActionManager::get().recordAction(std::move(action));
                beforeState = components::DirectionalLightComponent::Memento{};
            }
            ImGui::TreePop();
        }
    }
}
