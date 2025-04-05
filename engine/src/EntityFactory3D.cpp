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
#include "components/Light.hpp"
#include "components/Shapes3D.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "components/Camera.hpp"
#include "Application.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <filesystem>

namespace nexo {

    ecs::Entity EntityFactory3D::createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};

        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        components::Material material{};
        material.albedoColor = color;
        auto cube = std::make_shared<components::Cube>();
        auto renderable = std::make_shared<components::Renderable3D>(material, cube);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newCube, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newCube, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newCube, uuid);

        return newCube;
    }

    ecs::Entity EntityFactory3D::createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, const components::Material &material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        auto cube = std::make_shared<components::Cube>();
        auto renderable = std::make_shared<components::Renderable3D>(material, cube);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newCube, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newCube, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newCube, uuid);
        return newCube;
    }

    ecs::Entity EntityFactory3D::createTetrahedron(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};

        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        components::Material material{};
        material.albedoColor = color;
        auto tetrahedron = std::make_shared<components::Tetrahedron>();
        auto renderable = std::make_shared<components::Renderable3D>(material, tetrahedron);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newTetrahedron = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newTetrahedron, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newTetrahedron, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newTetrahedron, uuid);
        return newTetrahedron;
    }

    ecs::Entity EntityFactory3D::createTetrahedron(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, const components::Material &material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        auto tetrahedron = std::make_shared<components::Tetrahedron>();
        auto renderable = std::make_shared<components::Renderable3D>(material, tetrahedron);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newTetrahedron = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newTetrahedron, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newTetrahedron, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newTetrahedron, uuid);
        return newTetrahedron;
    }

    ecs::Entity EntityFactory3D::createPyramid(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};

        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        components::Material material{};
        material.albedoColor = color;
        auto pyramid = std::make_shared<components::Pyramid>();
        auto renderable = std::make_shared<components::Renderable3D>(material, pyramid);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newPyramid = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newPyramid, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newPyramid, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newPyramid, uuid);
        return newPyramid;
    }

    ecs::Entity EntityFactory3D::createPyramid(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, const components::Material &material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        auto pyramid = std::make_shared<components::Pyramid>();
        auto renderable = std::make_shared<components::Renderable3D>(material, pyramid);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newPyramid = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newPyramid, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newPyramid, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newPyramid, uuid);
        return newPyramid;
    }

        ecs::Entity EntityFactory3D::createCylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};

        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        components::Material material{};
        material.albedoColor = color;
        auto cylinder = std::make_shared<components::Cylinder>();
        auto renderable = std::make_shared<components::Renderable3D>(material, cylinder);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newCylinder = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newCylinder, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newCylinder, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newCylinder, uuid);
        return newCylinder;
    }

    ecs::Entity EntityFactory3D::createCylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, const components::Material &material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);
        auto cylinder = std::make_shared<components::Cylinder>();
        auto renderable = std::make_shared<components::Renderable3D>(material, cylinder);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newCylinder = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newCylinder, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newCylinder, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newCylinder, uuid);
        return newCylinder;
    }

    ecs::Entity EntityFactory3D::createBillboard(const glm::vec3 &pos, const glm::vec3 &size, const glm::vec4 &color)
    {
        components::TransformComponent transform{};

        transform.pos = pos;
        transform.size = size;
        components::Material material{};
        material.albedoColor = color;
        auto billboard = std::make_shared<components::BillBoard>();
        auto renderable = std::make_shared<components::Renderable3D>(material, billboard);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newBillboard, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newBillboard, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newBillboard, uuid);

        return newBillboard;
    }

    ecs::Entity EntityFactory3D::createBillboard(const glm::vec3 &pos, const glm::vec3 &size, const components::Material &material)
    {
        components::TransformComponent transform{};

        transform.pos = pos;
        transform.size = size;
        auto billboard = std::make_shared<components::BillBoard>();
        auto renderable = std::make_shared<components::Renderable3D>(material, billboard);
        components::RenderComponent renderComponent(renderable, components::RenderType::RENDER_3D);

        ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent<components::TransformComponent>(newBillboard, transform);
        Application::m_coordinator->addComponent<components::RenderComponent>(newBillboard, renderComponent);
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newBillboard, uuid);

        return newBillboard;
    }
}
