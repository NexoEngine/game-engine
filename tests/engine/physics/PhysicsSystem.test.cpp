//// PhysicsSystem.test.cpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz     zzz  zzzz               zzzz  zzzz      zzzz        zzzz
//  zzz   zzz   zzz  zzzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz  zzz  zzzz               zzzz  zzzz      zzzz        zzzz
//  zzz     zz  zzz  zzzzzzzzzzzzz    zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        27/05/2025
//  Description: Source file for physics tests
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include "ecs/Coordinator.hpp"
#include "components/Transform.hpp"
#include "components/PhysicsBodyComponent.hpp"
#include "systems/PhysicsSystem.hpp"

using namespace nexo;

class PhysicsSystemTest : public ::testing::Test {
    protected:
        std::shared_ptr<ecs::Coordinator> coordinator;
        std::shared_ptr<system::PhysicsSystem> physicsSystem;

    void SetUp() override {
        coordinator = std::make_shared<ecs::Coordinator>();
        ecs::System::coord = coordinator;
        coordinator->init();
        coordinator->registerComponent<components::TransformComponent>();
        coordinator->registerComponent<components::PhysicsBodyComponent>();
        physicsSystem = std::make_shared<system::PhysicsSystem>();
        physicsSystem->init();
    }
};

TEST_F(PhysicsSystemTest, CreateDynamicBody) {
    ecs::Entity entity = coordinator->createEntity();
    components::TransformComponent transform{};
    transform.pos = {0.0f, 5.0f, 0.0f};
    transform.quat = {1.0f, 0.0f, 0.0f, 0.0f};
    transform.size = {1.0f, 1.0f, 1.0f};
    coordinator->addComponent(entity, transform);

    auto bodyID = physicsSystem->createDynamicBody(entity, transform);
    auto& bodyComp = coordinator->getComponent<components::PhysicsBodyComponent>(entity);

    EXPECT_EQ(bodyID, bodyComp.bodyID);
    EXPECT_EQ(bodyComp.type, components::PhysicsBodyComponent::Type::Dynamic);
}

TEST_F(PhysicsSystemTest, CreateStaticBody) {
    ecs::Entity entity = coordinator->createEntity();
    components::TransformComponent transform{};
    transform.pos = {0.0f, 0.25f, 0.0f};
    transform.quat = {1.0f, 0.0f, 0.0f, 0.0f};
    transform.size = {20.0f, 0.5f, 20.0f};
    coordinator->addComponent(entity, transform);

    auto bodyID = physicsSystem->createStaticBody(entity, transform);
    auto& bodyComp = coordinator->getComponent<components::PhysicsBodyComponent>(entity);

    EXPECT_EQ(bodyID, bodyComp.bodyID);
    EXPECT_EQ(bodyComp.type, components::PhysicsBodyComponent::Type::Static);
}

TEST_F(PhysicsSystemTest, PhysicsUpdatesTransformPosition) {
    ecs::Entity entity = coordinator->createEntity();
    components::TransformComponent transform{};
    transform.pos = {0.0f, 5.0f, 0.0f};
    transform.quat = {1.0f, 0.0f, 0.0f, 0.0f};
    transform.size = {1.0f, 1.0f, 1.0f};
    coordinator->addComponent(entity, transform);

    physicsSystem->createDynamicBody(entity, transform);
    physicsSystem->update(); // simulate one frame

    auto& updated = coordinator->getComponent<components::TransformComponent>(entity);
    EXPECT_NEAR(updated.pos.y, transform.pos.y, 1.0f); // should be falling slightly
}
