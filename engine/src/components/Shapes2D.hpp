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
#include "renderer/RendererContext.hpp"

#include <glm/gtc/matrix_transform.hpp>

namespace nexo::components {

    struct Shape2D {
        virtual ~Shape2D() = default;

        virtual void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf, const SpriteComponent &sprite, int entityID) const = 0;
        virtual bool isClicked(const TransformComponent &transf, const glm::vec2 &mouseWorldPos) = 0;
    };

    struct Quad final : Shape2D {
        void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf, const SpriteComponent &sprite, int entityID) const override
        {
            const auto renderer2D = context->renderer2D;
            if (sprite.sprite != nullptr)
                renderer2D.drawQuad(transf.pos, {transf.size.x, transf.size.y}, transf.rotation.z,
                                               sprite.sprite);
            else if (sprite.texture != nullptr)
                renderer2D.drawQuad(transf.pos, {transf.size.x, transf.size.y}, transf.rotation.z,
                                               sprite.texture);
            else
                renderer2D.drawQuad(transf.pos, {transf.size.x, transf.size.y}, transf.rotation.z,
                                               sprite.color);
        }

        bool isClicked(const TransformComponent &transf, const glm::vec2 &mouseWorldPos) override
        {
            const glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(transf.pos.x, transf.pos.y, 0.0f)) *
                                        glm::rotate(glm::mat4(1.0f), glm::radians(transf.rotation.z), glm::vec3(0.0f, 0.0f, 1.0f)) *
                                        glm::scale(glm::mat4(1.0f), glm::vec3(transf.size.x, transf.size.y, 1.0f));

            glm::mat4 inverseTransform = glm::inverse(transform);

            glm::vec4 localMousePos = inverseTransform * glm::vec4(mouseWorldPos, 0.0f, 1.0f);

            auto halfSize = glm::vec2(0.5f);
            return localMousePos.x >= -halfSize.x && localMousePos.x <= halfSize.x &&
                   localMousePos.y >= -halfSize.y && localMousePos.y <= halfSize.y;
        }
    };
}
