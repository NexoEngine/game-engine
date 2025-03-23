//// EntityFactory2D.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/11/2024
//  Description: Source for the 2D entity factory
//
///////////////////////////////////////////////////////////////////////////////

#include "EntityFactory2D.hpp"

#include <Application.hpp>

#include "components/Components.hpp"

namespace nexo {
    ecs::Entity EntityFactory2D::createQuad(glm::vec3 pos, glm::vec2 size, [[maybe_unused]] float rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = {size.x, size.y, 1.0f};
        //transform.rotation = {0.0f, 0.0f, rotation};
        components::SpriteComponent sprite;
        sprite.color = color;
        auto quad = std::make_shared<components::Quad>();
        auto renderable = std::make_shared<components::Renderable2D>(sprite, quad);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_2D);

        ecs::Entity newQuad = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newQuad, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newQuad, renderComponent);
        return newQuad;
    }
}
