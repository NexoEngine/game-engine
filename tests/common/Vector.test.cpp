//// Vector.test.cpp ///////////////////////////////////////////////////////////////
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
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Test file for the vector utils functions
//
///////////////////////////////////////////////////////////////////////////////

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <gtest/gtest.h>
#include "math/Vector.hpp"
#include "../utils/comparison.hpp"

class ExtractCameraComponentsTest : public ::testing::Test {
protected:
    // You can add common setup code here if needed.
};

TEST_F(ExtractCameraComponentsTest, IdentityRotation) {
    // We want a default camera orientation where:
    //   - cameraComponent.front should be (1, 0, 0),
    //   - cameraComponent.right should be (0, 0, 1),
    //   - cameraComponent.up should be (0, 1, 0).
    // According to your conversion:
    //   pitch = rotation.x - 180 and yaw = rotation.y + 90.
    // So, for pitch=0 and yaw=0 we need:
    //   rotation.x = 180 and rotation.y = -90.
    glm::vec3 rotation(180.0f, -90.0f, 0.0f);
    glm::vec3 front, right, up;
    nexo::math::extractCameraComponents(rotation, front, right, up);

    glm::vec3 expectedFront(1.0f, 0.0f, 0.0f);
    glm::vec3 expectedRight(0.0f, 0.0f, 1.0f);
    glm::vec3 expectedUp(0.0f, 1.0f, 0.0f);

    EXPECT_VEC3_NEAR(front, expectedFront, 0.01f);
    EXPECT_VEC3_NEAR(right, expectedRight, 0.01f);
    EXPECT_VEC3_NEAR(up, expectedUp, 0.01f);
}

