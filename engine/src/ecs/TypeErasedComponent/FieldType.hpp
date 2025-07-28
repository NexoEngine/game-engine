//// FieldType.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        25/06/2025
//  Description: Header file for the field type enumeration
//               used in UI components
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>

namespace nexo::ecs {

    enum class FieldType : uint64_t {
        // Special type, if blank, the field is not used
        Blank,
        Section, // Used to create a section with title in the UI

        // Primitive types
        Bool,
        Int8,
        Int16,
        Int32,
        Int64,
        UInt8,
        UInt16,
        UInt32,
        UInt64,
        Float,
        Double,

        // Widgets
        Vector3,
        Vector4,

        _Count // Count of the number of field types, used for validation
    };

} // namespace nexo::ecs
