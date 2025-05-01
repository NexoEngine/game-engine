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
//  Description: Header file for init the physics system
//
///////////////////////////////////////////////////////////////////////////////

#include "PhysicsSystem.hpp"

namespace nexo::components {
    struct PhysicsBodyComponent;
    struct TransformComponent;
}

namespace nexo::ecs {
    class Coordinator;
}

namespace nexo::system {
    PhysicsSystem::PhysicsSystem() {
        std::cout << "Init Jolt Physics..." << std::endl;

        // Init physics system
        JPH::RegisterDefaultAllocator();
        JPH::Factory::sInstance = new JPH::Factory();
        JPH::RegisterTypes();

        // Temporary allocator
        tempAllocator = new JPH::TempAllocatorImpl(10 * 1024 * 1024);

        // Multithreading
        jobSystem = new JPH::JobSystemThreadPool(JPH::cMaxPhysicsJobs, JPH::cMaxPhysicsBarriers, std::thread::hardware_concurrency() - 1);

        // Init physics system
        physicsSystem = new JPH::PhysicsSystem();
        physicsSystem->Init(1024, 0, 1024, 1024, broadPhaseLayerInterface, objectVsBroadPhaseLayerFilter, objectLayerPairFilter);

        physicsSystem->SetGravity(JPH::Vec3(0, -9.81f, 0));

        bodyInterface = &physicsSystem->GetBodyInterface();
        bodyLockInterface = &physicsSystem->GetBodyLockInterface();

        std::cout << "Jolt Physics init" << std::endl;
    }

    PhysicsSystem::~PhysicsSystem() {
        delete physicsSystem;
        delete jobSystem;
        delete tempAllocator;
        delete JPH::Factory::sInstance;
        JPH::Factory::sInstance = nullptr;
    }

    void PhysicsSystem::Update(float deltaTime) {
        const int collisionSteps = 5;
        // TODO: fix temporaire, update appelé trop souvent donc delta a 60ips
        physicsSystem->Update(1.0f / 60.0f, collisionSteps, tempAllocator, jobSystem);
    }

    JPH::BodyID PhysicsSystem::CreateBody(const components::TransformComponent& transform, JPH::EMotionType motionType)
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


    void PhysicsSystem::SyncTransformsToBodies(const std::vector<ecs::Entity>& entities, ecs::Coordinator& coordinator) const {
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



    void PhysicsSystem::ApplyForce(JPH::BodyID bodyID, const JPH::Vec3& force) {
        bodyInterface->AddForce(bodyID, force);
    }

    void PhysicsSystem::SetGravity(const JPH::Vec3& gravity) {
        physicsSystem->SetGravity(gravity);
    }

    void PhysicsSystem::ActivateBody(JPH::BodyID bodyID) {
        bodyInterface->ActivateBody(bodyID);
    }

    void PhysicsSystem::DeactivateBody(JPH::BodyID bodyID) {
        bodyInterface->DeactivateBody(bodyID);
    }

}
