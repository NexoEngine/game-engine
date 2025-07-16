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

namespace nexo
{
    /**
     * @brief Creates a cube entity with the specified position, size, rotation, and flat color material.
     *
     * Initializes a cube entity in the ECS with transform, static mesh, material, and UUID components. The cube uses a flat color material defined by the provided color vector.
     *
     * @param pos The position of the cube in world space.
     * @param size The scale of the cube along each axis.
     * @param rotation The Euler angles (in radians) for the cube's orientation.
     * @param color The RGBA color used for the cube's material.
     * @return ecs::Entity The created cube entity.
     */
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

        const components::UuidComponent uuid;

        const ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newCube, transform);
        Application::m_coordinator->addComponent(newCube, mesh);
        Application::m_coordinator->addComponent(newCube, matComponent);

        Application::m_coordinator->addComponent(newCube, uuid);

        return newCube;
    }

    /**
     * @brief Creates a cube entity with the specified transform and material.
     *
     * The cube is initialized with position, size, and rotation, and uses the provided material for its appearance. The entity is assigned transform, static mesh, material, and UUID components.
     *
     * @param pos The position of the cube in world space.
     * @param size The size (scale) of the cube.
     * @param rotation The Euler angles (in radians) for the cube's rotation.
     * @param material The material to apply to the cube.
     * @return The created cube entity.
     */
    ecs::Entity EntityFactory3D::createCube(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                            const components::Material& material)
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

        const components::UuidComponent uuid;

        const ecs::Entity newCube = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newCube, transform);
        Application::m_coordinator->addComponent(newCube, mesh);
        Application::m_coordinator->addComponent(newCube, matComponent);
        Application::m_coordinator->addComponent(newCube, uuid);

        return newCube;
    }

    /**
     * @brief Creates a billboard entity at the specified position and size with a flat color material.
     *
     * The billboard entity is initialized with transform, billboard mesh, material, and UUID components.
     *
     * @param pos The world position of the billboard.
     * @param size The size of the billboard.
     * @param color The flat color to use for the billboard's material.
     * @return ecs::Entity The created billboard entity.
     */
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

        const components::UuidComponent uuid;

        const ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newBillboard, transform);
        Application::m_coordinator->addComponent(newBillboard, mesh);
        Application::m_coordinator->addComponent(newBillboard, matComponent);
        Application::m_coordinator->addComponent(newBillboard, uuid);

        return newBillboard;
    }

    /**
     * @brief Creates a billboard entity with the specified position, size, and material.
     *
     * The billboard entity is initialized with transform, billboard, material, and UUID components.
     *
     * @param pos The world position of the billboard.
     * @param size The size of the billboard.
     * @param material The material to apply to the billboard.
     * @return The created billboard entity.
     */
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

        const components::UuidComponent uuid;

        const ecs::Entity newBillboard = Application::m_coordinator->createEntity();
        Application::m_coordinator->addComponent(newBillboard, transform);
        Application::m_coordinator->addComponent(newBillboard, mesh);
        Application::m_coordinator->addComponent(newBillboard, matComponent);
        Application::m_coordinator->addComponent(newBillboard, uuid);

        return newBillboard;
    }

    /**
     * @brief Creates a tetrahedron entity with the specified position, size, rotation, and flat color material.
     *
     * The entity is initialized with transform, static mesh, material, and UUID components, and uses a flat color for its material.
     *
     * @param pos The position of the tetrahedron in world space.
     * @param size The scale of the tetrahedron along each axis.
     * @param rotation The Euler angles (in radians) for the tetrahedron's orientation.
     * @param color The RGBA color to use for the flat color material.
     * @return ecs::Entity The created tetrahedron entity.
     */
    ecs::Entity EntityFactory3D::createTetrahedron(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getTetrahedronVAO();

        auto material = std::make_unique<components::Material>();
        material->albedoColor = color;
        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CubeMatFlatColor@_internal"),
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

    /**
     * @brief Creates a tetrahedron entity with the specified transform and material.
     *
     * Initializes a new entity with position, size, and rotation, assigns a static tetrahedron mesh, and applies the provided material. The entity is also assigned a unique identifier.
     *
     * @param pos The position of the tetrahedron in world space.
     * @param size The scale of the tetrahedron along each axis.
     * @param rotation The rotation of the tetrahedron in Euler angles (radians).
     * @param material The material to apply to the tetrahedron.
     * @return The created tetrahedron entity.
     */
    ecs::Entity EntityFactory3D::createTetrahedron(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                                   const components::Material& material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getTetrahedronVAO();

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CubeMat@_internal"),
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

    /**
     * @brief Creates a pyramid entity with the specified position, size, rotation, and flat color material.
     *
     * Initializes a 3D pyramid entity with transform, static mesh, material, and UUID components, using a flat color for the material.
     *
     * @param pos The position of the pyramid in world space.
     * @param size The scale of the pyramid along each axis.
     * @param rotation The Euler angles (in radians) for the pyramid's orientation.
     * @param color The RGBA color used for the pyramid's material.
     * @return ecs::Entity The created pyramid entity.
     */
    ecs::Entity EntityFactory3D::createPyramid(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation, glm::vec4 color)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getPyramidVAO();

        auto material = std::make_unique<components::Material>();
        material->albedoColor = color;
        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CubeMatFlatColor@_internal"),
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

    /**
     * @brief Creates a pyramid entity with the specified transform and material.
     *
     * Initializes a 3D pyramid entity at the given position, size, and rotation, using the provided material for rendering. The entity is assigned transform, static mesh, material, and UUID components.
     *
     * @param pos The position of the pyramid in world space.
     * @param size The scale of the pyramid along each axis.
     * @param rotation The rotation of the pyramid in Euler angles (radians).
     * @param material The material to apply to the pyramid's surface.
     * @return ecs::Entity The created pyramid entity.
     */
    ecs::Entity EntityFactory3D::createPyramid(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                               const components::Material& material)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getPyramidVAO();

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CubeMat@_internal"),
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

    /**
     * @brief Creates a cylinder entity with the specified transform, flat color material, and segment count.
     *
     * The cylinder is initialized with position, size, and rotation, uses a static mesh with the given number of segments, and applies a flat color material.
     *
     * @param pos The position of the cylinder in world space.
     * @param size The scale of the cylinder along each axis.
     * @param rotation The Euler angles (in radians) for the cylinder's rotation.
     * @param color The RGBA color to use for the cylinder's material.
     * @param nbSegment The number of segments used to construct the cylinder mesh.
     * @return ecs::Entity The created cylinder entity.
     */
    ecs::Entity EntityFactory3D::createCylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                                glm::vec4 color, unsigned int nbSegment)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getCylinderVAO(nbSegment);

        auto material = std::make_unique<components::Material>();
        material->albedoColor = color;
        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CubeMatFlatColor@_internal"),
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

    /**
     * @brief Creates a cylinder entity with the specified transform, material, and segment count.
     *
     * The cylinder entity is initialized with transform, static mesh, material, and UUID components.
     *
     * @param pos The position of the cylinder in world space.
     * @param size The scale of the cylinder along each axis.
     * @param rotation The Euler angles (in radians) for the cylinder's orientation.
     * @param material The material to apply to the cylinder.
     * @param nbSegment The number of segments used to construct the cylinder mesh.
     * @return ecs::Entity The created cylinder entity.
     */
    ecs::Entity EntityFactory3D::createCylinder(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                                const components::Material& material, unsigned int nbSegment)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getCylinderVAO(nbSegment);

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CubeMat@_internal"),
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

    /**
     * @brief Creates a sphere entity with the specified transform, flat color material, and mesh subdivision level.
     *
     * The sphere entity is initialized with position, size, and rotation, uses a static mesh with the given subdivision count for detail, and applies a flat color material.
     *
     * @param pos The position of the sphere in world space.
     * @param size The scale of the sphere along each axis.
     * @param rotation The Euler angles (in radians) for the sphere's orientation.
     * @param color The RGBA color to use for the sphere's material.
     * @param nbSubdivision The number of subdivisions for the sphere mesh, controlling its smoothness.
     * @return ecs::Entity The created sphere entity.
     */
    ecs::Entity EntityFactory3D::createSphere(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                              glm::vec4 color, const unsigned int nbSubdivision)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getSphereVAO(nbSubdivision);

        auto material = std::make_unique<components::Material>();
        material->albedoColor = color;
        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CubeMatFlatColor@_internal"),
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

    /**
     * @brief Creates a sphere entity with the specified transform, material, and mesh subdivision.
     *
     * The sphere entity is initialized with position, size, and rotation, uses a static mesh with the given subdivision level, and is assigned the provided material.
     *
     * @param pos The position of the sphere in world space.
     * @param size The scale of the sphere along each axis.
     * @param rotation The Euler angles (in radians) for the sphere's orientation.
     * @param material The material to apply to the sphere.
     * @param nbSubdivision The number of subdivisions for the sphere mesh, controlling its detail.
     * @return The created sphere entity.
     */
    ecs::Entity EntityFactory3D::createSphere(glm::vec3 pos, glm::vec3 size, glm::vec3 rotation,
                                              const components::Material& material, const unsigned int nbSubdivision)
    {
        components::TransformComponent transform{};
        transform.pos = pos;
        transform.size = size;
        transform.quat = glm::quat(rotation);

        components::StaticMeshComponent mesh;
        mesh.vao = renderer::NxRenderer3D::getSphereVAO(nbSubdivision);

        const auto materialRef = assets::AssetCatalog::getInstance().createAsset<assets::Material>(
            assets::AssetLocation("_internal::CubeMat@_internal"),
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

    /**
     * @brief Creates a root entity representing a 3D model hierarchy from a model asset.
     *
     * Initializes a root entity with transform and root components, assigns a name based on the model asset, and recursively creates child entities for each node and mesh in the model. Returns the root entity of the constructed hierarchy, or `ecs::INVALID_ENTITY` if the model asset or its data is invalid.
     *
     * @param model Reference to the model asset to instantiate.
     * @param pos World position for the root entity.
     * @param size Scale for the root entity.
     * @param rotation Euler angles (degrees) for the root entity's rotation.
     * @return ecs::Entity The root entity of the model hierarchy, or `ecs::INVALID_ENTITY` on failure.
     */
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
        std::filesystem::path fsPath{ rawPath };
        rootComp.name = fsPath.stem().string();

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
