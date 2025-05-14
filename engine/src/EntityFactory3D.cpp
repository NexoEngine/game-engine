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
#include "assets/AssetLocation.hpp"
#include "components/BillboardMesh.hpp"
#include "components/Light.hpp"
#include "components/Shapes3D.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "components/Camera.hpp"
#include "components/StaticMesh.hpp"
#include "components/MaterialComponent.hpp"
#include "assets/AssetCatalog.hpp"
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

        std::unique_ptr<components::Material> material = std::make_unique<components::Material>();
        material->albedoColor = color;
        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
                                assets::AssetLocation("_internal::CubeMatFlatColor@_internal"),
                                std::move(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        components::UuidComponent uuid;

        ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newCube, transform);
        Application::m_coordinator->addComponent(newCube, mesh);
        std::cout << "la" << std::endl;
        Application::m_coordinator->addComponent(newCube, matComponent);
        std::cout << "apres" << std::endl;

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

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
                                assets::AssetLocation("_internal::CubeMat@_internal"),
                                std::make_unique<components::Material>(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        components::UuidComponent uuid;

        ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newCube, transform);
        Application::m_coordinator->addComponent(newCube, mesh);
        Application::m_coordinator->addComponent(newCube, matComponent);
        Application::m_coordinator->addComponent(newCube, uuid);

        return newCube;
    }

    ecs::Entity EntityFactory3D::createBillboard(const glm::vec3 &pos, const glm::vec3 &size, const glm::vec4 &color)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;

        std::unique_ptr<components::Material> material = std::make_unique<components::Material>();
        material->albedoColor = color;
        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
                                assets::AssetLocation("_internal::BillboardMatFlatColor@_internal"),
                                std::move(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        components::BillboardComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getBillboardVAO();

        components::UuidComponent uuid;

        ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newBillboard, transform);
        Application::m_coordinator->addComponent(newBillboard, mesh);
        Application::m_coordinator->addComponent(newBillboard, matComponent);
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

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
                                assets::AssetLocation("_internal::BillboardMaterial@_internal"),
                                std::make_unique<components::Material>(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        components::UuidComponent uuid;

        ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newBillboard, transform);
        Application::m_coordinator->addComponent(newBillboard, mesh);
        Application::m_coordinator->addComponent(newBillboard, matComponent);
        Application::m_coordinator->addComponent(newBillboard, uuid);

        return newBillboard;
    }
}
