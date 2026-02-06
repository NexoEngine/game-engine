//// Factories.test.cpp //////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Claude AI
//  Date:        13/12/2025
//  Description: Comprehensive test file for CameraFactory and LightFactory
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <set>
#include "CameraFactory.hpp"
#include "LightFactory.hpp"
#include "Application.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "components/Transform.hpp"
#include "components/Uuid.hpp"
#include "ecs/Coordinator.hpp"

namespace nexo {

// =============================================================================
// Test Fixture
// =============================================================================

class FactoriesTest : public ::testing::Test {
protected:
    void SetUp() override {
        coordinator = std::make_shared<ecs::Coordinator>();
        coordinator->init();

        coordinator->registerComponent<components::CameraComponent>();
        coordinator->registerComponent<components::TransformComponent>();
        coordinator->registerComponent<components::UuidComponent>();
        coordinator->registerComponent<components::AmbientLightComponent>();
        coordinator->registerComponent<components::DirectionalLightComponent>();
        coordinator->registerComponent<components::PointLightComponent>();
        coordinator->registerComponent<components::SpotLightComponent>();

        Application::m_coordinator = coordinator;
    }

    void TearDown() override {
        Application::m_coordinator.reset();
    }

    std::shared_ptr<ecs::Coordinator> coordinator;
};

// =============================================================================
// CameraFactory Tests
// =============================================================================

class CameraFactoryTest : public FactoriesTest {};

TEST_F(CameraFactoryTest, CreatePerspectiveCameraWithDefaultParameters) {
    glm::vec3 position{5.0f, 10.0f, 15.0f};
    unsigned int width = 1920;
    unsigned int height = 1080;

    ecs::Entity camera = CameraFactory::createPerspectiveCamera(position, width, height);

    EXPECT_NE(camera, 0);
}

TEST_F(CameraFactoryTest, CameraHasRequiredComponents) {
    glm::vec3 position{1.0f, 2.0f, 3.0f};
    ecs::Entity camera = CameraFactory::createPerspectiveCamera(position, 800, 600);

    EXPECT_TRUE(coordinator->tryGetComponent<components::CameraComponent>(camera).has_value());
    EXPECT_TRUE(coordinator->tryGetComponent<components::TransformComponent>(camera).has_value());
    EXPECT_TRUE(coordinator->tryGetComponent<components::UuidComponent>(camera).has_value());
}

TEST_F(CameraFactoryTest, TransformComponentHasCorrectPosition) {
    glm::vec3 expectedPosition{7.5f, 12.3f, -4.2f};
    ecs::Entity camera = CameraFactory::createPerspectiveCamera(expectedPosition, 800, 600);

    auto& transform = coordinator->getComponent<components::TransformComponent>(camera);
    EXPECT_EQ(transform.pos, expectedPosition);
}

TEST_F(CameraFactoryTest, CameraComponentHasCorrectDimensions) {
    unsigned int expectedWidth = 1280;
    unsigned int expectedHeight = 720;
    ecs::Entity camera = CameraFactory::createPerspectiveCamera({0, 0, 0}, expectedWidth, expectedHeight);

    auto& cameraComp = coordinator->getComponent<components::CameraComponent>(camera);
    EXPECT_EQ(cameraComp.width, expectedWidth);
    EXPECT_EQ(cameraComp.height, expectedHeight);
}

TEST_F(CameraFactoryTest, CameraComponentHasDefaultFOV) {
    float defaultFov = 45.0f;
    ecs::Entity camera = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600);

    auto& cameraComp = coordinator->getComponent<components::CameraComponent>(camera);
    EXPECT_FLOAT_EQ(cameraComp.fov, defaultFov);
}

TEST_F(CameraFactoryTest, CameraComponentHasCustomFOV) {
    float customFov = 90.0f;
    ecs::Entity camera = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600, nullptr, {0, 0, 0, 1}, customFov);

    auto& cameraComp = coordinator->getComponent<components::CameraComponent>(camera);
    EXPECT_FLOAT_EQ(cameraComp.fov, customFov);
}

