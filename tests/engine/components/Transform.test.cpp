//// Transform.test.cpp ///////////////////////////////////////////////////////
//
//  Author:      Claude AI
//  Date:        03/12/2025
//  Description: Test file for TransformComponent (memento pattern and child management)
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/epsilon.hpp>
#include <limits>
#include "components/Transform.hpp"

namespace nexo::components {

class TransformComponentTest : public ::testing::Test {
protected:
    TransformComponent transform;

    // Helper to compare vec3 with epsilon
    static bool compareVec3(const glm::vec3& a, const glm::vec3& b, float epsilon = 0.0001f) {
        return glm::all(glm::epsilonEqual(a, b, epsilon));
    }

    // Helper to compare quaternions with epsilon
    static bool compareQuat(const glm::quat& a, const glm::quat& b, float epsilon = 0.0001f) {
        return glm::all(glm::epsilonEqual(glm::vec4(a.x, a.y, a.z, a.w),
                                          glm::vec4(b.x, b.y, b.z, b.w), epsilon));
    }

    // Helper to compare mat4 with epsilon
    static bool compareMat4(const glm::mat4& a, const glm::mat4& b, float epsilon = 0.0001f) {
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                if (std::abs(a[i][j] - b[i][j]) > epsilon)
                    return false;
            }
        }
        return true;
    }
};

// =============================================================================
// Default State Tests
// =============================================================================

TEST_F(TransformComponentTest, DefaultPosition) {
    // Default position should be zero (note: pos is uninitialized by default)
    TransformComponent t;
    t.pos = glm::vec3(0.0f);
    EXPECT_TRUE(compareVec3(t.pos, glm::vec3(0.0f, 0.0f, 0.0f)));
}

TEST_F(TransformComponentTest, DefaultScale) {
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(1.0f, 1.0f, 1.0f)));
}

TEST_F(TransformComponentTest, DefaultRotation) {
    // Identity quaternion (w=1, x=y=z=0)
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(1.0f, 0.0f, 0.0f, 0.0f)));
}

TEST_F(TransformComponentTest, DefaultWorldMatrix) {
    EXPECT_TRUE(compareMat4(transform.worldMatrix, glm::mat4(1.0f)));
}

TEST_F(TransformComponentTest, DefaultLocalMatrix) {
    EXPECT_TRUE(compareMat4(transform.localMatrix, glm::mat4(1.0f)));
}

TEST_F(TransformComponentTest, DefaultLocalCenter) {
    EXPECT_TRUE(compareVec3(transform.localCenter, glm::vec3(0.0f, 0.0f, 0.0f)));
}

TEST_F(TransformComponentTest, DefaultChildrenEmpty) {
    EXPECT_TRUE(transform.children.empty());
}

// =============================================================================
// Save/Restore Memento Tests
// =============================================================================

TEST_F(TransformComponentTest, SaveCapturesPosition) {
    transform.pos = glm::vec3(1.0f, 2.0f, 3.0f);
    auto memento = transform.save();
    EXPECT_TRUE(compareVec3(memento.position, glm::vec3(1.0f, 2.0f, 3.0f)));
}

TEST_F(TransformComponentTest, SaveCapturesRotation) {
    transform.quat = glm::quat(glm::vec3(0.5f, 1.0f, 1.5f));  // Euler to quat
    auto memento = transform.save();
    EXPECT_TRUE(compareQuat(memento.rotation, transform.quat));
}

TEST_F(TransformComponentTest, SaveCapturesScale) {
    transform.size = glm::vec3(2.0f, 3.0f, 4.0f);
    auto memento = transform.save();
    EXPECT_TRUE(compareVec3(memento.scale, glm::vec3(2.0f, 3.0f, 4.0f)));
}

TEST_F(TransformComponentTest, SaveCapturesLocalMatrix) {
    glm::mat4 testMatrix(2.0f);
    transform.localMatrix = testMatrix;
    auto memento = transform.save();
    EXPECT_TRUE(compareMat4(memento.localMatrix, testMatrix));
}

TEST_F(TransformComponentTest, SaveCapturesLocalCenter) {
    transform.localCenter = glm::vec3(5.0f, 6.0f, 7.0f);
    auto memento = transform.save();
    EXPECT_TRUE(compareVec3(memento.localCenter, glm::vec3(5.0f, 6.0f, 7.0f)));
}

