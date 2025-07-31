///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Jean CARDONNE
//  Date: Today lol
//  Description: Test file for the TransformMatrixSystem
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "systems/TransformMatrixSystem.hpp"
#include "utils/TestHelpers.hpp"
#include "components/Transform.hpp"
#include "components/SceneComponents.hpp"
#include "components/RenderContext.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace nexo::system {

using namespace nexo::test;

class TransformMatrixSystemTest : public SystemTestFixture<TransformMatrixSystem> {
protected:
    void setupSystemRequirements() override {
        // Set up system signature
        ecs::ComponentSignature signature;
        signature.set(coordinator->getComponentType<components::TransformComponent>());
        signature.set(coordinator->getComponentType<components::SceneTag>());
        coordinator->setSystemSignature<TransformMatrixSystem>(signature);
    }
    
    // Helper to verify matrix composition
    void verifyTransformMatrix(const glm::mat4& matrix, 
                               const glm::vec3& expectedPos,
                               const glm::quat& expectedRot,
                               const glm::vec3& expectedScale) {
        glm::vec3 scale;
        glm::quat rotation;
        glm::vec3 translation;
        glm::vec3 skew;
        glm::vec4 perspective;
        
        glm::decompose(matrix, scale, rotation, translation, skew, perspective);
        
        // Allow small epsilon for floating point comparison
        const float epsilon = 0.0001f;
        
        EXPECT_TRUE(glm::all(glm::epsilonEqual(translation, expectedPos, epsilon)));
        EXPECT_TRUE(glm::all(glm::epsilonEqual(scale, expectedScale, epsilon)));
        
        // Compare quaternions (accounting for equivalent rotations)
        float dotProduct = glm::dot(rotation, expectedRot);
        EXPECT_NEAR(std::abs(dotProduct), 1.0f, epsilon);
    }
};

TEST_F(TransformMatrixSystemTest, NoSceneRendered) {
    // Set render context with no scene
    setRenderContext(-1);
    
    // Create entities with transforms
    auto entity1 = coordinator->createEntity();
    coordinator->addComponent(entity1, components::TransformComponent{});
    coordinator->addComponent(entity1, components::SceneTag{0});
    
    auto entity2 = coordinator->createEntity();
    coordinator->addComponent(entity2, components::TransformComponent{});
    coordinator->addComponent(entity2, components::SceneTag{1});
    
    // Run system - should not process any entities
    system->update();
    
    // Matrices should remain identity
    auto& transform1 = coordinator->getComponent<components::TransformComponent>(entity1);
    auto& transform2 = coordinator->getComponent<components::TransformComponent>(entity2);
    
    EXPECT_EQ(transform1.localMatrix, glm::mat4(1.0f));
    EXPECT_EQ(transform2.localMatrix, glm::mat4(1.0f));
}

TEST_F(TransformMatrixSystemTest, BasicTransformMatrix) {
    // Set render context to scene 0
    setRenderContext(0);
    
    // Create entity with transform
    auto entity = coordinator->createEntity();
    components::TransformComponent transform;
    transform.pos = glm::vec3(10.0f, 20.0f, 30.0f);
    transform.quat = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    transform.size = glm::vec3(2.0f, 3.0f, 4.0f);
    
    coordinator->addComponent(entity, transform);
    coordinator->addComponent(entity, components::SceneTag{0});
    
    // Run system
    system->update();
    
    // Verify matrices were updated
    auto& updatedTransform = coordinator->getComponent<components::TransformComponent>(entity);
    
    EXPECT_NE(updatedTransform.localMatrix, glm::mat4(1.0f));
    EXPECT_EQ(updatedTransform.worldMatrix, updatedTransform.localMatrix);
    
    // Verify matrix composition
    verifyTransformMatrix(updatedTransform.localMatrix, 
                         transform.pos, transform.quat, transform.size);
}