TEST_F(CameraFactoryTest, CameraComponentHasDefaultNearPlane) {
    float defaultNear = 1.0f;
    ecs::Entity camera = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600);

    auto& cameraComp = coordinator->getComponent<components::CameraComponent>(camera);
    EXPECT_FLOAT_EQ(cameraComp.nearPlane, defaultNear);
}

TEST_F(CameraFactoryTest, CameraComponentHasCustomNearPlane) {
    float customNear = 0.5f;
    ecs::Entity camera = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600, nullptr, {0, 0, 0, 1}, 45.0f, customNear);

    auto& cameraComp = coordinator->getComponent<components::CameraComponent>(camera);
    EXPECT_FLOAT_EQ(cameraComp.nearPlane, customNear);
}

TEST_F(CameraFactoryTest, CameraComponentHasDefaultFarPlane) {
    float defaultFar = 100.0f;
    ecs::Entity camera = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600);

    auto& cameraComp = coordinator->getComponent<components::CameraComponent>(camera);
    EXPECT_FLOAT_EQ(cameraComp.farPlane, defaultFar);
}

TEST_F(CameraFactoryTest, CameraComponentHasCustomFarPlane) {
    float customFar = 500.0f;
    ecs::Entity camera = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600, nullptr, {0, 0, 0, 1}, 45.0f, 1.0f, customFar);

    auto& cameraComp = coordinator->getComponent<components::CameraComponent>(camera);
    EXPECT_FLOAT_EQ(cameraComp.farPlane, customFar);
}

TEST_F(CameraFactoryTest, CameraComponentIsPerspectiveType) {
    ecs::Entity camera = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600);

    auto& cameraComp = coordinator->getComponent<components::CameraComponent>(camera);
    EXPECT_EQ(cameraComp.type, components::CameraType::PERSPECTIVE);
}

TEST_F(CameraFactoryTest, CameraComponentHasDefaultClearColor) {
    glm::vec4 defaultClearColor{37.0f/255.0f, 35.0f/255.0f, 50.0f/255.0f, 111.0f/255.0f};
    ecs::Entity camera = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600);

    auto& cameraComp = coordinator->getComponent<components::CameraComponent>(camera);
    EXPECT_EQ(cameraComp.clearColor, defaultClearColor);
}

TEST_F(CameraFactoryTest, CameraComponentHasCustomClearColor) {
    glm::vec4 customClearColor{1.0f, 0.0f, 0.0f, 1.0f};
    ecs::Entity camera = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600, nullptr, customClearColor);

    auto& cameraComp = coordinator->getComponent<components::CameraComponent>(camera);
    EXPECT_EQ(cameraComp.clearColor, customClearColor);
}

TEST_F(CameraFactoryTest, MultipleCamerasHaveUniqueUUIDs) {
    ecs::Entity camera1 = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600);
    ecs::Entity camera2 = CameraFactory::createPerspectiveCamera({1, 1, 1}, 1920, 1080);
    ecs::Entity camera3 = CameraFactory::createPerspectiveCamera({2, 2, 2}, 1280, 720);

    auto& uuid1 = coordinator->getComponent<components::UuidComponent>(camera1);
    auto& uuid2 = coordinator->getComponent<components::UuidComponent>(camera2);
    auto& uuid3 = coordinator->getComponent<components::UuidComponent>(camera3);

    EXPECT_NE(uuid1.uuid, uuid2.uuid);
    EXPECT_NE(uuid1.uuid, uuid3.uuid);
    EXPECT_NE(uuid2.uuid, uuid3.uuid);
}

