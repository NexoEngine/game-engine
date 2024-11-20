//// Shapes3D.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Header for the 3D shapes
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Render3D.hpp"
#include "renderer/RendererContext.hpp"

namespace nexo::components {

    struct Shape3D {
        virtual ~Shape3D() = default;

        virtual void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf,
                          const MaterialComponent &material) = 0;
    };

    struct Cube final : Shape3D {
        void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf,
                  const MaterialComponent &material) override
        {
            auto renderer3D = context->renderer3D;
            renderer3D.drawCube(transf.pos, transf.size, material.color);
        }
    };

}
