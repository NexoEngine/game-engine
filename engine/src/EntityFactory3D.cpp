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

    ecs::Entity EntityFactory3D::createModel(assets::AssetRef<assets::Model> model, glm::vec3 pos, glm::vec3 size, glm::vec3 rotation)
    {
        auto modelAsset = model.lock();
        if (!modelAsset || !modelAsset->getData())
            return ecs::MAX_ENTITIES;

        ecs::Entity rootEntity = Application::m_coordinator->createEntity();

        components::TransformComponent rootTransform;
        rootTransform.pos = pos;
        rootTransform.size = size;
        rootTransform.quat = glm::quat(glm::radians(rotation));
        Application::m_coordinator->addComponent(rootEntity, rootTransform);

        const assets::MeshNode& rootNode = *modelAsset->getData();
        std::vector<components::SubMeshIndex> children = processModelNode(rootEntity, rootNode);

        components::ModelComponent modelComponent;
        modelComponent.model = model;
        modelComponent.children = children;
        Application::m_coordinator->addComponent(rootEntity, modelComponent);

        components::UuidComponent uuid;
        Application::m_coordinator->addComponent(rootEntity, uuid);

        return rootEntity;
    }

    std::vector<components::SubMeshIndex> EntityFactory3D::processModelNode(ecs::Entity parentEntity, const assets::MeshNode& node)
    {
        std::vector<components::SubMeshIndex> nodeIndices;

        for (const auto& mesh : node.meshes)
        {
            ecs::Entity meshEntity = Application::m_coordinator->createEntity();

            components::UuidComponent uuid;
            Application::m_coordinator->addComponent(meshEntity, uuid);

            // Add local transform component
            components::LocalTransformComponent localTransform;
            localTransform.position = glm::vec3(0.0f); // Local position is zero initially
            localTransform.scale = glm::vec3(1.0f);    // Local scale is identity initially
            localTransform.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f); // Identity quaternion
            Application::m_coordinator->addComponent(meshEntity, localTransform);

            // Add world transform component (will be updated by TransformHierarchySystem)
            components::TransformComponent worldTransform;
            worldTransform.pos = glm::vec3(0.0f);
            worldTransform.size = glm::vec3(1.0f);
            worldTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            Application::m_coordinator->addComponent(meshEntity, worldTransform);

            // Add mesh component with vertex array
            components::StaticMeshComponent staticMesh;
            staticMesh.vao = mesh.vao;
            staticMesh.name = mesh.name;
            Application::m_coordinator->addComponent(meshEntity, staticMesh);

            // Add material component if available
            if (mesh.material)
            {
                components::MaterialComponent materialComponent;
                materialComponent.material = mesh.material;
                Application::m_coordinator->addComponent(meshEntity, materialComponent);
            }

            // Add parent component to establish hierarchy
            components::ParentComponent parentComponent;
            parentComponent.parent = parentEntity;
            Application::m_coordinator->addComponent(meshEntity, parentComponent);

            // Add this mesh entity to the indices for this node
            components::SubMeshIndex meshIndex;
            meshIndex.child = meshEntity;
            meshIndex.children = {};
            nodeIndices.push_back(meshIndex);
        }

        // Process each child node recursively
        for (const auto& childNode : node.children)
        {
            // Create entity for this child node
            ecs::Entity nodeEntity = Application::m_coordinator->createEntity();

            components::UuidComponent uuid;
            Application::m_coordinator->addComponent(nodeEntity, uuid);

            // Extract translation, rotation, scale from the node's transformation matrix
            glm::vec3 translation, scale;
            glm::quat rotation;
            nexo::math::decomposeTransformQuat(childNode.transform, translation, rotation, scale);

            // Add local transform component with the node's local transform
            components::LocalTransformComponent localTransform;
            localTransform.position = translation;
            localTransform.scale = scale;
            localTransform.rotation = rotation;
            Application::m_coordinator->addComponent(nodeEntity, localTransform);

            // Add world transform component (will be updated by TransformHierarchySystem)
            components::TransformComponent worldTransform;
            worldTransform.pos = glm::vec3(0.0f);
            worldTransform.size = glm::vec3(1.0f);
            worldTransform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            Application::m_coordinator->addComponent(nodeEntity, worldTransform);

            // Add parent component
            components::ParentComponent parentComponent;
            parentComponent.parent = parentEntity;
            Application::m_coordinator->addComponent(nodeEntity, parentComponent);

            // Process this child node recursively and get its children
            components::SubMeshIndex childIndex;
            childIndex.child = nodeEntity;
            childIndex.children = processModelNode(nodeEntity, childNode);

            // Add this node and its children to the indices
            nodeIndices.push_back(childIndex);
        }

        return nodeIndices;
    }
}
