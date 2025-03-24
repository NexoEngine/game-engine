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

        bodyInterface = &physicsSystem->GetBodyInterface();

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
        const int collisionSteps = 1;
        physicsSystem->Update(deltaTime, collisionSteps, tempAllocator, jobSystem);
    }

    JPH::BodyID PhysicsSystem::CreateBody(const JPH::Vec3& position, JPH::EMotionType motionType, ShapeType shapeType) {
        JPH::ShapeRefC shape;
        switch (shapeType) {
            case ShapeType::Box:
                shape = JPH::BoxShapeSettings(JPH::Vec3(1.0f, 1.0f, 1.0f)).Create().Get();
            break;
            case ShapeType::Sphere:
                shape = JPH::SphereShapeSettings(1.0f).Create().Get();
            break;
            case ShapeType::Capsule:
                shape = JPH::CapsuleShapeSettings(1.0f, 0.5f).Create().Get();
            break;
        }

        JPH::BodyCreationSettings bodySettings(shape, position, JPH::Quat::sIdentity(), motionType, Layers::MOVING);
        JPH::Body* body = bodyInterface->CreateBody(bodySettings);
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
            JPH::Vec3 position(transform.pos.x, transform.pos.y, transform.pos.z);
            physicsSystem->GetBodyInterface().SetPosition(bodyComp.bodyID, position, JPH::EActivation::Activate);
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
