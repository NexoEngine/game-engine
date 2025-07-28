//// Passes.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/07/2025
//  Description: Header file for the common pases enums
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "RenderPass.hpp"

namespace nexo::renderer {
    enum Passes : PassId {
        FORWARD,
        GRID,
        MASK,
        OUTLINE,
        NB_PASSES
    };
}
