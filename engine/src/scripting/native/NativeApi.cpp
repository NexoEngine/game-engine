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
#include "ui/Field.hpp"

namespace nexo::scripting {

    // Static message to return to C#
    static const char* nativeMessage = "Hello from C++ native code!";

    // Implementation of the native functions
    extern "C" {

        void NxHelloFromNative() {
            std::cout << "Hello World from C++ native code!" << std::endl;
        }

        Int32 NxAddNumbers(const Int32 a, const Int32 b) {
            std::cout << "Native AddNumbers called with " << a << " and " << b << std::endl;
            return a + b;
        }

        const char* NxGetNativeMessage() {
            std::cout << "GetNativeMessage called from C#" << std::endl;
            return nativeMessage;
        }

        void NxLog(UInt32 level, const char *message) {
            LOG(static_cast<LogLevel>(level), "[Scripting] {}", message);
        }

        ecs::Entity NxCreateCube(const Vector3 pos, const Vector3 size, const Vector3 rotation, const Vector4 color)
        {
            auto &app = getApp();
            const ecs::Entity basicCube = EntityFactory3D::createCube(pos, size, rotation, color);
            app.getSceneManager().getScene(0).addEntity(basicCube);
            return basicCube;
        }

        components::TransformComponent *NxGetTransformComponent(ecs::Entity entity)
        {
            const auto opt = Application::m_coordinator->tryGetComponent<components::TransformComponent>(entity);
            if (!opt.has_value()) {
                LOG(NEXO_WARN, "GetTransformComponent: Entity {} does not have a TransformComponent", entity);
                return nullptr;
            }
            return &opt.value().get();
        }

        void* NxGetComponent(const ecs::Entity entity, const UInt32 componentTypeId)
        {
            auto& coordinator = *Application::m_coordinator;
            const auto opt = coordinator.tryGetComponentById(componentTypeId, entity);
            return opt;
        }

        void NxAddComponent(const ecs::Entity entity, const UInt32 typeId, const void *componentData)
        {
            if (componentData == nullptr) {
                LOG(NEXO_ERROR, "NxAddComponent: componentData is null for entity {}", entity);
                return;
            }
            const auto& coordinator = *Application::m_coordinator;

            coordinator.addComponent(entity, typeId, componentData);
        }

        bool NxHasComponent(const ecs::Entity entity, const UInt32 typeId)
        {
            const auto& coordinator = *Application::m_coordinator;

            return coordinator.entityHasComponent(entity, typeId);
        }

        Int64 NxRegisterComponent(const char *name, const UInt64 componentSize, const Field *fields, const UInt64 fieldCount)
        {
            if (!name || !fields || fieldCount == 0 || componentSize == 0) {
                LOG(NEXO_ERROR, "Invalid parameters for component registration");
                return -1;
            }

            auto& coordinator = *Application::m_coordinator;

            for (UInt64 i = 0; i < fieldCount; ++i) {
                if (!fields[i].name) {
                    LOG(NEXO_WARN, "Field {} has null name", i);
                    return -1;
                }
                LOG(NEXO_DEV, "Registering field {}: {} of type {}", i, static_cast<char*>(fields[i].name), static_cast<UInt64>(fields[i].type));
            }

            const auto componentType = coordinator.registerComponent(componentSize);

            std::vector<ecs::Field> fieldVector;
            fieldVector.reserve(fieldCount);
            static_assert(sizeof(ecs::FieldType) == sizeof(FieldType), "FieldType enum size mismatch");
            static_assert(static_cast<uint64_t>(ecs::FieldType::_Count) == static_cast<uint64_t>(FieldType::_Count), "FieldType enum value count mismatch");
            for (UInt64 i = 0; i < fieldCount; ++i) {
                fieldVector.emplace_back(ecs::Field {
                    .name = fields[i].name ? std::string(static_cast<char*>(fields[i].name)) : "(null)",
                    .type = static_cast<ecs::FieldType>(fields[i].type),
                    .size = fields[i].size,
                    .offset = fields[i].offset,
                });
            }

            coordinator.addComponentDescription(
                componentType,
                ecs::ComponentDescription {
                    .name = name,
                    .fields = std::move(fieldVector),
                }
            );

            return componentType;
        }

        ComponentTypeIds NxGetComponentTypeIds()
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
