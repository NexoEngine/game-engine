//// PhysicsSystemWrapper.cpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        21/03/2025
//  Description: Wrapper for the Physics System with ecs
//
///////////////////////////////////////////////////////////////////////////////

#include "PhysicsSystemWrapper.hpp"

namespace nexo::system {

    void PhysicsSystemWrapper::Init(ecs::Coordinator& coordinator) {
        m_coordinator = &coordinator;

        physicsSystem.SetGravity(JPH::Vec3(0.0f, -9.81f, 0.0f));
    }

    void PhysicsSystemWrapper::Update(float dt, ecs::Coordinator& coordinator) {
        physicsSystem.Update(dt);

        auto entities = coordinator.getEntitiesWithComponents<components::TransformComponent, components::PhysicsBodyComponent>();
        physicsSystem.SyncTransformsToBodies(entities, coordinator);
    }

    void PhysicsSystemWrapper::AddPhysicsBody(ecs::Entity entity, const components::TransformComponent& transform) {
        JPH::Vec3 position(transform.pos.x, transform.pos.y, transform.pos.z);
        JPH::BodyID id = physicsSystem.CreateBody(position, JPH::EMotionType::Dynamic, ShapeType::Box);

        m_coordinator->addComponent<components::PhysicsBodyComponent>(entity, components::PhysicsBodyComponent{});
        auto& bodyComp = m_coordinator->getComponent<components::PhysicsBodyComponent>(entity);
        bodyComp.bodyID = id;
    }

    void PhysicsSystemWrapper::AddStaticBody(ecs::Entity entity, const components::TransformComponent& transform)
    {
        // Crée la forme de la plaque
        JPH::BoxShapeSettings shapeSettings(JPH::Vec3(transform.size.x / 2.0f, transform.size.y / 2.0f, transform.size.z / 2.0f));
        JPH::ShapeRefC shape = shapeSettings.Create().Get();

        // Création du body statique
        JPH::BodyCreationSettings bodySettings(
            shape,
            JPH::Vec3(transform.pos.x, transform.pos.y, transform.pos.z),
            JPH::Quat(transform.quat.x, transform.quat.y, transform.quat.z, transform.quat.w), // <- ici
            JPH::EMotionType::Static,
            Layers::NON_MOVING
        );


        auto* bodyInterface = physicsSystem.getBodyInterface();
        JPH::Body* body = bodyInterface->CreateBody(bodySettings);
        bodyInterface->AddBody(body->GetID(), JPH::EActivation::DontActivate);
    }


}