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
#include "core/event/Input.hpp"

namespace nexo::scripting {

    // Define the global callbacks instance
    NativeApiCallbacks nativeApiCallbacks;

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
            const ecs::Entity basicCube = EntityFactory3D::createCube(std::move(pos), std::move(size), std::move(rotation), std::move(color));
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
            (void)name; // TODO: unused for now
            auto& coordinator = *Application::m_coordinator;

            for (UInt64 i = 0; i < fieldCount; ++i) {
                LOG(NEXO_DEV, "Registering field {}: {} of type {}", i, static_cast<char*>(fields[i].name), static_cast<UInt64>(fields[i].type));
            }

            const auto componentType = coordinator.registerComponent(componentSize);

            std::vector<ecs::Field> fieldVector;
            fieldVector.reserve(fieldCount);
            static_assert(sizeof(ecs::FieldType) == sizeof(FieldType), "FieldType enum size mismatch");
            static_assert(static_cast<uint64_t>(ecs::FieldType::_Count) == static_cast<uint64_t>(FieldType::_Count), "FieldType enum value count mismatch");
            for (UInt64 i = 0; i < fieldCount; ++i) {
                fieldVector.emplace_back(ecs::Field {
                    .name = static_cast<char*>(fields[i].name),
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

        bool NxIsKeyPressed(const Int32 keycode)
        {
            return event::isKeyPressed(keycode);
        }

        bool NxIsKeyReleased(const Int32 keycode)
        {
            return event::isKeyReleased(keycode);
        }

        bool NxIsAnyKeyPressed()
        {
            // GLFW key range: 32-96 (printable characters), 256-348 (special keys)
            // Check printable ASCII range
            for (Int32 key = 32; key <= 96; ++key) {
                if (event::isKeyPressed(key)) {
                    return true;
                }
            }
            
            // Check special keys range
            for (Int32 key = 256; key <= 348; ++key) {
                if (event::isKeyPressed(key)) {
                    return true;
                }
            }
            
            return false;
        }

        bool NxIsMouseDown(const Int32 button)
        {
            return event::isMouseDown(button);
        }

        bool NxIsMouseReleased(const Int32 button)
        {
            return event::isMouseReleased(button);
        }

        void NxGetMousePosition(Vector2 *position)
        {
            if (position) {
                const auto mousePos = event::getMousePosition();
                position->x = mousePos.x;
                position->y = mousePos.y;
            }
        }
    }

    // Initialize the callbacks with the extern "C" functions
    void initializeNativeApiCallbacks() {
        nativeApiCallbacks.NxHelloFromNative = &NxHelloFromNative;
        nativeApiCallbacks.NxAddNumbers = &NxAddNumbers;
        nativeApiCallbacks.NxGetNativeMessage = &NxGetNativeMessage;
        nativeApiCallbacks.NxLog = &NxLog;
        nativeApiCallbacks.NxCreateCube = &NxCreateCube;
        nativeApiCallbacks.NxGetTransformComponent = &NxGetTransformComponent;
        nativeApiCallbacks.NxGetComponent = &NxGetComponent;
        nativeApiCallbacks.NxAddComponent = &NxAddComponent;
        nativeApiCallbacks.NxHasComponent = &NxHasComponent;
        nativeApiCallbacks.NxRegisterComponent = &NxRegisterComponent;
        nativeApiCallbacks.NxGetComponentTypeIds = &NxGetComponentTypeIds;
        nativeApiCallbacks.NxIsKeyPressed = &NxIsKeyPressed;
        nativeApiCallbacks.NxIsKeyReleased = &NxIsKeyReleased;
        nativeApiCallbacks.NxIsAnyKeyPressed = &NxIsAnyKeyPressed;
        nativeApiCallbacks.NxIsMouseDown = &NxIsMouseDown;
        nativeApiCallbacks.NxIsMouseReleased = &NxIsMouseReleased;
        nativeApiCallbacks.NxGetMousePosition = &NxGetMousePosition;
    }

    // Static initialization
    static struct NativeApiCallbacksInitializer {
        NativeApiCallbacksInitializer() {
            initializeNativeApiCallbacks();
        }
    } nativeApiCallbacksInitializer;

} // namespace nexo::scripting