TEST_F(CameraFactoryTest, CreateMultipleCameras) {
    std::vector<ecs::Entity> cameras;
    for (int i = 0; i < 10; i++) {
        cameras.push_back(CameraFactory::createPerspectiveCamera({float(i), 0, 0}, 800, 600));
    }

    for (const auto& camera : cameras) {
        EXPECT_TRUE(coordinator->tryGetComponent<components::CameraComponent>(camera).has_value());
        EXPECT_TRUE(coordinator->tryGetComponent<components::TransformComponent>(camera).has_value());
        EXPECT_TRUE(coordinator->tryGetComponent<components::UuidComponent>(camera).has_value());
    }
}

// =============================================================================
// LightFactory - Ambient Light Tests
// =============================================================================

class AmbientLightFactoryTest : public FactoriesTest {};

TEST_F(AmbientLightFactoryTest, CreateAmbientLightWithColor) {
    glm::vec3 color{1.0f, 1.0f, 1.0f};
    ecs::Entity light = LightFactory::createAmbientLight(color);

    EXPECT_NE(light, 0);
}

TEST_F(AmbientLightFactoryTest, AmbientLightHasRequiredComponents) {
    glm::vec3 color{0.5f, 0.5f, 0.5f};
    ecs::Entity light = LightFactory::createAmbientLight(color);

    EXPECT_TRUE(coordinator->tryGetComponent<components::AmbientLightComponent>(light).has_value());
    EXPECT_TRUE(coordinator->tryGetComponent<components::UuidComponent>(light).has_value());
}

TEST_F(AmbientLightFactoryTest, AmbientLightHasCorrectColor) {
    glm::vec3 expectedColor{0.8f, 0.6f, 0.4f};
    ecs::Entity light = LightFactory::createAmbientLight(expectedColor);

    auto& lightComp = coordinator->getComponent<components::AmbientLightComponent>(light);
    EXPECT_EQ(lightComp.color, expectedColor);
}

TEST_F(AmbientLightFactoryTest, MultipleAmbientLightsHaveUniqueUUIDs) {
    ecs::Entity light1 = LightFactory::createAmbientLight({1.0f, 0.0f, 0.0f});
    ecs::Entity light2 = LightFactory::createAmbientLight({0.0f, 1.0f, 0.0f});
    ecs::Entity light3 = LightFactory::createAmbientLight({0.0f, 0.0f, 1.0f});

    auto& uuid1 = coordinator->getComponent<components::UuidComponent>(light1);
    auto& uuid2 = coordinator->getComponent<components::UuidComponent>(light2);
    auto& uuid3 = coordinator->getComponent<components::UuidComponent>(light3);

    EXPECT_NE(uuid1.uuid, uuid2.uuid);
    EXPECT_NE(uuid1.uuid, uuid3.uuid);
    EXPECT_NE(uuid2.uuid, uuid3.uuid);
}

// =============================================================================
// LightFactory - Directional Light Tests
// =============================================================================

class DirectionalLightFactoryTest : public FactoriesTest {};

TEST_F(DirectionalLightFactoryTest, CreateDirectionalLightWithDefaultColor) {
    glm::vec3 direction{0.0f, -1.0f, 0.0f};
    ecs::Entity light = LightFactory::createDirectionalLight(direction);

    EXPECT_NE(light, 0);
}

TEST_F(DirectionalLightFactoryTest, DirectionalLightHasRequiredComponents) {
    glm::vec3 direction{1.0f, -1.0f, 0.0f};
    ecs::Entity light = LightFactory::createDirectionalLight(direction);

    EXPECT_TRUE(coordinator->tryGetComponent<components::DirectionalLightComponent>(light).has_value());
    EXPECT_TRUE(coordinator->tryGetComponent<components::UuidComponent>(light).has_value());
}

TEST_F(DirectionalLightFactoryTest, DirectionalLightHasCorrectDirection) {
    glm::vec3 expectedDirection{-1.0f, -1.0f, -1.0f};
    ecs::Entity light = LightFactory::createDirectionalLight(expectedDirection);

    auto& lightComp = coordinator->getComponent<components::DirectionalLightComponent>(light);
    EXPECT_EQ(lightComp.direction, expectedDirection);
}