TEST_F(TransformComponentTest, SaveCapturesChildren) {
    transform.children = {1, 2, 3, 4, 5};
    auto memento = transform.save();
    EXPECT_EQ(memento.children.size(), 5u);
    EXPECT_EQ(memento.children, transform.children);
}

TEST_F(TransformComponentTest, RestoreAppliesPosition) {
    TransformComponent::Memento memento;
    memento.position = glm::vec3(10.0f, 20.0f, 30.0f);
    memento.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    memento.scale = glm::vec3(1.0f);
    memento.localMatrix = glm::mat4(1.0f);
    memento.localCenter = glm::vec3(0.0f);

    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(10.0f, 20.0f, 30.0f)));
}

TEST_F(TransformComponentTest, RestoreAppliesRotation) {
    glm::quat testQuat = glm::quat(glm::vec3(1.0f, 0.0f, 0.0f));

    TransformComponent::Memento memento;
    memento.position = glm::vec3(0.0f);
    memento.rotation = testQuat;
    memento.scale = glm::vec3(1.0f);
    memento.localMatrix = glm::mat4(1.0f);
    memento.localCenter = glm::vec3(0.0f);

    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, testQuat));
}

TEST_F(TransformComponentTest, RestoreAppliesScale) {
    TransformComponent::Memento memento;
    memento.position = glm::vec3(0.0f);
    memento.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    memento.scale = glm::vec3(5.0f, 10.0f, 15.0f);
    memento.localMatrix = glm::mat4(1.0f);
    memento.localCenter = glm::vec3(0.0f);

    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(5.0f, 10.0f, 15.0f)));
}

TEST_F(TransformComponentTest, RestoreAppliesChildren) {
    TransformComponent::Memento memento;
    memento.position = glm::vec3(0.0f);
    memento.rotation = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    memento.scale = glm::vec3(1.0f);
    memento.localMatrix = glm::mat4(1.0f);
    memento.localCenter = glm::vec3(0.0f);
    memento.children = {100, 200, 300};

    transform.restore(memento);
    EXPECT_EQ(transform.children.size(), 3u);
    EXPECT_EQ(transform.children[0], 100u);
    EXPECT_EQ(transform.children[1], 200u);
    EXPECT_EQ(transform.children[2], 300u);
}

TEST_F(TransformComponentTest, SaveRestoreRoundTrip) {
    // Set up complex state
    transform.pos = glm::vec3(1.0f, 2.0f, 3.0f);
    transform.quat = glm::quat(glm::vec3(0.1f, 0.2f, 0.3f));
    transform.size = glm::vec3(4.0f, 5.0f, 6.0f);
    transform.localMatrix = glm::mat4(3.0f);
    transform.localCenter = glm::vec3(7.0f, 8.0f, 9.0f);
    transform.children = {10, 20, 30};

    // Save state
    auto memento = transform.save();

    // Modify all fields
    transform.pos = glm::vec3(0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f);
    transform.localMatrix = glm::mat4(1.0f);
    transform.localCenter = glm::vec3(0.0f);
    transform.children.clear();

    // Restore from memento
    transform.restore(memento);

    // Verify all fields restored
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(1.0f, 2.0f, 3.0f)));
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(0.1f, 0.2f, 0.3f))));
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(4.0f, 5.0f, 6.0f)));
    EXPECT_TRUE(compareMat4(transform.localMatrix, glm::mat4(3.0f)));
    EXPECT_TRUE(compareVec3(transform.localCenter, glm::vec3(7.0f, 8.0f, 9.0f)));
    EXPECT_EQ(transform.children.size(), 3u);
}

// =============================================================================
// Child Management Tests
// =============================================================================

TEST_F(TransformComponentTest, AddChildAppendsToList) {
    transform.addChild(42);
    ASSERT_EQ(transform.children.size(), 1u);
    EXPECT_EQ(transform.children[0], 42u);
}

