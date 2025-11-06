//// ComponentDescription.hpp /////////////////////////////////////////////////
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

#include <string>
#include <vector>

#include "Field.hpp"

namespace nexo::ecs {

    struct ComponentDescription {
        std::string name;                     // Name of the component
        std::vector<Field> fields;            // List of fields in the component
        bool internalComponent = false;       // Indicates if the component is internal
        void (*constructor)(void* memoryDst); // Pointer to the constructor function
    };

} // namespace nexo::ecs
