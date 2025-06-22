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

namespace nexo::system {
    PhysicsSystem::PhysicsSystem() {

    }

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
            JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1
        );

        physicsSystem = new JPH::PhysicsSystem();
        physicsSystem->Init(1024, 0, 1024, 1024, broadPhaseLayerInterface, objectVsBroadPhaseLayerFilter, objectLayerPairFilter);
        physicsSystem->SetGravity(JPH::Vec3(0, -9.81f, 0));

        bodyInterface = &physicsSystem->GetBodyInterface();
        bodyLockInterface = &physicsSystem->GetBodyLockInterface();
    }

    void PhysicsSystem::update(float timestep) {
        const int collisionSteps = 5;
        physicsSystem->Update(timestep, collisionSteps, tempAllocator, jobSystem);

        for (ecs::Entity entity : entities) {
            auto& transform = getComponent<components::TransformComponent>(entity);
            auto& physicsBody = getComponent<components::PhysicsBodyComponent>(entity);

            const JPH::Vec3 pos = bodyInterface->GetPosition(physicsBody.bodyID);
            transform.pos = glm::vec3(pos.GetX(), pos.GetY(), pos.GetZ());

            const JPH::Quat rot = bodyInterface->GetRotation(physicsBody.bodyID);
            transform.quat = glm::quat(rot.GetW(), rot.GetX(), rot.GetY(), rot.GetZ());
        }
    }

JPH::BodyID PhysicsSystem::createBodyFromShape(
    ecs::Entity entity,
    const components::TransformComponent& transform,
    ShapeType shapeType,
    JPH::EMotionType motionType
) {
    JPH::ShapeSettings* shapeSettings = nullptr;

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

    case ShapeType::Cylinder:
        shapeSettings = new JPH::CylinderShapeSettings(
            -transform.size.y * 0.5f,  // half-height min
             transform.size.y * 0.5f,  // half-height max
             transform.size.x * 0.5f   // radius (assuming X = Z)
        );
        break;

        default:
            LOG(NEXO_ERROR, "Unsupported shape type");
            return JPH::BodyID();
    }

    JPH::ShapeRefC shape = shapeSettings->Create().Get();
    delete shapeSettings;

    JPH::BodyCreationSettings bodySettings(
        shape,
        JPH::Vec3(transform.pos.x, transform.pos.y, transform.pos.z),
        JPH::Quat(transform.quat.x, transform.quat.y, transform.quat.z, transform.quat.w),
        motionType,
        motionType == JPH::EMotionType::Dynamic ? Layers::MOVING : Layers::NON_MOVING
    );

    JPH::Body* body = bodyInterface->CreateBody(bodySettings);
    bodyInterface->AddBody(body->GetID(),
        motionType == JPH::EMotionType::Dynamic ? JPH::EActivation::Activate : JPH::EActivation::DontActivate
    );

    components::PhysicsBodyComponent::Type type =
        motionType == JPH::EMotionType::Dynamic
            ? components::PhysicsBodyComponent::Type::Dynamic
            : components::PhysicsBodyComponent::Type::Static;

    coord->addComponent(entity, components::PhysicsBodyComponent{ body->GetID(), type });
    return body->GetID();
}


    JPH::BodyID PhysicsSystem::createDynamicBody(ecs::Entity entity, const components::TransformComponent& transform) {
        JPH::Vec3 halfExtent(transform.size.x * 0.5f, transform.size.y * 0.5f, transform.size.z * 0.5f);
        JPH::BoxShapeSettings shapeSettings(halfExtent);
        JPH::ShapeRefC shape = shapeSettings.Create().Get();

        JPH::BodyCreationSettings bodySettings(
            shape,
            JPH::Vec3(transform.pos.x, transform.pos.y, transform.pos.z),
            JPH::Quat(transform.quat.x, transform.quat.y, transform.quat.z, transform.quat.w),
            JPH::EMotionType::Dynamic,
            Layers::MOVING
        );

        JPH::Body* body = bodyInterface->CreateBody(bodySettings);

        // Inertia => Non tested
        // if (body->IsDynamic()) {
        //     body->GetMotionProperties()->SetInverseInertia(JPH::Vec3::sReplicate(1.0f), JPH::Quat::sIdentity());
        // }

        bodyInterface->AddBody(body->GetID(), JPH::EActivation::Activate);

        coord->addComponent(entity, components::PhysicsBodyComponent{ body->GetID(), components::PhysicsBodyComponent::Type::Dynamic });
        return body->GetID();
    }

    JPH::BodyID PhysicsSystem::createStaticBody(ecs::Entity entity, const components::TransformComponent& transform) {
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

    JPH::BodyID PhysicsSystem::createBody(const components::TransformComponent& transform, JPH::EMotionType motionType)
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


    void PhysicsSystem::syncTransformsToBodies(const std::vector<ecs::Entity>& entities, ecs::Coordinator& coordinator) const {
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



    void PhysicsSystem::applyForce(JPH::BodyID bodyID, const JPH::Vec3& force) {
        bodyInterface->AddForce(bodyID, force);
    }

    void PhysicsSystem::setGravity(const JPH::Vec3& gravity) {
        physicsSystem->SetGravity(gravity);
    }

    void PhysicsSystem::activateBody(JPH::BodyID bodyID) {
        bodyInterface->ActivateBody(bodyID);
    }

    void PhysicsSystem::deactivateBody(JPH::BodyID bodyID) {
        bodyInterface->DeactivateBody(bodyID);
    }

}
