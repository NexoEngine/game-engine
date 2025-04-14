///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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

        // Use an OpenGL query to count the number of triangles drawn.
        GLuint query;
        glGenQueries(1, &query);
        glBeginQuery(GL_PRIMITIVES_GENERATED, query);

        renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        EXPECT_NO_THROW(renderer3D->drawCube(position, size, color));
        renderer3D->endScene();

        glEndQuery(GL_PRIMITIVES_GENERATED);
        GLuint primitivesGenerated = 0;
        glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
        // A cube is made of 12 triangles.
        EXPECT_EQ(primitivesGenerated, 12);

        glDeleteQueries(1, &query);

        // Validate vertex buffer data:
        GLuint vertexBufferId = renderer3D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        std::vector<Vertex> vertexData(36); // Expecting 36 vertices
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 36 * sizeof(Vertex), vertexData.data());

        // Expected vertex positions for a unit cube
        const glm::vec3 expectedPositions[36] = {
            // Front face
            {-0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
            {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
            // Bottom face
            {-0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f},
            // Back face
            {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
            // Top face
            {-0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
            // Right face
            {0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, -0.5f},
            {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f},
            // Left face
            {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f},
            {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, -0.5f}
        };

        // Expected texture coordinates for each vertex
        const glm::vec2 expectedTexCoords[36] = {
            // Front face
            {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            // Bottom face
            {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            // Back face
            {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            // Top face
            {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            // Right face
            {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            // Left face
            {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
        };

        // Expected normal vectors for each face (same normal for all vertices in a face)
        const glm::vec3 expectedNormals[36] = {
            // Front face - normal is (0,0,1)
            {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
            {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
            // Bottom face - normal is (0,-1,0)
            {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
            {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
            // Back face - normal is (0,0,-1)
            {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
            {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
            // Top face - normal is (0,1,0)
            {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
            {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
            // Right face - normal is (1,0,0)
            {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
            {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
            // Left face - normal is (-1,0,0)
            {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
            {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}
        };

        // Check vertex data
        for (unsigned int i = 0; i < 36; ++i)
        {
            // Compare the vertex position
            EXPECT_VEC3_NEAR(vertexData[i].position, expectedPositions[i], 0.01f);
            // Compare texture coordinates
            EXPECT_VEC2_NEAR(vertexData[i].texCoord, expectedTexCoords[i], 0.01f);
            // Compare normals
            EXPECT_VEC3_NEAR(vertexData[i].normal, expectedNormals[i], 0.01f);
            // Check that the entityID was correctly set (here we passed -1)
            EXPECT_EQ(vertexData[i].entityID, -1);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Validate index buffer content:
        GLuint indexBufferId = renderer3D->getInternalStorage()->indexBuffer->getId();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        std::vector<unsigned int> indexData(36); // Expecting 36 indices
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 36 * sizeof(unsigned int), indexData.data());

        // Since we fill indices sequentially from 0 to 35:
        for (unsigned int i = 0; i < 36; ++i)
        {
            EXPECT_EQ(indexData[i], i);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

	TEST_F(Renderer3DTest, DrawCubeWithMaterial)
	{
	    glm::vec3 position = {0.0f, 0.0f, 0.0f};
	    glm::vec3 size = {1.0f, 1.0f, 1.0f};

	    renderer::Material material;
	    material.albedoColor = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color
	    material.albedoTexture = Texture2D::create(4, 4); // Example texture

	    GLuint query;
	    glGenQueries(1, &query);
	    glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	    renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	    EXPECT_NO_THROW(renderer3D->drawCube(position, size, material));
	    renderer3D->endScene();

	    // Validate number of primitives drawn
	    glEndQuery(GL_PRIMITIVES_GENERATED);
	    GLuint primitivesGenerated = 0;
	    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	    EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
	    glDeleteQueries(1, &query);

	    // Validate render stats
	    Renderer3DStats stats = renderer3D->getStats();
	    EXPECT_EQ(stats.cubeCount, 1);
	    EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices per cube (as defined in struct)
	    EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
	}

	TEST_F(Renderer3DTest, DrawCubeWithRotation)
	{
	    glm::vec3 position = {1.0f, 2.0f, 3.0f};
	    glm::vec3 size = {2.0f, 2.0f, 2.0f};
	    glm::vec3 rotation = {45.0f, 30.0f, 60.0f};
	    glm::vec4 color = {0.0f, 1.0f, 0.0f, 1.0f}; // Green color

	    // Use an OpenGL query to count the number of triangles drawn
	    GLuint query;
	    glGenQueries(1, &query);
	    glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	    renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	    EXPECT_NO_THROW(renderer3D->drawCube(position, size, rotation, color));
	    renderer3D->endScene();

	    // Validate number of primitives drawn
	    glEndQuery(GL_PRIMITIVES_GENERATED);
	    GLuint primitivesGenerated = 0;
	    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	    EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
	    glDeleteQueries(1, &query);

	    // Validate render stats
	    Renderer3DStats stats = renderer3D->getStats();
	    EXPECT_EQ(stats.cubeCount, 1);
	    EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices
	    EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
	}

	TEST_F(Renderer3DTest, DrawCubeWithTransformMatrix)
	{
	    glm::mat4 transform = glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 3.0f}) *
	                         glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});
	    glm::vec4 color = {0.0f, 0.0f, 1.0f, 1.0f}; // Blue color

	    // Use an OpenGL query to count the number of triangles drawn
	    GLuint query;
	    glGenQueries(1, &query);
	    glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	    renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	    EXPECT_NO_THROW(renderer3D->drawCube(transform, color));
	    renderer3D->endScene();

	    // Validate number of primitives drawn
	    glEndQuery(GL_PRIMITIVES_GENERATED);
	    GLuint primitivesGenerated = 0;
	    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	    EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
	    glDeleteQueries(1, &query);

	    // Validate render stats
	    Renderer3DStats stats = renderer3D->getStats();
	    EXPECT_EQ(stats.cubeCount, 1);
	    EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices
	    EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
	}

	TEST_F(Renderer3DTest, DrawCubeWithRotationAndMaterial)
	{
	    glm::vec3 position = {1.0f, 2.0f, 3.0f};
	    glm::vec3 size = {2.0f, 2.0f, 2.0f};
	    glm::vec3 rotation = {45.0f, 30.0f, 60.0f};

	    renderer::Material material;
	    material.albedoColor = {0.0f, 1.0f, 1.0f, 1.0f}; // Cyan color
	    material.albedoTexture = Texture2D::create(4, 4); // Example texture
	    material.specularColor = {1.0f, 1.0f, 1.0f, 1.0f};
	    material.metallicMap = Texture2D::create(2, 2); // Example specular texture

	    GLuint query;
	    glGenQueries(1, &query);
	    glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	    renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	    EXPECT_NO_THROW(renderer3D->drawCube(position, size, rotation, material));
	    renderer3D->endScene();

	    // Validate number of primitives drawn
	    glEndQuery(GL_PRIMITIVES_GENERATED);
	    GLuint primitivesGenerated = 0;
	    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	    EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
	    glDeleteQueries(1, &query);

	    // Validate render stats
	    Renderer3DStats stats = renderer3D->getStats();
	    EXPECT_EQ(stats.cubeCount, 1);
	    EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices
	    EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
	}

	TEST_F(Renderer3DTest, DrawCubeWithTransformAndMaterial)
	{
	    glm::mat4 transform = glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 3.0f}) *
	                          glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), {0.0f, 1.0f, 0.0f}) *
	                          glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});

	    renderer::Material material;
	    material.albedoColor = {1.0f, 1.0f, 0.0f, 1.0f}; // Yellow color
	    material.albedoTexture = Texture2D::create(4, 4); // Example texture

	    GLuint query;
	    glGenQueries(1, &query);
	    glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	    renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	    EXPECT_NO_THROW(renderer3D->drawCube(transform, material));
	    renderer3D->endScene();

	    // Validate number of primitives drawn
	    glEndQuery(GL_PRIMITIVES_GENERATED);
	    GLuint primitivesGenerated = 0;
	    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	    EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
	    glDeleteQueries(1, &query);

	    // Validate render stats
	    Renderer3DStats stats = renderer3D->getStats();
	    EXPECT_EQ(stats.cubeCount, 1);
	    EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices
	    EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
	}

	TEST_F(Renderer3DTest, DrawMesh)
	{
	    // Create a simple mesh (a triangle)
	    std::vector<Vertex> vertices = {
	        {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, -1},
	        {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, -1},
	        {{ 0.0f,  0.5f, 0.0f}, {0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, -1}
	    };
	    std::vector<unsigned int> indices = {0, 1, 2};
	    auto texture = Texture2D::create(4, 4);

	    // Use an OpenGL query to count the number of triangles drawn
	    GLuint query;
	    glGenQueries(1, &query);
	    glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	    renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	    EXPECT_NO_THROW(renderer3D->drawMesh(vertices, indices, texture));
	    renderer3D->endScene();

	    // Validate number of primitives drawn
	    glEndQuery(GL_PRIMITIVES_GENERATED);
	    GLuint primitivesGenerated = 0;
	    glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	    EXPECT_EQ(primitivesGenerated, 1); // One triangle
	    glDeleteQueries(1, &query);

	    // Validate vertex buffer data
	    GLuint vertexBufferId = renderer3D->getInternalStorage()->vertexBuffer->getId();
	    glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	    std::vector<Vertex> vertexData(3); // Expecting 3 vertices for a triangle
	    glGetBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(Vertex), vertexData.data());

	    // Check vertex data
	    for (unsigned int i = 0; i < 3; ++i)
	    {
	        // Compare the vertex position
	        EXPECT_VEC3_NEAR(vertexData[i].position, vertices[i].position, 0.01f);
	        // Compare texture coordinates
	        EXPECT_VEC2_NEAR(vertexData[i].texCoord, vertices[i].texCoord, 0.01f);
	        // Compare normals
	        EXPECT_VEC3_NEAR(vertexData[i].normal, vertices[i].normal, 0.01f);
	        // Check that the entityID was correctly set
	        EXPECT_EQ(vertexData[i].entityID, -1);
	    }
	    glBindBuffer(GL_ARRAY_BUFFER, 0);

	    // Validate index buffer content
	    GLuint indexBufferId = renderer3D->getInternalStorage()->indexBuffer->getId();
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	    std::vector<unsigned int> indexData(3); // Expecting 3 indices for a triangle
	    glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 3 * sizeof(unsigned int), indexData.data());

	    // Check indices (should be 0, 1, 2)
	    for (unsigned int i = 0; i < 3; ++i)
	    {
	        EXPECT_EQ(indexData[i], i);
	    }
	    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	    EXPECT_THROW(renderer3D->drawCube(position, size, color), RendererSceneLifeCycleFailure);
	}
}