TEST_F(DirectionalLightFactoryTest, DirectionalLightHasDefaultColor) {
    glm::vec3 defaultColor{1.0f, 1.0f, 1.0f};
    glm::vec3 direction{0.0f, -1.0f, 0.0f};
    ecs::Entity light = LightFactory::createDirectionalLight(direction);

    auto& lightComp = coordinator->getComponent<components::DirectionalLightComponent>(light);
    EXPECT_EQ(lightComp.color, defaultColor);
}

TEST_F(DirectionalLightFactoryTest, DirectionalLightHasCustomColor) {
    glm::vec3 customColor{1.0f, 0.5f, 0.0f};
    glm::vec3 direction{0.0f, -1.0f, 0.0f};
    ecs::Entity light = LightFactory::createDirectionalLight(direction, customColor);

    auto& lightComp = coordinator->getComponent<components::DirectionalLightComponent>(light);
    EXPECT_EQ(lightComp.color, customColor);
}

TEST_F(DirectionalLightFactoryTest, MultipleDirectionalLightsHaveUniqueUUIDs) {
    ecs::Entity light1 = LightFactory::createDirectionalLight({0, -1, 0});
    ecs::Entity light2 = LightFactory::createDirectionalLight({1, -1, 0});
    ecs::Entity light3 = LightFactory::createDirectionalLight({-1, -1, 0});

    auto& uuid1 = coordinator->getComponent<components::UuidComponent>(light1);
    auto& uuid2 = coordinator->getComponent<components::UuidComponent>(light2);
    auto& uuid3 = coordinator->getComponent<components::UuidComponent>(light3);

    EXPECT_NE(uuid1.uuid, uuid2.uuid);
    EXPECT_NE(uuid1.uuid, uuid3.uuid);
    EXPECT_NE(uuid2.uuid, uuid3.uuid);
}

// =============================================================================
// LightFactory - Point Light Tests
// =============================================================================

class PointLightFactoryTest : public FactoriesTest {};

TEST_F(PointLightFactoryTest, CreatePointLightWithDefaultParameters) {
    glm::vec3 position{0.0f, 5.0f, 0.0f};
    ecs::Entity light = LightFactory::createPointLight(position);

    EXPECT_NE(light, 0);
}

TEST_F(PointLightFactoryTest, PointLightHasRequiredComponents) {
    glm::vec3 position{1.0f, 2.0f, 3.0f};
    ecs::Entity light = LightFactory::createPointLight(position);

    EXPECT_TRUE(coordinator->tryGetComponent<components::PointLightComponent>(light).has_value());
    EXPECT_TRUE(coordinator->tryGetComponent<components::TransformComponent>(light).has_value());
    EXPECT_TRUE(coordinator->tryGetComponent<components::UuidComponent>(light).has_value());
}

TEST_F(PointLightFactoryTest, PointLightTransformHasCorrectPosition) {
    glm::vec3 expectedPosition{10.0f, 20.0f, 30.0f};
    ecs::Entity light = LightFactory::createPointLight(expectedPosition);

    auto& transform = coordinator->getComponent<components::TransformComponent>(light);
    EXPECT_EQ(transform.pos, expectedPosition);
}

TEST_F(PointLightFactoryTest, PointLightHasDefaultColor) {
    glm::vec3 defaultColor{1.0f, 1.0f, 1.0f};
    ecs::Entity light = LightFactory::createPointLight({0, 0, 0});

    auto& lightComp = coordinator->getComponent<components::PointLightComponent>(light);
    EXPECT_EQ(lightComp.color, defaultColor);
}

TEST_F(PointLightFactoryTest, PointLightHasCustomColor) {
    glm::vec3 customColor{1.0f, 0.0f, 0.5f};
    ecs::Entity light = LightFactory::createPointLight({0, 0, 0}, customColor);

    auto& lightComp = coordinator->getComponent<components::PointLightComponent>(light);
    EXPECT_EQ(lightComp.color, customColor);
}

