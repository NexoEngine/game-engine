//// Masks.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/05/2025
//  Description: Header file for the pass masks
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <cstdint>

namespace nexo::renderer {
    constexpr uint32_t F_FORWARD_PASS  = 1 << 0;
    constexpr uint32_t F_OUTLINE_MASK  = 1 << 1;
    constexpr uint32_t F_OUTLINE_PASS  = 1 << 2;
    constexpr uint32_t F_GRID_PASS  = 1 << 3;
}
