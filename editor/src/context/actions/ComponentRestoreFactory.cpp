//// ComponentRestoreFactory.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        30/04/2025
//  Description: Source file for the component restore action factory
//
///////////////////////////////////////////////////////////////////////////////

#include "ComponentRestoreFactory.hpp"
#include "EntityActions.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "components/Render.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"

namespace nexo::editor {

    std::unique_ptr<Action> ComponentRestoreFactory::createRestoreComponent(ecs::Entity entity, const std::type_index typeIndex)
    {
        if (typeIndex == typeid(components::TransformComponent))
            return std::make_unique<ComponentRestoreAction<components::TransformComponent>>(entity);
        else if (typeIndex == typeid(components::RenderComponent))
            return std::make_unique<ComponentRestoreAction<components::RenderComponent>>(entity);
        else if (typeIndex == typeid(components::SceneTag))
            return std::make_unique<ComponentRestoreAction<components::SceneTag>>(entity);
        else if (typeIndex == typeid(components::CameraComponent))
            return std::make_unique<ComponentRestoreAction<components::CameraComponent>>(entity);
        else if (typeIndex == typeid(components::AmbientLightComponent))
            return std::make_unique<ComponentRestoreAction<components::AmbientLightComponent>>(entity);
        else if (typeIndex == typeid(components::DirectionalLightComponent))
            return std::make_unique<ComponentRestoreAction<components::DirectionalLightComponent>>(entity);
        else if (typeIndex == typeid(components::PointLightComponent))
            return std::make_unique<ComponentRestoreAction<components::PointLightComponent>>(entity);
        else if (typeIndex == typeid(components::SpotLightComponent))
            return std::make_unique<ComponentRestoreAction<components::SpotLightComponent>>(entity);
        else if (typeIndex == typeid(components::UuidComponent))
            return std::make_unique<ComponentRestoreAction<components::UuidComponent>>(entity);
        else if (typeIndex == typeid(components::PerspectiveCameraController))
            return std::make_unique<ComponentRestoreAction<components::PerspectiveCameraController>>(entity);
        else if (typeIndex == typeid(components::PerspectiveCameraTarget))
            return std::make_unique<ComponentRestoreAction<components::PerspectiveCameraTarget>>(entity);
        return nullptr;
    }
}
