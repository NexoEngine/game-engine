//// Renderer3D.test.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/11/2024
//  Description: Test file for the renderer 3D
//
///////////////////////////////////////////////////////////////////////////////
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glm/glm.hpp>
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
                renderer3D = std::make_unique<Renderer3D>();
                Renderer::init();
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

            std::unique_ptr<Renderer3D> renderer3D;
    };

    TEST_F(Renderer3DTest, BeginEndScene)
    {
        glm::mat4 viewProjection = glm::mat4(1.0f);
        glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};

        // Validate basic begin and end scene
        EXPECT_NO_THROW(renderer3D->beginScene(viewProjection, cameraPosition));
        EXPECT_NO_THROW(renderer3D->endScene());
    }

    TEST_F(Renderer3DTest, DrawCubeWithoutTexture)
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 size = {1.0f, 1.0f, 1.0f};
        glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color

        GLuint query;
        glGenQueries(1, &query);
        glBeginQuery(GL_PRIMITIVES_GENERATED, query);
        renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});

        //EXPECT_NO_THROW(renderer3D->drawCube(position, size, color));

        renderer3D->endScene();
        // Validate number of primitives drawn
        glEndQuery(GL_PRIMITIVES_GENERATED);
        GLuint primitivesGenerated = 0;
        glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
        EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles

        glDeleteQueries(1, &query);

        // Validate vertex buffer data
        GLuint vertexBufferId = renderer3D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        std::vector<Vertex> vertexData(8); // Expecting 8 vertices
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(Vertex), vertexData.data());
        // Expected vertex positions for a unit cube at origin
        glm::vec3 expectedPositions[8] = {
            {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
        };
        // Validate vertex positions and colors
        for (int i = 0; i < 8; ++i)
        {
            EXPECT_VEC3_NEAR(vertexData[i].position, expectedPositions[i], 0.01f);
            //EXPECT_VEC4_NEAR(vertexData[i].color, color, 0.01f);
            //EXPECT_EQ(vertexData[i].texIndex, 0.0f);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Validate index buffer content
        GLuint indexBufferId = renderer3D->getInternalStorage()->indexBuffer->getId();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        std::vector<unsigned int> indexData(36); // Expecting 36 indices
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 36 * sizeof(unsigned int), indexData.data());
        // Expected indices for a unit cube
        unsigned int expectedIndices[36] = {
            0, 1, 2, 2, 3, 0, // Front face
            4, 5, 6, 6, 7, 4, // Back face
            0, 1, 5, 5, 4, 0, // Bottom face
            3, 2, 6, 6, 7, 3, // Top face
            0, 3, 7, 7, 4, 0, // Left face
            1, 2, 6, 6, 5, 1 // Right face
        };
        for (int i = 0; i < 36; ++i)
        {
            EXPECT_EQ(indexData[i], expectedIndices[i]);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Validate render stats
        Renderer3DStats stats = renderer3D->getStats();
        EXPECT_EQ(stats.cubeCount, 1);
        EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices
        EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
    }

    TEST_F(Renderer3DTest, DrawCubeWithTexture)
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 size = {1.0f, 1.0f, 1.0f};
        auto texture = Texture2D::create(4, 4); // Example texture

        GLuint query;
        glGenQueries(1, &query);
        glBeginQuery(GL_PRIMITIVES_GENERATED, query);
        renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        //EXPECT_NO_THROW(renderer3D->drawCube(position, size, texture));
        renderer3D->endScene();

        // Validate number of primitives drawn
        glEndQuery(GL_PRIMITIVES_GENERATED);
        GLuint primitivesGenerated = 0;
        glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
        EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
        glDeleteQueries(1, &query);

        // Validate vertex buffer data
        GLuint vertexBufferId = renderer3D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        std::vector<Vertex> vertexData(8); // Expecting 8 vertices
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(Vertex), vertexData.data());
        // Expected vertex positions for a unit cube at origin
        glm::vec3 expectedPositions[8] = {
            {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
        };
        // Expected texture coordinates
        glm::vec2 expectedTexCoords[4] = {
            {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 1.0f}, {0.0f, 1.0f}
        };
        // Validate vertex positions and colors
        for (int i = 0; i < 8; ++i)
        {
            EXPECT_VEC3_NEAR(vertexData[i].position, expectedPositions[i], 0.01f);
            EXPECT_VEC2_NEAR(vertexData[i].texCoord, expectedTexCoords[i % 4], 0.01f);
            //EXPECT_EQ(vertexData[i].texIndex, 1.0f);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Validate index buffer content
        GLuint indexBufferId = renderer3D->getInternalStorage()->indexBuffer->getId();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        std::vector<unsigned int> indexData(36); // Expecting 36 indices
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 36 * sizeof(unsigned int), indexData.data());
        // Expected indices for a unit cube
        unsigned int expectedIndices[36] = {
            0, 1, 2, 2, 3, 0, // Front face
            4, 5, 6, 6, 7, 4, // Back face
            0, 1, 5, 5, 4, 0, // Bottom face
            3, 2, 6, 6, 7, 3, // Top face
            0, 3, 7, 7, 4, 0, // Left face
            1, 2, 6, 6, 5, 1 // Right face
        };
        for (int i = 0; i < 36; ++i)
        {
            EXPECT_EQ(indexData[i], expectedIndices[i]);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

        // Validate render stats
        Renderer3DStats stats = renderer3D->getStats();
        EXPECT_EQ(stats.cubeCount, 1);
        EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices
        EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
    }

    TEST_F(Renderer3DTest, DrawMesh)
    {

    }

    TEST_F(Renderer3DTest, ResetAndRetrieveStats)
    {
        renderer3D->resetStats();

        Renderer3DStats stats = renderer3D->getStats();
        EXPECT_EQ(stats.drawCalls, 0);
        EXPECT_EQ(stats.cubeCount, 0);
    }

    TEST_F(Renderer3DTest, BeginSceneWithoutInit)
    {
        renderer3D->shutdown();

        glm::mat4 viewProjection = glm::mat4(1.0f);
        glm::vec3 cameraPosition = {0.0f, 0.0f, 0.0f};

        EXPECT_THROW(renderer3D->beginScene(viewProjection, cameraPosition), RendererNotInitialized);
        // Re-init for TearDown function
        renderer3D->init();
    }

    TEST_F(Renderer3DTest, DrawCubeWithoutBeginScene)
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 size = {1.0f, 1.0f, 1.0f};
        glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

        //EXPECT_THROW(renderer3D->drawCube(position, size, color), RendererSceneLifeCycleFailure);
    }

    TEST_F(Renderer3DTest, ResetStatsWithoutInit) {
        // Manually delete the storage to simulate an uninitialized renderer
        renderer3D->shutdown();

        // Expect RendererNotInitialized exception
        EXPECT_THROW(renderer3D->resetStats(), RendererNotInitialized);
        // Re-init for TearDown function
        renderer3D->init();
    }

    TEST_F(Renderer3DTest, GetStatsWithoutInit) {
        // Manually delete the storage to simulate an uninitialized renderer
        renderer3D->shutdown();

        // Expect RendererNotInitialized exception
        EXPECT_THROW(renderer3D->getStats(), RendererNotInitialized);
        // Re-init for TearDown function
        renderer3D->init();
    }
}
