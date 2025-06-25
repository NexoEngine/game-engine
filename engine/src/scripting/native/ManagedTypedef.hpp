//// ManagedTypedef.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        07/05/2025
//  Description: Header file for defining typedef equivalent to C# managed types
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

#include <glm/glm.hpp>

namespace nexo::scripting {

    extern "C" {

        // Category: Integer
        using Byte = uint8_t;   // An 8-bit unsigned integer.
        using SByte = int8_t;  // An 8-bit signed integer. Not CLS-compliant.
        using Int16 = int16_t;  // A 16-bit signed integer.
        using Int32 = int32_t;    // A 32-bit signed integer.
        using Int64 = int64_t;   // A 64-bit signed integer.
        using UInt16 = uint16_t; // A 16-bit unsigned integer. Not CLS-compliant.
        using UInt32 = uint32_t;   // A 32-bit unsigned integer. Not CLS-compliant.
        using UInt64 = uint64_t;  // A 64-bit unsigned integer. Not CLS-compliant.

        // Category: Floating Point
        //    Half                A half-precision (16-bit) floating-point number. Unsupported in C++
        using Single = float;  // A 32-bit single-precision floating point number.
        using Double = double; // A 64-bit double-precision floating point number.

        // Category: Logical
        using Boolean = bool;  // A Boolean value (true or false).
        
        // Category: Other
        using Char = uint16_t; // A Unicode (16-bit) character.
        //    Decimal             A decimal (128-bit) value. Unsupported in C++
        using IntPtr = void*;  // A pointer to an unspecified type.

        using Vector2 = glm::vec2;
        using Vector3 = glm::vec3;
        using Vector4 = glm::vec4;

        enum class NativeComponents : UInt32 {
            Transform = 0,
            AmbientLight = 1,
            DirectionalLight = 2,
            PointLight = 3,
            SpotLight = 4,
        };
    }

} // namespace nexo::scripting
