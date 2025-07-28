//// EntityFactory3D.cpp //////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
#include "components/Name.hpp"
#include "components/Parent.hpp"
#include "components/Render.hpp"
#include "components/Render3D.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "components/StaticMesh.hpp"
#include "components/MaterialComponent.hpp"
#include "assets/AssetCatalog.hpp"
#include "Application.hpp"
#include "math/Matrix.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <filesystem>

namespace nexo
{
    ecs::Entity EntityFactory3D::createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(glm::radians(rotation));

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
        components::RenderComponent renderComponent;
        renderComponent.isRendered = true;
        renderComponent.type = components::PrimitiveType::CUBE;

        const ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newCube, transform);
        Application::m_coordinator->addComponent(newCube, mesh);
        Application::m_coordinator->addComponent(newCube, matComponent);

        Application::m_coordinator->addComponent(newCube, uuid);
        Application::m_coordinator->addComponent(newCube, renderComponent);

        return newCube;
    }

    ecs::Entity EntityFactory3D::createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                            const components::Material& material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(glm::radians(rotation));

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getCubeVAO();

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CubeMat@_internal"),
            std::make_unique<components::Material>(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        components::UuidComponent uuid;
        components::RenderComponent renderComponent;
        renderComponent.isRendered = true;
        renderComponent.type = components::PrimitiveType::CUBE;

        const ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newCube, transform);
        Application::m_coordinator->addComponent(newCube, mesh);
        Application::m_coordinator->addComponent(newCube, matComponent);
        Application::m_coordinator->addComponent(newCube, uuid);
        Application::m_coordinator->addComponent(newCube, renderComponent);

        return newCube;
    }

    ecs::Entity EntityFactory3D::createBillboard(const glm::vec3& pos, const glm::vec3& size, const glm::vec4& color)
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
        components::RenderComponent renderComponent;
        renderComponent.isRendered = true;
        renderComponent.type = components::PrimitiveType::BILLBOARD;

        const ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newBillboard, transform);
        Application::m_coordinator->addComponent(newBillboard, mesh);
        Application::m_coordinator->addComponent(newBillboard, matComponent);
        Application::m_coordinator->addComponent(newBillboard, uuid);
        Application::m_coordinator->addComponent(newBillboard, renderComponent);

        return newBillboard;
    }

    ecs::Entity EntityFactory3D::createBillboard(const glm::vec3& pos, const glm::vec3& size,
                                                 const components::Material& material)
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
        components::RenderComponent renderComponent;
        renderComponent.isRendered = true;
        renderComponent.type = components::PrimitiveType::BILLBOARD;

        const ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newBillboard, transform);
        Application::m_coordinator->addComponent(newBillboard, mesh);
        Application::m_coordinator->addComponent(newBillboard, matComponent);
        Application::m_coordinator->addComponent(newBillboard, uuid);
        Application::m_coordinator->addComponent(newBillboard, renderComponent);

        return newBillboard;
    }

    ecs::Entity EntityFactory3D::createTetrahedron(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(glm::radians(rotation));

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getTetrahedronVAO();

        auto material = std::make_unique<components::Material>();
        material->albedoColor = color;
        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::TetrahedronMatFlatColor@_internal"),
            std::move(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        const components::UuidComponent uuid;

        const ecs::Entity newTetrahedron = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newTetrahedron, transform);
        Application::m_coordinator->addComponent(newTetrahedron, mesh);
        Application::m_coordinator->addComponent(newTetrahedron, matComponent);

        Application::m_coordinator->addComponent(newTetrahedron, uuid);

        return newTetrahedron;
    }

    ecs::Entity EntityFactory3D::createTetrahedron(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                                   const components::Material& material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(glm::radians(rotation));

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getTetrahedronVAO();

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::TetrahedronMat@_internal"),
            std::make_unique<components::Material>(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        const components::UuidComponent uuid;

        const ecs::Entity newTetrahedron = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newTetrahedron, transform);
        Application::m_coordinator->addComponent(newTetrahedron, mesh);
        Application::m_coordinator->addComponent(newTetrahedron, matComponent);
        Application::m_coordinator->addComponent(newTetrahedron, uuid);

        return newTetrahedron;
    }

    ecs::Entity EntityFactory3D::createPyramid(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(glm::radians(rotation));

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getPyramidVAO();

        auto material = std::make_unique<components::Material>();
        material->albedoColor = color;
        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::PyramidMatFlatColor@_internal"),
            std::move(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        const components::UuidComponent uuid;

        const ecs::Entity newPyramid = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newPyramid, transform);
        Application::m_coordinator->addComponent(newPyramid, mesh);
        Application::m_coordinator->addComponent(newPyramid, matComponent);
        Application::m_coordinator->addComponent(newPyramid, uuid);

        return newPyramid;
    }

    ecs::Entity EntityFactory3D::createPyramid(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                               const components::Material& material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(glm::radians(rotation));

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getPyramidVAO();

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::PyramidMat@_internal"),
            std::make_unique<components::Material>(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        const components::UuidComponent uuid;

        const ecs::Entity newPyramid = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newPyramid, transform);
        Application::m_coordinator->addComponent(newPyramid, mesh);
        Application::m_coordinator->addComponent(newPyramid, matComponent);
        Application::m_coordinator->addComponent(newPyramid, uuid);

        return newPyramid;
    }

    ecs::Entity EntityFactory3D::createCylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                                glm::vec4 color, unsigned int nbSegment)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(glm::radians(rotation));

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getCylinderVAO(nbSegment);

        auto material = std::make_unique<components::Material>();
        material->albedoColor = color;
        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CylinderMatFlatColor@_internal"),
            std::move(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        const components::UuidComponent uuid;

        const ecs::Entity newCylinder = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newCylinder, transform);
        Application::m_coordinator->addComponent(newCylinder, mesh);
        Application::m_coordinator->addComponent(newCylinder, matComponent);
        Application::m_coordinator->addComponent(newCylinder, uuid);

        return newCylinder;
    }

    ecs::Entity EntityFactory3D::createCylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                                const components::Material& material, unsigned int nbSegment)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(glm::radians(rotation));

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getCylinderVAO(nbSegment);

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CylinderMat@_internal"),
            std::make_unique<components::Material>(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        const components::UuidComponent uuid;

        const ecs::Entity newCylinder = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newCylinder, transform);
        Application::m_coordinator->addComponent(newCylinder, mesh);
        Application::m_coordinator->addComponent(newCylinder, matComponent);
        Application::m_coordinator->addComponent(newCylinder, uuid);

        return newCylinder;
    }

    ecs::Entity EntityFactory3D::createSphere(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                              glm::vec4 color, const unsigned int nbSubdivision)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(glm::radians(rotation));

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getSphereVAO(nbSubdivision);

        auto material = std::make_unique<components::Material>();
        material->albedoColor = color;
        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::SphereMatFlatColor@_internal"),
            std::move(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        const components::UuidComponent uuid;

        const ecs::Entity newSphere = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newSphere, transform);
        Application::m_coordinator->addComponent(newSphere, mesh);
        Application::m_coordinator->addComponent(newSphere, matComponent);
        Application::m_coordinator->addComponent(newSphere, uuid);

        return newSphere;
    }

    ecs::Entity EntityFactory3D::createSphere(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                              const components::Material& material, const unsigned int nbSubdivision)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(glm::radians(rotation));

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getSphereVAO(nbSubdivision);

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::SphereMat@_internal"),
            std::make_unique<components::Material>(material));
        components::MaterialComponent matComponent;
        matComponent.material = materialRef;

        const components::UuidComponent uuid;

        const ecs::Entity newSphere = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newSphere, transform);
        Application::m_coordinator->addComponent(newSphere, mesh);
        Application::m_coordinator->addComponent(newSphere, matComponent);
        Application::m_coordinator->addComponent(newSphere, uuid);

        return newSphere;
    }

    ecs::Entity EntityFactory3D::createModel(assets::AssetRef<assets::Model> model, glm::vec3 pos, glm::vec3 size,
                                             glm::vec3 rotation)
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

        const std::string rawPath = modelAsset->getMetadata().location.getName().data();
        std::filesystem::path fsPath{rawPath};
        rootComp.name = fsPath.stem().string();

        Application::m_coordinator->addComponent(rootEntity, rootTransform);
        Application::m_coordinator->addComponent(rootEntity, rootComp);

        // Process model nodes to create entity hierarchy
        const assets::MeshNode& rootNode = *modelAsset->getData();
        int childCount = processModelNode(rootEntity, rootNode);

        // Update child count in root component
        auto &storedRoot = Application::m_coordinator->getComponent<components::RootComponent>(rootEntity);
        storedRoot.childCount = childCount;
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

        glm::vec3 translation;
        glm::vec3 scale;
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

        auto parentTransform = Application::m_coordinator->tryGetComponent<
            components::TransformComponent>(parentEntity);
        if (parentTransform)
            parentTransform->get().children.push_back(nodeEntity);


        if (!node.name.empty())
        {
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

            components::RenderComponent renderComponent;
            renderComponent.isRendered = true;
            renderComponent.type = components::PrimitiveType::MESH;

            Application::m_coordinator->addComponent(meshEntity, meshTransform);
            Application::m_coordinator->addComponent(meshEntity, staticMesh);
            Application::m_coordinator->addComponent(meshEntity, renderComponent);

            if (!mesh.name.empty())
            {
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

            auto nodeTransform = Application::m_coordinator->tryGetComponent<
                components::TransformComponent>(nodeEntity);
            if (nodeTransform)
                nodeTransform->get().children.push_back(meshEntity);
        }

        for (const auto& childNode : node.children)
            totalChildrenCreated += processModelNode(nodeEntity, childNode);

        return totalChildrenCreated;
    }
}
