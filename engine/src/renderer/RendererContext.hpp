//// RendererContext.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Header file for the renderer context class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Renderer2D.hpp"
#include "Renderer3D.hpp"

namespace nexo::renderer {
    class NxRendererContext {
        public:
            NxRendererContext() = default;
            NxRenderer2D renderer2D;
            NxRenderer3D renderer3D;
    };
}
