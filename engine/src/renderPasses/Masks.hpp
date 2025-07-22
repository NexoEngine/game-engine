//// Masks.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
