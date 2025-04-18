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
#include "components/Camera.hpp"
#include "ImNexo/Elements.hpp"
#include "ImNexo/EntityProperties.hpp"

namespace nexo::editor {

    void CameraTarget::show(const ecs::Entity entity)
    {
        auto& targetComponent = Application::getEntityComponent<components::PerspectiveCameraTarget>(entity);

        if (ImNexo::Header("##ControllerNode", "Camera Controller"))
        {
            ImGui::Spacing();
            ImNexo::CameraTarget(targetComponent),
            ImGui::TreePop();
        }
    }
}
