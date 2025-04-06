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

namespace nexo::components
{
    enum class RenderType
    {
        RENDER_2D,
        RENDER_3D
    };

    struct Renderable
    {
        virtual ~Renderable() = default;

        bool isRendered = true;

        virtual void draw(std::shared_ptr<renderer::RendererContext> &context,
                          const TransformComponent &transf, int entityID) const = 0;
        [[nodiscard]] virtual std::shared_ptr<Renderable> clone() const = 0;
    };

    struct Renderable2D final : Renderable
    {
        SpriteComponent sprite;
        std::shared_ptr<Shape2D> shape;

        explicit Renderable2D(SpriteComponent sprite,
                              const std::shared_ptr<Shape2D>& shape) : sprite(std::move(sprite)), shape(shape)
        {
        };

        void draw(std::shared_ptr<renderer::RendererContext> &context,
                  const TransformComponent &transform, int entityID) const override
        {
            shape->draw(context, transform, sprite, entityID);
        }

        [[nodiscard]] std::shared_ptr<Renderable> clone() const override
        {
            //std::shared_ptr<Shape2D> clonedShape = shape ? shape->clone() : nullptr;
            //return std::make_shared<Renderable2D>(sprite, clonedShape);
            return nullptr;
        }
    };

    struct Renderable3D final : Renderable {
        Material material;
        std::shared_ptr<Shape3D> shape;

        explicit Renderable3D(Material  material,
                              const std::shared_ptr<Shape3D> &shape) : material(std::move(material)), shape(shape) {};

        void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transf, int entityID) const override
        {
            shape->draw(context, transf, material, entityID);
        }

        [[nodiscard]] std::shared_ptr<Renderable> clone() const override
        {
            std::shared_ptr<Shape3D> clonedShape = shape ? shape->clone() : nullptr;
            return std::make_shared<Renderable3D>(material, clonedShape);
        }
    };

    struct RenderComponent {
        bool isRendered = true;
        RenderType type = RenderType::RENDER_2D;

        std::shared_ptr<Renderable> renderable;

        RenderComponent() = default;

        explicit RenderComponent(const std::shared_ptr<Renderable>& renderable, const RenderType type)
            : type(type), renderable(renderable)
        {
        }

        void draw(std::shared_ptr<renderer::RendererContext> &context, const TransformComponent &transform, const int entityID = -1) const
        {
            if (isRendered && renderable)
                renderable->draw(context, transform, entityID);
        }

        [[nodiscard]] RenderComponent clone() const {
            RenderComponent copy;
            copy.isRendered = isRendered;
            copy.type = type;
            if (renderable)
                copy.renderable = renderable->clone();
            else
                copy.renderable = nullptr;
            return copy;
        }
    };
}
