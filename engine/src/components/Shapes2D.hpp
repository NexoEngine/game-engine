//// Shapes2D.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header files for the 2D shapes components
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Transform.hpp"
#include "Render2D.hpp"
#include "renderer/Renderer2D.hpp"

namespace nexo::components {

    struct Shape2D {
        virtual ~Shape2D() = default;

        virtual void draw(const TransformComponent &transf, const SpriteComponent &sprite) const = 0;
    };

    struct Quad final : Shape2D {
        void draw(const TransformComponent &transf, const SpriteComponent &sprite) const override
        {
            if (sprite.sprite != nullptr)
                renderer::Renderer2D::drawQuad(transf.pos, {transf.size.x, transf.size.y}, transf.rotation.z,
                                               sprite.sprite);
            else if (sprite.texture != nullptr)
                renderer::Renderer2D::drawQuad(transf.pos, {transf.size.x, transf.size.y}, transf.rotation.z,
                                               sprite.texture);
            else
                renderer::Renderer2D::drawQuad(transf.pos, {transf.size.x, transf.size.y}, transf.rotation.z,
                                               sprite.color);
        }
    };
}