TEST_F(TransformComponentTest, AddChildMultiple) {
    transform.addChild(1);
    transform.addChild(2);
    transform.addChild(3);

    ASSERT_EQ(transform.children.size(), 3u);
    EXPECT_EQ(transform.children[0], 1u);
    EXPECT_EQ(transform.children[1], 2u);
    EXPECT_EQ(transform.children[2], 3u);
}

TEST_F(TransformComponentTest, AddChildPreventsDuplicates) {
    transform.addChild(42);
    transform.addChild(42);  // Duplicate
    transform.addChild(42);  // Duplicate

    EXPECT_EQ(transform.children.size(), 1u);
}

TEST_F(TransformComponentTest, AddChildPreventsDuplicatesAmongMany) {
    transform.addChild(1);
    transform.addChild(2);
    transform.addChild(3);
    transform.addChild(2);  // Duplicate
    transform.addChild(1);  // Duplicate

    EXPECT_EQ(transform.children.size(), 3u);
}

TEST_F(TransformComponentTest, RemoveChildRemovesExisting) {
    transform.children = {1, 2, 3, 4, 5};

    transform.removeChild(3);

    ASSERT_EQ(transform.children.size(), 4u);
    EXPECT_EQ(transform.children[0], 1u);
    EXPECT_EQ(transform.children[1], 2u);
    EXPECT_EQ(transform.children[2], 4u);
    EXPECT_EQ(transform.children[3], 5u);
}

TEST_F(TransformComponentTest, RemoveChildFromEmpty) {
    EXPECT_NO_THROW(transform.removeChild(42));
    EXPECT_TRUE(transform.children.empty());
}

TEST_F(TransformComponentTest, RemoveChildNonExistent) {
    transform.children = {1, 2, 3};

    transform.removeChild(999);  // Not in list

    EXPECT_EQ(transform.children.size(), 3u);
}

TEST_F(TransformComponentTest, RemoveChildFirst) {
    transform.children = {1, 2, 3};

    transform.removeChild(1);

    ASSERT_EQ(transform.children.size(), 2u);
    EXPECT_EQ(transform.children[0], 2u);
    EXPECT_EQ(transform.children[1], 3u);
}

TEST_F(TransformComponentTest, RemoveChildLast) {
    transform.children = {1, 2, 3};

    transform.removeChild(3);

    ASSERT_EQ(transform.children.size(), 2u);
    EXPECT_EQ(transform.children[0], 1u);
    EXPECT_EQ(transform.children[1], 2u);
}

TEST_F(TransformComponentTest, RemoveChildAll) {
    transform.children = {1, 2, 3};

    transform.removeChild(1);
    transform.removeChild(2);
    transform.removeChild(3);

    EXPECT_TRUE(transform.children.empty());
}

TEST_F(TransformComponentTest, AddAfterRemove) {
    transform.addChild(1);
    transform.removeChild(1);
    transform.addChild(1);

    ASSERT_EQ(transform.children.size(), 1u);
    EXPECT_EQ(transform.children[0], 1u);
}

// =============================================================================
// Edge Case Tests - Scale
// =============================================================================

TEST_F(TransformComponentTest, ScaleNearZero) {
    // Very small but non-zero scale values
    transform.size = glm::vec3(0.0001f, 0.0001f, 0.0001f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(0.0001f, 0.0001f, 0.0001f)));
}

TEST_F(TransformComponentTest, ScaleZero) {
    // Zero scale (degenerate case)
    transform.size = glm::vec3(0.0f, 0.0f, 0.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(0.0f, 0.0f, 0.0f)));
}

TEST_F(TransformComponentTest, ScaleNegative) {
    // Negative scale (mirrors/flips)
    transform.size = glm::vec3(-1.0f, -2.0f, -3.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(-1.0f, -2.0f, -3.0f)));
}

TEST_F(TransformComponentTest, ScaleMixed) {
    // Mixed positive and negative scale
    transform.size = glm::vec3(-1.0f, 2.0f, -3.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(-1.0f, 2.0f, -3.0f)));
}

TEST_F(TransformComponentTest, ScaleVeryLarge) {
    // Very large scale values
    transform.size = glm::vec3(10000.0f, 10000.0f, 10000.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(10000.0f, 10000.0f, 10000.0f)));
}