TEST_F(TransformMatrixSystemTest, MultipleEntitiesInScene) {
    // Set render context to scene 1
    setRenderContext(1);
    
    // Create entities in different scenes
    std::vector<ecs::Entity> scene0Entities;
    std::vector<ecs::Entity> scene1Entities;
    
    // Scene 0 entities (should not be processed)
    for (int i = 0; i < 3; ++i) {
        auto entity = coordinator->createEntity();
        components::TransformComponent transform;
        transform.pos = glm::vec3(i, i, i);
        coordinator->addComponent(entity, transform);
        coordinator->addComponent(entity, components::SceneTag{0});
        scene0Entities.push_back(entity);
    }
    
    // Scene 1 entities (should be processed)
    for (int i = 0; i < 5; ++i) {
        auto entity = coordinator->createEntity();
        components::TransformComponent transform;
        transform.pos = glm::vec3(i * 10, i * 20, i * 30);
        transform.quat = glm::angleAxis(glm::radians(float(i * 30)), glm::vec3(0, 1, 0));
        transform.size = glm::vec3(1.0f + i * 0.5f);
        coordinator->addComponent(entity, transform);
        coordinator->addComponent(entity, components::SceneTag{1});
        scene1Entities.push_back(entity);
    }
    
    // Run system
    system->update();
    
    // Verify scene 0 entities were not processed
    for (auto entity : scene0Entities) {
        auto& transform = coordinator->getComponent<components::TransformComponent>(entity);
        EXPECT_EQ(transform.localMatrix, glm::mat4(1.0f));
        EXPECT_EQ(transform.worldMatrix, glm::mat4(1.0f));
    }
    
    // Verify scene 1 entities were processed
    for (size_t i = 0; i < scene1Entities.size(); ++i) {
        auto& transform = coordinator->getComponent<components::TransformComponent>(scene1Entities[i]);
        EXPECT_NE(transform.localMatrix, glm::mat4(1.0f));
        EXPECT_EQ(transform.worldMatrix, transform.localMatrix);
        
        verifyTransformMatrix(transform.localMatrix,
                             glm::vec3(i * 10, i * 20, i * 30),
                             glm::angleAxis(glm::radians(float(i * 30)), glm::vec3(0, 1, 0)),
                             glm::vec3(1.0f + i * 0.5f));
    }
}

TEST_F(TransformMatrixSystemTest, IdentityTransform) {
    setRenderContext(0);
    
    // Create entity with identity transform
    auto entity = coordinator->createEntity();
    components::TransformComponent transform;
    // Default values: pos(0,0,0), quat(1,0,0,0), size(1,1,1)
    
    coordinator->addComponent(entity, transform);
    coordinator->addComponent(entity, components::SceneTag{0});
    
    // Run system
    system->update();
    
    // Should produce identity matrix
    auto& updatedTransform = coordinator->getComponent<components::TransformComponent>(entity);
    EXPECT_EQ(updatedTransform.localMatrix, glm::mat4(1.0f));
    EXPECT_EQ(updatedTransform.worldMatrix, glm::mat4(1.0f));
}

TEST_F(TransformMatrixSystemTest, TranslationOnly) {
    setRenderContext(0);
    
    auto entity = coordinator->createEntity();
    components::TransformComponent transform;
    transform.pos = glm::vec3(100.0f, 200.0f, 300.0f);
    // Default rotation and scale
    
    coordinator->addComponent(entity, transform);
    coordinator->addComponent(entity, components::SceneTag{0});
    
    system->update();
    
    auto& updatedTransform = coordinator->getComponent<components::TransformComponent>(entity);
    
    // Check translation component
    EXPECT_EQ(updatedTransform.localMatrix[3][0], 100.0f);
    EXPECT_EQ(updatedTransform.localMatrix[3][1], 200.0f);
    EXPECT_EQ(updatedTransform.localMatrix[3][2], 300.0f);
    
    verifyTransformMatrix(updatedTransform.localMatrix,
                         glm::vec3(100.0f, 200.0f, 300.0f),
                         glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                         glm::vec3(1.0f));
}

TEST_F(TransformMatrixSystemTest, RotationOnly) {
    setRenderContext(0);
    
    auto entity = coordinator->createEntity();
    components::TransformComponent transform;
    transform.quat = glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    // Default position and scale
    
    coordinator->addComponent(entity, transform);
    coordinator->addComponent(entity, components::SceneTag{0});
    
    system->update();
    
    auto& updatedTransform = coordinator->getComponent<components::TransformComponent>(entity);
    
    verifyTransformMatrix(updatedTransform.localMatrix,
                         glm::vec3(0.0f),
                         glm::angleAxis(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f)),
                         glm::vec3(1.0f));
}

