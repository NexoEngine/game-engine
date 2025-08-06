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
#include <tracy/Tracy.hpp>

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
            ZoneScoped;
            ZoneName("Script API: Hello From Native", 28);

            std::cout << "Hello World from C++ native code!" << std::endl;
        }

        Int32 NxAddNumbers(const Int32 a, const Int32 b) {
            ZoneScoped;
            ZoneName("Script API: Add Numbers", 23);
            ZoneValue(static_cast<int64_t>(a + b));

            std::cout << "Native AddNumbers called with " << a << " and " << b << std::endl;
            return a + b;
        }

        const char* NxGetNativeMessage() {
            ZoneScoped;
            ZoneName("Script API: Get Native Message", 31);

            std::cout << "GetNativeMessage called from C#" << std::endl;
            return nativeMessage;
        }

        void NxLog(UInt32 level, const char *message) {
            ZoneScoped;
            ZoneName("Script API: Log", 16);
            ZoneText(message, strlen(message));

            LOG(static_cast<LogLevel>(level), "[Scripting] {}", message);
        }

        ecs::Entity NxCreateCube(const Vector3 position, const Vector3 size, const Vector3 rotation, const Vector4 color)
        {
            ZoneScoped;
            ZoneName("Script API: Create Cube", 23);
            ZoneText("Cube", 4);

            auto& app = Application::getInstance();
            const ecs::Entity basicCube = EntityFactory3D::createCube(position, size, rotation, color);
            app.getSceneManager().getScene(0).addEntity(basicCube);
            return basicCube;
        }

        ecs::Entity NxCreateTetrahedron(const Vector3 position, const Vector3 size, const Vector3 rotation, const Vector4 color)
        {
            ZoneScoped;
            ZoneName("Script API: Create Tetrahedron", 30);
            ZoneText("Tetrahedron", 11);

            auto& app = Application::getInstance();
            const ecs::Entity entity = EntityFactory3D::createTetrahedron(position, size, rotation, color);
            app.getSceneManager().getScene(0).addEntity(entity);
            return entity;
        }

        ecs::Entity NxCreatePyramid(const Vector3 position, const Vector3 size, const Vector3 rotation, const Vector4 color)
        {
            ZoneScoped;
            ZoneName("Script API: Create Pyramid", 26);
            ZoneText("Pyramid", 7);

            auto& app = Application::getInstance();
            const ecs::Entity entity = EntityFactory3D::createPyramid(position, size, rotation, color);
            app.getSceneManager().getScene(0).addEntity(entity);
            return entity;
        }

        ecs::Entity NxCreateCylinder(const Vector3 position, const Vector3 size, const Vector3 rotation, const Vector4 color, const UInt32 nbSegment)
        {
            ZoneScoped;
            ZoneName("Script API: Create Cylinder", 27);
            ZoneText("Cylinder", 8);
            ZoneValue(static_cast<int64_t>(nbSegment));

            auto& app = Application::getInstance();
            const ecs::Entity entity = EntityFactory3D::createCylinder(position, size, rotation, color, nbSegment);
            app.getSceneManager().getScene(0).addEntity(entity);
            return entity;
        }

        ecs::Entity NxCreateSphere(const Vector3 position, const Vector3 size, const Vector3 rotation, const Vector4 color, const UInt32 nbSubdivision)
        {
            ZoneScoped;
            ZoneName("Script API: Create Sphere", 25);
            ZoneText("Sphere", 6);
            ZoneValue(static_cast<int64_t>(nbSubdivision));

            auto& app = Application::getInstance();
            const ecs::Entity entity = EntityFactory3D::createSphere(position, size, rotation, color, nbSubdivision);
            app.getSceneManager().getScene(0).addEntity(entity);
            return entity;
        }

        components::TransformComponent *NxGetTransformComponent(ecs::Entity entity)
        {
            ZoneScoped;
            ZoneName("Script API: Get Transform Component", 34);
            ZoneValue(static_cast<int64_t>(entity));

            const auto opt = Application::m_coordinator->tryGetComponent<components::TransformComponent>(entity);
            if (!opt.has_value()) {
                LOG(NEXO_WARN, "GetTransformComponent: Entity {} does not have a TransformComponent", entity);
                return nullptr;
            }
            return &opt.value().get();
        }

        void* NxGetComponent(const ecs::Entity entity, const UInt32 componentTypeId)
        {
            ZoneScoped;
            ZoneName("Script API: Get Component", 24);
            ZoneValue(static_cast<int64_t>(entity));
            ZoneValue(static_cast<int64_t>(componentTypeId));

            const auto& coordinator = *Application::m_coordinator;
            const auto opt = coordinator.tryGetComponentById(componentTypeId, entity);
            return opt;
        }

        void NxAddComponent(const ecs::Entity entity, const UInt32 typeId, const void *componentData)
        {
            ZoneScoped;
            ZoneName("Script API: Add Component", 24);
            ZoneValue(static_cast<int64_t>(entity));
            ZoneValue(static_cast<int64_t>(typeId));

            if (componentData == nullptr) {
                LOG(NEXO_ERROR, "NxAddComponent: componentData is null for entity {}", entity);
                return;
            }
            const auto& coordinator = *Application::m_coordinator;

            coordinator.addComponent(entity, typeId, componentData);
        }

        void NxRemoveComponent(const ecs::Entity entity, const UInt32 componentTypeId)
        {
            ZoneScoped;
            ZoneName("Script API: Remove Component", 25);
            ZoneValue(static_cast<int64_t>(entity));
            ZoneValue(static_cast<int64_t>(componentTypeId));

            auto& coordinator = *Application::m_coordinator;
            coordinator.removeComponent(entity, componentTypeId);
        }

        void NxDestroyEntity(const ecs::Entity entity)
        {
            ZoneScoped;
            ZoneName("Script API: Destroy Entity", 22);
            ZoneValue(static_cast<int64_t>(entity));

            auto& coordinator = *Application::m_coordinator;
            coordinator.destroyEntity(entity);
        }

        bool NxHasComponent(const ecs::Entity entity, const UInt32 typeId)
        {
            ZoneScoped;
            ZoneName("Script API: Has Component", 22);
            ZoneValue(static_cast<int64_t>(entity));
            ZoneValue(static_cast<int64_t>(typeId));

            const auto& coordinator = *Application::m_coordinator;

            return coordinator.entityHasComponent(entity, typeId);
        }

        Int64 NxRegisterComponent(const char *name, const UInt64 componentSize, const Field *fields, const UInt64 fieldCount)
        {
            ZoneScoped;
            ZoneName("Script API: Register Component", 28);
            ZoneText(name, strlen(name));
            ZoneValue(static_cast<int64_t>(componentSize));
            ZoneValue(static_cast<int64_t>(fieldCount));

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
            ZoneScoped;
            ZoneName("Script API: Get Component Type Ids", 27);

            const auto& coordinator = *Application::m_coordinator;

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
            ZoneScoped;
            ZoneName("Script API: Create Body From Shape", 34);
            ZoneValue(static_cast<int64_t>(entity));
            ZoneValue(static_cast<int64_t>(shapeType));
            ZoneValue(static_cast<int64_t>(motionType));

            const auto& app = Application::getInstance();
            auto physicsSystem = app.getPhysicsSystem();

            if (!physicsSystem) {
                LOG(NEXO_ERROR, "Physics system not available");
                return;
            }

            components::TransformComponent transform;
            transform.pos = {position.x, position.y, position.z};
            transform.size = {size.x, size.y, size.z};
            transform.quat = glm::quat(glm::radians(glm::vec3(rotation.x, rotation.y, rotation.z)));

            auto cppShapeType = static_cast<system::ShapeType>(shapeType);
            auto cppMotionType = static_cast<JPH::EMotionType>(motionType);

            physicsSystem->createBodyFromShape(entity, transform, cppShapeType, cppMotionType);

            LOG(NEXO_DEV, "Physics body created");
        }

        void NxApplyForce(ecs::Entity entity, const Vector3 force)
        {
            ZoneScoped;
            ZoneName("Script API: Apply Force", 24);
            ZoneValue(static_cast<int64_t>(entity));
            ZoneValue(static_cast<int64_t>(force.x));
            ZoneValue(static_cast<int64_t>(force.y));
            ZoneValue(static_cast<int64_t>(force.z));

            const auto& app = Application::getInstance();
            const auto physicsSystem = app.getPhysicsSystem();
            if (!physicsSystem) {
                LOG(NEXO_ERROR, "Physics system not available");
                return;
            }
            auto& coordinator = *Application::m_coordinator;
            if (!coordinator.entityHasComponent<components::PhysicsBodyComponent>(entity)) {
                LOG(NEXO_ERROR, "Entity {} has no PhysicsBodyComponent", entity);
                return;
            }
            const auto& bodyComp = coordinator.getComponent<components::PhysicsBodyComponent>(entity);
            const JPH::BodyID joltBodyID = bodyComp.bodyID;
            const JPH::Vec3 joltForce(force.x, force.y, force.z);
            physicsSystem->applyForce(joltBodyID, joltForce);
        }
    }

} // namespace nexo::scripting