TEST_F(TransformComponentTest, ScaleNonUniform) {
    // Non-uniform scale with extreme differences
    transform.size = glm::vec3(0.001f, 1000.0f, 1.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(0.001f, 1000.0f, 1.0f)));
}

// =============================================================================
// Edge Case Tests - Euler Angles and Quaternions
// =============================================================================

TEST_F(TransformComponentTest, RotationZeroDegrees) {
    // Zero rotation (identity)
    transform.quat = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(0.0f, 0.0f, 0.0f))));
}

TEST_F(TransformComponentTest, Rotation90DegreesX) {
    // 90 degrees around X axis
    transform.quat = glm::quat(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f));
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(glm::radians(90.0f), 0.0f, 0.0f))));
}

TEST_F(TransformComponentTest, Rotation90DegreesY) {
    // 90 degrees around Y axis (gimbal lock risk)
    transform.quat = glm::quat(glm::vec3(0.0f, glm::radians(90.0f), 0.0f));
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(0.0f, glm::radians(90.0f), 0.0f))));
}

TEST_F(TransformComponentTest, Rotation90DegreesZ) {
    // 90 degrees around Z axis
    transform.quat = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)));
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(90.0f)))));
}

TEST_F(TransformComponentTest, Rotation180Degrees) {
    // 180 degrees around Y axis
    transform.quat = glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f));
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(0.0f, glm::radians(180.0f), 0.0f))));
}

TEST_F(TransformComponentTest, Rotation270Degrees) {
    // 270 degrees around Z axis
    transform.quat = glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(270.0f)));
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(0.0f, 0.0f, glm::radians(270.0f)))));
}

TEST_F(TransformComponentTest, Rotation360Degrees) {
    // 360 degrees (should equal 0 or be negated - quaternion double cover)
    glm::quat quat360 = glm::quat(glm::vec3(glm::radians(360.0f), 0.0f, 0.0f));
    glm::quat quat0 = glm::quat(glm::vec3(0.0f, 0.0f, 0.0f));
    // Quaternions q and -q represent the same rotation
    bool equivalent = compareQuat(quat360, quat0) || compareQuat(quat360, -quat0);
    EXPECT_TRUE(equivalent);
}

TEST_F(TransformComponentTest, RotationGimbalLock) {
    // Pitch at 90 degrees (classic gimbal lock scenario)
    transform.quat = glm::quat(glm::vec3(glm::radians(90.0f), glm::radians(45.0f), glm::radians(45.0f)));
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(glm::radians(90.0f), glm::radians(45.0f), glm::radians(45.0f)))));
}

TEST_F(TransformComponentTest, RotationNegativeAngles) {
    // Negative angles
    transform.quat = glm::quat(glm::vec3(glm::radians(-45.0f), glm::radians(-90.0f), glm::radians(-135.0f)));
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(glm::radians(-45.0f), glm::radians(-90.0f), glm::radians(-135.0f)))));
}

TEST_F(TransformComponentTest, RotationVerySmallAngles) {
    // Very small angles (near zero)
    transform.quat = glm::quat(glm::vec3(glm::radians(0.001f), glm::radians(0.001f), glm::radians(0.001f)));
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(glm::radians(0.001f), glm::radians(0.001f), glm::radians(0.001f)))));
}

TEST_F(TransformComponentTest, QuaternionNormalized) {
    // Create a quaternion and ensure it's normalized
    glm::quat q(1.0f, 2.0f, 3.0f, 4.0f);
    q = glm::normalize(q);
    transform.quat = q;

    float length = glm::length(transform.quat);
    EXPECT_NEAR(length, 1.0f, 0.0001f);
}

TEST_F(TransformComponentTest, QuaternionIdentity) {
    // Identity quaternion
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(1.0f, 0.0f, 0.0f, 0.0f)));
}

TEST_F(TransformComponentTest, QuaternionConjugate) {
    // Test that conjugate represents inverse rotation
    glm::quat q = glm::quat(glm::vec3(glm::radians(45.0f), glm::radians(30.0f), glm::radians(60.0f)));
    glm::quat qConj = glm::conjugate(q);
    glm::quat result = q * qConj;

    // q * conjugate(q) should be identity
    EXPECT_TRUE(compareQuat(result, glm::quat(1.0f, 0.0f, 0.0f, 0.0f)));
}

