//// Init.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the inspector window setup
//
///////////////////////////////////////////////////////////////////////////////

#include "InspectorWindow.hpp"
#include "../EntityProperties/RenderProperty.hpp"
#include "../EntityProperties/TransformProperty.hpp"
#include "../EntityProperties/AmbientLightProperty.hpp"
#include "../EntityProperties/DirectionalLightProperty.hpp"
#include "../EntityProperties/PointLightProperty.hpp"
#include "../EntityProperties/SpotLightProperty.hpp"
#include "../EntityProperties/CameraProperty.hpp"
#include "../EntityProperties/CameraController.hpp"

namespace nexo::editor {

    void InspectorWindow::setup()
    {
        registerProperty<components::TransformComponent, TransformProperty>();
        registerProperty<components::RenderComponent, RenderProperty>();
        registerProperty<components::AmbientLightComponent, AmbientLightProperty>();
        registerProperty<components::DirectionalLightComponent, DirectionalLightProperty>();
        registerProperty<components::PointLightComponent, PointLightProperty>();
        registerProperty<components::SpotLightComponent, SpotLightProperty>();
        registerProperty<components::CameraComponent, CameraProperty>();
        registerProperty<components::PerspectiveCameraController, CameraController>();
    }
}
