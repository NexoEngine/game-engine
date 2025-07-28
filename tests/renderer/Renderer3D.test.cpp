///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Test file for the renderer 3D
//
///////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "renderer/Renderer3D.hpp"
#include "renderer/Texture.hpp"
#include "renderer/RendererExceptions.hpp"
#include "renderer/Renderer.hpp"
#include "contexts/opengl.hpp"
#include "../utils/comparison.hpp"

namespace nexo::renderer {
    class Renderer3DTest : public ::testing::Test {
        GLFWwindow *window = nullptr;

        protected:
            void SetUp() override
            {
                if (!glfwInit())
                {
                    GTEST_SKIP() << "GLFW initialization failed. Skipping OpenGL tests.";
                }

                glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
                glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
                glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

                window = glfwCreateWindow(800, 600, "Test Window", nullptr, nullptr);
                if (!window)
                {
                    glfwTerminate();
                    GTEST_SKIP() << "Failed to create GLFW window. Skipping OpenGL tests.";
                }

                glfwMakeContextCurrent(window);

                if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
                {
                    glfwDestroyWindow(window);
                    glfwTerminate();
                    GTEST_SKIP() << "Failed to initialize GLAD. Skipping OpenGL tests.";
                }

                GLint major = 0, minor = 0;
                glGetIntegerv(GL_MAJOR_VERSION, &major);
                glGetIntegerv(GL_MINOR_VERSION, &minor);
                if (major < 4 || (major == 4 && minor < 5))
                {
                    glfwDestroyWindow(window);
                    glfwTerminate();
                    GTEST_SKIP() << "OpenGL 4.5 is required. Skipping OpenGL tests.";
                }
                renderer3D = std::make_unique<NxRenderer3D>();
                NxRenderer::init();
                EXPECT_NO_THROW(renderer3D->init());
            }

            void TearDown() override
            {
                EXPECT_NO_THROW(renderer3D->shutdown());
                if (window)
                {
                    glfwDestroyWindow(window);
                }
            }

            std::unique_ptr<NxRenderer3D> renderer3D;
    };

    TEST_F(Renderer3DTest, BeginEndScene)
    {
        glm::mat4 viewProjection = glm::mat4(1.0f);
        glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};

        // Validate basic begin and end scene
        EXPECT_NO_THROW(renderer3D->beginScene(viewProjection, cameraPosition));
        EXPECT_NO_THROW(renderer3D->endScene());
    }

	TEST_F(Renderer3DTest, BeginSceneWithoutInit)
	{
	    renderer3D->shutdown();

	    glm::mat4 viewProjection = glm::mat4(1.0f);
	    glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};

	    EXPECT_THROW(renderer3D->beginScene(viewProjection, cameraPosition), NxRendererNotInitialized);
	    // Re-init for TearDown function
	    renderer3D->init();
	}

}