// =============================================================================
// Edge Case Tests - Position
// =============================================================================

TEST_F(TransformComponentTest, PositionVeryLarge) {
    // Very large position values
    transform.pos = glm::vec3(100000.0f, 100000.0f, 100000.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(100000.0f, 100000.0f, 100000.0f)));
}

TEST_F(TransformComponentTest, PositionVerySmall) {
    // Very small position values
    transform.pos = glm::vec3(0.00001f, 0.00001f, 0.00001f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(0.00001f, 0.00001f, 0.00001f)));
}

TEST_F(TransformComponentTest, PositionNegative) {
    // Negative position values
    transform.pos = glm::vec3(-1000.0f, -2000.0f, -3000.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(-1000.0f, -2000.0f, -3000.0f)));
}

TEST_F(TransformComponentTest, PositionMixed) {
    // Mixed positive and negative
    transform.pos = glm::vec3(-100.0f, 200.0f, -300.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(-100.0f, 200.0f, -300.0f)));
}

// =============================================================================
// Edge Case Tests - Combined Transformations
// =============================================================================

TEST_F(TransformComponentTest, CombinedTransformAllZero) {
    // Position at zero, identity rotation, zero scale
    transform.pos = glm::vec3(0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(0.0f);

    auto memento = transform.save();
    transform.restore(memento);

    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(0.0f)));
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(1.0f, 0.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(0.0f)));
}

TEST_F(TransformComponentTest, CombinedTransformExtremeValues) {
    // Extreme combinations
    transform.pos = glm::vec3(10000.0f, -10000.0f, 5000.0f);
    transform.quat = glm::quat(glm::vec3(glm::radians(90.0f), glm::radians(180.0f), glm::radians(270.0f)));
    transform.size = glm::vec3(0.001f, 1000.0f, -10.0f);

    auto memento = transform.save();
    transform.restore(memento);

    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(10000.0f, -10000.0f, 5000.0f)));
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(glm::radians(90.0f), glm::radians(180.0f), glm::radians(270.0f)))));
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(0.001f, 1000.0f, -10.0f)));
}

TEST_F(TransformComponentTest, CombinedTransformNegativeScale) {
    // Negative scale with rotation and translation
    transform.pos = glm::vec3(5.0f, 10.0f, 15.0f);
    transform.quat = glm::quat(glm::vec3(glm::radians(45.0f), 0.0f, 0.0f));
    transform.size = glm::vec3(-1.0f, -1.0f, -1.0f);

    auto memento = transform.save();
    transform.restore(memento);

    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(5.0f, 10.0f, 15.0f)));
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(glm::radians(45.0f), 0.0f, 0.0f))));
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(-1.0f, -1.0f, -1.0f)));
}

TEST_F(TransformComponentTest, CombinedTransformAllIdentity) {
    // All identity values
    transform.pos = glm::vec3(0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f);

    auto memento = transform.save();
    transform.restore(memento);

    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(0.0f)));
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(1.0f, 0.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(1.0f)));
}

// =============================================================================
// Edge Case Tests - Matrix Operations
// =============================================================================

TEST_F(TransformComponentTest, LocalMatrixIdentity) {
    // Identity matrix preservation
    transform.localMatrix = glm::mat4(1.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareMat4(transform.localMatrix, glm::mat4(1.0f)));
}

TEST_F(TransformComponentTest, LocalMatrixZero) {
    // Zero matrix
    transform.localMatrix = glm::mat4(0.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareMat4(transform.localMatrix, glm::mat4(0.0f)));
}

TEST_F(TransformComponentTest, LocalMatrixLargeValues) {
    // Matrix with large values
    glm::mat4 largeMatrix(10000.0f);
    transform.localMatrix = largeMatrix;
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareMat4(transform.localMatrix, largeMatrix));
}

TEST_F(TransformComponentTest, LocalMatrixSmallValues) {
    // Matrix with small values
    glm::mat4 smallMatrix(0.0001f);
    transform.localMatrix = smallMatrix;
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareMat4(transform.localMatrix, smallMatrix));
}

