//// Matrix.test.cpp //////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        21/11/2024
//  Description: NEXO common math test file
//
///////////////////////////////////////////////////////////////////////////////

#include <glm/ext/matrix_transform.hpp>
#include <gtest/gtest.h>
#include <glm/gtc/quaternion.hpp>

#include "math/Matrix.hpp"
#include "../utils/comparison.hpp"


TEST(DecomposeTransformEulerTest, IdentityMatrix) {
    auto identity = glm::mat4(1.0f);
    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(identity, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_VEC3_NEAR(rotation, glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_VEC3_NEAR(scale, glm::vec3(1.0f, 1.0f, 1.0f), 0.0001f);
}

TEST(DecomposeTransformEulerTest, TranslationOnly) {
    const glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -3.0f, 2.0f));
    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(transform, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(5.0f, -3.0f, 2.0f), 0.0001f);
    EXPECT_VEC3_NEAR(rotation, glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_VEC3_NEAR(scale, glm::vec3(1.0f, 1.0f, 1.0f), 0.0001f);
}

TEST(DecomposeTransformEulerTest, ScaleOnly) {
    const glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 4.0f));
    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(transform, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_VEC3_NEAR(rotation, glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_VEC3_NEAR(scale, glm::vec3(2.0f, 3.0f, 4.0f), 0.0001f);
}

TEST(DecomposeTransformEulerTest, RotationOnly) {
    const glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(transform, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_NEAR(rotation.y, glm::radians(45.0f), 0.0001f); // Y-axis rotation
    EXPECT_VEC3_NEAR(scale, glm::vec3(1.0f, 1.0f, 1.0f), 0.0001f);
}

TEST(DecomposeTransformEulerTest, TranslationRotationScale) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -5.0f, 3.0f));
    transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::scale(transform, glm::vec3(2.0f, 3.0f, 4.0f));

    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(transform, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(10.0f, -5.0f, 3.0f), 0.0001f);
    EXPECT_NEAR(rotation.x, glm::radians(30.0f), 0.0001f); // X-axis rotation
    EXPECT_VEC3_NEAR(scale, glm::vec3(2.0f, 3.0f, 4.0f), 0.0001f);
}

TEST(DecomposeTransformEulerTest, NegativeScale) {
    glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(-1.0f, 2.0f, 3.0f));
    glm::vec3 translation, rotation, scale;

    nexo::math::decomposeTransformEuler(transform, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_VEC3_NEAR(rotation, glm::vec3(0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_VEC3_NEAR(scale, glm::vec3(-1.0f, 2.0f, 3.0f), 0.0001f);
}

TEST(DecomposeTransformQuatTest, IdentityMatrix) {
    auto identity = glm::mat4(1.0f);
    glm::vec3 translation, scale;
    glm::quat rotation;

    nexo::math::decomposeTransformQuat(identity, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(0.0f), 0.0001f);
    // Identity rotation is represented by a unit quaternion with w == 1.
    EXPECT_QUAT_NEAR(rotation, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_VEC3_NEAR(scale, glm::vec3(1.0f), 0.0001f);
}

TEST(DecomposeTransformQuatTest, TranslationOnly) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(5.0f, -3.0f, 2.0f));
    glm::vec3 translation, scale;
    glm::quat rotation;

    nexo::math::decomposeTransformQuat(transform, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(5.0f, -3.0f, 2.0f), 0.0001f);
    EXPECT_QUAT_NEAR(rotation, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_VEC3_NEAR(scale, glm::vec3(1.0f), 0.0001f);
}

TEST(DecomposeTransformQuatTest, ScaleOnly) {
    glm::mat4 transform = glm::scale(glm::mat4(1.0f), glm::vec3(2.0f, 3.0f, 4.0f));
    glm::vec3 translation, scale;
    glm::quat rotation;

    nexo::math::decomposeTransformQuat(transform, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(0.0f), 0.0001f);
    EXPECT_QUAT_NEAR(rotation, glm::quat(1.0f, 0.0f, 0.0f, 0.0f), 0.0001f);
    EXPECT_VEC3_NEAR(scale, glm::vec3(2.0f, 3.0f, 4.0f), 0.0001f);
}

TEST(DecomposeTransformQuatTest, RotationOnly) {
    glm::mat4 transform = glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    glm::vec3 translation, scale;
    glm::quat rotation;

    nexo::math::decomposeTransformQuat(transform, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(0.0f), 0.0001f);
    // The expected rotation is around the Y-axis by 45 degrees.
    glm::quat expectedRotation = glm::angleAxis(glm::radians(45.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    EXPECT_QUAT_NEAR(rotation, expectedRotation, 0.0001f);
    EXPECT_VEC3_NEAR(scale, glm::vec3(1.0f), 0.0001f);
}

TEST(DecomposeTransformQuatTest, CombinedTransform) {
    glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(10.0f, -5.0f, 3.0f));
    transform = glm::rotate(transform, glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    transform = glm::scale(transform, glm::vec3(2.0f, 3.0f, 4.0f));
    glm::vec3 translation, scale;
    glm::quat rotation;

    nexo::math::decomposeTransformQuat(transform, translation, rotation, scale);

    EXPECT_VEC3_NEAR(translation, glm::vec3(10.0f, -5.0f, 3.0f), 0.0001f);
    // Expect a 30 degree rotation about the x-axis.
    glm::quat expectedRotation = glm::angleAxis(glm::radians(30.0f), glm::vec3(1.0f, 0.0f, 0.0f));
    EXPECT_QUAT_NEAR(rotation, expectedRotation, 0.0001f);
    EXPECT_VEC3_NEAR(scale, glm::vec3(2.0f, 3.0f, 4.0f), 0.0001f);
}
