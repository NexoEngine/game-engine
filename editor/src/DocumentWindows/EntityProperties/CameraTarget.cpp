//// CameraTarget.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        18/04/2025
//  Description: Source file for the camera target property
//
///////////////////////////////////////////////////////////////////////////////

#include "CameraTarget.hpp"
#include "Definitions.hpp"
#include "ImNexo/ImNexo.hpp"
#include "components/Camera.hpp"
#include "ImNexo/Elements.hpp"
#include "ImNexo/EntityProperties.hpp"
#include "context/ActionManager.hpp"

namespace nexo::editor {

    void CameraTarget::show(const ecs::Entity entity)
    {
        auto& targetComponent = Application::getEntityComponent<components::PerspectiveCameraTarget>(entity);
        static components::PerspectiveCameraTarget::Memento beforeState;

        if (ImNexo::Header("##TargetNode", "Camera Target"))
        {
            auto targetComponentCopy = targetComponent;
            ImGui::Spacing();
            ImNexo::resetItemStates();
            ImNexo::CameraTarget(targetComponent);
            if (ImNexo::isItemActivated()) {
                beforeState = targetComponentCopy.save();
            } else if (ImNexo::isItemDeactivated()) {
                auto afterState = targetComponent.save();
                auto action = std::make_unique<ComponentChangeAction<components::PerspectiveCameraTarget>>(entity, beforeState, afterState);
                ActionManager::get().recordAction(std::move(action));
                beforeState = components::PerspectiveCameraTarget::Memento{};
            }
            ImGui::TreePop();
        }
    }
}
