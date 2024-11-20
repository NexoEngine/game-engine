//// EntityFactory3D.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Source file for the 3D entity factory
//
///////////////////////////////////////////////////////////////////////////////

#include "EntityFactory3D.hpp"
#include "components/Components.hpp"
#include "Application.hpp"


namespace nexo {

    ecs::Entity EntityFactory3D::createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.rotation = rotation;
        components::MaterialComponent material{};
        material.color = color;
        auto cube = std::make_shared<components::Cube>();
        auto renderable = std::make_shared<components::Renderable3D>(material, cube);
        components::RenderComponent renderComponent(renderable);

        ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newCube, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newCube, renderComponent);
        return newCube;
    }

}