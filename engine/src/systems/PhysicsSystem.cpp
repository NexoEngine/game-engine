//// PhysicsSystem.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        02/03/2025
//  Description: Source file for the physics system
//
///////////////////////////////////////////////////////////////////////////////

#include "PhysicsSystem.hpp"
#include <Jolt/Physics/Collision/Shape/ConvexHullShape.h>
#include <Jolt/Physics/Collision/Shape/BoxShape.h>
#include <Jolt/Physics/Collision/Shape/SphereShape.h>
#include <Jolt/Physics/Collision/Shape/RotatedTranslatedShape.h>
#include <Jolt/Physics/Collision/Shape/CylinderShape.h>



#include <Jolt/RegisterTypes.h>

namespace nexo::system {
    PhysicsSystem::PhysicsSystem() = default;

    PhysicsSystem::~PhysicsSystem() {
        delete physicsSystem;
        delete jobSystem;
        delete tempAllocator;
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    void PhysicsSystem::init() {
        JPH::RegisterDefaultAllocator();
        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();

        tempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);
        jobSystem = new JPH::JobSystemThreadPool(
            JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, static_cast<int>(std::thread::hardware_concurrency()) - 1
        );

        physicsSystem = new JPH::PhysicsSystem();
        physicsSystem->Init(1024, 0, 1024, 1024, broadPhaseLayerInterface, objectVsBroadPhaseLayerFilter, objectLayerPairFilter);
        physicsSystem->SetGravity(JPH::Vec3(0, -9.81f, 0));

        bodyInterface = &physicsSystem->GetBodyInterface();
        bodyLockInterface = &physicsSystem->GetBodyLockInterface();
    }

    void PhysicsSystem::update()
    {
        const double currentTime = std::chrono::duration_cast<std::chrono::duration<double>>(
            std::chrono::high_resolution_clock::now().time_since_epoch()).count();

        const double delta = currentTime - m_lastPhysicsTime;

        if (delta < fixedTimestep)
            return;

        m_lastPhysicsTime = currentTime;

        constexpr int collisionSteps = 5;
        physicsSystem->Update(fixedTimestep, collisionSteps, tempAllocator, jobSystem);

        for (const ecs::Entity entity : entities) {
            auto& transform = getComponent<components::TransformComponent>(entity);
            const auto& physicsBody = getComponent<components::PhysicsBodyComponent>(entity);

            const JPH::Vec3 pos = bodyInterface->GetPosition(physicsBody.bodyID);
            transform.pos = glm::vec3(pos.GetX(), pos.GetY(), pos.GetZ());

            const JPH::Quat rot = bodyInterface->GetRotation(physicsBody.bodyID);
            transform.quat = glm::quat(rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ());
        }
    }