TEST_F(TransformComponentTest, WorldMatrixIdentity) {
    // World matrix identity
    transform.worldMatrix = glm::mat4(1.0f);
    EXPECT_TRUE(compareMat4(transform.worldMatrix, glm::mat4(1.0f)));
}

TEST_F(TransformComponentTest, LocalCenterExtremePositive) {
    // Local center with extreme positive values
    transform.localCenter = glm::vec3(99999.0f, 99999.0f, 99999.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.localCenter, glm::vec3(99999.0f, 99999.0f, 99999.0f)));
}

TEST_F(TransformComponentTest, LocalCenterExtremeNegative) {
    // Local center with extreme negative values
    transform.localCenter = glm::vec3(-99999.0f, -99999.0f, -99999.0f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.localCenter, glm::vec3(-99999.0f, -99999.0f, -99999.0f)));
}

TEST_F(TransformComponentTest, LocalCenterNearZero) {
    // Local center with very small values
    transform.localCenter = glm::vec3(0.00001f, 0.00001f, 0.00001f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.localCenter, glm::vec3(0.00001f, 0.00001f, 0.00001f)));
}

// =============================================================================
// Edge Case Tests - Memento with Extreme Values
// =============================================================================

TEST_F(TransformComponentTest, MementoAllExtremePositive) {
    // All extreme positive values
    transform.pos = glm::vec3(std::numeric_limits<float>::max() / 2.0f);
    transform.quat = glm::quat(glm::vec3(glm::radians(180.0f), glm::radians(180.0f), glm::radians(180.0f)));
    transform.size = glm::vec3(10000.0f, 10000.0f, 10000.0f);
    transform.localCenter = glm::vec3(50000.0f, 50000.0f, 50000.0f);

    auto memento = transform.save();

    // Reset
    transform.pos = glm::vec3(0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f);
    transform.localCenter = glm::vec3(0.0f);

    // Restore
    transform.restore(memento);

    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(std::numeric_limits<float>::max() / 2.0f)));
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(glm::radians(180.0f), glm::radians(180.0f), glm::radians(180.0f)))));
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(10000.0f, 10000.0f, 10000.0f)));
    EXPECT_TRUE(compareVec3(transform.localCenter, glm::vec3(50000.0f, 50000.0f, 50000.0f)));
}

TEST_F(TransformComponentTest, MementoAllExtremeNegative) {
    // All extreme negative values
    transform.pos = glm::vec3(-50000.0f, -50000.0f, -50000.0f);
    transform.quat = glm::quat(glm::vec3(glm::radians(-180.0f), glm::radians(-90.0f), glm::radians(-270.0f)));
    transform.size = glm::vec3(-1000.0f, -1000.0f, -1000.0f);
    transform.localCenter = glm::vec3(-50000.0f, -50000.0f, -50000.0f);

    auto memento = transform.save();

    // Reset
    transform.pos = glm::vec3(0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f);
    transform.localCenter = glm::vec3(0.0f);

    // Restore
    transform.restore(memento);

    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(-50000.0f, -50000.0f, -50000.0f)));
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(glm::radians(-180.0f), glm::radians(-90.0f), glm::radians(-270.0f)))));
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(-1000.0f, -1000.0f, -1000.0f)));
    EXPECT_TRUE(compareVec3(transform.localCenter, glm::vec3(-50000.0f, -50000.0f, -50000.0f)));
}

TEST_F(TransformComponentTest, MementoAllNearZero) {
    // All near-zero values
    transform.pos = glm::vec3(0.0001f, 0.0001f, 0.0001f);
    transform.quat = glm::quat(glm::vec3(0.0001f, 0.0001f, 0.0001f));
    transform.size = glm::vec3(0.0001f, 0.0001f, 0.0001f);
    transform.localCenter = glm::vec3(0.0001f, 0.0001f, 0.0001f);

    auto memento = transform.save();

    // Reset
    transform.pos = glm::vec3(0.0f);
    transform.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
    transform.size = glm::vec3(1.0f);
    transform.localCenter = glm::vec3(0.0f);

    // Restore
    transform.restore(memento);

    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(0.0001f, 0.0001f, 0.0001f)));
    EXPECT_TRUE(compareQuat(transform.quat, glm::quat(glm::vec3(0.0001f, 0.0001f, 0.0001f))));
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(0.0001f, 0.0001f, 0.0001f)));
    EXPECT_TRUE(compareVec3(transform.localCenter, glm::vec3(0.0001f, 0.0001f, 0.0001f)));
}

