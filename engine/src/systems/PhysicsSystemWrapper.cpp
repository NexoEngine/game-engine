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

}