    JPH::BodyID PhysicsSystem::createBodyFromShape(
        const ecs::Entity entity,
        const components::TransformComponent& transform,
        const ShapeType shapeType,
        const JPH::EMotionType motionType
    ) const
    {
        const JPH::ShapeSettings* shapeSettings = nullptr;

        switch (shapeType) {
            case ShapeType::Box:
                shapeSettings = new JPH::BoxShapeSettings(JPH::Vec3(
                    transform.size.x * 0.5f,
                    transform.size.y * 0.5f,
                    transform.size.z * 0.5f));
                break;

            case ShapeType::Sphere:
                shapeSettings = new JPH::SphereShapeSettings(transform.size.x);
                break;

            case ShapeType::Cylinder: {
                const float halfHeight = transform.size.y;
                const float radius = transform.size.x;
                shapeSettings = new JPH::CylinderShapeSettings(halfHeight, radius);
                break;
            }

            case ShapeType::Tetrahedron: {
                const auto size = transform.size.x;
                // Define the vertices of the tetrahedron relative to its center
                const JPH::Vec3 vertices[] = {
                    JPH::Vec3(-size, -size, -size),
                    JPH::Vec3(size, -size, size),
                    JPH::Vec3(-size, size, size),
                    JPH::Vec3(size, size, -size)
                };

                // Create a convex hull shape using the vertices
                shapeSettings = new JPH::ConvexHullShapeSettings(vertices, std::size(vertices));
                break;
            }

            case ShapeType::Pyramid: {
                // Define the vertices of the pyramid relative to its center
                const JPH::Vec3 vertices[] = {
                    JPH::Vec3(0.0f, 1.0f, 0.0f) * transform.size.y, // Apex
                    JPH::Vec3(-1.0f, -1.0f, -1.0f) * transform.size.x, // Base vertex 1
                    JPH::Vec3(1.0f, -1.0f, -1.0f) * transform.size.x,  // Base vertex 2
                    JPH::Vec3(1.0f, -1.0f, 1.0f) * transform.size.x,   // Base vertex 3
                    JPH::Vec3(-1.0f, -1.0f, 1.0f) * transform.size.x   // Base vertex 4
                };

                // Create a convex hull shape using the vertices
                shapeSettings = new JPH::ConvexHullShapeSettings(vertices, std::size(vertices));
                break;
            }

            default:
                LOG(NEXO_ERROR, "Unsupported shape type");
                return {};
        }

        JPH::ShapeRefC shape = nullptr;
        try {
            shape = shapeSettings->Create().Get();
        } catch (const std::exception& e) {
            LOG(NEXO_ERROR, "Exception during shape creation: {}", e.what());
            delete shapeSettings;
            return {};
        }

        delete shapeSettings;

        if (!shape) {
            LOG(NEXO_ERROR, "Shape was null after creation.");
            return {};
        }

        const JPH::Vec3 position(transform.pos.x, transform.pos.y, transform.pos.z);
        const JPH::Quat rotation(transform.quat.x, transform.quat.y, transform.quat.z, transform.quat.w);

        const JPH::BodyCreationSettings bodySettings(
            shape, position, rotation, motionType,
            motionType == JPH::EMotionType::Dynamic ? Layers::MOVING : Layers::NON_MOVING
        );

        const JPH::Body* body = bodyInterface->CreateBody(bodySettings);
        if (!body) {
            LOG(NEXO_ERROR, "Body creation failed.");
            return {};
        }

        bodyInterface->AddBody(body->GetID(),
            motionType == JPH::EMotionType::Dynamic ? JPH::EActivation::Activate : JPH::EActivation::DontActivate
        );

        const auto type = motionType == JPH::EMotionType::Dynamic
            ? components::PhysicsBodyComponent::Type::Dynamic
            : components::PhysicsBodyComponent::Type::Static;

        coord->addComponent(entity, components::PhysicsBodyComponent{ body->GetID(), type });

        return body->GetID();
    }

    JPH::BodyID PhysicsSystem::createDynamicBody(const ecs::Entity entity, const components::TransformComponent& transform) const
    {
        const JPH::Vec3 halfExtent(transform.size.x * 0.5f, transform.size.y * 0.5f, transform.size.z * 0.5f);
        const JPH::BoxShapeSettings shapeSettings(halfExtent);
        const JPH::ShapeRefC shape = shapeSettings.Create().Get();

        const JPH::BodyCreationSettings bodySettings(
            shape,
            JPH::Vec3(transform.pos.x, transform.pos.y, transform.pos.z),
            JPH::Quat(transform.quat.x, transform.quat.y, transform.quat.z, transform.quat.w),
            JPH::EMotionType::Dynamic,
            Layers::MOVING
        );

        const JPH::Body* body = bodyInterface->CreateBody(bodySettings);

        // Inertia => Non tested
        // if (body->IsDynamic()) {
        //     body->GetMotionProperties()->SetInverseInertia(JPH::Vec3::sReplicate(1.0f), JPH::Quat::sIdentity());
        // }

        bodyInterface->AddBody(body->GetID(), JPH::EActivation::Activate);

        coord->addComponent(entity, components::PhysicsBodyComponent{ body->GetID(), components::PhysicsBodyComponent::Type::Dynamic });
        return body->GetID();
    }

