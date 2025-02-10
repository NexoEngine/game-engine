//// Render.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/11/2024
//  Description: Header file for the render component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Render2D.hpp"
#include "Render3D.hpp"
#include "Transform.hpp"
#include "Shapes2D.hpp"
#include "Shapes3D.hpp"

#include <memory>
#include <utility>

namespace nexo::components {

    struct Renderable {
        virtual ~Renderable() = default;

        bool isRendered = true;

        virtual void draw(std::shared_ptr<renderer::RendererContext> &context,
                          const TransformComponent &transf, int entityID) const = 0;

        virtual bool isClicked(const TransformComponent &transf, const glm::vec2 &mouseWorldPos) = 0;
    };

    struct Renderable2D final : Renderable {
        SpriteComponent sprite;
        std::shared_ptr<Shape2D> shape;

        explicit Renderable2D(SpriteComponent sprite,
                              const std::shared_ptr<Shape2D> &shape) : sprite(std::move(sprite)), shape(shape)
        {};

        void draw(std::shared_ptr<renderer::RendererContext> &context,
                  const TransformComponent &transform, int entityID) const override
        {
            shape->draw(context, transform, sprite, entityID);
        }

        bool isClicked(const TransformComponent &transf, const glm::vec2 &mouseWorldPos) override
        {
            return shape->isClicked(transf, mouseWorldPos);
        }
    };

    struct Renderable3D final : Renderable {
        Material material;
        std::shared_ptr<Shape3D> shape;

        explicit Renderable3D(const Material material,
                              const std::shared_ptr<Shape3D> &shape) : material(material), shape(shape) {};

        void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf, int entityID) const override
        {
            shape->draw(context, transf, entityID);
        }

        bool isClicked([[maybe_unused]] const TransformComponent &transf, [[maybe_unused]] const glm::vec2 &mouseWorldPos) override
        {
            return false;
        }
    };

    struct RenderComponent {
        bool isRendered = true;

        std::shared_ptr<Renderable> renderable;

        RenderComponent() = default;

        explicit RenderComponent(const std::shared_ptr<Renderable> &renderable)
            : renderable(renderable) {}

        void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transform, int entityID = -1) const
        {
            if (isRendered && renderable)
                renderable->draw(context, transform, entityID);
        }
    };
}
