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
//  Description: Test file for the TransformComponent
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "components/Transform.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace nexo::components {

class TransformComponentTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create a default transform for testing
        transform = TransformComponent{};
    }
    
    TransformComponent transform;
};

// Test default initialization
TEST_F(TransformComponentTest, DefaultInitialization) {
    TransformComponent defaultTransform;
    
    // Check position is uninitialized (we don't set a default)
    // Check size defaults to 1.0f
    EXPECT_EQ(defaultTransform.size, glm::vec3(1.0f, 1.0f, 1.0f));
    
    // Check quaternion defaults to identity
    EXPECT_EQ(defaultTransform.quat, glm::quat(1.0f, 0.0f, 0.0f, 0.0f));
    
    // Check matrices default to identity
    EXPECT_EQ(defaultTransform.worldMatrix, glm::mat4(1.0f));
    EXPECT_EQ(defaultTransform.localMatrix, glm::mat4(1.0f));
    
    // Check local center defaults to origin
    EXPECT_EQ(defaultTransform.localCenter, glm::vec3(0.0f, 0.0f, 0.0f));
    
    // Check children vector is empty
    EXPECT_TRUE(defaultTransform.children.empty());
}

// Test memento pattern - save
TEST_F(TransformComponentTest, SaveMemento) {
    // Set up transform with specific values
    transform.pos = glm::vec3(10.0f, 20.0f, 30.0f);
    transform.quat = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    transform.size = glm::vec3(2.0f, 3.0f, 4.0f);
    transform.localMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(1.0f, 2.0f, 3.0f));
    transform.localCenter = glm::vec3(0.5f, 0.5f, 0.5f);
    transform.children = {1, 2, 3};
    
    // Save memento
    auto memento = transform.save();
    
    // Verify memento contains correct values
    EXPECT_EQ(memento.position, transform.pos);
    EXPECT_EQ(memento.rotation, transform.quat);
    EXPECT_EQ(memento.scale, transform.size);
    EXPECT_EQ(memento.localMatrix, transform.localMatrix);
    EXPECT_EQ(memento.localCenter, transform.localCenter);
    EXPECT_EQ(memento.children, transform.children);
}

// Test memento pattern - restore
TEST_F(TransformComponentTest, RestoreMemento) {
    // Create a memento with specific values
    TransformComponent::Memento memento{
        .position = glm::vec3(5.0f, 10.0f, 15.0f),
        .rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f)),
        .scale = glm::vec3(0.5f, 0.5f, 0.5f),
        .localMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f)),
        .localCenter = glm::vec3(-1.0f, -1.0f, -1.0f),
        .children = {10, 20, 30}
    };
    
    // Restore from memento
    transform.restore(memento);
    
    // Verify all values were restored
    EXPECT_EQ(transform.pos, memento.position);
    EXPECT_EQ(transform.quat, memento.rotation);
    EXPECT_EQ(transform.size, memento.scale);
    EXPECT_EQ(transform.localMatrix, memento.localMatrix);
    EXPECT_EQ(transform.localCenter, memento.localCenter);
    EXPECT_EQ(transform.children, memento.children);
}

// Test save and restore round trip
TEST_F(TransformComponentTest, SaveRestoreRoundTrip) {
    // Set up original transform
    transform.pos = glm::vec3(100.0f, 200.0f, 300.0f);
    transform.quat = glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    transform.size = glm::vec3(10.0f, 20.0f, 30.0f);
    transform.children = {100, 200};
    
    // Save state
    auto memento = transform.save();
    
    // Modify transform
    transform.pos = glm::vec3(0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f);
    transform.children.clear();
    
    // Restore state
    transform.restore(memento);
    
    // Verify original state is restored
    EXPECT_EQ(transform.pos, glm::vec3(100.0f, 200.0f, 300.0f));
    EXPECT_EQ(transform.quat, glm::angleAxis(glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)));
    EXPECT_EQ(transform.size, glm::vec3(10.0f, 20.0f, 30.0f));
    EXPECT_EQ(transform.children, (std::vector<ecs::Entity>{100, 200}));
}

// Test adding children
TEST_F(TransformComponentTest, AddChild) {
    EXPECT_TRUE(transform.children.empty());
    
    // Add first child
    transform.addChild(1);
    EXPECT_EQ(transform.children.size(), 1);
    EXPECT_EQ(transform.children[0], 1);
    
    // Add second child
    transform.addChild(2);
    EXPECT_EQ(transform.children.size(), 2);
    EXPECT_EQ(transform.children[1], 2);
    
    // Add third child
    transform.addChild(3);
    EXPECT_EQ(transform.children.size(), 3);
    EXPECT_EQ(transform.children[2], 3);
}

// Test adding duplicate child
TEST_F(TransformComponentTest, AddDuplicateChild) {
    // Add child
    transform.addChild(5);
    EXPECT_EQ(transform.children.size(), 1);
    
    // Try to add same child again
    transform.addChild(5);
    EXPECT_EQ(transform.children.size(), 1); // Should not add duplicate
    
    // Add different child
    transform.addChild(6);
    EXPECT_EQ(transform.children.size(), 2);
    
    // Try to add first child again
    transform.addChild(5);
    EXPECT_EQ(transform.children.size(), 2); // Should still not add duplicate
}

