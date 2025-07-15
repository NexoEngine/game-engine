//// Passes.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
