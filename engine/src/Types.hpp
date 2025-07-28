//// Types.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        18/04/2025
//  Description: Header file for the common engine types
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

namespace nexo {
    enum class RenderingType {
        WINDOW,
        FRAMEBUFFER
    };

    enum class SceneType {
        EDITOR,
        GAME
    };
}
