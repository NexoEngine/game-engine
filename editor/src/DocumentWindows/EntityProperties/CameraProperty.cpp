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
#include "components/Camera.hpp"
#include "ImNexo/Elements.hpp"
#include "ImNexo/EntityProperties.hpp"

namespace nexo::editor {

	void CameraProperty::show(const ecs::Entity entity)
	{
        auto& cameraComponent = Application::getEntityComponent<components::CameraComponent>(entity);

        if (ImNexo::Header("##CameraNode", "Camera"))
        {
            ImNexo::Camera(cameraComponent);
            ImGui::TreePop();
        }
	}
}