TEST_F(ExtractCameraComponentsTest, ArbitraryRotation) {
    // Test with an arbitrary rotation.
    // For instance, let's use rotation = (200, -80, 0).
    // Then, according to your conversion:
    //   pitch = 200 - 180 = 20 degrees
    //   yaw   = -80 + 90 = 10 degrees.
    glm::vec3 rotation(200.0f, -80.0f, 0.0f);
    glm::vec3 front, right, up;
    nexo::math::extractCameraComponents(rotation, front, right, up);

    // Manually compute expected vectors:
    float pitch = 20.0f; // 200 - 180
    float yaw = 10.0f;   // -80 + 90
    glm::vec3 expectedFront;
    expectedFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    expectedFront.y = sin(glm::radians(pitch));
    expectedFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    expectedFront = glm::normalize(expectedFront);

    glm::vec3 expectedRight = glm::normalize(glm::cross(expectedFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 expectedUp = glm::normalize(glm::cross(expectedRight, expectedFront));

    EXPECT_VEC3_NEAR(front, expectedFront, 0.01f);
    EXPECT_VEC3_NEAR(right, expectedRight, 0.01f);
    EXPECT_VEC3_NEAR(up, expectedUp, 0.01f);
}

TEST_F(ExtractCameraComponentsTest, NonZeroRotation) {
    // Another test: use rotation = (210, -100, 0).
    // Then, pitch = 210 - 180 = 30 degrees, and yaw = -100 + 90 = -10 degrees.
    glm::vec3 rotation(210.0f, -100.0f, 0.0f);
    glm::vec3 front, right, up;
    nexo::math::extractCameraComponents(rotation, front, right, up);

    float pitch = 30.0f;
    float yaw = -10.0f;
    glm::vec3 expectedFront;
    expectedFront.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    expectedFront.y = sin(glm::radians(pitch));
    expectedFront.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    expectedFront = glm::normalize(expectedFront);

    glm::vec3 expectedRight = glm::normalize(glm::cross(expectedFront, glm::vec3(0.0f, 1.0f, 0.0f)));
    glm::vec3 expectedUp = glm::normalize(glm::cross(expectedRight, expectedFront));

    EXPECT_VEC3_NEAR(front, expectedFront, 0.01f);
    EXPECT_VEC3_NEAR(right, expectedRight, 0.01f);
    EXPECT_VEC3_NEAR(up, expectedUp, 0.01f);
}

// =============================================================================
// isPosInBounds Tests
// =============================================================================

class IsPosInBoundsTest : public ::testing::Test {};

TEST_F(IsPosInBoundsTest, PointInsideBounds) {
    glm::vec2 pos(5.0f, 5.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOnMinBoundary) {
    glm::vec2 pos(0.0f, 0.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOnMaxBoundary) {
    glm::vec2 pos(10.0f, 10.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOnLeftEdge) {
    glm::vec2 pos(0.0f, 5.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOnRightEdge) {
    glm::vec2 pos(10.0f, 5.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOnTopEdge) {
    glm::vec2 pos(5.0f, 10.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOnBottomEdge) {
    glm::vec2 pos(5.0f, 0.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOutsideLeft) {
    glm::vec2 pos(-1.0f, 5.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_FALSE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOutsideRight) {
    glm::vec2 pos(11.0f, 5.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_FALSE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOutsideTop) {
    glm::vec2 pos(5.0f, 11.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_FALSE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOutsideBottom) {
    glm::vec2 pos(5.0f, -1.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_FALSE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, PointOutsideCorner) {
    glm::vec2 pos(-1.0f, -1.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(10.0f, 10.0f);
    EXPECT_FALSE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, NegativeCoordinateBounds) {
    glm::vec2 pos(-5.0f, -5.0f);
    glm::vec2 min(-10.0f, -10.0f);
    glm::vec2 max(0.0f, 0.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, ZeroSizeBounds) {
    glm::vec2 pos(5.0f, 5.0f);
    glm::vec2 min(5.0f, 5.0f);
    glm::vec2 max(5.0f, 5.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, LargeBounds) {
    glm::vec2 pos(500.0f, 500.0f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(1000.0f, 1000.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

TEST_F(IsPosInBoundsTest, SmallBounds) {
    glm::vec2 pos(0.5f, 0.5f);
    glm::vec2 min(0.0f, 0.0f);
    glm::vec2 max(1.0f, 1.0f);
    EXPECT_TRUE(nexo::math::isPosInBounds(pos, min, max));
}

// =============================================================================
// customQuatToEuler Tests
// =============================================================================
//
// The customQuatToEuler function converts quaternions to Euler angles using
// a specific convention:
// - euler.x = pitch (extracted from q.w * q.y - q.z * q.x)
// - euler.y = yaw (extracted from q.z terms)
// - euler.z = roll (extracted from q.x terms)
//
// Due to this convention, a quaternion rotation around the Y-axis in 3D space
// maps to euler.x (pitch), not euler.y.

class CustomQuatToEulerTest : public ::testing::Test {};

TEST_F(CustomQuatToEulerTest, IdentityQuaternion) {
    glm::quat identity(1.0f, 0.0f, 0.0f, 0.0f);
    glm::vec3 euler = nexo::math::customQuatToEuler(identity);
    EXPECT_NEAR(euler.x, 0.0f, 0.01f);
    EXPECT_NEAR(euler.y, 0.0f, 0.01f);
    EXPECT_NEAR(euler.z, 0.0f, 0.01f);
}

TEST_F(CustomQuatToEulerTest, Rotation90DegreesAroundX) {
    // 90 degrees around X axis
    float angle = glm::radians(90.0f);
    glm::quat q = glm::angleAxis(angle, glm::vec3(1.0f, 0.0f, 0.0f));
    glm::vec3 euler = nexo::math::customQuatToEuler(q);
    // The result should have approximately 90 degrees in some component
    float magnitude = std::abs(euler.x) + std::abs(euler.y) + std::abs(euler.z);
    EXPECT_GT(magnitude, 80.0f);
}

TEST_F(CustomQuatToEulerTest, Rotation90DegreesAroundYAxis) {
    // A quaternion rotation around Y axis (vertical) maps to euler.x (pitch)
    // due to the conversion convention in customQuatToEuler
    float angle = glm::radians(90.0f);
    glm::quat q = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 euler = nexo::math::customQuatToEuler(q);
    // The rotation should produce 90 degrees in euler.x
    EXPECT_NEAR(euler.x, 90.0f, 1.0f);
}

TEST_F(CustomQuatToEulerTest, Rotation90DegreesAroundZAxis) {
    // A quaternion rotation around Z axis
    float angle = glm::radians(90.0f);
    glm::quat q = glm::angleAxis(angle, glm::vec3(0.0f, 0.0f, 1.0f));
    glm::vec3 euler = nexo::math::customQuatToEuler(q);
    // Due to the conversion order, check that the magnitude is correct
    float magnitude = std::abs(euler.x) + std::abs(euler.y) + std::abs(euler.z);
    EXPECT_GT(magnitude, 80.0f);
}

TEST_F(CustomQuatToEulerTest, Rotation45DegreesAroundYAxis) {
    // Y-axis quaternion rotation maps to euler.x
    float angle = glm::radians(45.0f);
    glm::quat q = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 euler = nexo::math::customQuatToEuler(q);
    EXPECT_NEAR(euler.x, 45.0f, 1.0f);
}

TEST_F(CustomQuatToEulerTest, Rotation180DegreesAroundYAxis) {
    // 180 degrees is a special case in quaternion math
    // q = (0, 0, 1, 0) which gives sinp = 0, resulting in euler.x = 0
    // but the rotation is represented through other euler components
    float angle = glm::radians(180.0f);
    glm::quat q = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 euler = nexo::math::customQuatToEuler(q);
    // The result should be valid (no NaN or Inf)
    EXPECT_FALSE(std::isnan(euler.x));
    EXPECT_FALSE(std::isnan(euler.y));
    EXPECT_FALSE(std::isnan(euler.z));
    // Either euler.y or euler.z should have significant rotation
    // representing the 180-degree turn
    float totalMagnitude = std::abs(euler.x) + std::abs(euler.y) + std::abs(euler.z);
    EXPECT_GT(totalMagnitude, 170.0f);
}

TEST_F(CustomQuatToEulerTest, NegativeRotationAroundYAxis) {
    float angle = glm::radians(-45.0f);
    glm::quat q = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 euler = nexo::math::customQuatToEuler(q);
    EXPECT_NEAR(euler.x, -45.0f, 1.0f);
}

TEST_F(CustomQuatToEulerTest, SmallRotationAroundYAxis) {
    float angle = glm::radians(5.0f);
    glm::quat q = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 euler = nexo::math::customQuatToEuler(q);
    EXPECT_NEAR(euler.x, 5.0f, 0.5f);
}

TEST_F(CustomQuatToEulerTest, GimbalLockHandled) {
    // Test gimbal lock case where sinp approaches ±1
    // Create a quaternion that triggers the gimbal lock path
    float angle = glm::radians(90.0f);
    glm::quat q = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 euler = nexo::math::customQuatToEuler(q);
    // Result should still be valid (no NaN or Inf)
    EXPECT_FALSE(std::isnan(euler.x));
    EXPECT_FALSE(std::isnan(euler.y));
    EXPECT_FALSE(std::isnan(euler.z));
    EXPECT_FALSE(std::isinf(euler.x));
    EXPECT_FALSE(std::isinf(euler.y));
    EXPECT_FALSE(std::isinf(euler.z));
}

TEST_F(CustomQuatToEulerTest, CombinedRotation) {
    // Combined rotation around multiple axes
    glm::quat qx = glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    glm::quat qy = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat combined = qy * qx;
    glm::vec3 euler = nexo::math::customQuatToEuler(combined);
    // Result should be valid
    EXPECT_FALSE(std::isnan(euler.x));
    EXPECT_FALSE(std::isnan(euler.y));
    EXPECT_FALSE(std::isnan(euler.z));
}

TEST_F(CustomQuatToEulerTest, NormalizedQuaternion) {
    glm::quat q = glm::normalize(glm::quat(1.0f, 0.5f, 0.3f, 0.2f));
    glm::vec3 euler = nexo::math::customQuatToEuler(q);
    // Result should be valid and in degrees
    EXPECT_FALSE(std::isnan(euler.x));
    EXPECT_FALSE(std::isnan(euler.y));
    EXPECT_FALSE(std::isnan(euler.z));
    // Euler angles should be in reasonable range (-180 to 180)
    EXPECT_GE(euler.x, -180.0f);
    EXPECT_LE(euler.x, 180.0f);
    EXPECT_GE(euler.y, -180.0f);
    EXPECT_LE(euler.y, 180.0f);
    EXPECT_GE(euler.z, -180.0f);
    EXPECT_LE(euler.z, 180.0f);
}

TEST_F(CustomQuatToEulerTest, OutputInDegrees) {
    // Verify output is in degrees, not radians
    // A Y-axis rotation of 45 degrees should produce ~45 in euler.x
    float angle = glm::radians(45.0f);
    glm::quat q = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 euler = nexo::math::customQuatToEuler(q);
    // If output were in radians, euler.x would be ~0.785
    // In degrees, it should be ~45
    EXPECT_GT(euler.x, 40.0f);
}

TEST_F(CustomQuatToEulerTest, ConsistentRoundTrip) {
    // Test that the conversion is internally consistent
    glm::quat q1 = glm::angleAxis(glm::radians(30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat q2 = glm::angleAxis(glm::radians(60.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 euler1 = nexo::math::customQuatToEuler(q1);
    glm::vec3 euler2 = nexo::math::customQuatToEuler(q2);
    // 60 degree rotation should give approximately double the euler angle of 30 degrees
    EXPECT_NEAR(euler2.x, euler1.x * 2.0f, 2.0f);
}
