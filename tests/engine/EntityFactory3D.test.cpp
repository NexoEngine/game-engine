//// EntityFactory3D.test.cpp ////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Claude AI
//  Date:        13/12/2025
//  Description: Test file for the EntityFactory3D
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "EntityFactory3D.hpp"
#include "Application.hpp"
#include "ecs/Coordinator.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "components/Render.hpp"
#include "components/StaticMesh.hpp"
#include "components/MaterialComponent.hpp"
#include "components/Render3D.hpp"
#include "assets/AssetCatalog.hpp"

namespace nexo {

    class EntityFactory3DTest : public ::testing::Test {
        protected:
        void SetUp() override {
            // Initialize the coordinator
            Application::m_coordinator = std::make_unique<ecs::Coordinator>();
            Application::m_coordinator->init();

            // Register all components used by EntityFactory3D
            Application::m_coordinator->registerComponent<components::TransformComponent>();
            Application::m_coordinator->registerComponent<components::UuidComponent>();
            Application::m_coordinator->registerComponent<components::RenderComponent>();
            Application::m_coordinator->registerComponent<components::StaticMeshComponent>();
            Application::m_coordinator->registerComponent<components::MaterialComponent>();
        }

        void TearDown() override {
            Application::m_coordinator.reset();
        }

        // Helper function to verify common components
        void verifyBasicComponents(ecs::Entity entity) {
            EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::TransformComponent>(entity));
            EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::UuidComponent>(entity));
            EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::StaticMeshComponent>(entity));
            EXPECT_TRUE(Application::m_coordinator->entityHasComponent<components::MaterialComponent>(entity));
        }

        // Helper function to verify transform values
        void verifyTransform(ecs::Entity entity, const glm::vec3& expectedPos,
                           const glm::vec3& expectedSize, const glm::vec3& expectedRotation) {
            auto& transform = Application::m_coordinator->getComponent<components::TransformComponent>(entity);

            EXPECT_FLOAT_EQ(transform.pos.x, expectedPos.x);
            EXPECT_FLOAT_EQ(transform.pos.y, expectedPos.y);
            EXPECT_FLOAT_EQ(transform.pos.z, expectedPos.z);

            EXPECT_FLOAT_EQ(transform.size.x, expectedSize.x);
            EXPECT_FLOAT_EQ(transform.size.y, expectedSize.y);
            EXPECT_FLOAT_EQ(transform.size.z, expectedSize.z);

            // Convert rotation to quaternion for comparison
            glm::quat expectedQuat = glm::quat(glm::radians(expectedRotation));
            EXPECT_FLOAT_EQ(transform.quat.x, expectedQuat.x);
            EXPECT_FLOAT_EQ(transform.quat.y, expectedQuat.y);
            EXPECT_FLOAT_EQ(transform.quat.z, expectedQuat.z);
            EXPECT_FLOAT_EQ(transform.quat.w, expectedQuat.w);
        }

        // Helper function to verify material color
        void verifyMaterialColor(ecs::Entity entity, const glm::vec4& expectedColor) {
            auto& matComponent = Application::m_coordinator->getComponent<components::MaterialComponent>(entity);
            auto materialAsset = matComponent.material.lock();
            ASSERT_NE(materialAsset, nullptr);

            const auto& materialData = materialAsset->getData();
            ASSERT_NE(materialData, nullptr);

            EXPECT_FLOAT_EQ(materialData->albedoColor.r, expectedColor.r);
            EXPECT_FLOAT_EQ(materialData->albedoColor.g, expectedColor.g);
            EXPECT_FLOAT_EQ(materialData->albedoColor.b, expectedColor.b);
            EXPECT_FLOAT_EQ(materialData->albedoColor.a, expectedColor.a);
        }
    };

    // =============================================================================
    // Cube Creation Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateCube_WithDefaultColor) {
        glm::vec3 pos(1.0f, 2.0f, 3.0f);
        glm::vec3 size(2.0f, 2.0f, 2.0f);
        glm::vec3 rotation(0.0f, 45.0f, 0.0f);

        ecs::Entity cube = EntityFactory3D::createCube(pos, size, rotation);

        EXPECT_NE(cube, ecs::INVALID_ENTITY);
        verifyBasicComponents(cube);
        verifyTransform(cube, pos, size, rotation);

        // Default color should be red (1, 0, 0, 1)
        verifyMaterialColor(cube, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));

        // Verify RenderComponent
        auto& renderComp = Application::m_coordinator->getComponent<components::RenderComponent>(cube);
        EXPECT_TRUE(renderComp.isRendered);
        EXPECT_EQ(renderComp.type, components::PrimitiveType::CUBE);
    }

    TEST_F(EntityFactory3DTest, CreateCube_WithCustomColor) {
        glm::vec3 pos(0.0f, 0.0f, 0.0f);
        glm::vec3 size(1.0f, 1.0f, 1.0f);
        glm::vec3 rotation(0.0f, 0.0f, 0.0f);
        glm::vec4 customColor(0.5f, 0.3f, 0.8f, 0.9f);

        ecs::Entity cube = EntityFactory3D::createCube(pos, size, rotation, customColor);

        EXPECT_NE(cube, ecs::INVALID_ENTITY);
        verifyMaterialColor(cube, customColor);
    }

    TEST_F(EntityFactory3DTest, CreateCube_WithMaterial) {
        glm::vec3 pos(5.0f, 10.0f, 15.0f);
        glm::vec3 size(3.0f, 3.0f, 3.0f);
        glm::vec3 rotation(90.0f, 0.0f, 0.0f);

        components::Material customMaterial;
        customMaterial.albedoColor = glm::vec4(0.2f, 0.4f, 0.6f, 1.0f);
        customMaterial.roughness = 0.5f;
        customMaterial.metallic = 0.8f;

        ecs::Entity cube = EntityFactory3D::createCube(pos, size, rotation, customMaterial);

        EXPECT_NE(cube, ecs::INVALID_ENTITY);
        verifyBasicComponents(cube);
        verifyTransform(cube, pos, size, rotation);
        verifyMaterialColor(cube, customMaterial.albedoColor);
    }

    TEST_F(EntityFactory3DTest, CreateCube_HasStaticMesh) {
        ecs::Entity cube = EntityFactory3D::createCube(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );

        auto& mesh = Application::m_coordinator->getComponent<components::StaticMeshComponent>(cube);
        EXPECT_NE(mesh.vao, nullptr);
    }

    // =============================================================================
    // Tetrahedron Creation Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateTetrahedron_WithDefaultColor) {
        glm::vec3 pos(1.0f, 2.0f, 3.0f);
        glm::vec3 size(1.5f, 1.5f, 1.5f);
        glm::vec3 rotation(0.0f, 0.0f, 0.0f);

        ecs::Entity tetrahedron = EntityFactory3D::createTetrahedron(pos, size, rotation);

        EXPECT_NE(tetrahedron, ecs::INVALID_ENTITY);
        verifyBasicComponents(tetrahedron);
        verifyTransform(tetrahedron, pos, size, rotation);
        verifyMaterialColor(tetrahedron, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    TEST_F(EntityFactory3DTest, CreateTetrahedron_WithCustomColor) {
        glm::vec3 pos(0.0f, 0.0f, 0.0f);
        glm::vec3 size(1.0f, 1.0f, 1.0f);
        glm::vec3 rotation(0.0f, 0.0f, 0.0f);
        glm::vec4 customColor(0.1f, 0.9f, 0.5f, 1.0f);

        ecs::Entity tetrahedron = EntityFactory3D::createTetrahedron(pos, size, rotation, customColor);

        EXPECT_NE(tetrahedron, ecs::INVALID_ENTITY);
        verifyMaterialColor(tetrahedron, customColor);
    }

    TEST_F(EntityFactory3DTest, CreateTetrahedron_WithMaterial) {
        components::Material material;
        material.albedoColor = glm::vec4(0.7f, 0.2f, 0.9f, 1.0f);

        ecs::Entity tetrahedron = EntityFactory3D::createTetrahedron(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), material
        );

        EXPECT_NE(tetrahedron, ecs::INVALID_ENTITY);
        verifyMaterialColor(tetrahedron, material.albedoColor);
    }

    // =============================================================================
    // Pyramid Creation Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreatePyramid_WithDefaultColor) {
        glm::vec3 pos(2.0f, 3.0f, 4.0f);
        glm::vec3 size(2.0f, 3.0f, 2.0f);
        glm::vec3 rotation(0.0f, 90.0f, 0.0f);

        ecs::Entity pyramid = EntityFactory3D::createPyramid(pos, size, rotation);

        EXPECT_NE(pyramid, ecs::INVALID_ENTITY);
        verifyBasicComponents(pyramid);
        verifyTransform(pyramid, pos, size, rotation);
        verifyMaterialColor(pyramid, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    TEST_F(EntityFactory3DTest, CreatePyramid_WithCustomColor) {
        glm::vec4 customColor(0.9f, 0.7f, 0.1f, 1.0f);

        ecs::Entity pyramid = EntityFactory3D::createPyramid(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), customColor
        );

        EXPECT_NE(pyramid, ecs::INVALID_ENTITY);
        verifyMaterialColor(pyramid, customColor);
    }

    TEST_F(EntityFactory3DTest, CreatePyramid_WithMaterial) {
        components::Material material;
        material.albedoColor = glm::vec4(0.3f, 0.6f, 0.9f, 1.0f);

        ecs::Entity pyramid = EntityFactory3D::createPyramid(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), material
        );

        EXPECT_NE(pyramid, ecs::INVALID_ENTITY);
        verifyMaterialColor(pyramid, material.albedoColor);
    }

    // =============================================================================
    // Cylinder Creation Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateCylinder_WithDefaultColorAndSegments) {
        glm::vec3 pos(1.0f, 0.0f, 1.0f);
        glm::vec3 size(1.0f, 2.0f, 1.0f);
        glm::vec3 rotation(0.0f, 0.0f, 0.0f);

        ecs::Entity cylinder = EntityFactory3D::createCylinder(pos, size, rotation);

        EXPECT_NE(cylinder, ecs::INVALID_ENTITY);
        verifyBasicComponents(cylinder);
        verifyTransform(cylinder, pos, size, rotation);
        verifyMaterialColor(cylinder, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    TEST_F(EntityFactory3DTest, CreateCylinder_WithCustomSegments) {
        unsigned int customSegments = 24;

        ecs::Entity cylinder = EntityFactory3D::createCylinder(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f),
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), customSegments
        );

        EXPECT_NE(cylinder, ecs::INVALID_ENTITY);
        verifyBasicComponents(cylinder);
    }

    TEST_F(EntityFactory3DTest, CreateCylinder_WithCustomColor) {
        glm::vec4 customColor(0.4f, 0.8f, 0.2f, 1.0f);

        ecs::Entity cylinder = EntityFactory3D::createCylinder(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), customColor
        );

        EXPECT_NE(cylinder, ecs::INVALID_ENTITY);
        verifyMaterialColor(cylinder, customColor);
    }

    TEST_F(EntityFactory3DTest, CreateCylinder_WithMaterial) {
        components::Material material;
        material.albedoColor = glm::vec4(0.6f, 0.3f, 0.7f, 1.0f);
        unsigned int segments = 16;

        ecs::Entity cylinder = EntityFactory3D::createCylinder(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), material, segments
        );

        EXPECT_NE(cylinder, ecs::INVALID_ENTITY);
        verifyMaterialColor(cylinder, material.albedoColor);
    }

    // =============================================================================
    // Sphere Creation Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateSphere_WithDefaultColorAndSubdivisions) {
        glm::vec3 pos(0.0f, 5.0f, 0.0f);
        glm::vec3 size(1.5f, 1.5f, 1.5f);
        glm::vec3 rotation(0.0f, 0.0f, 0.0f);

        ecs::Entity sphere = EntityFactory3D::createSphere(pos, size, rotation);

        EXPECT_NE(sphere, ecs::INVALID_ENTITY);
        verifyBasicComponents(sphere);
        verifyTransform(sphere, pos, size, rotation);
        verifyMaterialColor(sphere, glm::vec4(1.0f, 0.0f, 0.0f, 1.0f));
    }

    TEST_F(EntityFactory3DTest, CreateSphere_WithCustomSubdivisions) {
        unsigned int customSubdivisions = 4;

        ecs::Entity sphere = EntityFactory3D::createSphere(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f),
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), customSubdivisions
        );

        EXPECT_NE(sphere, ecs::INVALID_ENTITY);
        verifyBasicComponents(sphere);
    }

    TEST_F(EntityFactory3DTest, CreateSphere_WithCustomColor) {
        glm::vec4 customColor(0.8f, 0.1f, 0.4f, 1.0f);

        ecs::Entity sphere = EntityFactory3D::createSphere(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), customColor
        );

        EXPECT_NE(sphere, ecs::INVALID_ENTITY);
        verifyMaterialColor(sphere, customColor);
    }

    TEST_F(EntityFactory3DTest, CreateSphere_WithMaterial) {
        components::Material material;
        material.albedoColor = glm::vec4(0.2f, 0.5f, 0.8f, 1.0f);
        unsigned int subdivisions = 3;

        ecs::Entity sphere = EntityFactory3D::createSphere(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), material, subdivisions
        );

        EXPECT_NE(sphere, ecs::INVALID_ENTITY);
        verifyMaterialColor(sphere, material.albedoColor);
    }

    // =============================================================================
    // UUID Uniqueness Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateMultipleEntities_UUIDsAreUnique) {
        ecs::Entity cube1 = EntityFactory3D::createCube(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );
        ecs::Entity cube2 = EntityFactory3D::createCube(
            glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );
        ecs::Entity sphere = EntityFactory3D::createSphere(
            glm::vec3(2.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );

        auto& uuid1 = Application::m_coordinator->getComponent<components::UuidComponent>(cube1);
        auto& uuid2 = Application::m_coordinator->getComponent<components::UuidComponent>(cube2);
        auto& uuid3 = Application::m_coordinator->getComponent<components::UuidComponent>(sphere);

        EXPECT_NE(uuid1.uuid, uuid2.uuid);
        EXPECT_NE(uuid1.uuid, uuid3.uuid);
        EXPECT_NE(uuid2.uuid, uuid3.uuid);

        // UUIDs should not be empty
        EXPECT_FALSE(uuid1.uuid.empty());
        EXPECT_FALSE(uuid2.uuid.empty());
        EXPECT_FALSE(uuid3.uuid.empty());
    }

    TEST_F(EntityFactory3DTest, CreateEntitiesRapidly_AllHaveUniqueUUIDs) {
        std::vector<ecs::Entity> entities;
        std::set<std::string> uuids;

        // Create 100 entities rapidly
        for (int i = 0; i < 100; ++i) {
            entities.push_back(EntityFactory3D::createCube(
                glm::vec3(static_cast<float>(i)), glm::vec3(1.0f), glm::vec3(0.0f)
            ));
        }

        // Collect all UUIDs
        for (const auto& entity : entities) {
            auto& uuid = Application::m_coordinator->getComponent<components::UuidComponent>(entity);
            uuids.insert(uuid.uuid);
        }

        // All UUIDs should be unique
        EXPECT_EQ(uuids.size(), 100u);
    }

    // =============================================================================
    // Transform Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateEntity_TransformDefaultValues) {
        ecs::Entity cube = EntityFactory3D::createCube(
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(1.0f, 1.0f, 1.0f),
            glm::vec3(0.0f, 0.0f, 0.0f)
        );

        auto& transform = Application::m_coordinator->getComponent<components::TransformComponent>(cube);

        // Verify default matrix values
        EXPECT_EQ(transform.worldMatrix, glm::mat4(1.0f));
        EXPECT_EQ(transform.localMatrix, glm::mat4(1.0f));

        // Verify default local center
        EXPECT_EQ(transform.localCenter, glm::vec3(0.0f));

        // Verify children vector is empty
        EXPECT_TRUE(transform.children.empty());
    }

    TEST_F(EntityFactory3DTest, CreateEntity_TransformWithNegativeValues) {
        glm::vec3 pos(-5.0f, -10.0f, -15.0f);
        glm::vec3 size(0.5f, 0.5f, 0.5f);
        glm::vec3 rotation(-45.0f, -90.0f, -180.0f);

        ecs::Entity cube = EntityFactory3D::createCube(pos, size, rotation);

        EXPECT_NE(cube, ecs::INVALID_ENTITY);
        verifyTransform(cube, pos, size, rotation);
    }

    TEST_F(EntityFactory3DTest, CreateEntity_TransformWithLargeValues) {
        glm::vec3 pos(1000.0f, 2000.0f, 3000.0f);
        glm::vec3 size(100.0f, 200.0f, 300.0f);
        glm::vec3 rotation(360.0f, 720.0f, 1080.0f);

        ecs::Entity sphere = EntityFactory3D::createSphere(pos, size, rotation);

        EXPECT_NE(sphere, ecs::INVALID_ENTITY);
        verifyTransform(sphere, pos, size, rotation);
    }

    TEST_F(EntityFactory3DTest, CreateEntity_TransformWithZeroSize) {
        glm::vec3 pos(1.0f, 2.0f, 3.0f);
        glm::vec3 size(0.0f, 0.0f, 0.0f);
        glm::vec3 rotation(0.0f, 0.0f, 0.0f);

        ecs::Entity pyramid = EntityFactory3D::createPyramid(pos, size, rotation);

        EXPECT_NE(pyramid, ecs::INVALID_ENTITY);
        verifyTransform(pyramid, pos, size, rotation);
    }

    // =============================================================================
    // Material Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateEntity_MaterialColorBoundaries) {
        // Test with color values at boundaries
        glm::vec4 color1(0.0f, 0.0f, 0.0f, 0.0f);
        glm::vec4 color2(1.0f, 1.0f, 1.0f, 1.0f);

        ecs::Entity cube1 = EntityFactory3D::createCube(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), color1
        );
        ecs::Entity cube2 = EntityFactory3D::createCube(
            glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(0.0f), color2
        );

        EXPECT_NE(cube1, ecs::INVALID_ENTITY);
        EXPECT_NE(cube2, ecs::INVALID_ENTITY);

        verifyMaterialColor(cube1, color1);
        verifyMaterialColor(cube2, color2);
    }

    TEST_F(EntityFactory3DTest, CreateEntity_MaterialIsValid) {
        ecs::Entity cube = EntityFactory3D::createCube(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );

        auto& matComponent = Application::m_coordinator->getComponent<components::MaterialComponent>(cube);
        auto materialAsset = matComponent.material.lock();

        ASSERT_NE(materialAsset, nullptr);
        const auto& materialData = materialAsset->getData();
        ASSERT_NE(materialData, nullptr);

        EXPECT_EQ(materialData->shader, "Phong");
        EXPECT_TRUE(materialData->isOpaque);
    }

    // =============================================================================
    // Entity Validity Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateEntity_ReturnsValidEntity) {
        ecs::Entity cube = EntityFactory3D::createCube(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );
        ecs::Entity tetrahedron = EntityFactory3D::createTetrahedron(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );
        ecs::Entity pyramid = EntityFactory3D::createPyramid(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );
        ecs::Entity cylinder = EntityFactory3D::createCylinder(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );
        ecs::Entity sphere = EntityFactory3D::createSphere(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );

        EXPECT_NE(cube, ecs::INVALID_ENTITY);
        EXPECT_NE(tetrahedron, ecs::INVALID_ENTITY);
        EXPECT_NE(pyramid, ecs::INVALID_ENTITY);
        EXPECT_NE(cylinder, ecs::INVALID_ENTITY);
        EXPECT_NE(sphere, ecs::INVALID_ENTITY);
    }

    TEST_F(EntityFactory3DTest, CreateMultipleEntities_AllDistinct) {
        std::vector<ecs::Entity> entities;

        for (int i = 0; i < 50; ++i) {
            entities.push_back(EntityFactory3D::createCube(
                glm::vec3(static_cast<float>(i)), glm::vec3(1.0f), glm::vec3(0.0f)
            ));
        }

        // All entities should be distinct
        std::set<ecs::Entity> uniqueEntities(entities.begin(), entities.end());
        EXPECT_EQ(uniqueEntities.size(), 50u);
    }

    // =============================================================================
    // Component Count Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateCube_HasCorrectNumberOfComponents) {
        ecs::Entity cube = EntityFactory3D::createCube(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );

        auto componentTypes = Application::m_coordinator->getAllComponentTypes(cube);

        // Should have: Transform, UUID, StaticMesh, Material, and RenderComponent
        EXPECT_EQ(componentTypes.size(), 5u);
    }

    TEST_F(EntityFactory3DTest, CreateTetrahedron_HasCorrectNumberOfComponents) {
        ecs::Entity tetrahedron = EntityFactory3D::createTetrahedron(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f)
        );

        auto componentTypes = Application::m_coordinator->getAllComponentTypes(tetrahedron);

        // Should have: Transform, UUID, StaticMesh, and Material (no RenderComponent)
        EXPECT_EQ(componentTypes.size(), 4u);
    }

    // =============================================================================
    // Edge Cases Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateEntity_WithExtremeRotationValues) {
        glm::vec3 rotation(10000.0f, -10000.0f, 5000.0f);

        ecs::Entity cube = EntityFactory3D::createCube(
            glm::vec3(0.0f), glm::vec3(1.0f), rotation
        );

        EXPECT_NE(cube, ecs::INVALID_ENTITY);

        // Verify the quaternion was created (even if rotation is extreme)
        auto& transform = Application::m_coordinator->getComponent<components::TransformComponent>(cube);
        glm::quat expectedQuat = glm::quat(glm::radians(rotation));
        EXPECT_FLOAT_EQ(transform.quat.x, expectedQuat.x);
        EXPECT_FLOAT_EQ(transform.quat.y, expectedQuat.y);
        EXPECT_FLOAT_EQ(transform.quat.z, expectedQuat.z);
        EXPECT_FLOAT_EQ(transform.quat.w, expectedQuat.w);
    }

    TEST_F(EntityFactory3DTest, CreateCylinder_WithMinimumSegments) {
        unsigned int minSegments = 3;

        ecs::Entity cylinder = EntityFactory3D::createCylinder(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f),
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), minSegments
        );

        EXPECT_NE(cylinder, ecs::INVALID_ENTITY);
        verifyBasicComponents(cylinder);
    }

    TEST_F(EntityFactory3DTest, CreateSphere_WithMinimumSubdivisions) {
        unsigned int minSubdivisions = 1;

        ecs::Entity sphere = EntityFactory3D::createSphere(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f),
            glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), minSubdivisions
        );

        EXPECT_NE(sphere, ecs::INVALID_ENTITY);
        verifyBasicComponents(sphere);
    }

    // =============================================================================
    // Material Properties Tests
    // =============================================================================

    TEST_F(EntityFactory3DTest, CreateEntity_MaterialPropertiesAreSetCorrectly) {
        components::Material customMaterial;
        customMaterial.albedoColor = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        customMaterial.roughness = 0.7f;
        customMaterial.metallic = 0.3f;
        customMaterial.opacity = 0.9f;
        customMaterial.isOpaque = false;
        customMaterial.shader = "CustomShader";

        ecs::Entity cube = EntityFactory3D::createCube(
            glm::vec3(0.0f), glm::vec3(1.0f), glm::vec3(0.0f), customMaterial
        );

        auto& matComponent = Application::m_coordinator->getComponent<components::MaterialComponent>(cube);
        auto materialAsset = matComponent.material.lock();
        ASSERT_NE(materialAsset, nullptr);

        const auto& materialData = materialAsset->getData();
        ASSERT_NE(materialData, nullptr);

        EXPECT_FLOAT_EQ(materialData->roughness, 0.7f);
        EXPECT_FLOAT_EQ(materialData->metallic, 0.3f);
        EXPECT_FLOAT_EQ(materialData->opacity, 0.9f);
        EXPECT_FALSE(materialData->isOpaque);
    }

}  // namespace nexo
