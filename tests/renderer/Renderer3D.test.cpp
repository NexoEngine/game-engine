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
#include <Logger.hpp>
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

	// Cube tests
    TEST_F(Renderer3DTest, DrawCubeWithoutTexture)
    {
        // glm::vec3 position = {0.0f, 0.0f, 0.0f};
        // glm::vec3 size = {1.0f, 1.0f, 1.0f};
        // glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color

        // // Use an OpenGL query to count the number of triangles drawn.
        // GLuint query;
        // glGenQueries(1, &query);
        // glBeginQuery(GL_PRIMITIVES_GENERATED, query);

        // renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        // EXPECT_NO_THROW(renderer3D->drawCube(position, size, color));
        // renderer3D->endScene();

        // glEndQuery(GL_PRIMITIVES_GENERATED);
        // GLuint primitivesGenerated = 0;
        // glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
        // // A cube is made of 12 triangles.
        // EXPECT_EQ(primitivesGenerated, 12);

        // glDeleteQueries(1, &query);

        // // Validate vertex buffer data:
        // GLuint vertexBufferId = renderer3D->getInternalStorage()->vertexBuffer->getId();
        // glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        // std::vector<NxVertex> vertexData(36); // Expecting 36 vertices
        // glGetBufferSubData(GL_ARRAY_BUFFER, 0, 36 * sizeof(NxVertex), vertexData.data());

        // // Expected vertex positions for a unit cube
        // const glm::vec3 expectedPositions[36] = {
        //     // Front face
        //     {-0.5f, 0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
        //     {0.5f, -0.5f, 0.5f}, {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f},
        //     // Bottom face
        //     {-0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
        //     {0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, 0.5f}, {-0.5f, -0.5f, 0.5f},
        //     // Back face
        //     {-0.5f, -0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, -0.5f},
        //     {0.5f, 0.5f, -0.5f}, {0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f},
        //     // Top face
        //     {-0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, 0.5f},
        //     {0.5f, 0.5f, 0.5f}, {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
        //     // Right face
        //     {0.5f, 0.5f, 0.5f}, {0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, -0.5f},
        //     {0.5f, -0.5f, -0.5f}, {0.5f, 0.5f, -0.5f}, {0.5f, 0.5f, 0.5f},
        //     // Left face
        //     {-0.5f, 0.5f, -0.5f}, {-0.5f, -0.5f, -0.5f}, {-0.5f, -0.5f, 0.5f},
        //     {-0.5f, -0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, -0.5f}
        // };

        // // Expected texture coordinates for each vertex
        // const glm::vec2 expectedTexCoords[36] = {
        //     // Front face
        //     {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
        //     {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
        //     // Bottom face
        //     {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
        //     {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
        //     // Back face
        //     {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
        //     {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
        //     // Top face
        //     {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
        //     {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
        //     // Right face
        //     {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
        //     {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
        //     // Left face
        //     {0.0f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
        //     {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}
        // };

        // // Expected normal vectors for each face (same normal for all vertices in a face)
        // const glm::vec3 expectedNormals[36] = {
        //     // Front face - normal is (0,0,1)
        //     {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
        //     {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f, 1.0f},
        //     // Bottom face - normal is (0,-1,0)
        //     {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
        //     {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
        //     // Back face - normal is (0,0,-1)
        //     {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
        //     {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f}, {0.0f, 0.0f, -1.0f},
        //     // Top face - normal is (0,1,0)
        //     {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
        //     {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
        //     // Right face - normal is (1,0,0)
        //     {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
        //     {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f},
        //     // Left face - normal is (-1,0,0)
        //     {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f},
        //     {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}, {-1.0f, 0.0f, 0.0f}
        // };

        // // Check vertex data
        // for (unsigned int i = 0; i < 36; ++i)
        // {
        //     // Compare the vertex position
        //     EXPECT_VEC3_NEAR(vertexData[i].position, expectedPositions[i], 0.01f);
        //     // Compare texture coordinates
        //     EXPECT_VEC2_NEAR(vertexData[i].texCoord, expectedTexCoords[i], 0.01f);
        //     // Compare normals
        //     EXPECT_VEC3_NEAR(vertexData[i].normal, expectedNormals[i], 0.01f);
        //     // Check that the entityID was correctly set (here we passed -1)
        //     EXPECT_EQ(vertexData[i].entityID, -1);
        // }
        // glBindBuffer(GL_ARRAY_BUFFER, 0);

        // // Validate index buffer content:
        // GLuint indexBufferId = renderer3D->getInternalStorage()->indexBuffer->getId();
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        // std::vector<unsigned int> indexData(36); // Expecting 36 indices
        // glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 36 * sizeof(unsigned int), indexData.data());

        // // Since we fill indices sequentially from 0 to 35:
        // for (unsigned int i = 0; i < 36; ++i)
        // {
        //     EXPECT_EQ(indexData[i], i);
        // }
        // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

	TEST_F(Renderer3DTest, DrawCubeWithMaterial)
	{
	//     glm::vec3 position = {0.0f, 0.0f, 0.0f};
	//     glm::vec3 size = {1.0f, 1.0f, 1.0f};

	//     renderer::NxMaterial material;
	//     material.albedoColor = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color
	//     material.albedoTexture = NxTexture2D::create(4, 4); // Example texture

	//     GLuint query;
	//     glGenQueries(1, &query);
	//     glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	//     renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	//     EXPECT_NO_THROW(renderer3D->drawCube(position, size, material));
	//     renderer3D->endScene();

	//     // Validate number of primitives drawn
	//     glEndQuery(GL_PRIMITIVES_GENERATED);
	//     GLuint primitivesGenerated = 0;
	//     glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	//     EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
	//     glDeleteQueries(1, &query);

	//     // Validate render stats
	//     NxRenderer3DStats stats = renderer3D->getStats();
	//     EXPECT_EQ(stats.cubeCount, 1);
	//     EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices per cube (as defined in struct)
	//     EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
	// }

	// TEST_F(Renderer3DTest, DrawCubeWithRotation)
	// {
	//     glm::vec3 position = {1.0f, 2.0f, 3.0f};
	//     glm::vec3 size = {2.0f, 2.0f, 2.0f};
	//     glm::vec3 rotation = {45.0f, 30.0f, 60.0f};
	//     glm::vec4 color = {0.0f, 1.0f, 0.0f, 1.0f}; // Green color

	//     // Use an OpenGL query to count the number of triangles drawn
	//     GLuint query;
	//     glGenQueries(1, &query);
	//     glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	//     renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	//     EXPECT_NO_THROW(renderer3D->drawCube(position, size, rotation, color));
	//     renderer3D->endScene();

	//     // Validate number of primitives drawn
	//     glEndQuery(GL_PRIMITIVES_GENERATED);
	//     GLuint primitivesGenerated = 0;
	//     glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	//     EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
	//     glDeleteQueries(1, &query);

	//     // Validate render stats
	//     NxRenderer3DStats stats = renderer3D->getStats();
	//     EXPECT_EQ(stats.cubeCount, 1);
	//     EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices
	//     EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
	}

	TEST_F(Renderer3DTest, DrawCubeWithTransformMatrix)
	{
	    // glm::mat4 transform = glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 3.0f}) *
	    //                      glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});
	    // glm::vec4 color = {0.0f, 0.0f, 1.0f, 1.0f}; // Blue color

	    // // Use an OpenGL query to count the number of triangles drawn
	    // GLuint query;
	    // glGenQueries(1, &query);
	    // glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	    // renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	    // EXPECT_NO_THROW(renderer3D->drawCube(transform, color));
	    // renderer3D->endScene();

	    // // Validate number of primitives drawn
	    // glEndQuery(GL_PRIMITIVES_GENERATED);
	    // GLuint primitivesGenerated = 0;
	    // glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	    // EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
	    // glDeleteQueries(1, &query);

	    // // Validate render stats
	    // NxRenderer3DStats stats = renderer3D->getStats();
	    // EXPECT_EQ(stats.cubeCount, 1);
	    // EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices
	    // EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
	}

	TEST_F(Renderer3DTest, DrawCubeWithRotationAndMaterial)
	{
	    // glm::vec3 position = {1.0f, 2.0f, 3.0f};
	    // glm::vec3 size = {2.0f, 2.0f, 2.0f};
	    // glm::vec3 rotation = {45.0f, 30.0f, 60.0f};

	    // renderer::NxMaterial material;
	    // material.albedoColor = {0.0f, 1.0f, 1.0f, 1.0f}; // Cyan color
	    // material.albedoTexture = NxTexture2D::create(4, 4); // Example texture
	    // material.specularColor = {1.0f, 1.0f, 1.0f, 1.0f};
	    // material.metallicMap = NxTexture2D::create(2, 2); // Example specular texture

	    // GLuint query;
	    // glGenQueries(1, &query);
	    // glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	    // renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	    // EXPECT_NO_THROW(renderer3D->drawCube(position, size, rotation, material));
	    // renderer3D->endScene();

	    // // Validate number of primitives drawn
	    // glEndQuery(GL_PRIMITIVES_GENERATED);
	    // GLuint primitivesGenerated = 0;
	    // glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	    // EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
	    // glDeleteQueries(1, &query);

	    // // Validate render stats
	    // NxRenderer3DStats stats = renderer3D->getStats();
	    // EXPECT_EQ(stats.cubeCount, 1);
	    // EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices
	    // EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
	}

	TEST_F(Renderer3DTest, DrawCubeWithTransformAndMaterial)
	{
	    // glm::mat4 transform = glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 3.0f}) *
	    //                       glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), {0.0f, 1.0f, 0.0f}) *
	    //                       glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});

	    // renderer::NxMaterial material;
	    // material.albedoColor = {1.0f, 1.0f, 0.0f, 1.0f}; // Yellow color
	    // material.albedoTexture = NxTexture2D::create(4, 4); // Example texture

	    // GLuint query;
	    // glGenQueries(1, &query);
	    // glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	    // renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	    // EXPECT_NO_THROW(renderer3D->drawCube(transform, material));
	    // renderer3D->endScene();

	    // // Validate number of primitives drawn
	    // glEndQuery(GL_PRIMITIVES_GENERATED);
	    // GLuint primitivesGenerated = 0;
	    // glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	    // EXPECT_EQ(primitivesGenerated, 12); // A cube is made of 12 triangles
	    // glDeleteQueries(1, &query);

	    // // Validate render stats
	    // NxRenderer3DStats stats = renderer3D->getStats();
	    // EXPECT_EQ(stats.cubeCount, 1);
	    // EXPECT_EQ(stats.getTotalVertexCount(), 8); // 1 cube * 8 vertices
	    // EXPECT_EQ(stats.getTotalIndexCount(), 36); // 1 cube * 36 indices
	}

	// Tetrahedron tests
	TEST_F(Renderer3DTest, DrawTetrahedronWithoutTexture)
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 size = {1.0f, 1.0f, 1.0f};
        glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color

        // Use an OpenGL query to count the number of triangles drawn.
        GLuint query;
        glGenQueries(1, &query);
        glBeginQuery(GL_PRIMITIVES_GENERATED, query);

        renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        EXPECT_NO_THROW(renderer3D->drawTetrahedron(position, size, color));
        renderer3D->endScene();

        glEndQuery(GL_PRIMITIVES_GENERATED);
        GLuint primitivesGenerated = 0;
        glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
        // A tetrahedron is made of 4 triangles.
        EXPECT_EQ(primitivesGenerated, 4);

        glDeleteQueries(1, &query);

        // Validate vertex buffer data:
        GLuint vertexBufferId = renderer3D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        std::vector<Vertex> vertexData(12); // Expecting 12 vertices
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 12 * sizeof(Vertex), vertexData.data());

        // Expected vertex positions for a unit tetrahedron
        constexpr glm::vec3 expectedPositions[12] = {
	        {0.0f, 0.5f, 0.0f}, {-0.5, -0.5, -0.5}, {0.5, -0.5, -0.5}, // v0 v1 v2 front face
			{0, 0.5, 0}, {0.5, -0.5, -0.5}, {0, -0.5, 0.5}, // v0 v2 v3 right face
			{0, 0.5, 0}, {0, -0.5, 0.5}, {-0.5, -0.5, -0.5}, // v0 v3 v1 left face
			{-0.5, -0.5, -0.5}, {0, -0.5, 0.5}, {0.5, -0.5, -0.5} // v1 v3 v2 bottom face
        };

        // Expected texture coordinates for each vertex
        const glm::vec2 expectedTexCoords[12] = {
	        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Front face
			{1.0f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f}, // Right face
			{0.0f, 0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f}, // Left face
			{0.0f, 1.0f}, {1.0f, 1.0f}, {0.5f, 0.0f} // Bottom face
        };

        // Expected normal vectors for each face (same normal for all vertices in a face)
        constexpr glm::vec3 expectedNormals[12] = {
            {0, -0.447214, 0.894427}, {0, -0.447214, 0.894427}, {0, -0.447214, 0.894427},
            {-0.872872, -0.218218, -0.436436}, {-0.872872, -0.218218, -0.436436}, {-0.872872, -0.218218, -0.436436},
            {0.872872, -0.218218, -0.436436}, {0.872872, -0.218218, -0.436436}, {0.872872, -0.218218, -0.436436},
            {0, 1, 0}, {0, 1, 0}, {0, 1, 0}
        };

        // Check vertex data
        for (unsigned int i = 0; i < 12; ++i) {
            // Compare the vertex position
            EXPECT_VEC3_NEAR(vertexData[i].position, expectedPositions[i], 0.01f);
            // Compare texture coordinates
            EXPECT_VEC2_NEAR(vertexData[i].texCoord, expectedTexCoords[i], 0.01f);
            // Compare normals
        	// std::cout << "Normal: " << vertexData[i].normal.x << ", " << vertexData[i].normal.y << ", " << vertexData[i].normal.z << std::endl;
            EXPECT_VEC3_NEAR(vertexData[i].normal, expectedNormals[i], 0.01f);
            // Check that the entityID was correctly set (here we passed -1)
            EXPECT_EQ(vertexData[i].entityID, -1);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Validate index buffer content:
        GLuint indexBufferId = renderer3D->getInternalStorage()->indexBuffer->getId();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        std::vector<unsigned int> indexData(12); // Expecting 12 indices
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 12 * sizeof(unsigned int), indexData.data());

        // Since we fill indices sequentially from 0 to 12:
        for (unsigned int i = 0; i < 12; ++i)
        {
            EXPECT_EQ(indexData[i], i);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

	TEST_F(Renderer3DTest, DrawTetrahedronWithMaterial)
    {
    	glm::vec3 position = {0.0f, 0.0f, 0.0f};
    	glm::vec3 size = {1.0f, 1.0f, 1.0f};

    	components::Material material;
    	material.albedoColor = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color
    	material.albedoTexture = Texture2D::create(4, 4); // Example texture

    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawTetrahedron(position, size, material));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 4); // A tetrahedron is made of 4 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
    	// Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 4); // 1 tetrahedron * 4 vertices per tetrahedron (as defined in struct)
    	// EXPECT_EQ(stats.getTotalIndexCount(), 12); // 1 tetrahedron * 12 indices
    }

	TEST_F(Renderer3DTest, DrawTetrahedronWithRotation)
    {
	    constexpr glm::vec3 position = {1.0f, 2.0f, 3.0f};
    	constexpr glm::vec3 size = {2.0f, 2.0f, 2.0f};
    	constexpr glm::vec3 rotation = {45.0f, 30.0f, 60.0f};
    	constexpr glm::vec4 color = {0.0f, 1.0f, 0.0f, 1.0f}; // Green color

    	// Use an OpenGL query to count the number of triangles drawn
    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawTetrahedron(position, size, rotation, color));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 4); // A tetrahedron is made of 4 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
	    // const Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 4); // 1 tetrahedron * 4 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 12); // 1 tetrahedron * 12 indices
    }

	TEST_F(Renderer3DTest, DrawTetrahedronWithTransformMatrix)
    {
    	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 3.0f}) *
							 glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});
    	constexpr glm::vec4 color = {0.0f, 0.0f, 1.0f, 1.0f}; // Blue color

    	// Use an OpenGL query to count the number of triangles drawn
    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawTetrahedron(transform, color));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 4); // A tetrahedron is made of 4 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
	    // const Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 4); // 1 tetrahedron * 4 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 12); // 1 tetrahedron * 12 indices
    }

	TEST_F(Renderer3DTest, DrawTetrahedronWithRotationAndMaterial)
    {
    	constexpr glm::vec3 position = {1.0f, 2.0f, 3.0f};
    	constexpr glm::vec3 size = {2.0f, 2.0f, 2.0f};
    	constexpr glm::vec3 rotation = {45.0f, 30.0f, 60.0f};

    	components::Material material;
    	material.albedoColor = {0.0f, 1.0f, 1.0f, 1.0f}; // Cyan color
    	material.albedoTexture = Texture2D::create(4, 4); // Example texture
    	material.specularColor = {1.0f, 1.0f, 1.0f, 1.0f};
    	material.metallicMap = Texture2D::create(2, 2); // Example specular texture

    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawTetrahedron(position, size, rotation, material));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 4); // A tetrahedron is made of 4 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
    	// Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 4); // 1 tetrahedron * 4 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 12); // 1 tetrahedron * 12 indices
    }

	TEST_F(Renderer3DTest, DrawTetrahedronWithTransformAndMaterial)
    {
    	glm::mat4 transform = glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 3.0f}) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), {0.0f, 1.0f, 0.0f}) *
							  glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});

    	components::Material material;
    	material.albedoColor = {1.0f, 1.0f, 0.0f, 1.0f}; // Yellow color
    	material.albedoTexture = Texture2D::create(4, 4); // Example texture

    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawTetrahedron(transform, material));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 4); // A tetrahedron is made of 4 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
    	// Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 4); // 1 tetrahedron * 4 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 12); // 1 tetrahedron * 12 indices
    }

	// Pyramid tests
	TEST_F(Renderer3DTest, DrawPyramidWithoutTexture)
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 size = {1.0f, 1.0f, 1.0f};
        glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color

        // Use an OpenGL query to count the number of triangles drawn.
        GLuint query;
        glGenQueries(1, &query);
        glBeginQuery(GL_PRIMITIVES_GENERATED, query);

        renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        EXPECT_NO_THROW(renderer3D->drawPyramid(position, size, color));
        renderer3D->endScene();

        glEndQuery(GL_PRIMITIVES_GENERATED);
        GLuint primitivesGenerated = 0;
        glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
        // A pyramid is made of 6 triangles.
        EXPECT_EQ(primitivesGenerated, 6);

        glDeleteQueries(1, &query);

        // Validate vertex buffer data:
        GLuint vertexBufferId = renderer3D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        std::vector<Vertex> vertexData(18); // Expecting 18 vertices
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 18 * sizeof(Vertex), vertexData.data());

        // Expected vertex positions for a unit tetrahedron
        constexpr glm::vec3 expectedPositions[18] = {
        	// Base face
	        {-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, 1.0f},
        	{-1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, 1.0f}, {-1.0f, -1.0f, 1.0f},
        	// Side faces
			{0.0f, 1.0f, 0.0f}, {1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, -1.0f},
			{0.0f, 1.0f, 0.0f}, {1.0f, -1.0f, 1.0f}, {1.0f, -1.0f, -1.0f},
			{0.0f, 1.0f, 0.0f}, {-1.0f, -1.0f, 1.0f}, {1.0f, -1.0f, 1.0f},
			{0.0f, 1.0f, 0.0f}, {-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f}
        };

        // Expected texture coordinates for each vertex
        const glm::vec2 expectedTexCoords[18] = {
        	// Base face
	        {0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
			{0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
			// Side faces
			{0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
			{0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
			{0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
			{0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}
        };

        // Expected normal vectors for each face (same normal for all vertices in a face)
    	constexpr glm::vec3 expectedNormals[18] = {
    		// Base face
    		{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
    		// Side faces
    		{0.0f, 0.447214f, -0.894427f}, {0.0f, 0.447214f, -0.894427f}, {0.0f, 0.447214f, -0.894427f},
			{0.894427f, 0.447214f, 0.0f}, {0.894427f, 0.447214f, 0.0f}, {0.894427f, 0.447214f, 0.0f},
			{0.0f, 0.447214f, 0.894427f}, {0.0f, 0.447214f, 0.894427f}, {0.0f, 0.447214f, 0.894427f},
			{-0.894427f, 0.447214f, 0.0f}, {-0.894427f, 0.447214f, 0.0f}, {-0.894427f, 0.447214f, 0.0f}
    	};

        // Check vertex data
        for (unsigned int i = 0; i < 18; ++i)
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
        std::vector<unsigned int> indexData(18); // Expecting 18 indices
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 18 * sizeof(unsigned int), indexData.data());

        // Since we fill indices sequentially from 0 to 18:
        for (unsigned int i = 0; i < 18; ++i)
        {
            EXPECT_EQ(indexData[i], i);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

	TEST_F(Renderer3DTest, DrawPyramidWithMaterial)
    {
    	glm::vec3 position = {0.0f, 0.0f, 0.0f};
    	glm::vec3 size = {1.0f, 1.0f, 1.0f};

    	components::Material material;
    	material.albedoColor = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color
    	material.albedoTexture = Texture2D::create(4, 4); // Example texture

    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawPyramid(position, size, material));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 6); // A pyramid is made of 6 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
    	// Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 5); // 1 pyramid * 5 vertices per pyramid (as defined in struct)
    	// EXPECT_EQ(stats.getTotalIndexCount(), 18); // 1 pyramid * 18 indices
    }

	TEST_F(Renderer3DTest, DrawPyramidWithRotation)
    {
	    constexpr glm::vec3 position = {1.0f, 2.0f, 3.0f};
    	constexpr glm::vec3 size = {2.0f, 2.0f, 2.0f};
    	constexpr glm::vec3 rotation = {45.0f, 30.0f, 60.0f};
    	constexpr glm::vec4 color = {0.0f, 1.0f, 0.0f, 1.0f}; // Green color

    	// Use an OpenGL query to count the number of triangles drawn
    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawPyramid(position, size, rotation, color));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 6); // A pyramid is made of 6 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
	    // const Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 5); // 1 pyramid * 5 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 18); // 1 pyramid * 18 indices
    }

	TEST_F(Renderer3DTest, DrawPyramidWithTransformMatrix)
    {
    	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 3.0f}) *
							 glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});
    	constexpr glm::vec4 color = {0.0f, 0.0f, 1.0f, 1.0f}; // Blue color

    	// Use an OpenGL query to count the number of triangles drawn
    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawPyramid(transform, color));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 6); // A pyramid is made of 6 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
	    // const Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 5); // 1 pyramid * 5 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 18); // 1 pyramid * 18 indices
    }

	TEST_F(Renderer3DTest, DrawPyramidWithRotationAndMaterial)
    {
    	constexpr glm::vec3 position = {1.0f, 2.0f, 3.0f};
    	constexpr glm::vec3 size = {2.0f, 2.0f, 2.0f};
    	constexpr glm::vec3 rotation = {45.0f, 30.0f, 60.0f};

    	components::Material material;
    	material.albedoColor = {0.0f, 1.0f, 1.0f, 1.0f}; // Cyan color
    	material.albedoTexture = Texture2D::create(4, 4); // Example texture
    	material.specularColor = {1.0f, 1.0f, 1.0f, 1.0f};
    	material.metallicMap = Texture2D::create(2, 2); // Example specular texture

    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawPyramid(position, size, rotation, material));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 6); // A pyramid is made of 6 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
    	// Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 5); // 1 pyramid * 5 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 18); // 1 pyramid * 18 indices
    }

	TEST_F(Renderer3DTest, DrawPyramidWithTransformAndMaterial)
    {
    	glm::mat4 transform = glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 3.0f}) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), {0.0f, 1.0f, 0.0f}) *
							  glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});

    	components::Material material;
    	material.albedoColor = {1.0f, 1.0f, 0.0f, 1.0f}; // Yellow color
    	material.albedoTexture = Texture2D::create(4, 4); // Example texture

    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawPyramid(transform, material));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 6); // A pyramid is made of 6 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
    	// Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 5); // 1 pyramid * 5 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 18); // 1 pyramid * 18 indices
    }


	// Cylinder 8 tests
	TEST_F(Renderer3DTest, DrawCylinder8WithoutTexture)
    {
        glm::vec3 position = {0.0f, 0.0f, 0.0f};
        glm::vec3 size = {1.0f, 1.0f, 1.0f};
        glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color

        // Use an OpenGL query to count the number of triangles drawn.
        GLuint query;
        glGenQueries(1, &query);
        glBeginQuery(GL_PRIMITIVES_GENERATED, query);

        renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
        EXPECT_NO_THROW(renderer3D->drawCylinder(position, size, color));
        renderer3D->endScene();

        glEndQuery(GL_PRIMITIVES_GENERATED);
        GLuint primitivesGenerated = 0;
        glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
        // A cylinder is made of 28 triangles.
        EXPECT_EQ(primitivesGenerated, 28);

        glDeleteQueries(1, &query);

        // Validate vertex buffer data:
        GLuint vertexBufferId = renderer3D->getInternalStorage()->vertexBuffer->getId();
        glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
        std::vector<Vertex> vertexData(32); // Expecting 32 vertices
        glGetBufferSubData(GL_ARRAY_BUFFER, 0, 32 * sizeof(Vertex), vertexData.data());

        // Expected vertex positions for a unit tetrahedron
        constexpr glm::vec3 expectedPositions[32] = {
        	{0.707107f, 1, -0.707107f},
		   {1.19249e-08f, 1, -1},
		   {-0.707107f, 1, -0.707107f},
		   {-1, 1, -8.74228e-08f},
		   {-0.707107f, 1, 0.707107f},
		   {-4.37114e-08f, 1, 1},
		   {0.707107f, 1, 0.707107f},
		   {1, 1, 0},
		   {0.707107f, -1, -0.707107f},
		   {1.19249e-08f, -1, -1},
		   {-0.707107f, -1, -0.707107f},
		   {-1, -1, -8.74228e-08f},
		   {-0.707107f, -1, 0.707107f},
		   {-4.37114e-08f, -1, 1},
		   {0.707107f, -1, 0.707107f},
		   {1, -1, 0},
		   {1, 1, 0},
		   {0.707107f, 1, 0.707107f},
		   {-4.37114e-08f, 1, 1},
		   {-0.707107f, 1, 0.707107f},
		   {-1, 1, -8.74228e-08f},
		   {-0.707107f, 1, -0.707107f},
		   {1.19249e-08f, 1, -1},
		   {0.707107f, 1, -0.707107f},
		   {1, -1, 0},
		   {0.707107f, -1, 0.707107f},
		   {-4.37114e-08f, -1, 1},
		   {-0.707107f, -1, 0.707107f},
		   {-1, -1, -8.74228e-08f},
		   {-0.707107f, -1, -0.707107f},
		   {1.19249e-08f, -1, -1},
		   {0.707107f, -1, -0.707107f}
        };

        // Expected texture coordinates for each vertex
        const glm::vec2 expectedTexCoords[32] = {
	        {0.0f, 1.0f}, {0.125f, 1.0f}, {0.25f, 1.0f}, {0.375f, 1.0f},
			{0.5f, 1.0f}, {0.625f, 1.0f}, {0.75f, 1.0f}, {0.875f, 1.0f},
			{1.0f, 1.0f}, {1.125f, 1.0f}, {1.25f, 1.0f}, {1.375f, 1.0f},
			{1.5f, 1.0f}, {1.625f, 1.0f}, {1.75f, 1.0f}, {1.875f, 1.0f},
			{2.0f, 1.0f}, {2.125f, 1.0f}, {2.25f, 1.0f}, {2.375f, 1.0f},
			{2.5f, 1.0f}, {2.625f, 1.0f}, {2.75f, 1.0f}, {2.875f, 1.0f},
			{3.0f, 1.0f}, {3.125f, 1.0f}, {3.25f, 1.0f}, {3.375f, 1.0f},
			{3.5f, 1.0f}, {3.625f, 1.0f}, {3.75f, 1.0f}, {3.875f, 1.0f}
        };

        // Expected normal vectors for each face (same normal for all vertices in a face)
    	constexpr glm::vec3 expectedNormals[32] = {
    		{0.707107f, 0.0f, -0.707107f}, {1.19249e-08f, 0.0f, -1.0f},
			{-0.707107f, 0.0f, -0.707107f}, {-1.0f, 0.0f, -8.74228e-08f},
			{-0.707107f, 0.0f, 0.707107f}, {-4.37114e-08f, 0.0f, 1.0f},
			{0.707107f, 0.0f, 0.707107f}, {1.0f, 0.0f, 0.0f},
			{0.707107f, 0.0f, -0.707107f}, {1.19249e-08f, 0.0f, -1.0f},
			{-0.707107f, 0.0f, -0.707107f}, {-1.0f, 0.0f, -8.74228e-08f},
			{-0.707107f, 0.0f, 0.707107f}, {-4.37114e-08f, 0.0f, 1.0f},
			{0.707107f, 0.0f, 0.707107f}, {1.0f, 0.0f, 0.0f},
			{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
			{0.0f, 1.0f, 0.0f}, {0.0f, 1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f},
			{0.0f, -1.0f, 0.0f}, {0.0f, -1.0f, 0.0f}
		};

    	constexpr int expectedIndices[28*3] = {
    		0, 1, 8,
			1, 9, 8,
			1, 2, 9,
			2, 10, 9,
			2, 3, 10,
			3, 11, 10,
			3, 4, 11,
			4, 12, 11,
			4, 5, 12,
			5, 13, 12,
			5, 6, 13,
			6, 14, 13,
			6, 7, 14,
			7, 15, 14,
			7, 0, 15,
			0, 8, 15,
			16, 19, 22,
			16, 17, 18,
			16, 18, 19,
			19, 20, 21,
			19, 21, 22,
			22, 23, 16,
			24, 27, 30,
			24, 25, 26,
			24, 26, 27,
			27, 28, 29,
			27, 29, 30,
			30, 31, 24
    	};

        // Check vertex data
        for (unsigned int i = 0; i < 32; ++i)
        {
            // Compare the vertex position
        	std::cout << "Vertex " << i << ": " << vertexData[i].position.x << ", "
					  << vertexData[i].position.y << ", " << vertexData[i].position.z << std::endl;
            EXPECT_VEC3_NEAR(vertexData[i].position, expectedPositions[i], 0.01f);
            // Compare texture coordinates
            EXPECT_VEC2_NEAR(vertexData[i].texCoord, expectedTexCoords[i], 0.01f);
            // Compare normals
            // EXPECT_VEC3_NEAR(vertexData[i].normal, expectedNormals[i], 0.01f);
            // Check that the entityID was correctly set (here we passed -1)
            EXPECT_EQ(vertexData[i].entityID, -1);
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Validate index buffer content:
        GLuint indexBufferId = renderer3D->getInternalStorage()->indexBuffer->getId();
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
        std::vector<unsigned int> indexData(28*3); // Expecting 28*3 indices
        glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 28*3 * sizeof(unsigned int), indexData.data());

        for (unsigned int i = 0; i < 28*3; ++i)
        {
            EXPECT_EQ(indexData[i], expectedIndices[i]);
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

	TEST_F(Renderer3DTest, DrawCylinder8WithMaterial)
    {
    	glm::vec3 position = {0.0f, 0.0f, 0.0f};
    	glm::vec3 size = {1.0f, 1.0f, 1.0f};

    	components::Material material;
    	material.albedoColor = {1.0f, 0.0f, 0.0f, 1.0f}; // Red color
    	material.albedoTexture = Texture2D::create(4, 4); // Example texture

    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawCylinder(position, size, material));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 28); // A cylinder is made of 28 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
    	// Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 32); // 1 cylinder * 32 vertices per cylinder (as defined in struct)
    	// EXPECT_EQ(stats.getTotalIndexCount(), 28); // 1 cylinder * 28 indices
    }

	TEST_F(Renderer3DTest, DrawCylinder8WithRotation)
    {
	    constexpr glm::vec3 position = {1.0f, 2.0f, 3.0f};
    	constexpr glm::vec3 size = {2.0f, 2.0f, 2.0f};
    	constexpr glm::vec3 rotation = {45.0f, 30.0f, 60.0f};
    	constexpr glm::vec4 color = {0.0f, 1.0f, 0.0f, 1.0f}; // Green color

    	// Use an OpenGL query to count the number of triangles drawn
    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawCylinder(position, size, rotation, color));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 28); // A cylinder is made of 28 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
	    // const Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 32); // 1 cylinder * 32 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 28); // 1 cylinder * 28 indices
    }

	TEST_F(Renderer3DTest, DrawCylinder8WithTransformMatrix)
    {
    	const glm::mat4 transform = glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 3.0f}) *
							 glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});
    	constexpr glm::vec4 color = {0.0f, 0.0f, 1.0f, 1.0f}; // Blue color

    	// Use an OpenGL query to count the number of triangles drawn
    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawCylinder(transform, color));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 28); // A cylinder is made of 28 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
	    // const Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 32); // 1 cylinder * 32 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 28); // 1 cylinder * 28 indices
    }

	TEST_F(Renderer3DTest, DrawCylinder8WithRotationAndMaterial)
    {
    	constexpr glm::vec3 position = {1.0f, 2.0f, 3.0f};
    	constexpr glm::vec3 size = {2.0f, 2.0f, 2.0f};
    	constexpr glm::vec3 rotation = {45.0f, 30.0f, 60.0f};

    	components::Material material;
    	material.albedoColor = {0.0f, 1.0f, 1.0f, 1.0f}; // Cyan color
    	material.albedoTexture = Texture2D::create(4, 4); // Example texture
    	material.specularColor = {1.0f, 1.0f, 1.0f, 1.0f};
    	material.metallicMap = Texture2D::create(2, 2); // Example specular texture

    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawCylinder(position, size, rotation, material));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 28); // A cylinder is made of 28 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
    	// Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 32); // 1 cylinder * 32 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 28); // 1 cylinder * 28 indices
    }

	TEST_F(Renderer3DTest, DrawCylinder8WithTransformAndMaterial)
    {
    	glm::mat4 transform = glm::translate(glm::mat4(1.0f), {1.0f, 2.0f, 3.0f}) *
							  glm::rotate(glm::mat4(1.0f), glm::radians(45.0f), {0.0f, 1.0f, 0.0f}) *
							  glm::scale(glm::mat4(1.0f), {2.0f, 2.0f, 2.0f});

    	components::Material material;
    	material.albedoColor = {1.0f, 1.0f, 0.0f, 1.0f}; // Yellow color
    	material.albedoTexture = Texture2D::create(4, 4); // Example texture

    	GLuint query;
    	glGenQueries(1, &query);
    	glBeginQuery(GL_PRIMITIVES_GENERATED, query);

    	renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
    	EXPECT_NO_THROW(renderer3D->drawCylinder(transform, material));
    	renderer3D->endScene();

    	// Validate number of primitives drawn
    	glEndQuery(GL_PRIMITIVES_GENERATED);
    	GLuint primitivesGenerated = 0;
    	glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
    	EXPECT_EQ(primitivesGenerated, 28); // A cylinder is made of 28 triangles
    	glDeleteQueries(1, &query);

    	// Validate render stats
    	// Renderer3DStats stats = renderer3D->getStats();
    	// EXPECT_EQ(stats.cubeCount, 1);
    	// EXPECT_EQ(stats.getTotalVertexCount(), 32); // 1 cylinder * 32 vertices
    	// EXPECT_EQ(stats.getTotalIndexCount(), 28); // 1 cylinder * 28 indices
    }


	TEST_F(Renderer3DTest, DrawMesh)
	{
	    // // Create a simple mesh (a triangle)
	    // std::vector<NxVertex> vertices = {
	    //     {{-0.5f, -0.5f, 0.0f}, {0.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, -1},
	    //     {{ 0.5f, -0.5f, 0.0f}, {1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, -1},
	    //     {{ 0.0f,  0.5f, 0.0f}, {0.5f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, -1}
	    // };
	    // std::vector<unsigned int> indices = {0, 1, 2};
	    // auto texture = NxTexture2D::create(4, 4);

	    // // Use an OpenGL query to count the number of triangles drawn
	    // GLuint query;
	    // glGenQueries(1, &query);
	    // glBeginQuery(GL_PRIMITIVES_GENERATED, query);

	    // renderer3D->beginScene(glm::mat4(1.0f), {0.0f, 0.0f, 0.0f});
	    // EXPECT_NO_THROW(renderer3D->drawMesh(vertices, indices, texture));
	    // renderer3D->endScene();

	    // // Validate number of primitives drawn
	    // glEndQuery(GL_PRIMITIVES_GENERATED);
	    // GLuint primitivesGenerated = 0;
	    // glGetQueryObjectuiv(query, GL_QUERY_RESULT, &primitivesGenerated);
	    // EXPECT_EQ(primitivesGenerated, 1); // One triangle
	    // glDeleteQueries(1, &query);

	    // // Validate vertex buffer data
	    // GLuint vertexBufferId = renderer3D->getInternalStorage()->vertexBuffer->getId();
	    // glBindBuffer(GL_ARRAY_BUFFER, vertexBufferId);
	    // std::vector<NxVertex> vertexData(3); // Expecting 3 vertices for a triangle
	    // glGetBufferSubData(GL_ARRAY_BUFFER, 0, 3 * sizeof(NxVertex), vertexData.data());

	    // // Check vertex data
	    // for (unsigned int i = 0; i < 3; ++i)
	    // {
	    //     // Compare the vertex position
	    //     EXPECT_VEC3_NEAR(vertexData[i].position, vertices[i].position, 0.01f);
	    //     // Compare texture coordinates
	    //     EXPECT_VEC2_NEAR(vertexData[i].texCoord, vertices[i].texCoord, 0.01f);
	    //     // Compare normals
	    //     EXPECT_VEC3_NEAR(vertexData[i].normal, vertices[i].normal, 0.01f);
	    //     // Check that the entityID was correctly set
	    //     EXPECT_EQ(vertexData[i].entityID, -1);
	    // }
	    // glBindBuffer(GL_ARRAY_BUFFER, 0);

	    // // Validate index buffer content
	    // GLuint indexBufferId = renderer3D->getInternalStorage()->indexBuffer->getId();
	    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferId);
	    // std::vector<unsigned int> indexData(3); // Expecting 3 indices for a triangle
	    // glGetBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, 3 * sizeof(unsigned int), indexData.data());

	    // // Check indices (should be 0, 1, 2)
	    // for (unsigned int i = 0; i < 3; ++i)
	    // {
	    //     EXPECT_EQ(indexData[i], i);
	    // }
	    // glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
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

	TEST_F(Renderer3DTest, DrawCubeWithoutBeginScene)
	{
	    // glm::vec3 position = {0.0f, 0.0f, 0.0f};
	    // glm::vec3 size = {1.0f, 1.0f, 1.0f};
	    // glm::vec4 color = {1.0f, 0.0f, 0.0f, 1.0f};

	    // EXPECT_THROW(renderer3D->drawCube(position, size, color), NxRendererSceneLifeCycleFailure);
	}
}
