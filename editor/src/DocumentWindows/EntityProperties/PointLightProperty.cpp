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
#include "ImNexo/EntityProperties.hpp"
#include "ImNexo/ImNexo.hpp"
#include "components/Light.hpp"
#include "components/Transform.hpp"
#include "context/actions/EntityActions.hpp"
#include "math/Light.hpp"
#include "ImNexo/Widgets.hpp"
#include "context/ActionManager.hpp"

namespace nexo::editor {

	void PointLightProperty::show(const ecs::Entity entity)
	{
        auto& pointComponent = nexo::Application::getEntityComponent<components::PointLightComponent>(entity);
        auto &transform = Application::getEntityComponent<components::TransformComponent>(entity);

        static components::PointLightComponent::Memento beforeStatePoint;
        static components::TransformComponent::Memento beforeStateTransform;

        if (ImNexo::Header("##PointNode", "Point light"))
        {
            auto pointComponentCopy = pointComponent;
            auto transformComponentCopy = transform;
            ImNexo::resetItemStates();
            ImNexo::PointLight(pointComponent, transform);
            if (ImNexo::isItemActivated()) {
                beforeStatePoint = pointComponentCopy.save();
                beforeStateTransform = transformComponentCopy.save();
            } else if (ImNexo::isItemDeactivated()) {
                auto afterStatePoint = pointComponent.save();
                auto afterStateTransform = transform.save();
                auto actionGroup = ActionManager::get().createActionGroup();
                auto pointAction = std::make_unique<ComponentChangeAction<components::PointLightComponent>>(entity, beforeStatePoint, afterStatePoint);
                auto transformAction = std::make_unique<ComponentChangeAction<components::TransformComponent>>(entity, beforeStateTransform, afterStateTransform);
                actionGroup->addAction(std::move(pointAction));
                actionGroup->addAction(std::move(transformAction));
                ActionManager::get().recordAction(std::move(actionGroup));
            }
        	ImGui::TreePop();
        }
	}
}