TEST_F(TransformMatrixSystemTest, ScaleOnly) {
    setRenderContext(0);
    
    auto entity = coordinator->createEntity();
    components::TransformComponent transform;
    transform.size = glm::vec3(2.0f, 0.5f, 3.0f);
    // Default position and rotation
    
    coordinator->addComponent(entity, transform);
    coordinator->addComponent(entity, components::SceneTag{0});
    
    system->update();
    
    auto& updatedTransform = coordinator->getComponent<components::TransformComponent>(entity);
    
    verifyTransformMatrix(updatedTransform.localMatrix,
                         glm::vec3(0.0f),
                         glm::quat(1.0f, 0.0f, 0.0f, 0.0f),
                         glm::vec3(2.0f, 0.5f, 3.0f));
}

TEST_F(TransformMatrixSystemTest, ExtremeCases) {
    setRenderContext(0);
    
    // Very large values
    auto entity1 = coordinator->createEntity();
    components::TransformComponent transform1;
    transform1.pos = glm::vec3(1e6f, -1e6f, 1e6f);
    transform1.size = glm::vec3(1000.0f, 1000.0f, 1000.0f);
    coordinator->addComponent(entity1, transform1);
    coordinator->addComponent(entity1, components::SceneTag{0});
    
    // Very small values
    auto entity2 = coordinator->createEntity();
    components::TransformComponent transform2;
    transform2.pos = glm::vec3(0.001f, -0.001f, 0.001f);
    transform2.size = glm::vec3(0.001f, 0.001f, 0.001f);
    coordinator->addComponent(entity2, transform2);
    coordinator->addComponent(entity2, components::SceneTag{0});
    
    // Zero scale (degenerate case)
    auto entity3 = coordinator->createEntity();
    components::TransformComponent transform3;
    transform3.size = glm::vec3(0.0f, 1.0f, 1.0f); // Zero X scale
    coordinator->addComponent(entity3, transform3);
    coordinator->addComponent(entity3, components::SceneTag{0});
    
    system->update();
    
    // Verify large values
    auto& updated1 = coordinator->getComponent<components::TransformComponent>(entity1);
    verifyTransformMatrix(updated1.localMatrix, transform1.pos, transform1.quat, transform1.size);
    
    // Verify small values
    auto& updated2 = coordinator->getComponent<components::TransformComponent>(entity2);
    verifyTransformMatrix(updated2.localMatrix, transform2.pos, transform2.quat, transform2.size);
    
    // Zero scale should still produce a valid matrix
    auto& updated3 = coordinator->getComponent<components::TransformComponent>(entity3);
    EXPECT_FALSE(glm::any(glm::isnan(updated3.localMatrix[0])));
    EXPECT_FALSE(glm::any(glm::isnan(updated3.localMatrix[1])));
    EXPECT_FALSE(glm::any(glm::isnan(updated3.localMatrix[2])));
    EXPECT_FALSE(glm::any(glm::isnan(updated3.localMatrix[3])));
}

TEST_F(TransformMatrixSystemTest, ComplexRotations) {
    setRenderContext(0);
    
    // Test various rotation representations
    std::vector<glm::quat> rotations = {
        glm::quat(1.0f, 0.0f, 0.0f, 0.0f), // Identity
        glm::angleAxis(glm::radians(45.0f), glm::vec3(1, 0, 0)), // X-axis
        glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 1, 0)), // Y-axis
        glm::angleAxis(glm::radians(180.0f), glm::vec3(0, 0, 1)), // Z-axis
        glm::angleAxis(glm::radians(30.0f), glm::normalize(glm::vec3(1, 1, 1))), // Arbitrary axis
        glm::quat(0.707f, 0.707f, 0.0f, 0.0f), // 90 degrees around X
        glm::quat(0.5f, 0.5f, 0.5f, 0.5f), // Combined rotation
    };
    
    for (size_t i = 0; i < rotations.size(); ++i) {
        auto entity = coordinator->createEntity();
        components::TransformComponent transform;
        transform.quat = rotations[i];
        coordinator->addComponent(entity, transform);
        coordinator->addComponent(entity, components::SceneTag{0});
    }
    
    system->update();
    
    // All rotations should produce valid matrices
    auto entities = coordinator->getEntities();
    for (auto entity : entities) {
        auto& transform = coordinator->getComponent<components::TransformComponent>(entity);
        
        // Check for NaN or invalid values
        EXPECT_FALSE(glm::any(glm::isnan(transform.localMatrix[0])));
        EXPECT_FALSE(glm::any(glm::isnan(transform.localMatrix[1])));
        EXPECT_FALSE(glm::any(glm::isnan(transform.localMatrix[2])));
        EXPECT_FALSE(glm::any(glm::isnan(transform.localMatrix[3])));
        
        // Rotation matrix should be orthonormal (for non-zero scale)
        if (transform.size != glm::vec3(0.0f)) {
            glm::mat3 rotationPart = glm::mat3(transform.localMatrix);
            // Remove scale from rotation
            rotationPart[0] = glm::normalize(rotationPart[0]);
            rotationPart[1] = glm::normalize(rotationPart[1]);
            rotationPart[2] = glm::normalize(rotationPart[2]);
            
            // Check orthogonality
            float dot01 = glm::dot(rotationPart[0], rotationPart[1]);
            float dot02 = glm::dot(rotationPart[0], rotationPart[2]);
            float dot12 = glm::dot(rotationPart[1], rotationPart[2]);
            
            EXPECT_NEAR(dot01, 0.0f, 0.001f);
            EXPECT_NEAR(dot02, 0.0f, 0.001f);
            EXPECT_NEAR(dot12, 0.0f, 0.001f);
        }
    }
}

