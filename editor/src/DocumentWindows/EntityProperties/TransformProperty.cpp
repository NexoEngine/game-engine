//// TransformProperty.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/03/2025
//  Description: Source file for the transform property inspector
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>

#include "TransformProperty.hpp"
#include "ImNexo/Elements.hpp"
#include "ImNexo/EntityProperties.hpp"
#include "ImNexo/ImNexo.hpp"
#include "components/Light.hpp"
#include "components/Transform.hpp"
#include "context/ActionManager.hpp"

namespace nexo::editor {

    void TransformProperty::show(ecs::Entity entity)
    {
        if (Application::m_coordinator->entityHasComponent<components::PointLightComponent>(entity) ||
            Application::m_coordinator->entityHasComponent<components::SpotLightComponent>(entity))
            return;
        auto& transformComponent = Application::getEntityComponent<components::TransformComponent>(entity);
        static glm::vec3 lastDisplayedEuler(0.0f);
        static components::TransformComponent::Memento beforeState;

        if (ImNexo::Header("##TransformNode", "Transform Component"))
        {
            auto transformComponentCopy = transformComponent;
            ImNexo::resetItemStates();
            ImNexo::Transform(transformComponent, lastDisplayedEuler);
            if (ImNexo::isItemActivated()) {
                beforeState = transformComponentCopy.save();
            } else if (ImNexo::isItemDeactivated()) {
                auto afterState = transformComponent.save();
                auto action = std::make_unique<ComponentChangeAction<components::TransformComponent>>(entity, beforeState, afterState);
                ActionManager::get().recordAction(std::move(action));
            }
            ImGui::TreePop();
        }
    }
}