    JPH::BodyID PhysicsSystem::createStaticBody(ecs::Entity entity, const components::TransformComponent& transform) const
    {
        JPH::BoxShapeSettings baseShapeSettings(
            JPH::Vec3(transform.size.x * 0.5f, transform.size.y * 0.5f, transform.size.z * 0.5f)
        );
        JPH::ShapeRefC baseShape = baseShapeSettings.Create().Get();

        JPH::RotatedTranslatedShapeSettings compoundSettings(
            JPH::Vec3::sZero(),
            JPH::Quat(transform.quat.x, transform.quat.y, transform.quat.z, transform.quat.w),
            baseShape
        );
        JPH::ShapeRefC rotatedShape = compoundSettings.Create().Get();

        JPH::BodyCreationSettings bodySettings(
            rotatedShape,
            JPH::Vec3(transform.pos.x, transform.pos.y, transform.pos.z),
            JPH::Quat::sIdentity(),
            JPH::EMotionType::Static,
            Layers::NON_MOVING
        );

        JPH::Body* body = bodyInterface->CreateBody(bodySettings);
        bodyInterface->AddBody(body->GetID(), JPH::EActivation::DontActivate);

        coord->addComponent(entity, components::PhysicsBodyComponent{ body->GetID(), components::PhysicsBodyComponent::Type::Static });
        return body->GetID();
    }

    JPH::BodyID PhysicsSystem::createBody(const components::TransformComponent& transform, JPH::EMotionType motionType) const
    {
        JPH::Vec3 halfExtent(transform.size.x * 0.5f, transform.size.y * 0.5f, transform.size.z * 0.5f);
        JPH::BoxShapeSettings shapeSettings(halfExtent);
        JPH::ShapeRefC shape = shapeSettings.Create().Get();

        JPH::Vec3 position(transform.pos.x, transform.pos.y, transform.pos.z);
        JPH::Quat rotation(transform.quat.x, transform.quat.y, transform.quat.z, transform.quat.w);

        JPH::BodyCreationSettings bodySettings(shape, position, rotation, motionType, Layers::MOVING);
        JPH::Body* body = bodyInterface->CreateBody(bodySettings);

        body->GetMotionProperties()->SetInverseInertia(JPH::Vec3::sReplicate(1.0f), JPH::Quat::sIdentity());

        bodyInterface->AddBody(body->GetID(), JPH::EActivation::Activate);
        return body->GetID();
    }


    void PhysicsSystem::syncTransformsToBodies(const std::vector<ecs::Entity>& entities, ecs::Coordinator& coordinator) const
    {
        for (const auto& entity : entities) {
            if (!coordinator.entityHasComponent<components::TransformComponent>(entity) ||
                !coordinator.entityHasComponent<components::PhysicsBodyComponent>(entity))
                continue;

            auto& transform = coordinator.getComponent<components::TransformComponent>(entity);
            auto& bodyComp = coordinator.getComponent<components::PhysicsBodyComponent>(entity);

            const JPH::Vec3 position = physicsSystem->GetBodyInterface().GetPosition(bodyComp.bodyID);
            transform.pos = glm::vec3(position.GetX(), position.GetY(), position.GetZ());

            JPH::Quat rot = physicsSystem->GetBodyInterface().GetRotation(bodyComp.bodyID);

            transform.quat = glm::quat(rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ());
        }
    }

    void PhysicsSystem::applyForce(const JPH::BodyID bodyID, const JPH::Vec3& force) const
    {
        bodyInterface->AddForce(bodyID, force);
    }

    void PhysicsSystem::setGravity(const JPH::Vec3& gravity) const
    {
        physicsSystem->SetGravity(gravity);
    }

    void PhysicsSystem::activateBody(const JPH::BodyID bodyID) const
    {
        bodyInterface->ActivateBody(bodyID);
    }

    void PhysicsSystem::deactivateBody(const JPH::BodyID bodyID) const
    {
        bodyInterface->DeactivateBody(bodyID);
    }

}