// Test removing children
TEST_F(TransformComponentTest, RemoveChild) {
    // Add multiple children
    transform.children = {1, 2, 3, 4, 5};
    
    // Remove middle child
    transform.removeChild(3);
    EXPECT_EQ(transform.children.size(), 4);
    EXPECT_THAT(transform.children, ::testing::ElementsAre(1, 2, 4, 5));
    
    // Remove first child
    transform.removeChild(1);
    EXPECT_EQ(transform.children.size(), 3);
    EXPECT_THAT(transform.children, ::testing::ElementsAre(2, 4, 5));
    
    // Remove last child
    transform.removeChild(5);
    EXPECT_EQ(transform.children.size(), 2);
    EXPECT_THAT(transform.children, ::testing::ElementsAre(2, 4));
}

// Test removing non-existent child
TEST_F(TransformComponentTest, RemoveNonExistentChild) {
    transform.children = {1, 2, 3};
    
    // Try to remove child that doesn't exist
    transform.removeChild(10);
    EXPECT_EQ(transform.children.size(), 3); // Size should remain unchanged
    EXPECT_THAT(transform.children, ::testing::ElementsAre(1, 2, 3));
}

// Test removing from empty children list
TEST_F(TransformComponentTest, RemoveFromEmptyChildren) {
    EXPECT_TRUE(transform.children.empty());
    
    // Should not crash when removing from empty list
    transform.removeChild(1);
    EXPECT_TRUE(transform.children.empty());
}

// Test multiple operations
TEST_F(TransformComponentTest, MultipleChildOperations) {
    // Add children
    transform.addChild(1);
    transform.addChild(2);
    transform.addChild(3);
    EXPECT_EQ(transform.children.size(), 3);
    
    // Remove and re-add
    transform.removeChild(2);
    EXPECT_EQ(transform.children.size(), 2);
    transform.addChild(2); // Should add at end
    EXPECT_EQ(transform.children.size(), 3);
    EXPECT_THAT(transform.children, ::testing::ElementsAre(1, 3, 2));
    
    // Clear all
    transform.removeChild(1);
    transform.removeChild(2);
    transform.removeChild(3);
    EXPECT_TRUE(transform.children.empty());
}

// Test extreme values
TEST_F(TransformComponentTest, ExtremeValues) {
    // Test with very large position values
    transform.pos = glm::vec3(1e10f, -1e10f, 1e10f);
    auto memento = transform.save();
    EXPECT_EQ(memento.position, transform.pos);
    
    // Test with very small scale values
    transform.size = glm::vec3(1e-10f, 1e-10f, 1e-10f);
    memento = transform.save();
    EXPECT_EQ(memento.scale, transform.size);
    
    // Test with many children
    for (ecs::Entity i = 0; i < 1000; ++i) {
        transform.addChild(i);
    }
    EXPECT_EQ(transform.children.size(), 1000);
    
    // Verify save/restore works with many children
    memento = transform.save();
    EXPECT_EQ(memento.children.size(), 1000);
}

// Test quaternion normalization
TEST_F(TransformComponentTest, QuaternionValues) {
    // Test with various rotation angles
    std::vector<float> angles = {0.0f, 45.0f, 90.0f, 180.0f, 270.0f, 360.0f};
    std::vector<glm::vec3> axes = {
        glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 1.0f),
        glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f))
    };
    
    for (auto angle : angles) {
        for (const auto& axis : axes) {
            transform.quat = glm::angleAxis(glm::radians(angle), axis);
            auto memento = transform.save();
            
            // Quaternions should be equal within floating point precision
            EXPECT_NEAR(memento.rotation.w, transform.quat.w, 1e-6f);
            EXPECT_NEAR(memento.rotation.x, transform.quat.x, 1e-6f);
            EXPECT_NEAR(memento.rotation.y, transform.quat.y, 1e-6f);
            EXPECT_NEAR(memento.rotation.z, transform.quat.z, 1e-6f);
        }
    }
}

// Test matrix values
TEST_F(TransformComponentTest, MatrixValues) {
    // Test various matrix transformations
    transform.localMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, 20.0f, 30.0f));
    transform.worldMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    
    auto memento = transform.save();
    EXPECT_EQ(memento.localMatrix, transform.localMatrix);
    
    // Note: worldMatrix is not saved in memento, which is correct
    // as it's typically computed from other values
}

// Test zero entity ID
TEST_F(TransformComponentTest, ZeroEntityChild) {
    // Entity ID 0 might be NULL_ENTITY, but the component should still handle it
    transform.addChild(0);
    EXPECT_EQ(transform.children.size(), 1);
    EXPECT_EQ(transform.children[0], 0);
    
    transform.removeChild(0);
    EXPECT_TRUE(transform.children.empty());
}

// Test maximum entity ID
TEST_F(TransformComponentTest, MaxEntityChild) {
    ecs::Entity maxEntity = std::numeric_limits<ecs::Entity>::max();
    
    transform.addChild(maxEntity);
    EXPECT_EQ(transform.children.size(), 1);
    EXPECT_EQ(transform.children[0], maxEntity);
    
    transform.removeChild(maxEntity);
    EXPECT_TRUE(transform.children.empty());
}

} // namespace nexo::components