//// CameraController.cpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Source file for the camera controller
//
///////////////////////////////////////////////////////////////////////////////

#include "CameraController.hpp"
#include "components/Camera.hpp"
#include "ImNexo/Elements.hpp"
#include "ImNexo/EntityProperties.hpp"
#include "context/ActionManager.hpp"
#include "context/actions/EntityActions.hpp"

namespace nexo::editor {

	void CameraController::show(const ecs::Entity entity)
	{
        auto& controllerComponent = Application::getEntityComponent<components::PerspectiveCameraController>(entity);
        static components::PerspectiveCameraController::Memento beforeState;

        if (ImNexo::Header("##ControllerNode", "Camera Controller"))
        {
            ImGui::Spacing();

            ImNexo::InteractionState state = ImNexo::CameraController(controllerComponent, beforeState);
            if (state == ImNexo::InteractionState::RELEASED) {
                auto afterState = controllerComponent.save();
                auto action = std::make_unique<ComponentChangeAction<components::PerspectiveCameraController>>(entity, beforeState, afterState);
                ActionManager::get().recordAction(std::move(action));
                beforeState = components::PerspectiveCameraController::Memento{};
            }
            ImGui::TreePop();
        }
	}
}
