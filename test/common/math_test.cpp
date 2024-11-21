//// math_test.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        21/11/2024
//  Description: NEXO common math test file
//
///////////////////////////////////////////////////////////////////////////////

#include <glm/ext/matrix_transform.hpp>
#include <gtest/gtest.h>

#include "math/Matrix.hpp"

// Helper function to compare two glm::vec3 values
bool compareVec3(const glm::vec3 &v1, const glm::vec3 &v2, float epsilon = 0.0001f) {
    return glm::length(v1 - v2) < epsilon;
}

TEST(DecomposeTransformEulerTest, IdentityMatrix) {
    auto identity = glm::mat4(1.0f);
    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(identity, translation, rotation, scale);

    EXPECT_TRUE(compareVec3(translation, glm::vec3(0.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(rotation, glm::vec3(0.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(scale, glm::vec3(1.0f, 1.0f, 1.0f)));
}

TEST(DecomposeTransformEulerTest, TranslationOnly) {
    const glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -3.0f, 2.0f));
    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(transform, translation, rotation, scale);

    EXPECT_TRUE(compareVec3(translation, glm::vec3(5.0f, -3.0f, 2.0f)));
    EXPECT_TRUE(compareVec3(rotation, glm::vec3(0.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(scale, glm::vec3(1.0f, 1.0f, 1.0f)));
}

TEST(DecomposeTransformEulerTest, ScaleOnly) {
    const glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 4.0f));
    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(transform, translation, rotation, scale);

    EXPECT_TRUE(compareVec3(translation, glm::vec3(0.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(rotation, glm::vec3(0.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(scale, glm::vec3(2.0f, 3.0f, 4.0f)));
}

TEST(DecomposeTransformEulerTest, RotationOnly) {
    const glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(transform, translation, rotation, scale);

    EXPECT_TRUE(compareVec3(translation, glm::vec3(0.0f, 0.0f, 0.0f)));
    EXPECT_NEAR(rotation.y, glm::radians(45.0f), 0.0001f); // Y-axis rotation
    EXPECT_TRUE(compareVec3(scale, glm::vec3(1.0f, 1.0f, 1.0f)));
}

TEST(DecomposeTransformEulerTest, TranslationRotationScale) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -5.0f, 3.0f));
    transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::scale(transform, glm::vec3(2.0f, 3.0f, 4.0f));

    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(transform, translation, rotation, scale);

    EXPECT_TRUE(compareVec3(translation, glm::vec3(10.0f, -5.0f, 3.0f)));
    EXPECT_NEAR(rotation.x, glm::radians(30.0f), 0.0001f); // X-axis rotation
    EXPECT_TRUE(compareVec3(scale, glm::vec3(2.0f, 3.0f, 4.0f)));
}

TEST(DecomposeTransformEulerTest, NegativeScale) {
    glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 2.0f, 3.0f));
    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(transform, translation, rotation, scale);

    EXPECT_TRUE(compareVec3(translation, glm::vec3(0.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(rotation, glm::vec3(0.0f, 0.0f, 0.0f)));
    EXPECT_TRUE(compareVec3(scale, glm::vec3(-1.0f, 2.0f, 3.0f)));
}
