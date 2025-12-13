//// PhysicsBodyComponent.hpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        21/03/2024
//  Description: Header file for the Physics Body components
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>
#include <glm/gtc/quaternion.hpp>

namespace nexo::components {
    struct PhysicsBodyComponent {
        enum class Type { Static, Dynamic };

        struct Memento {
            JPH::BodyID bodyID{};
            Type type{Type::Static};
        };

        void restore(const Memento &memento)
        {
            bodyID = memento.bodyID;
            type   = memento.type;
        }

        [[nodiscard]] Memento save() const
        {
            return Memento{bodyID, type};
        }

        JPH::BodyID bodyID;
        Type type{};
        glm::vec3 lastPos{};
        glm::quat lastRot{};
    };
} // namespace nexo::components

#include "PhysicsBodyComponent.serializer.hpp"
