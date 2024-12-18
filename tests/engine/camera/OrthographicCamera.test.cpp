//// OrthographicCamera.test.cpp //////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/11/2024
//  Description: Test file for the orthographic camera
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "core/camera/OrthographicCamera.hpp"
#include "../../utils/comparison.hpp"

namespace nexo::camera {
    class OrthographicCameraTest : public ::testing::Test {
        protected:
            OrthographicCameraTest() = default;

            ~OrthographicCameraTest() override = default;

            float left = -10.0f;
            float right = 10.0f;
            float bottom = -5.0f;
            float top = 5.0f;
            OrthographicCamera camera = OrthographicCamera(left, right, bottom, top);
    };

    TEST_F(OrthographicCameraTest, InitialProjectionMatrix)
    {
        glm::mat4 expectedProjectionMatrix = glm::ortho(left, right, bottom, top);
        EXPECT_MAT4_NEAR(camera.getProjectionMatrix(), expectedProjectionMatrix, 0.01f);
    }

    TEST_F(OrthographicCameraTest, InitialViewMatrix)
    {
        glm::mat4 expectedViewMatrix = glm::mat4(1.0f);
        EXPECT_MAT4_NEAR(camera.getViewMatrix(), expectedViewMatrix, 0.1f);
    }

    TEST_F(OrthographicCameraTest, InitialViewProjectionMatrix)
    {
        glm::mat4 expectedViewProjectionMatrix = glm::ortho(left, right, bottom, top) * glm::mat4(1.0f);
        EXPECT_MAT4_NEAR(camera.getViewProjectionMatrix(), expectedViewProjectionMatrix, 0.01f);
    }

    TEST_F(OrthographicCameraTest, SetProjection)
    {
        float newLeft = -20.0f;
        float newRight = 20.0f;
        float newBottom = -10.0f;
        float newTop = 10.0f;

        camera.setProjection(newLeft, newRight, newBottom, newTop);

        glm::mat4 expectedProjectionMatrix = glm::ortho(newLeft, newRight, newBottom, newTop);
        EXPECT_MAT4_NEAR(camera.getProjectionMatrix(), expectedProjectionMatrix, 0.01f);
    }

    TEST_F(OrthographicCameraTest, SetPosition)
    {
        glm::vec3 newPosition = {5.0f, 5.0f, 0.0f};
        camera.setPosition(newPosition);

        glm::mat4 expectedViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), newPosition));
        EXPECT_MAT4_NEAR(camera.getViewMatrix(), expectedViewMatrix, 0.01f);

        glm::mat4 expectedViewProjectionMatrix = camera.getProjectionMatrix() * expectedViewMatrix;
        EXPECT_MAT4_NEAR(camera.getViewProjectionMatrix(), expectedViewProjectionMatrix, 0.01f);
    }

    TEST_F(OrthographicCameraTest, SetRotation)
    {
        glm::vec3 rotation = {0.0f, 0.0f, 45.0f}; // Rotate 45 degrees around Z-axis
        camera.setRotation(rotation);

        glm::mat4 rotationMatrix = glm::toMat4(glm::quat(glm::radians(rotation)));
        glm::mat4 expectedViewMatrix = glm::inverse(
            rotationMatrix * glm::translate(glm::mat4(1.0f), camera.getPosition()));
        EXPECT_MAT4_NEAR(camera.getViewMatrix(), expectedViewMatrix, 0.01f);

        glm::mat4 expectedViewProjectionMatrix = camera.getProjectionMatrix() * expectedViewMatrix;
        EXPECT_MAT4_NEAR(camera.getViewProjectionMatrix(), expectedViewProjectionMatrix, 0.01f);
    }

    TEST_F(OrthographicCameraTest, MoveCamera)
    {
        glm::vec3 initialPosition = camera.getPosition();
        glm::vec3 deltaPosition = {2.0f, -3.0f, 0.0f};

        glm::vec3 newPosition = initialPosition + deltaPosition;
        camera.setPosition(newPosition);

        EXPECT_VEC3_NEAR(camera.getPosition(), newPosition, 0.01f);

        const glm::mat4 rotationMatrix = glm::toMat4(glm::quat({1.0f, 0.0f, 0.0f, 0.0f}));
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), newPosition) *
                                    rotationMatrix;

        glm::mat4 expectedViewMatrix = glm::inverse(transform);
        EXPECT_MAT4_NEAR(camera.getViewMatrix(), expectedViewMatrix, 0.01f);
    }

    TEST_F(OrthographicCameraTest, RotateCamera)
    {
        glm::quat defaultQuat = {1.0f, 0.0f, 0.0f, 0.0f};
        glm::vec3 deltaRotation = {0.0f, 0.0f, 20.0f}; // Rotate 20 degrees around Z-axis
        camera.rotate(deltaRotation);

        const auto deltaQuat = glm::quat(glm::radians(deltaRotation));
        glm::quat newRotation = glm::normalize(deltaQuat * defaultQuat);
        glm::vec3 expectedRotation = glm::degrees(glm::eulerAngles(newRotation));
        EXPECT_VEC3_NEAR(camera.getRotation(), expectedRotation, 0.01f);
    }

    TEST_F(OrthographicCameraTest, ModeIsOrthographic)
    {
        EXPECT_EQ(camera.getMode(), CameraMode::ORTHOGRAPHIC);
    }
}
