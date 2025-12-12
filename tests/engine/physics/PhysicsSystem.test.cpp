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

// ============================================================================
// BPLayerInterfaceImpl Tests
// ============================================================================

class BPLayerInterfaceTest : public ::testing::Test {
protected:
    system::BPLayerInterfaceImpl bpLayerInterface;
};

TEST_F(BPLayerInterfaceTest, GetNumBroadPhaseLayers_ReturnsCorrectCount) {
    EXPECT_EQ(bpLayerInterface.GetNumBroadPhaseLayers(), system::BroadPhaseLayers::NUM_LAYERS);
    EXPECT_EQ(bpLayerInterface.GetNumBroadPhaseLayers(), 2);
}

TEST_F(BPLayerInterfaceTest, GetBroadPhaseLayer_NonMovingLayer) {
    JPH::BroadPhaseLayer layer = bpLayerInterface.GetBroadPhaseLayer(system::Layers::NON_MOVING);
    EXPECT_EQ(layer, system::BroadPhaseLayers::NON_MOVING);
}

TEST_F(BPLayerInterfaceTest, GetBroadPhaseLayer_MovingLayer) {
    JPH::BroadPhaseLayer layer = bpLayerInterface.GetBroadPhaseLayer(system::Layers::MOVING);
    EXPECT_EQ(layer, system::BroadPhaseLayers::MOVING);
}

// ============================================================================
// ObjectVsBroadPhaseLayerFilterImpl Tests
// ============================================================================

class ObjectVsBroadPhaseLayerFilterTest : public ::testing::Test {
protected:
    system::ObjectVsBroadPhaseLayerFilterImpl filter;
};

TEST_F(ObjectVsBroadPhaseLayerFilterTest, ShouldCollide_NonMovingVsNonMoving_ReturnsFalse) {
    // NON_MOVING objects should not collide with NON_MOVING broad phase layer
    bool result = filter.ShouldCollide(system::Layers::NON_MOVING, system::BroadPhaseLayers::NON_MOVING);
    EXPECT_FALSE(result);
}

TEST_F(ObjectVsBroadPhaseLayerFilterTest, ShouldCollide_NonMovingVsMoving_ReturnsTrue) {
    // NON_MOVING objects should collide with MOVING broad phase layer
    bool result = filter.ShouldCollide(system::Layers::NON_MOVING, system::BroadPhaseLayers::MOVING);
    EXPECT_TRUE(result);
}

TEST_F(ObjectVsBroadPhaseLayerFilterTest, ShouldCollide_MovingVsNonMoving_ReturnsTrue) {
    // MOVING objects should collide with NON_MOVING broad phase layer
    bool result = filter.ShouldCollide(system::Layers::MOVING, system::BroadPhaseLayers::NON_MOVING);
    EXPECT_TRUE(result);
}

TEST_F(ObjectVsBroadPhaseLayerFilterTest, ShouldCollide_MovingVsMoving_ReturnsTrue) {
    // MOVING objects should collide with MOVING broad phase layer
    bool result = filter.ShouldCollide(system::Layers::MOVING, system::BroadPhaseLayers::MOVING);
    EXPECT_TRUE(result);
}

// ============================================================================
// ObjectLayerPairFilterImpl Tests
// ============================================================================

class ObjectLayerPairFilterTest : public ::testing::Test {
protected:
    system::ObjectLayerPairFilterImpl filter;
};

TEST_F(ObjectLayerPairFilterTest, ShouldCollide_NonMovingVsNonMoving_ReturnsFalse) {
    // NON_MOVING objects should not collide with other NON_MOVING objects
    bool result = filter.ShouldCollide(system::Layers::NON_MOVING, system::Layers::NON_MOVING);
    EXPECT_FALSE(result);
}

TEST_F(ObjectLayerPairFilterTest, ShouldCollide_NonMovingVsMoving_ReturnsTrue) {
    // NON_MOVING objects should collide with MOVING objects
    bool result = filter.ShouldCollide(system::Layers::NON_MOVING, system::Layers::MOVING);
    EXPECT_TRUE(result);
}

