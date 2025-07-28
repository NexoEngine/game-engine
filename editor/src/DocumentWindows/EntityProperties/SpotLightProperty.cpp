//// SpotLightProperty.cpp ////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Source file for the spot light property class
//
///////////////////////////////////////////////////////////////////////////////

#include "SpotLightProperty.hpp"
#include "ImNexo/EntityProperties.hpp"
#include "ImNexo/ImNexo.hpp"
#include "components/Light.hpp"
#include "components/Transform.hpp"
#include "context/actions/EntityActions.hpp"
#include "ImNexo/Widgets.hpp"
#include "context/ActionManager.hpp"

namespace nexo::editor {

	void SpotLightProperty::show(ecs::Entity entity)
	{
        auto& spotComponent = Application::getEntityComponent<components::SpotLightComponent>(entity);
        auto &transformComponent = Application::getEntityComponent<components::TransformComponent>(entity);

        static components::SpotLightComponent::Memento beforeStateSpot;
        static components::TransformComponent::Memento beforeStateTransform;

        if (ImNexo::Header("##SpotNode", "Spot light"))
        {
            ImNexo::resetItemStates();
            auto spotComponentCopy = spotComponent;
            auto transformComponentCopy = transformComponent;
            ImNexo::SpotLight(spotComponent, transformComponent);
            if (ImNexo::isItemActivated()) {
                beforeStateSpot = spotComponentCopy.save();
                beforeStateTransform = transformComponentCopy.save();
            } else if (ImNexo::isItemDeactivated()) {
                auto afterStateSpot = spotComponent.save();
                auto afterStateTransform = transformComponent.save();
                auto actionGroup = ActionManager::createActionGroup();
                auto spotAction = std::make_unique<ComponentChangeAction<components::SpotLightComponent>>(entity, beforeStateSpot, afterStateSpot);
                auto transformAction = std::make_unique<ComponentChangeAction<components::TransformComponent>>(entity, beforeStateTransform, afterStateTransform);
                actionGroup->addAction(std::move(spotAction));
                actionGroup->addAction(std::move(transformAction));
                ActionManager::get().recordAction(std::move(actionGroup));
            }
        	ImGui::TreePop();
        }
	}
}
