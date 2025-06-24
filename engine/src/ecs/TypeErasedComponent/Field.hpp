//// Field.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        25/06/2025
//  Description: Header file for the field struct used in UI scripting,
//               which represents a field in the UI with its properties
//               this struct is passed by the C# code to the native code
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <string>

#include "FieldType.hpp"

namespace nexo::ecs {

    struct Field {
        std::string name;   // Pointer to the name of the field
        FieldType type;     // Type of the field (e.g., Int, Float, String, etc.)
        uint64_t size;      // Size of the field in bytes
        uint64_t offset;    // Offset of the field in the component
    };

} // namespace nexo::ecs
