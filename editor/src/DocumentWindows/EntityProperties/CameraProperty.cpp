//// CameraProperty.cpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Source file for the camera property class
//
///////////////////////////////////////////////////////////////////////////////

#include "CameraProperty.hpp"
#include "ImNexo/ImNexo.hpp"
#include "components/Camera.hpp"
#include "ImNexo/Elements.hpp"
#include "ImNexo/EntityProperties.hpp"
#include "context/ActionManager.hpp"
#include "context/actions/EntityActions.hpp"

namespace nexo::editor {

	void CameraProperty::show(const ecs::Entity entity)
	{
        auto& cameraComponent = Application::getEntityComponent<components::CameraComponent>(entity);
        static components::CameraComponent::Memento beforeState;

        if (ImNexo::Header("##CameraNode", "Camera"))
        {
            auto cameraComponentCopy = cameraComponent;
            ImNexo::resetItemStates();
            ImNexo::Camera(cameraComponent);
            if (ImNexo::isItemActivated()) {
                beforeState = cameraComponentCopy.save();
            } else if (ImNexo::isItemDeactivated()) {
                cameraComponent = cameraComponentCopy;
                auto afterState = cameraComponent.save();
                auto action = std::make_unique<ComponentChangeAction<components::CameraComponent>>(entity, beforeState, afterState);
                ActionManager::get().recordAction(std::move(action));
                beforeState = components::CameraComponent::Memento{};
            }
            ImGui::TreePop();
        }
	}
}
