//// Renderer2D.test.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/11/2024
//  Description: Test file for the renderer 2D
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glm/glm.hpp>
#include "renderer/Renderer2D.hpp"
#include "renderer/Texture.hpp"
#include "renderer/SubTexture2D.hpp"
#include "renderer/RendererExceptions.hpp"
#include "renderer/Renderer.hpp"
#include "contexts/opengl.hpp"

namespace nexo::renderer {
    class Renderer2DTest : public ::testing::Test {
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
                renderer2D = std::make_unique<Renderer2D>();
                Renderer::init();
                renderer2D->init();
            }

            void TearDown() override
            {
                renderer2D->shutdown();
                if (window)
                {
                    glfwDestroyWindow(window);
                }
            }

            std::unique_ptr<Renderer2D> renderer2D;
    };

    TEST_F(Renderer2DTest, BeginEndScene)
    {
        glm::mat4 viewProjection = glm::mat4(1.0f);

        EXPECT_NO_THROW(renderer2D->beginScene(viewProjection));
        EXPECT_NO_THROW(renderer2D->endScene());
    }

    TEST_F(Renderer2DTest, DrawQuadWithoutTexture)
    {
        glm::vec2 position = {0.0f, 0.0f};
        glm::vec2 size = {1.0f, 1.0f};
        glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color

        renderer2D->beginScene(glm::mat4(1.0f));

        // Draw quad without texture
        EXPECT_NO_THROW(renderer2D->drawQuad(position, size, color));

        // Validate number of primitives drawn
        GLuint query;
        glGenQueries(1, &query);
        glBeginQuery(GL_PRIMITIVES_GENERATED, query);
        renderer2D->endScene();
        glEndQuery(GL_PRIMITIVES_GENERATED);
        GLuint primitivesRendered = 0;
        glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesRendered);
        EXPECT_EQ(primitivesRendered, 2); // 2 triangles
        glDeleteQueries(1, &query);

        // Validate vertex and index buffers content
        GLuint vertexBufferId = renderer2D->getInternalStorage()->vertexBuffer->getId();
        GLuint indexBufferId = renderer2D->getInternalStorage()->indexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        std::vector<QuadVertex> vertexData(4); // Expecting 4 vertices
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(QuadVertex), vertexData.data());

        // Validate vertex positions
        EXPECT_EQ(vertexData[0].position, glm::vec3(-0.5f, -0.5f, 0.0f)); // Bottom-left
        EXPECT_EQ(vertexData[1].position, glm::vec3(0.5f, -0.5f, 0.0f)); // Bottom-right
        EXPECT_EQ(vertexData[2].position, glm::vec3(0.5f, 0.5f, 0.0f)); // Top-right
        EXPECT_EQ(vertexData[3].position, glm::vec3(-0.5f, 0.5f, 0.0f)); // Top-left
        // Validate vertex colors
        for (int i = 0; i < 4; ++i)
        {
            EXPECT_EQ(vertexData[i].color, color);
        }

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        std::vector<unsigned int> indexData(6); // Expecting 6 indices
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6 * sizeof(unsigned int), indexData.data());
        // Validate indices
        EXPECT_EQ(indexData[0], 0); // First triangle
        EXPECT_EQ(indexData[1], 1);
        EXPECT_EQ(indexData[2], 2);
        EXPECT_EQ(indexData[3], 2); // Second triangle
        EXPECT_EQ(indexData[4], 3);
        EXPECT_EQ(indexData[5], 0);

        // Validate stats
        RendererStats stats = renderer2D->getStats();
        EXPECT_EQ(stats.quadCount, 1);
        EXPECT_EQ(stats.getTotalVertexCount(), 4); // 1 quad * 4 vertices
        EXPECT_EQ(stats.getTotalIndexCount(), 6); // 1 quad * 6 indices
    }

    TEST_F(Renderer2DTest, DrawQuadWithTexture)
    {
        glm::vec2 position = {0.0f, 0.0f};
        glm::vec2 size = {1.0f, 1.0f};
        auto texture = Texture2D::create(2, 2); // Create a simple 2x2 texture
        glm::vec4 expectedColor = {1.0f, 1.0f, 1.0f, 1.0f}; // White color for textured quads

        renderer2D->beginScene(glm::mat4(1.0f));

        // Draw quad with texture
        EXPECT_NO_THROW(renderer2D->drawQuad(position, size, texture));
        renderer2D->endScene();

        // Validate vertex buffer content
        GLuint vertexBufferId = renderer2D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

        // Read back vertex data
        std::vector<QuadVertex> vertexData(4); // Expecting 4 vertices for the quad
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(QuadVertex), vertexData.data());

        // Expected vertex positions (untransformed quad)
        glm::vec3 expectedPositions[] = {
            {-0.5f, -0.5f, 0.0f}, // Bottom-left
            {0.5f, -0.5f, 0.0f}, // Bottom-right
            {0.5f, 0.5f, 0.0f}, // Top-right
            {-0.5f, 0.5f, 0.0f} // Top-left
        };

        // Expected texture coordinates
        glm::vec2 expectedTexCoords[] = {
            {0.0f, 0.0f}, // Bottom-left
            {1.0f, 0.0f}, // Bottom-right
            {1.0f, 1.0f}, // Top-right
            {0.0f, 1.0f} // Top-left
        };

        // Validate each vertex
        for (int i = 0; i < 4; ++i)
        {
            EXPECT_EQ(vertexData[i].position, expectedPositions[i]);
            EXPECT_EQ(vertexData[i].texCoord, expectedTexCoords[i]);
            EXPECT_EQ(vertexData[i].color, expectedColor); // Default white color for textures
            EXPECT_EQ(vertexData[i].texIndex, 1.0f); // Texture index in the shader
        }

        // Validate index buffer content
        GLuint indexBufferId = renderer2D->getInternalStorage()->indexBuffer->getId();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

        std::vector<unsigned int> indexData(6); // Expecting 6 indices for the quad
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 6 * sizeof(unsigned int), indexData.data());

        // Expected indices for two triangles
        unsigned int expectedIndices[] = {0, 1, 2, 2, 3, 0};
        for (int i = 0; i < 6; ++i)
        {
            EXPECT_EQ(indexData[i], expectedIndices[i]);
        }

        // Validate stats
        RendererStats stats = renderer2D->getStats();
        EXPECT_EQ(stats.quadCount, 1); // One quad drawn
        EXPECT_EQ(stats.getTotalVertexCount(), 4); // 1 quad * 4 vertices
        EXPECT_EQ(stats.getTotalIndexCount(), 6); // 1 quad * 6 indices
    }


    TEST_F(Renderer2DTest, DrawQuadWithSubTexture)
    {
        glm::vec2 position = {0.0f, 0.0f};
        glm::vec2 size = {1.0f, 1.0f};
        auto texture = Texture2D::create(4, 4); // Base texture
        auto subTexture = SubTexture2D::createFromCoords(texture, {1, 1}, {2, 2}, {1, 1}); // SubTexture

        renderer2D->beginScene(glm::mat4(1.0f));

        // Draw quad with SubTexture
        EXPECT_NO_THROW(renderer2D->drawQuad(position, size, subTexture));

        renderer2D->endScene();

        // Validate vertex buffer content
        GLuint vertexBufferId = renderer2D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        std::vector<QuadVertex> vertexData(4); // Expecting 4 vertices
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(QuadVertex), vertexData.data());

        // Normalize expected texture coordinates
        float texWidth = static_cast<float>(texture->getWidth());
        float texHeight = static_cast<float>(texture->getHeight());
        glm::vec2 expectedTexCoords[] = {
            {2.0f / texWidth, 2.0f / texHeight}, // Bottom-left
            {4.0f / texWidth, 2.0f / texHeight}, // Bottom-right
            {4.0f / texWidth, 4.0f / texHeight}, // Top-right
            {2.0f / texWidth, 4.0f / texHeight}, // Top-left
        };
        // Validate texture coordinates
        for (int i = 0; i < 4; ++i)
        {
            EXPECT_EQ(vertexData[i].texCoord, expectedTexCoords[i]);
        }

        // Validate stats
        RendererStats stats = renderer2D->getStats();
        EXPECT_EQ(stats.quadCount, 1);
    }


    TEST_F(Renderer2DTest, DrawQuadWithRotation)
    {
        glm::vec2 position = {0.0f, 0.0f};
        glm::vec2 size = {1.0f, 1.0f};
        float rotation = 45.0f; // Degrees
        glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

        renderer2D->beginScene(glm::mat4(1.0f));

        // Draw quad with rotation
        EXPECT_NO_THROW(renderer2D->drawQuad(position, size, rotation, color));

        renderer2D->endScene();

        GLuint vertexBufferId = renderer2D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        std::vector<QuadVertex> vertexData(4); // Expecting 4 vertices
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 4 * sizeof(QuadVertex), vertexData.data());

        glm::mat4 transform = glm::translate(glm::mat4(1.0f), glm::vec3(position, 0.0f)) *
                              glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                              glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));

        // Validate rotated positions
        glm::vec4 expectedPositions[] = {
            transform * glm::vec4(-0.5f, -0.5f, 0.0f, 1.0f),
            transform * glm::vec4(0.5f, -0.5f, 0.0f, 1.0f),
            transform * glm::vec4(0.5f, 0.5f, 0.0f, 1.0f),
            transform * glm::vec4(-0.5f, 0.5f, 0.0f, 1.0f),
        };
        for (int i = 0; i < 4; ++i)
        {
            EXPECT_NEAR(vertexData[i].position.x, expectedPositions[i].x, 0.01f);
            EXPECT_NEAR(vertexData[i].position.y, expectedPositions[i].y, 0.01f);
        }


        // Validate stats
        RendererStats stats = renderer2D->getStats();
        EXPECT_EQ(stats.quadCount, 1);
    }

    TEST_F(Renderer2DTest, DrawMultipleQuads)
    {
        glm::vec2 position1 = {0.0f, 0.0f};
        glm::vec2 position2 = {2.0f, 2.0f};
        glm::vec2 size = {1.0f, 1.0f};
        glm::vec4 color1 = {1.0f, 0.0f, 0.0f, 1.0f};
        glm::vec4 color2 = {0.0f, 1.0f, 0.0f, 1.0f};

        renderer2D->beginScene(glm::mat4(1.0f));

        // Draw two quads
        EXPECT_NO_THROW(renderer2D->drawQuad(position1, size, color1));
        EXPECT_NO_THROW(renderer2D->drawQuad(position2, size, color2));

        renderer2D->endScene();

        // Validate stats
        RendererStats stats = renderer2D->getStats();
        EXPECT_EQ(stats.quadCount, 2);
        EXPECT_EQ(stats.getTotalVertexCount(), 8); // 2 quads * 4 vertices
        EXPECT_EQ(stats.getTotalIndexCount(), 12); // 2 quads * 6 indices

        // Validate vertex buffer content
        GLuint vertexBufferId = renderer2D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        std::vector<QuadVertex> vertexData(8); // Expecting 8 vertices (2 quads * 4 vertices)
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(QuadVertex), vertexData.data());

        // Expected vertex positions for the two quads
        glm::vec3 expectedPositions[] = {
            // Quad 1 (position1)
            {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f},
            // Quad 2 (position2)
            {1.5f, 1.5f, 0.0f}, {2.5f, 1.5f, 0.0f}, {2.5f, 2.5f, 0.0f}, {1.5f, 2.5f, 0.0f}
        };
        // Expected colors for the two quads
        glm::vec4 expectedColors[] = {
            color1, color1, color1, color1, // Quad 1
            color2, color2, color2, color2 // Quad 2
        };
        // Validate vertex positions and colors
        for (int i = 0; i < 8; ++i)
        {
            EXPECT_EQ(vertexData[i].position, expectedPositions[i]);
            EXPECT_EQ(vertexData[i].color, expectedColors[i]);
        }

        // Validate index buffer content
        GLuint indexBufferId = renderer2D->getInternalStorage()->indexBuffer->getId();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        std::vector<unsigned int> indexData(12); // Expecting 12 indices (2 quads * 6 indices)
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 12 * sizeof(unsigned int), indexData.data());

        // Expected indices for the two quads
        unsigned int expectedIndices[] = {
            // Quad 1
            0, 1, 2, 2, 3, 0,
            // Quad 2
            4, 5, 6, 6, 7, 4
        };

        // Validate indices
        for (int i = 0; i < 12; ++i)
        {
            EXPECT_EQ(indexData[i], expectedIndices[i]);
        }
    }

    TEST_F(Renderer2DTest, DrawMultipleTexturedQuads)
    {
        glm::vec2 position1 = {0.0f, 0.0f};
        glm::vec2 position2 = {2.0f, 2.0f};
        glm::vec2 size = {1.0f, 1.0f};
        auto texture1 = Texture2D::create(4, 4); // Texture for the first quad
        auto texture2 = Texture2D::create(8, 8); // Texture for the second quad

        renderer2D->beginScene(glm::mat4(1.0f));

        // Draw two textured quads
        EXPECT_NO_THROW(renderer2D->drawQuad(position1, size, texture1));
        EXPECT_NO_THROW(renderer2D->drawQuad(position2, size, texture2));

        renderer2D->endScene();

        // Validate stats
        RendererStats stats = renderer2D->getStats();
        EXPECT_EQ(stats.quadCount, 2);
        EXPECT_EQ(stats.getTotalVertexCount(), 8); // 2 quads * 4 vertices
        EXPECT_EQ(stats.getTotalIndexCount(), 12); // 2 quads * 6 indices

        // Validate vertex buffer content
        GLuint vertexBufferId = renderer2D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);

        // Read back vertex data
        std::vector<QuadVertex> vertexData(8); // Expecting 8 vertices (2 quads * 4 vertices)
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 8 * sizeof(QuadVertex), vertexData.data());

        // Expected vertex positions for the two quads
        glm::vec3 expectedPositions[] = {
            // Quad 1 (position1)
            {-0.5f, -0.5f, 0.0f}, {0.5f, -0.5f, 0.0f}, {0.5f, 0.5f, 0.0f}, {-0.5f, 0.5f, 0.0f},
            // Quad 2 (position2)
            {1.5f, 1.5f, 0.0f}, {2.5f, 1.5f, 0.0f}, {2.5f, 2.5f, 0.0f}, {1.5f, 2.5f, 0.0f}
        };

        // Expected texture coordinates (default for full texture)
        glm::vec2 expectedTexCoords[] = {
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}, // Quad 1
            {0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f} // Quad 2
        };

        // Expected texture indices
        float expectedTexIndices[] = {
            1.0f, 1.0f, 1.0f, 1.0f, // Quad 1
            2.0f, 2.0f, 2.0f, 2.0f // Quad 2
        };

        // Validate vertex positions, texture coordinates, and texture indices
        for (int i = 0; i < 8; ++i)
        {
            EXPECT_EQ(vertexData[i].position, expectedPositions[i]) << "Mismatch at vertex " << i;
            EXPECT_EQ(vertexData[i].texCoord, expectedTexCoords[i]) << "Mismatch at vertex " << i;
            EXPECT_EQ(vertexData[i].texIndex, expectedTexIndices[i]) << "Mismatch at vertex " << i;
        }

        // Validate index buffer content
        GLuint indexBufferId = renderer2D->getInternalStorage()->indexBuffer->getId();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);

        // Read back index data
        std::vector<unsigned int> indexData(12); // Expecting 12 indices (2 quads * 6 indices)
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 12 * sizeof(unsigned int), indexData.data());

        // Expected indices for the two quads
        unsigned int expectedIndices[] = {
            // Quad 1
            0, 1, 2, 2, 3, 0,
            // Quad 2
            4, 5, 6, 6, 7, 4
        };

        // Validate indices
        for (int i = 0; i < 12; ++i)
        {
            EXPECT_EQ(indexData[i], expectedIndices[i]) << "Mismatch at index " << i;
        }
    }

    TEST_F(Renderer2DTest, BeginSceneWithoutInit) {
        // Manually delete the storage to simulate an uninitialized renderer
        renderer2D->shutdown();

        glm::mat4 viewProjection = glm::mat4(1.0f);

        // Expect RendererNotInitialized exception
        EXPECT_THROW(renderer2D->beginScene(viewProjection), RendererNotInitialized);
        // Re-init for TearDown function
        renderer2D->init();
    }

    TEST_F(Renderer2DTest, EndSceneWithoutBeginScene) {
        // Expect RendererSceneLifeCycleFailure exception
        EXPECT_THROW(renderer2D->endScene(), RendererSceneLifeCycleFailure);
    }

    TEST_F(Renderer2DTest, BeginSceneTwice) {
        glm::mat4 viewProjection = glm::mat4(1.0f);

        renderer2D->beginScene(viewProjection);

        // Expect RendererSceneLifeCycleFailure exception when calling beginScene again without ending the first one
        EXPECT_THROW(renderer2D->beginScene(viewProjection), RendererSceneLifeCycleFailure);

        renderer2D->endScene();
    }

    TEST_F(Renderer2DTest, DrawQuadWithoutBeginScene) {
        glm::vec2 position = {0.0f, 0.0f};
        glm::vec2 size = {1.0f, 1.0f};
        glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

        // Expect RendererSceneLifeCycleFailure exception
        EXPECT_THROW(renderer2D->drawQuad(position, size, color), RendererSceneLifeCycleFailure);
    }

    TEST_F(Renderer2DTest, ResetStatsWithoutInit) {
        // Manually delete the storage to simulate an uninitialized renderer
        renderer2D->shutdown();

        // Expect RendererNotInitialized exception
        EXPECT_THROW(renderer2D->resetStats(), RendererNotInitialized);
        // Re-init for TearDown function
        renderer2D->init();
    }

    TEST_F(Renderer2DTest, GetStatsWithoutInit) {
        // Manually delete the storage to simulate an uninitialized renderer
        renderer2D->shutdown();

        // Expect RendererNotInitialized exception
        EXPECT_THROW(renderer2D->getStats(), RendererNotInitialized);
        // Re-init for TearDown function
        renderer2D->init();
    }


}
