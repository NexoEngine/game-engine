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
#include "Renderer3D.hpp"
#include "components/BillboardMesh.hpp"
#include "components/Light.hpp"
#include "components/Shapes3D.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "components/Camera.hpp"
#include "components/StaticMesh.hpp"
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

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getCubeVAO();

        components::Material material{};
        material.albedoColor = color;

        components::UuidComponent uuid;

        ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newCube, transform);
        Application::m_coordinator->addComponent(newCube, mesh);
        Application::m_coordinator->addComponent(newCube, material);
        Application::m_coordinator->addComponent(newCube, uuid);

        return newCube;
    }

    ecs::Entity EntityFactory3D::createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, const components::Material &material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getCubeVAO();

        components::UuidComponent uuid;

        ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newCube, transform);
        Application::m_coordinator->addComponent(newCube, mesh);
        Application::m_coordinator->addComponent(newCube, material);
        Application::m_coordinator->addComponent(newCube, uuid);

        return newCube;
    }

    ecs::Entity EntityFactory3D::createBillboard(const glm::vec3 &pos, const glm::vec3 &size, const glm::vec4 &color)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;

        components::Material material{};
        material.albedoColor = color;

        components::BillboardComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getBillboardVAO();

        components::UuidComponent uuid;

        ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newBillboard, transform);
        Application::m_coordinator->addComponent(newBillboard, mesh);
        Application::m_coordinator->addComponent(newBillboard, material);
        Application::m_coordinator->addComponent(newBillboard, uuid);

        return newBillboard;
    }

    ecs::Entity EntityFactory3D::createBillboard(const glm::vec3 &pos, const glm::vec3 &size, const components::Material &material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;

        components::BillboardComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getBillboardVAO();

        components::UuidComponent uuid;

        ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newBillboard, transform);
        Application::m_coordinator->addComponent(newBillboard, mesh);
        Application::m_coordinator->addComponent(newBillboard, material);
        Application::m_coordinator->addComponent(newBillboard, uuid);

        return newBillboard;
    }
}
