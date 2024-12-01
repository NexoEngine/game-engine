//// Render2D.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header files for the render 2D components
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/SubTexture2D.hpp"

namespace nexo::components {

    struct SpriteComponent {
        glm::vec4 color;
        std::shared_ptr<renderer::Texture2D> texture = nullptr;
        std::shared_ptr<renderer::SubTexture2D> sprite = nullptr;
    };

}