//// Vector.test.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
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
