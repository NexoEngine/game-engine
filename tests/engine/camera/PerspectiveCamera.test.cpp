//// PerspectiveCamera.test.cpp ///////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/11/2024
//  Description: Test file for the perspective camera
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

#include "core/camera/PerspectiveCamera.hpp"
#include "../../utils/comparison.hpp"

namespace nexo::camera {
    class PerspectiveCameraTest : public ::testing::Test {
    protected:
        PerspectiveCameraTest()
            : camera(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f) {}

        ~PerspectiveCameraTest() override = default;

        PerspectiveCamera camera;
    };

    TEST_F(PerspectiveCameraTest, InitialProjectionMatrix)
    {
        glm::mat4 expectedProjectionMatrix = glm::perspective(45.0f, 16.0f / 9.0f, 0.1f, 1000.0f);
        EXPECT_MAT4_NEAR(camera.getProjectionMatrix(), expectedProjectionMatrix, 0.01f);
    }

    TEST_F(PerspectiveCameraTest, InitialViewMatrix)
    {
        glm::mat4 expectedViewMatrix = glm::mat4(1.0f);
        EXPECT_MAT4_NEAR(camera.getViewMatrix(), expectedViewMatrix, 0.01f);
    }

    TEST_F(PerspectiveCameraTest, InitialViewProjectionMatrix)
    {
        glm::mat4 expectedViewProjectionMatrix = camera.getProjectionMatrix() * glm::mat4(1.0f);
        EXPECT_MAT4_NEAR(camera.getViewProjectionMatrix(), expectedViewProjectionMatrix, 0.01f);
    }

    TEST_F(PerspectiveCameraTest, SetProjection)
    {
        camera.setProjection(glm::radians(60.0f), 4.0f / 3.0f, 0.5f, 500.0f);

        glm::mat4 expectedProjectionMatrix = glm::perspective(glm::radians(60.0f), 4.0f / 3.0f, 0.5f, 500.0f);
        EXPECT_MAT4_NEAR(camera.getProjectionMatrix(), expectedProjectionMatrix, 0.01f);
    }

    TEST_F(PerspectiveCameraTest, SetPosition)
    {
        glm::vec3 newPosition = {10.0f, 5.0f, -20.0f};
        camera.setPosition(newPosition);

        glm::mat4 expectedViewMatrix = glm::inverse(glm::translate(glm::mat4(1.0f), newPosition));
        EXPECT_MAT4_NEAR(camera.getViewMatrix(), expectedViewMatrix, 0.01f);

        glm::mat4 expectedViewProjectionMatrix = camera.getProjectionMatrix() * expectedViewMatrix;
        EXPECT_MAT4_NEAR(camera.getViewProjectionMatrix(), expectedViewProjectionMatrix, 0.01f);
    }

    TEST_F(PerspectiveCameraTest, SetRotation)
    {
        glm::vec3 rotation = {0.0f, 45.0f, 0.0f}; // Rotate 45 degrees around Y-axis
        camera.setRotation(rotation);

        glm::mat4 rotationMatrix = glm::toMat4(glm::quat(glm::radians(rotation)));
        glm::mat4 expectedViewMatrix = glm::inverse(
            rotationMatrix * glm::translate(glm::mat4(1.0f), camera.getPosition()));
        EXPECT_MAT4_NEAR(camera.getViewMatrix(), expectedViewMatrix, 0.01f);

        glm::mat4 expectedViewProjectionMatrix = camera.getProjectionMatrix() * expectedViewMatrix;
        EXPECT_MAT4_NEAR(camera.getViewProjectionMatrix(), expectedViewProjectionMatrix, 0.01f);
    }

    TEST_F(PerspectiveCameraTest, UpdateCamera)
    {
        glm::vec3 newPosition = {5.0f, 2.0f, -15.0f};
        float yaw = 90.0f;  // Rotate 90 degrees around Y-axis
        float pitch = 45.0f; // Rotate 45 degrees upward

        camera.update(newPosition, yaw, pitch);

        glm::vec3 front = glm::normalize(glm::vec3(
            cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
            sin(glm::radians(pitch)),
            sin(glm::radians(yaw)) * cos(glm::radians(pitch))
        ));
        glm::vec3 right = glm::normalize(glm::cross(front, glm::vec3(0.0f, 1.0f, 0.0f)));
        glm::vec3 up = glm::normalize(glm::cross(right, front));

        glm::mat4 expectedViewMatrix = glm::lookAt(newPosition, newPosition + front, up);
        EXPECT_MAT4_NEAR(camera.getViewMatrix(), expectedViewMatrix, 0.01f);

        glm::mat4 expectedViewProjectionMatrix = camera.getProjectionMatrix() * expectedViewMatrix;
        EXPECT_MAT4_NEAR(camera.getViewProjectionMatrix(), expectedViewProjectionMatrix, 0.01f);
    }

    TEST_F(PerspectiveCameraTest, SetAspectRatio)
    {
        float newAspectRatio = 1.0f;
        camera.setAspectRatio(newAspectRatio);

        glm::mat4 expectedProjectionMatrix = glm::perspective(45.0f, newAspectRatio, 0.1f, 1000.0f);
        EXPECT_MAT4_NEAR(camera.getProjectionMatrix(), expectedProjectionMatrix, 0.01f);
    }

    TEST_F(PerspectiveCameraTest, ModeIsPerspective)
    {
        EXPECT_EQ(camera.getMode(), CameraMode::PERSPECTIVE);
    }
}