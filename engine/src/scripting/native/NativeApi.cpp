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
#include "components/PhysicsBodyComponent.hpp"
#include "ui/Field.hpp"
#include "systems/PhysicsSystem.hpp"

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

        ecs::Entity NxCreateCube(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color)
        {
            auto &app = getApp();
            const ecs::Entity basicCube = EntityFactory3D::createCube(pos, size, rotation, color);
            app.getSceneManager().getScene(0).addEntity(basicCube);
            return basicCube;
        }

        ecs::Entity NxCreateTetrahedron(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color)
        {
            auto& app = Application::getInstance();
            const ecs::Entity entity = EntityFactory3D::createTetrahedron(
                {position.x, position.y, position.z},
                {size.x, size.y, size.z},
                {rotation.x, rotation.y, rotation.z},
                {color.x, color.y, color.z, color.w}
            );
            app.getSceneManager().getScene(0).addEntity(entity);
            return entity;
        }

        ecs::Entity NxCreatePyramid(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color)
        {
            auto& app = Application::getInstance();
            const ecs::Entity entity = EntityFactory3D::createPyramid(
                {position.x, position.y, position.z},
                {size.x, size.y, size.z},
                {rotation.x, rotation.y, rotation.z},
                {color.x, color.y, color.z, color.w}
            );
            app.getSceneManager().getScene(0).addEntity(entity);
            return entity;
        }

        ecs::Entity NxCreateCylinder(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color, UInt32 nbSegment)
        {
            auto& app = Application::getInstance();
            const ecs::Entity entity = EntityFactory3D::createCylinder(
                {position.x, position.y, position.z},
                {size.x, size.y, size.z},
                {rotation.x, rotation.y, rotation.z},
                {color.x, color.y, color.z, color.w},
                nbSegment
            );
            app.getSceneManager().getScene(0).addEntity(entity);
            return entity;
        }

        ecs::Entity NxCreateSphere(Vector3 position, Vector3 size, Vector3 rotation, Vector4 color, UInt32 nbSubdivision)
        {
            auto& app = Application::getInstance();
            const ecs::Entity entity = EntityFactory3D::createSphere(
                {position.x, position.y, position.z},
                {size.x, size.y, size.z},
                {rotation.x, rotation.y, rotation.z},
                {color.x, color.y, color.z, color.w},
                nbSubdivision
            );
            app.getSceneManager().getScene(0).addEntity(entity);
            return entity;
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

        void NxRemoveComponent(const ecs::Entity entity, const UInt32 componentTypeId)
        {
            auto& coordinator = *Application::m_coordinator;
            coordinator.removeComponent(entity, componentTypeId);
        }

        void NxDestroyEntity(const ecs::Entity entity)
        {
            auto& coordinator = *Application::m_coordinator;
            coordinator.destroyEntity(entity);
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
                .PhysicsBodyComponent = coordinator.getComponentType<components::PhysicsBodyComponent>(),
            };
        }

        void NxCreateBodyFromShape(ecs::Entity entity, Vector3 position, Vector3 size, Vector3 rotation, UInt32 shapeType, UInt32 motionType)
        {
            auto& app = Application::getInstance();
            auto physicsSystem = app.getPhysicsSystem();
            
            if (!physicsSystem) {
                LOG(NEXO_ERROR, "Physics system not available");
                return;
            }

            components::TransformComponent transform;
            transform.pos = {position.x, position.y, position.z};
            transform.size = {size.x, size.y, size.z};
            transform.quat = glm::quat(glm::radians(glm::vec3(rotation.x, rotation.y, rotation.z)));

            system::ShapeType cppShapeType = static_cast<system::ShapeType>(shapeType);
            JPH::EMotionType cppMotionType = static_cast<JPH::EMotionType>(motionType);

            JPH::BodyID bodyID = physicsSystem->createBodyFromShape(entity, transform, cppShapeType, cppMotionType);

            LOG(NEXO_DEV, "Physics body created");
        }

        void NxApplyForce(ecs::Entity entity, Vector3 force)
        {
            auto& app = Application::getInstance();
            auto physicsSystem = app.getPhysicsSystem();
            if (!physicsSystem) {
                LOG(NEXO_ERROR, "Physics system not available");
                return;
            }
            auto& coordinator = *Application::m_coordinator;
            if (!coordinator.entityHasComponent<components::PhysicsBodyComponent>(entity)) {
                LOG(NEXO_ERROR, "Entity {} has no PhysicsBodyComponent", entity);
                return;
            }
            auto& bodyComp = coordinator.getComponent<components::PhysicsBodyComponent>(entity);
            JPH::BodyID joltBodyID = bodyComp.bodyID;
            JPH::Vec3 joltForce(force.x, force.y, force.z);
            physicsSystem->applyForce(joltBodyID, joltForce);
        }
    }

} // namespace nexo::scripting
