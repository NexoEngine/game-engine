//// NativeApi.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        06/05/2025
//  Description: Implementation file for native API functions exposed to managed code
//
///////////////////////////////////////////////////////////////////////////////

#include <iostream>

#include "NativeApi.hpp"
#include "EntityFactory3D.hpp"
#include "Logger.hpp"
#include "Nexo.hpp"
#include "components/Uuid.hpp"

namespace nexo::scripting {

    // Static message to return to C#
    static const char* nativeMessage = "Hello from C++ native code!";

    // Implementation of the native functions
    extern "C" {

        void HelloFromNative() {
            std::cout << "Hello World from C++ native code!" << std::endl;
        }

        Int32 AddNumbers(const Int32 a, const Int32 b) {
            std::cout << "Native AddNumbers called with " << a << " and " << b << std::endl;
            return a + b;
        }

        const char* GetNativeMessage() {
            std::cout << "GetNativeMessage called from C#" << std::endl;
            return nativeMessage;
        }

        void NxLog(UInt32 level, const char *message) {
            LOG(static_cast<LogLevel>(level), "[Scripting] {}", message);
        }

        ecs::Entity CreateCube(const Vector3 pos, const Vector3 size, const Vector3 rotation, const Vector4 color)
        {
            auto &app = getApp();
            const ecs::Entity basicCube = EntityFactory3D::createCube(std::move(pos), std::move(size), std::move(rotation), std::move(color));
            app.getSceneManager().getScene(0).addEntity(basicCube);
            return basicCube;
        }

        components::TransformComponent *GetTransformComponent(ecs::Entity entity)
        {
            const auto &app = getApp();
            const auto opt = app.m_coordinator->tryGetComponent<components::TransformComponent>(entity);
            if (!opt.has_value()) {
                LOG(NEXO_WARN, "GetTransformComponent: Entity {} does not have a TransformComponent", entity);
                return nullptr;
            }
            return &opt.value().get();
        }

        void* GetComponent(const UInt32 componentTypeId, const ecs::Entity entity)
        {
            auto& coordinator = *Application::m_coordinator;
            const auto opt = coordinator.tryGetComponentById(componentTypeId, entity);
            return opt;
        }

        void AddComponent(const UInt32 typeId, const ecs::Entity entity, const void *componentData)
        {
            auto& coordinator = *Application::m_coordinator;

            coordinator.addComponent(entity, typeId, componentData);
        }

        bool HasComponent(UInt32 typeId, ecs::Entity entity)
        {
            auto& coordinator = *Application::m_coordinator;

            const auto& typeMap = coordinator.getTypeIdToTypeIndex();
            auto it = typeMap.find(typeId);
            if (it == typeMap.end())
            {
                LOG(NEXO_WARN, "HasComponent: Unknown typeId {}", typeId);
                return false;
            }

            ecs::ComponentType bitIndex = typeId;

            ecs::Signature signature = coordinator.getSignature(entity);

            // LOG(NEXO_WARN, "HasComponent: entity = {}, typeId = {}, bitIndex = {}, signature = {}",
            //     entity, typeId, bitIndex, signature.to_string());

            return signature.test(bitIndex);
        }

        Int64 NxRegisterComponent(const char* name, const UInt64 size)
        {
            (void)name; // TODO: unused for now
            auto& coordinator = *Application::m_coordinator;
            return coordinator.registerComponent(size);
        }

        ComponentTypeIds GetComponentTypeIds()
        {
            auto& coordinator = *Application::m_coordinator;

            return ComponentTypeIds {
                .Transform = coordinator.getComponentType<components::TransformComponent>(),
                .AmbientLight = coordinator.getComponentType<components::AmbientLightComponent>(),
                .DirectionalLight = coordinator.getComponentType<components::DirectionalLightComponent>(),
                .PointLight = coordinator.getComponentType<components::PointLightComponent>(),
                .SpotLight = coordinator.getComponentType<components::SpotLightComponent>(),
                .RenderComponent = coordinator.getComponentType<components::RenderComponent>(),
                .SceneTag = coordinator.getComponentType<components::SceneTag>(),
                .CameraComponent = coordinator.getComponentType<components::CameraComponent>(),
                .UuidComponent = coordinator.getComponentType<components::UuidComponent>(),
                .PerspectiveCameraController = coordinator.getComponentType<components::PerspectiveCameraController>(),
                .PerspectiveCameraTarget = coordinator.getComponentType<components::PerspectiveCameraTarget>(),
            };
        }
    }

} // namespace nexo::scripting