TEST_F(PointLightFactoryTest, PointLightHasDefaultAttenuation) {
    float defaultLinear = 0.09f;
    float defaultQuadratic = 0.032f;
    ecs::Entity light = LightFactory::createPointLight({0, 0, 0});

    auto& lightComp = coordinator->getComponent<components::PointLightComponent>(light);
    EXPECT_FLOAT_EQ(lightComp.linear, defaultLinear);
    EXPECT_FLOAT_EQ(lightComp.quadratic, defaultQuadratic);
}

TEST_F(PointLightFactoryTest, PointLightHasCustomAttenuation) {
    float customLinear = 0.14f;
    float customQuadratic = 0.07f;
    ecs::Entity light = LightFactory::createPointLight({0, 0, 0}, {1, 1, 1}, customLinear, customQuadratic);

    auto& lightComp = coordinator->getComponent<components::PointLightComponent>(light);
    EXPECT_FLOAT_EQ(lightComp.linear, customLinear);
    EXPECT_FLOAT_EQ(lightComp.quadratic, customQuadratic);
}

TEST_F(PointLightFactoryTest, MultiplePointLightsHaveUniqueUUIDs) {
    ecs::Entity light1 = LightFactory::createPointLight({0, 0, 0});
    ecs::Entity light2 = LightFactory::createPointLight({1, 1, 1});
    ecs::Entity light3 = LightFactory::createPointLight({2, 2, 2});

    auto& uuid1 = coordinator->getComponent<components::UuidComponent>(light1);
    auto& uuid2 = coordinator->getComponent<components::UuidComponent>(light2);
    auto& uuid3 = coordinator->getComponent<components::UuidComponent>(light3);

    EXPECT_NE(uuid1.uuid, uuid2.uuid);
    EXPECT_NE(uuid1.uuid, uuid3.uuid);
    EXPECT_NE(uuid2.uuid, uuid3.uuid);
}

// =============================================================================
// LightFactory - Spot Light Tests
// =============================================================================

class SpotLightFactoryTest : public FactoriesTest {};

TEST_F(SpotLightFactoryTest, CreateSpotLightWithDefaultParameters) {
    glm::vec3 position{0.0f, 5.0f, 0.0f};
    glm::vec3 direction{0.0f, -1.0f, 0.0f};
    ecs::Entity light = LightFactory::createSpotLight(position, direction);

    EXPECT_NE(light, 0);
}

TEST_F(SpotLightFactoryTest, SpotLightHasRequiredComponents) {
    glm::vec3 position{1.0f, 2.0f, 3.0f};
    glm::vec3 direction{0.0f, -1.0f, 0.0f};
    ecs::Entity light = LightFactory::createSpotLight(position, direction);

    EXPECT_TRUE(coordinator->tryGetComponent<components::SpotLightComponent>(light).has_value());
    EXPECT_TRUE(coordinator->tryGetComponent<components::TransformComponent>(light).has_value());
    EXPECT_TRUE(coordinator->tryGetComponent<components::UuidComponent>(light).has_value());
}

TEST_F(SpotLightFactoryTest, SpotLightTransformHasCorrectPosition) {
    glm::vec3 expectedPosition{15.0f, 25.0f, 35.0f};
    glm::vec3 direction{0.0f, -1.0f, 0.0f};
    ecs::Entity light = LightFactory::createSpotLight(expectedPosition, direction);

    auto& transform = coordinator->getComponent<components::TransformComponent>(light);
    EXPECT_EQ(transform.pos, expectedPosition);
}

TEST_F(SpotLightFactoryTest, SpotLightHasCorrectDirection) {
    glm::vec3 position{0.0f, 5.0f, 0.0f};
    glm::vec3 expectedDirection{1.0f, -1.0f, 0.0f};
    ecs::Entity light = LightFactory::createSpotLight(position, expectedDirection);

    auto& lightComp = coordinator->getComponent<components::SpotLightComponent>(light);
    EXPECT_EQ(lightComp.direction, expectedDirection);
}

