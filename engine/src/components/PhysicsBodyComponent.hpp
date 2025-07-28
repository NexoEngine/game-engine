//// PhysicsBodyComponent.hpp /////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        21/03/2024
//  Description: Header file for the Physics Body components
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <Jolt/Jolt.h>
#include <Jolt/Physics/Body/BodyID.h>

namespace nexo::components {
    struct PhysicsBodyComponent {
        enum class Type { Static, Dynamic };
        JPH::BodyID bodyID;
        Type type{};
    };
}
