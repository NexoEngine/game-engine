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
        using ActionFactory = std::function<std::unique_ptr<Action>(ecs::Entity)>;

        static const std::unordered_map<std::type_index, ActionFactory> factories{
            {typeid(components::TransformComponent), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::TransformComponent>>(e); }},
            {typeid(components::RenderComponent), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::RenderComponent>>(e); }},
            {typeid(components::SceneTag), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::SceneTag>>(e); }},
            {typeid(components::CameraComponent), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::CameraComponent>>(e); }},
            {typeid(components::AmbientLightComponent), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::AmbientLightComponent>>(e); }},
            {typeid(components::DirectionalLightComponent), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::DirectionalLightComponent>>(e); }},
            {typeid(components::PointLightComponent), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::PointLightComponent>>(e); }},
            {typeid(components::SpotLightComponent), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::SpotLightComponent>>(e); }},
            {typeid(components::UuidComponent), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::UuidComponent>>(e); }},
            {typeid(components::PerspectiveCameraController), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::PerspectiveCameraController>>(e); }},
            {typeid(components::PerspectiveCameraTarget), [](ecs::Entity e){ return std::make_unique<ComponentRestoreAction<components::PerspectiveCameraTarget>>(e); }},
        };

        auto it = factories.find(typeIndex);
        if (it != factories.end()) {
            return (it->second)(entity);
        }
        return nullptr;
    }
}