TEST_F(SpotLightFactoryTest, SpotLightHasDefaultColor) {
    glm::vec3 defaultColor{1.0f, 1.0f, 1.0f};
    ecs::Entity light = LightFactory::createSpotLight({0, 5, 0}, {0, -1, 0});

    auto& lightComp = coordinator->getComponent<components::SpotLightComponent>(light);
    EXPECT_EQ(lightComp.color, defaultColor);
}

TEST_F(SpotLightFactoryTest, SpotLightHasCustomColor) {
    glm::vec3 customColor{0.0f, 1.0f, 1.0f};
    ecs::Entity light = LightFactory::createSpotLight({0, 5, 0}, {0, -1, 0}, customColor);

    auto& lightComp = coordinator->getComponent<components::SpotLightComponent>(light);
    EXPECT_EQ(lightComp.color, customColor);
}

TEST_F(SpotLightFactoryTest, SpotLightHasDefaultAttenuation) {
    float defaultLinear = 0.09f;
    float defaultQuadratic = 0.032f;
    ecs::Entity light = LightFactory::createSpotLight({0, 5, 0}, {0, -1, 0});

    auto& lightComp = coordinator->getComponent<components::SpotLightComponent>(light);
    EXPECT_FLOAT_EQ(lightComp.linear, defaultLinear);
    EXPECT_FLOAT_EQ(lightComp.quadratic, defaultQuadratic);
}

TEST_F(SpotLightFactoryTest, SpotLightHasCustomAttenuation) {
    float customLinear = 0.22f;
    float customQuadratic = 0.20f;
    ecs::Entity light = LightFactory::createSpotLight({0, 5, 0}, {0, -1, 0}, {1, 1, 1}, customLinear, customQuadratic);

    auto& lightComp = coordinator->getComponent<components::SpotLightComponent>(light);
    EXPECT_FLOAT_EQ(lightComp.linear, customLinear);
    EXPECT_FLOAT_EQ(lightComp.quadratic, customQuadratic);
}

TEST_F(SpotLightFactoryTest, SpotLightHasDefaultCutoffAngles) {
    float defaultCutOff = glm::cos(glm::radians(12.5f));
    float defaultOuterCutOff = glm::cos(glm::radians(15.0f));
    ecs::Entity light = LightFactory::createSpotLight({0, 5, 0}, {0, -1, 0});

    auto& lightComp = coordinator->getComponent<components::SpotLightComponent>(light);
    EXPECT_FLOAT_EQ(lightComp.cutOff, defaultCutOff);
    EXPECT_FLOAT_EQ(lightComp.outerCutoff, defaultOuterCutOff);
}

TEST_F(SpotLightFactoryTest, SpotLightHasCustomCutoffAngles) {
    float customCutOff = glm::cos(glm::radians(20.0f));
    float customOuterCutOff = glm::cos(glm::radians(25.0f));
    ecs::Entity light = LightFactory::createSpotLight({0, 5, 0}, {0, -1, 0}, {1, 1, 1}, 0.09f, 0.032f, customCutOff, customOuterCutOff);

    auto& lightComp = coordinator->getComponent<components::SpotLightComponent>(light);
    EXPECT_FLOAT_EQ(lightComp.cutOff, customCutOff);
    EXPECT_FLOAT_EQ(lightComp.outerCutoff, customOuterCutOff);
}

