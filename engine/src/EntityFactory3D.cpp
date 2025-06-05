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

        // Create root transform
        components::TransformComponent rootTransform;
        rootTransform.pos = pos;
        rootTransform.size = size;
        rootTransform.quat = glm::quat(glm::radians(rotation));

        // Create RootComponent to identify this as the root of a hierarchy
        components::RootComponent rootComp;
        rootComp.modelRef = model;

        // Get model name from asset if available
        auto location = modelAsset->getMetadata().location.getName();
        rootComp.name = location.data();
        // Extract just the filename from path if needed
        size_t lastSlash = rootComp.name.find_last_of("/\\");
        if (lastSlash != std::string::npos) {
            rootComp.name = rootComp.name.substr(lastSlash + 1);
        }

        // Add components to root entity
        Application::m_coordinator->addComponent(rootEntity, rootTransform);
        Application::m_coordinator->addComponent(rootEntity, rootComp);

        // Process model nodes to create entity hierarchy
        const assets::MeshNode& rootNode = *modelAsset->getData();
        int childCount = processModelNode(rootEntity, rootNode);

        // Update child count in root component
        rootComp.childCount = childCount;

        // Add UUID to root entity
        components::UuidComponent uuid;
        Application::m_coordinator->addComponent(rootEntity, uuid);

        return rootEntity;
    }

    int EntityFactory3D::processModelNode(ecs::Entity parentEntity, const assets::MeshNode& node)
    {
        int totalChildrenCreated = 0;
        bool shouldCreateIntermediateNode = false;

        // Determine if this node is worth creating as a separate entity
        shouldCreateIntermediateNode =
            // Create a node if it has meshes
            !node.meshes.empty() ||
            // Create a node if it has multiple children (branch node)
            node.children.size() > 1 ||
            // Create a node if it appears to be an important node (e.g. named mesh)
            (!node.meshes.empty() && !node.meshes[0].name.empty());

        // If this is an intermediate node with only one child and no meshes, flatten it
        if (!shouldCreateIntermediateNode && node.children.size() == 1) {
            // Directly process the child node under the parent, combining transforms
            const auto& childNode = node.children[0];

            // Combine the transforms
            glm::mat4 combinedTransform = node.transform * childNode.transform;

            // Create a modified child node with the combined transform
            assets::MeshNode flattenedNode = childNode;
            flattenedNode.transform = combinedTransform;

            // Process this flattened node directly
            return processModelNode(parentEntity, flattenedNode);
        }

        // Standard node processing (create a new entity)
        ecs::Entity nodeEntity = parentEntity;

        // If we need an intermediate node, create it
        if (shouldCreateIntermediateNode && parentEntity != ecs::INVALID_ENTITY) {
            nodeEntity = Application::m_coordinator->createEntity();
            totalChildrenCreated++;

            // Add UUID
            components::UuidComponent uuid;
            Application::m_coordinator->addComponent(nodeEntity, uuid);

            // Extract transform from node matrix
            glm::vec3 translation, scale;
            glm::quat rotation;
            nexo::math::decomposeTransformQuat(node.transform, translation, rotation, scale);

            // Create transform component
            components::TransformComponent transform;
            transform.pos = translation;
            transform.size = scale;
            transform.quat = rotation;
            Application::m_coordinator->addComponent(nodeEntity, transform);

            // Set up parent-child relationship
            components::ParentComponent parentComponent;
            parentComponent.parent = parentEntity;
            Application::m_coordinator->addComponent(nodeEntity, parentComponent);

            // Add this node as a child in the parent's transform
            auto parentTransform = Application::m_coordinator->tryGetComponent<components::TransformComponent>(parentEntity);
            if (parentTransform) {
                parentTransform->get().children.push_back(nodeEntity);
            }

            // If node has a name, store it in the StaticMeshComponent or create a special component
            if (!node.meshes.empty() && !node.meshes[0].name.empty()) {
                // We could add a NameComponent here if needed
            }
        }

        // Process meshes at this node level
        for (const auto& mesh : node.meshes)
        {
            ecs::Entity meshEntity = Application::m_coordinator->createEntity();
            totalChildrenCreated++;

            // Add UUID
            components::UuidComponent uuid;
            Application::m_coordinator->addComponent(meshEntity, uuid);

            // Create transform with centroid information
            components::TransformComponent transform;
            transform.pos = glm::vec3(0.0f);
            transform.size = glm::vec3(1.0f);
            transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
            transform.localCenter = mesh.localCenter;

            // Add mesh component with name
            components::StaticMeshComponent staticMesh;
            staticMesh.vao = mesh.vao;
            staticMesh.name = mesh.name;

            // Add components to entity
            Application::m_coordinator->addComponent(meshEntity, transform);
            Application::m_coordinator->addComponent(meshEntity, staticMesh);

            // Add material if available
            if (mesh.material)
            {
                components::MaterialComponent materialComponent;
                materialComponent.material = mesh.material;
                Application::m_coordinator->addComponent(meshEntity, materialComponent);
            }

            // Set up parent-child relationship
            components::ParentComponent parentComponent;
            parentComponent.parent = nodeEntity;
            Application::m_coordinator->addComponent(meshEntity, parentComponent);

            // Add this child to parent's transform children list
            auto parentTransform = Application::m_coordinator->tryGetComponent<components::TransformComponent>(nodeEntity);
            if (parentTransform) {
                parentTransform->get().children.push_back(meshEntity);
            }
        }

        // Process child nodes recursively
        for (const auto& childNode : node.children)
        {
            // Process this child node recursively and add to total children count
            totalChildrenCreated += processModelNode(nodeEntity, childNode);
        }

        return totalChildrenCreated;
    }
}
