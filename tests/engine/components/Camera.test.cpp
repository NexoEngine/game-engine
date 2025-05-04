//// Camera.test.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/03/2025
//  Description: Test file for the camera components
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include "renderer/Framebuffer.hpp"
#include "components/Camera.hpp"
#include "components/Transform.hpp"
#include <cmath>

// Dummy implementation of the Framebuffer interface for testing.
class DummyFramebuffer : public nexo::renderer::NxFramebuffer {
public:
    void bind() override {}
    void unbind() override {}
    void setClearColor(const glm::vec4 &) override {}
    unsigned int getFramebufferId() const override { return 0; }
    glm::vec2 getSize() const override { return glm::vec2(0.0f); }
    void resize(unsigned int, unsigned int ) override {}
    void getPixelWrapper(unsigned int, int, int, void *, const std::type_info &) const override {}
    void clearAttachmentWrapper(unsigned int, const void *, const std::type_info &) const override {}
    [[nodiscard]] nexo::renderer::NxFramebufferSpecs &getSpecs() override { static nexo::renderer::NxFramebufferSpecs specs; return specs; }
    [[nodiscard]] const nexo::renderer::NxFramebufferSpecs &getSpecs() const override { static nexo::renderer::NxFramebufferSpecs specs; return specs; }
    [[nodiscard]] unsigned int getColorAttachmentId(unsigned int) const override { return 0; }
    unsigned int getDepthAttachmentId() const override { return 0; }
};

std::shared_ptr<nexo::renderer::NxFramebuffer> createDummyFramebuffer() {
    return std::make_shared<DummyFramebuffer>();
}

class CameraComponentTest : public ::testing::Test {
protected:
    // Helper function to compare two 4x4 matrices elementwise.
    static bool compareMat4(const glm::mat4 &m1, const glm::mat4 &m2, float epsilon = 0.0001f) {
        for (int i = 0; i < 4; ++i)
            for (int j = 0; j < 4; ++j)
                if (std::abs(m1[i][j] - m2[i][j]) > epsilon)
                    return false;
        return true;
    }
};

TEST_F(CameraComponentTest, PerspectiveProjectionMatrix) {
    nexo::components::CameraComponent cam;
    cam.width = 800;
    cam.height = 600;
    cam.fov = 45.0f;
    cam.nearPlane = 0.1f;
    cam.farPlane = 1000.0f;
    cam.type = nexo::components::CameraType::PERSPECTIVE;

    glm::mat4 proj = cam.getProjectionMatrix();
    glm::mat4 expected = glm::perspective(glm::radians(45.0f), 800.0f/600.0f, 0.1f, 1000.0f);

    EXPECT_TRUE(compareMat4(proj, expected));
}

TEST_F(CameraComponentTest, OrthographicProjectionMatrix) {
    nexo::components::CameraComponent cam;
    cam.width = 800;
    cam.height = 600;
    cam.type = nexo::components::CameraType::ORTHOGRAPHIC;

    glm::mat4 proj = cam.getProjectionMatrix();
    glm::mat4 expected = glm::ortho(0.0f, 800.0f, 600.0f, 0.0f, cam.nearPlane, cam.farPlane);

    EXPECT_TRUE(compareMat4(proj, expected));
}

TEST_F(CameraComponentTest, ViewMatrixCalculation) {
    nexo::components::CameraComponent cam;
    // Create a dummy transform for the camera.
    nexo::components::TransformComponent transform;
    transform.pos = glm::vec3(0.0f, 0.0f, 5.0f);
    transform.quat = glm::quat(glm::vec3(0.0f)); // Identity rotation

    glm::mat4 view = cam.getViewMatrix(transform);
    glm::mat4 expected = glm::lookAt(transform.pos, transform.pos + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

    EXPECT_TRUE(compareMat4(view, expected));
}

TEST_F(CameraComponentTest, ResizeViewportAndRenderTarget) {
    nexo::components::CameraComponent cam;
    cam.width = 640;
    cam.height = 480;
    // Set a dummy framebuffer.
    cam.m_renderTarget = createDummyFramebuffer();

    // Call resize to update the camera's viewport.
    cam.resize(1024, 768);
    EXPECT_EQ(cam.width, 1024u);
    EXPECT_EQ(cam.height, 768u);
    EXPECT_TRUE(cam.resizing);
}

TEST_F(CameraComponentTest, GetViewMatrixForOrthographicCamera) {
    nexo::components::CameraComponent cam;
    cam.width = 800;
    cam.height = 600;
    cam.type = nexo::components::CameraType::ORTHOGRAPHIC;

    // Create a dummy transform for the camera.
    nexo::components::TransformComponent transform;
    transform.pos = glm::vec3(100.0f, 50.0f, 0.0f);
    transform.quat = glm::quat(glm::vec3(0.0f)); // Identity rotation

    glm::mat4 view = cam.getViewMatrix(transform);
    glm::mat4 expected = glm::lookAt(transform.pos, transform.pos + glm::vec3(0, 0, -1), glm::vec3(0, 1, 0));

    EXPECT_TRUE(compareMat4(view, expected));
}