TEST_F(ObjectLayerPairFilterTest, ShouldCollide_MovingVsNonMoving_ReturnsTrue) {
    // MOVING objects should collide with NON_MOVING objects
    bool result = filter.ShouldCollide(system::Layers::MOVING, system::Layers::NON_MOVING);
    EXPECT_TRUE(result);
}

TEST_F(ObjectLayerPairFilterTest, ShouldCollide_MovingVsMoving_ReturnsTrue) {
    // MOVING objects should collide with other MOVING objects
    bool result = filter.ShouldCollide(system::Layers::MOVING, system::Layers::MOVING);
    EXPECT_TRUE(result);
}

// ============================================================================
// Integration Tests - Collision Logic Consistency
// ============================================================================

class PhysicsFilterIntegrationTest : public ::testing::Test {
protected:
    system::BPLayerInterfaceImpl bpLayerInterface;
    system::ObjectVsBroadPhaseLayerFilterImpl objectVsBroadPhaseFilter;
    system::ObjectLayerPairFilterImpl objectPairFilter;
};

TEST_F(PhysicsFilterIntegrationTest, CollisionLogic_Consistency) {
    // Verify that the collision logic is consistent across all filter implementations
    // For each object layer combination, check that the filters agree

    // NON_MOVING vs NON_MOVING
    JPH::BroadPhaseLayer bp_non_moving = bpLayerInterface.GetBroadPhaseLayer(system::Layers::NON_MOVING);
    bool objVsBp_nonMovingVsNonMoving = objectVsBroadPhaseFilter.ShouldCollide(system::Layers::NON_MOVING, bp_non_moving);
    bool objPair_nonMovingVsNonMoving = objectPairFilter.ShouldCollide(system::Layers::NON_MOVING, system::Layers::NON_MOVING);
    EXPECT_EQ(objVsBp_nonMovingVsNonMoving, objPair_nonMovingVsNonMoving);
    EXPECT_FALSE(objPair_nonMovingVsNonMoving) << "NON_MOVING should not collide with NON_MOVING";

    // NON_MOVING vs MOVING
    JPH::BroadPhaseLayer bp_moving = bpLayerInterface.GetBroadPhaseLayer(system::Layers::MOVING);
    bool objVsBp_nonMovingVsMoving = objectVsBroadPhaseFilter.ShouldCollide(system::Layers::NON_MOVING, bp_moving);
    bool objPair_nonMovingVsMoving = objectPairFilter.ShouldCollide(system::Layers::NON_MOVING, system::Layers::MOVING);
    EXPECT_EQ(objVsBp_nonMovingVsMoving, objPair_nonMovingVsMoving);
    EXPECT_TRUE(objPair_nonMovingVsMoving) << "NON_MOVING should collide with MOVING";

    // MOVING vs NON_MOVING
    bool objVsBp_movingVsNonMoving = objectVsBroadPhaseFilter.ShouldCollide(system::Layers::MOVING, bp_non_moving);
    bool objPair_movingVsNonMoving = objectPairFilter.ShouldCollide(system::Layers::MOVING, system::Layers::NON_MOVING);
    EXPECT_EQ(objVsBp_movingVsNonMoving, objPair_movingVsNonMoving);
    EXPECT_TRUE(objPair_movingVsNonMoving) << "MOVING should collide with NON_MOVING";

    // MOVING vs MOVING
    bool objVsBp_movingVsMoving = objectVsBroadPhaseFilter.ShouldCollide(system::Layers::MOVING, bp_moving);
    bool objPair_movingVsMoving = objectPairFilter.ShouldCollide(system::Layers::MOVING, system::Layers::MOVING);
    EXPECT_EQ(objVsBp_movingVsMoving, objPair_movingVsMoving);
    EXPECT_TRUE(objPair_movingVsMoving) << "MOVING should collide with MOVING";
}

TEST_F(PhysicsFilterIntegrationTest, BroadPhaseLayerMapping_IsCorrect) {
    // Verify that object layers map to the correct broad phase layers
    EXPECT_EQ(bpLayerInterface.GetBroadPhaseLayer(system::Layers::NON_MOVING),
              system::BroadPhaseLayers::NON_MOVING);
    EXPECT_EQ(bpLayerInterface.GetBroadPhaseLayer(system::Layers::MOVING),
              system::BroadPhaseLayers::MOVING);
}