TEST_F(TransformMatrixSystemTest, SceneSwitch) {
    // Create entities in multiple scenes
    auto entity0 = coordinator->createEntity();
    coordinator->addComponent(entity0, components::TransformComponent{});
    coordinator->addComponent(entity0, components::SceneTag{0});
    
    auto entity1 = coordinator->createEntity();
    coordinator->addComponent(entity1, components::TransformComponent{});
    coordinator->addComponent(entity1, components::SceneTag{1});
    
    auto entity2 = coordinator->createEntity();
    coordinator->addComponent(entity2, components::TransformComponent{});
    coordinator->addComponent(entity2, components::SceneTag{2});
    
    // Process scene 0
    setRenderContext(0);
    system->update();
    
    auto& transform0 = coordinator->getComponent<components::TransformComponent>(entity0);
    auto& transform1 = coordinator->getComponent<components::TransformComponent>(entity1);
    auto& transform2 = coordinator->getComponent<components::TransformComponent>(entity2);
    
    EXPECT_NE(transform0.localMatrix, glm::mat4(1.0f));
    EXPECT_EQ(transform1.localMatrix, glm::mat4(1.0f));
    EXPECT_EQ(transform2.localMatrix, glm::mat4(1.0f));
    
    // Process scene 1
    setRenderContext(1);
    transform1.pos = glm::vec3(10, 20, 30); // Modify before update
    system->update();
    
    auto& updated1 = coordinator->getComponent<components::TransformComponent>(entity1);
    EXPECT_NE(updated1.localMatrix, glm::mat4(1.0f));
    verifyTransformMatrix(updated1.localMatrix, glm::vec3(10, 20, 30), 
                         glm::quat(1, 0, 0, 0), glm::vec3(1));
    
    // Process scene 2
    setRenderContext(2);
    system->update();
    
    auto& updated2 = coordinator->getComponent<components::TransformComponent>(entity2);
    EXPECT_NE(updated2.localMatrix, glm::mat4(1.0f));
}

TEST_F(TransformMatrixSystemTest, PerformanceTest) {
    setRenderContext(0);
    
    // Create many entities
    const int entityCount = 10000;
    std::vector<ecs::Entity> entities;
    
    for (int i = 0; i < entityCount; ++i) {
        auto entity = coordinator->createEntity();
        components::TransformComponent transform;
        transform.pos = glm::vec3(i, i * 2, i * 3);
        transform.quat = glm::angleAxis(glm::radians(float(i % 360)), 
                                       glm::normalize(glm::vec3(1, 1, 1)));
        transform.size = glm::vec3(1.0f + (i % 10) * 0.1f);
        
        coordinator->addComponent(entity, transform);
        coordinator->addComponent(entity, components::SceneTag{0});
        entities.push_back(entity);
    }
    
    // Measure update time
    PerformanceTimer timer;
    timer.start();
    
    system->update();
    
    double elapsed = timer.elapsedMilliseconds();
    
    // Should process 10k entities reasonably fast
    EXPECT_LT(elapsed, 50.0) << "Processing " << entityCount 
                             << " entities took " << elapsed << "ms";
    
    // Verify all were processed
    for (auto entity : entities) {
        auto& transform = coordinator->getComponent<components::TransformComponent>(entity);
        EXPECT_NE(transform.localMatrix, glm::mat4(1.0f));
    }
}

} // namespace nexo::system