TEST_F(TransformComponentTest, MementoMultipleSaveRestore) {
    // Multiple save/restore cycles with different values
    transform.pos = glm::vec3(1.0f, 2.0f, 3.0f);
    auto memento1 = transform.save();

    transform.pos = glm::vec3(4.0f, 5.0f, 6.0f);
    auto memento2 = transform.save();

    transform.pos = glm::vec3(7.0f, 8.0f, 9.0f);
    auto memento3 = transform.save();

    // Restore in reverse order
    transform.restore(memento2);
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(4.0f, 5.0f, 6.0f)));

    transform.restore(memento1);
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(1.0f, 2.0f, 3.0f)));

    transform.restore(memento3);
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(7.0f, 8.0f, 9.0f)));
}

// =============================================================================
// Edge Case Tests - Children with Extreme Values
// =============================================================================

TEST_F(TransformComponentTest, ChildrenLargeCount) {
    // Large number of children
    for (unsigned int i = 0; i < 1000; ++i) {
        transform.addChild(i);
    }
    EXPECT_EQ(transform.children.size(), 1000u);

    auto memento = transform.save();
    transform.children.clear();
    transform.restore(memento);

    EXPECT_EQ(transform.children.size(), 1000u);
}

TEST_F(TransformComponentTest, ChildrenMaxEntityValue) {
    // Maximum entity ID value
    constexpr unsigned int MAX_ENTITY = std::numeric_limits<unsigned int>::max();
    transform.addChild(MAX_ENTITY);
    EXPECT_EQ(transform.children.size(), 1u);
    EXPECT_EQ(transform.children[0], MAX_ENTITY);

    auto memento = transform.save();
    transform.children.clear();
    transform.restore(memento);

    EXPECT_EQ(transform.children[0], MAX_ENTITY);
}

TEST_F(TransformComponentTest, ChildrenZeroEntityValue) {
    // Zero entity ID (edge case)
    transform.addChild(0);
    EXPECT_EQ(transform.children.size(), 1u);
    EXPECT_EQ(transform.children[0], 0u);
}

TEST_F(TransformComponentTest, ChildrenMixedExtreme) {
    // Mix of minimum, maximum, and middle values
    transform.addChild(0);
    transform.addChild(std::numeric_limits<unsigned int>::max());
    transform.addChild(std::numeric_limits<unsigned int>::max() / 2);

    EXPECT_EQ(transform.children.size(), 3u);

    auto memento = transform.save();
    transform.children.clear();
    transform.restore(memento);

    EXPECT_EQ(transform.children.size(), 3u);
}

// =============================================================================
// Edge Case Tests - Numerical Stability
// =============================================================================

TEST_F(TransformComponentTest, NumericalStabilityVerySmallScale) {
    // Test numerical stability with very small scale
    transform.size = glm::vec3(1e-7f, 1e-7f, 1e-7f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.size, glm::vec3(1e-7f, 1e-7f, 1e-7f), 1e-8f));
}

TEST_F(TransformComponentTest, NumericalStabilityVeryLargePosition) {
    // Test numerical stability with very large position
    transform.pos = glm::vec3(1e7f, 1e7f, 1e7f);
    auto memento = transform.save();
    transform.restore(memento);
    EXPECT_TRUE(compareVec3(transform.pos, glm::vec3(1e7f, 1e7f, 1e7f), 1.0f));
}

TEST_F(TransformComponentTest, NumericalStabilityMixedMagnitudes) {
    // Mixed magnitude values (numerical stability challenge)
    transform.pos = glm::vec3(1e-5f, 1e5f, 1.0f);
    transform.size = glm::vec3(1e-5f, 1e5f, 1.0f);

    auto memento = transform.save();
    transform.restore(memento);

    EXPECT_NEAR(transform.pos.x, 1e-5f, 1e-6f);
    EXPECT_NEAR(transform.pos.y, 1e5f, 10.0f);
    EXPECT_NEAR(transform.pos.z, 1.0f, 0.0001f);
}

}  // namespace nexo::components
