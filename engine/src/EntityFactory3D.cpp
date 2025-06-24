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
#include "Definitions.hpp"
#include "Renderer3D.hpp"
#include "assets/AssetLocation.hpp"
#include "components/BillboardMesh.hpp"
#include "components/Light.hpp"
#include "components/Model.hpp"
#include "components/Name.hpp"
#include "components/Parent.hpp"
#include "components/Render3D.hpp"
#include "components/Shapes3D.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "components/Camera.hpp"
#include "components/StaticMesh.hpp"
#include "components/MaterialComponent.hpp"
#include "assets/AssetCatalog.hpp"
#include "Application.hpp"
#include "math/Matrix.hpp"

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

        auto material = std::make_unique<components::Material>();
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
        Application::m_coordinator->addComponent(newCube, matComponent);

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

        auto material = std::make_unique<components::Material>();
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

    ecs::Entity EntityFactory3D::createModel(assets::AssetRef<assets::Model> model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation)
    {
        auto modelAsset = model.lock();
        if (!modelAsset || !modelAsset->getData())
            return ecs::INVALID_ENTITY;

        ecs::Entity rootEntity = Application::m_coordinator->createEntity();

        components::TransformComponent rootTransform;
        rootTransform.pos = pos;
        rootTransform.size = size;
        rootTransform.quat = glm::quat(glm::radians(rotation));

        // Create RootComponent to identify this as the root of a hierarchy
        components::RootComponent rootComp;
        rootComp.modelRef = model;

        auto location = modelAsset->getMetadata().location.getName();
        rootComp.name = location.data();
        // Extract just the filename from path if needed
        size_t lastSlash = rootComp.name.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            rootComp.name = rootComp.name.substr(lastSlash + 1);
        }

        Application::m_coordinator->addComponent(rootEntity, rootTransform);
        Application::m_coordinator->addComponent(rootEntity, rootComp);

        // Process model nodes to create entity hierarchy
        const assets::MeshNode& rootNode = *modelAsset->getData();
        int childCount = processModelNode(rootEntity, rootNode);

        // Update child count in root component
        rootComp.childCount = childCount;
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent(rootEntity, uuid);

        return rootEntity;
    }

    int EntityFactory3D::processModelNode(ecs::Entity parentEntity, const assets::MeshNode& node)
    {
        int totalChildrenCreated = 0;

        ecs::Entity nodeEntity = Application::m_coordinator->createEntity();
        totalChildrenCreated++;

        components::UuidComponent uuid;
        Application::m_coordinator->addComponent(nodeEntity, uuid);

        glm::vec3 translation, scale;
        glm::quat rotation;
        nexo::math::decomposeTransformQuat(node.transform, translation, rotation, scale);

        components::TransformComponent transform;
        transform.pos = translation;
        transform.size = scale;
        transform.quat = rotation;
        Application::m_coordinator->addComponent(nodeEntity, transform);

        components::ParentComponent parentComponent;
        parentComponent.parent = parentEntity;
        Application::m_coordinator->addComponent(nodeEntity, parentComponent);

        auto parentTransform = Application::m_coordinator->tryGetComponent<components::TransformComponent>(parentEntity);
        if (parentTransform)
            parentTransform->get().children.push_back(nodeEntity);


        if (!node.name.empty()) {
            components::NameComponent nameComponent;
            nameComponent.name = node.name;
            Application::m_coordinator->addComponent(nodeEntity, nameComponent);
        }

        for (const auto& mesh : node.meshes)
        {
            ecs::Entity meshEntity = Application::m_coordinator->createEntity();
            totalChildrenCreated++;

            components::UuidComponent meshUuid;
            Application::m_coordinator->addComponent(meshEntity, meshUuid);

            components::TransformComponent meshTransform;
            meshTransform.pos = glm::vec3(0.0f);
            meshTransform.size = glm::vec3(1.0f);
            meshTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            // Centroid
            meshTransform.localCenter = mesh.localCenter;

            components::StaticMeshComponent staticMesh;
            staticMesh.vao = mesh.vao;

            Application::m_coordinator->addComponent(meshEntity, meshTransform);
            Application::m_coordinator->addComponent(meshEntity, staticMesh);

            if (!mesh.name.empty()) {
                components::NameComponent nameComponent;
                nameComponent.name = mesh.name;
                Application::m_coordinator->addComponent(meshEntity, nameComponent);
            }

            if (mesh.material)
            {
                components::MaterialComponent materialComponent;
                materialComponent.material = mesh.material;
                Application::m_coordinator->addComponent(meshEntity, materialComponent);
            }

            components::ParentComponent meshParentComponent;
            meshParentComponent.parent = nodeEntity;
            Application::m_coordinator->addComponent(meshEntity, meshParentComponent);

            auto nodeTransform = Application::m_coordinator->tryGetComponent<components::TransformComponent>(nodeEntity);
            if (nodeTransform)
                nodeTransform->get().children.push_back(meshEntity);
        }

        for (const auto& childNode : node.children)
            totalChildrenCreated += processModelNode(nodeEntity, childNode);

        return totalChildrenCreated;
    }
}