TEST_F(SpotLightFactoryTest, MultipleSpotLightsHaveUniqueUUIDs) {
    ecs::Entity light1 = LightFactory::createSpotLight({0, 5, 0}, {0, -1, 0});
    ecs::Entity light2 = LightFactory::createSpotLight({1, 5, 1}, {0, -1, 0});
    ecs::Entity light3 = LightFactory::createSpotLight({2, 5, 2}, {0, -1, 0});

    auto& uuid1 = coordinator->getComponent<components::UuidComponent>(light1);
    auto& uuid2 = coordinator->getComponent<components::UuidComponent>(light2);
    auto& uuid3 = coordinator->getComponent<components::UuidComponent>(light3);

    EXPECT_NE(uuid1.uuid, uuid2.uuid);
    EXPECT_NE(uuid1.uuid, uuid3.uuid);
    EXPECT_NE(uuid2.uuid, uuid3.uuid);
}

// =============================================================================
// Cross-Factory Tests
// =============================================================================

class CrossFactoryTest : public FactoriesTest {};

TEST_F(CrossFactoryTest, AllEntitiesHaveUniqueUUIDs) {
    std::set<std::string> uuids;

    ecs::Entity camera1 = CameraFactory::createPerspectiveCamera({0, 0, 0}, 800, 600);
    ecs::Entity camera2 = CameraFactory::createPerspectiveCamera({1, 1, 1}, 800, 600);
    ecs::Entity ambientLight = LightFactory::createAmbientLight({1, 1, 1});
    ecs::Entity dirLight = LightFactory::createDirectionalLight({0, -1, 0});
    ecs::Entity pointLight = LightFactory::createPointLight({0, 5, 0});
    ecs::Entity spotLight = LightFactory::createSpotLight({0, 5, 0}, {0, -1, 0});

    uuids.insert(coordinator->getComponent<components::UuidComponent>(camera1).uuid);
    uuids.insert(coordinator->getComponent<components::UuidComponent>(camera2).uuid);
    uuids.insert(coordinator->getComponent<components::UuidComponent>(ambientLight).uuid);
    uuids.insert(coordinator->getComponent<components::UuidComponent>(dirLight).uuid);
    uuids.insert(coordinator->getComponent<components::UuidComponent>(pointLight).uuid);
    uuids.insert(coordinator->getComponent<components::UuidComponent>(spotLight).uuid);

    EXPECT_EQ(uuids.size(), 6);
}

TEST_F(CrossFactoryTest, CreateManyLightsOfDifferentTypes) {
    std::vector<ecs::Entity> lights;

    for (int i = 0; i < 5; i++) {
        lights.push_back(LightFactory::createAmbientLight({float(i)/5.0f, 1.0f, 1.0f}));
        lights.push_back(LightFactory::createDirectionalLight({0, -1, float(i)}));
        lights.push_back(LightFactory::createPointLight({float(i), 5, 0}));
        lights.push_back(LightFactory::createSpotLight({float(i), 5, 0}, {0, -1, 0}));
    }

    EXPECT_EQ(lights.size(), 20);

    for (const auto& light : lights) {
        EXPECT_TRUE(coordinator->tryGetComponent<components::UuidComponent>(light).has_value());
    }
}

TEST_F(CrossFactoryTest, MixedSceneCreation) {
    std::vector<ecs::Entity> entities;

    entities.push_back(CameraFactory::createPerspectiveCamera({0, 10, 20}, 1920, 1080));
    entities.push_back(LightFactory::createAmbientLight({0.2f, 0.2f, 0.2f}));
    entities.push_back(LightFactory::createDirectionalLight({-1, -1, -1}, {1, 1, 0.9f}));

    for (int i = 0; i < 3; i++) {
        entities.push_back(LightFactory::createPointLight({float(i * 5), 3, 0}, {1, 0.8f, 0.6f}));
    }

    entities.push_back(LightFactory::createSpotLight({0, 10, 0}, {0, -1, 0}, {1, 1, 1}));

    EXPECT_EQ(entities.size(), 7);

    std::set<std::string> uuids;
    for (const auto& entity : entities) {
        auto& uuid = coordinator->getComponent<components::UuidComponent>(entity);
        uuids.insert(uuid.uuid);
    }

    EXPECT_EQ(uuids.size(), entities.size());
}

}  // namespace nexo
