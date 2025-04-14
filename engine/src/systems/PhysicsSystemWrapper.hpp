//// PhysicsSystemWrapper.hpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        21/03/2025
//  Description: Header file for the wrapper of the physics system
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "PhysicsSystem.hpp"
#include "Coordinator.hpp"
#include "Components.hpp"

namespace nexo::system {

    class PhysicsSystemWrapper {
    public:
        void Init(ecs::Coordinator& coordinator);
        void Update(float dt, ecs::Coordinator& coordinator);
        void AddPhysicsBody(ecs::Entity entity, const components::TransformComponent& transform);
        void AddStaticBody(ecs::Entity entity, const components::TransformComponent& transform);

    private:
        ecs::Coordinator* m_coordinator = nullptr;
        PhysicsSystem physicsSystem;
    };

}
