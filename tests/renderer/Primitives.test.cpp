///////////////////////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Claude Code
//  Date:        12/12/2025
//  Description: Test file for renderer primitive geometry generation
//
///////////////////////////////////////////////////////////////////////////////

#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include <array>
#include <cmath>
#include <vector>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

namespace nexo::renderer {

    // Test helper functions to access geometry generation
    // These replicate the static functions from the source files for testing purposes

    /**
     * @brief Generates the vertex, texture coordinate, and normal data for a cube mesh.
     * Replicates the static genCubeMesh function from Cube.cpp for testing.
     */
    void testGenCubeMesh(std::array<glm::vec3, 36>& vertices, std::array<glm::vec2, 36>& texCoords,
                        std::array<glm::vec3, 36>& normals)
    {
        float x = 0.5f;
        float y = 0.5f;
        float z = 0.5f;

        glm::vec3 a0 = {+x, +y, +z}; // Front face top right
        glm::vec3 a1 = {-x, +y, +z}; // Front face top left
        glm::vec3 a2 = {-x, -y, +z}; // Front face bottom left
        glm::vec3 a3 = {+x, -y, +z}; // Front face bottom right
        glm::vec3 a4 = {+x, +y, -z}; // Back face top right
        glm::vec3 a5 = {-x, +y, -z}; // Back face top left
        glm::vec3 a6 = {-x, -y, -z}; // Back face bottom left
        glm::vec3 a7 = {+x, -y, -z}; // Back face bottom right

        glm::vec3 verts[] = {
            // Front face (Z+)
            a0, a1, a2, a0, a2, a3,
            // Back face (Z-)
            a4, a7, a6, a4, a6, a5,
            // Top face (Y+)
            a0, a4, a5, a0, a5, a1,
            // Bottom face (Y-)
            a3, a2, a6, a3, a6, a7,
            // Right face (X+)
            a0, a3, a7, a0, a7, a4,
            // Left face (X-)
            a1, a5, a6, a1, a6, a2
        };

        std::ranges::copy(verts, vertices.begin());

        glm::vec2 texturesCoord[] = {
            glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1),
            glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1),
            glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1),
            glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1),
            glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1),
            glm::vec2(0, 1), glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1),
        };

        std::ranges::copy(texturesCoord, texCoords.begin());
        glm::vec3 norm[36];

        for (int i = 0; i < 36; i += 3)
        {
            glm::vec3 normal = glm::normalize(
                glm::cross(
                    glm::vec3(verts[i + 1]) - glm::vec3(verts[i]),
                    glm::vec3(verts[i + 2]) - glm::vec3(verts[i])));

            norm[i] = normal;
            norm[i + 1] = normal;
            norm[i + 2] = normal;
        }

        std::ranges::copy(norm, normals.begin());
    }

    /**
     * @brief Generates the vertex, texture coordinate, and normal data for a pyramid mesh.
     * Replicates the static genPyramidMesh function from Pyramid.cpp for testing.
     */
    void testGenPyramidMesh(std::array<glm::vec3, 18>& vertices,
                           std::array<glm::vec2, 18>& texCoords,
                           std::array<glm::vec3, 18>& normals)
    {
        constexpr glm::vec3 v0 = {0.0f, 1.0f, 0.0f}; // Top vertex
        constexpr glm::vec3 v1 = {-1.0f, -1.0f, -1.0f}; // Bottom-left-back
        constexpr glm::vec3 v2 = {1.0f, -1.0f, -1.0f}; // Bottom-right-back
        constexpr glm::vec3 v3 = {1.0f, -1.0f, 1.0f}; // Bottom-right-front
        constexpr glm::vec3 v4 = {-1.0f, -1.0f, 1.0f}; // Bottom-left-front

        glm::vec3 verts[] = {
            v1, v2, v3, v1, v3, v4, // Base face
            // Side faces
            v0, v2, v1,
            v0, v3, v2,
            v0, v4, v3,
            v0, v1, v4
        };

        std::ranges::copy(verts, vertices.begin());

        glm::vec2 texturesCoord[] = {
            // Base face
            {0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
            {0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            // Side faces
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}
        };

        std::ranges::copy(texturesCoord, texCoords.begin());

        glm::vec3 norm[18];
        for (int i = 0; i < 18; i += 3)
        {
            const glm::vec3 normal = glm::normalize(
                glm::cross(
                    verts[i + 1] - verts[i],
                    verts[i + 2] - verts[i]));

            norm[i] = normal;
            norm[i + 1] = normal;
            norm[i + 2] = normal;
        }

        std::ranges::copy(norm, normals.begin());
    }

    /**
     * @brief Generates icosahedron vertices for a sphere.
     * Replicates the static generateSphereVertices function from Sphere.cpp for testing.
     */
    std::vector<glm::vec3> testGenerateSphereVertices()
    {
        std::vector<glm::vec3> vertices{};
        vertices.reserve(12);

        const float phi = (1.0f + sqrtf(5.0f)) * 0.5f; // golden ratio
        float a = 1.0f;
        float b = 1.0f / phi;

        vertices.emplace_back(0, b, -a);
        vertices.emplace_back(b, a, 0);
        vertices.emplace_back(-b, a, 0);
        vertices.emplace_back(0, b, a);
        vertices.emplace_back(0, -b, a);
        vertices.emplace_back(-a, 0, b);
        vertices.emplace_back(0, -b, -a);
        vertices.emplace_back(a, 0, -b);
        vertices.emplace_back(a, 0, b);
        vertices.emplace_back(-a, 0, -b);
        vertices.emplace_back(b, -a, 0);
        vertices.emplace_back(-b, -a, 0);

        // Normalize vertices
        for (auto& vertex : vertices)
        {
            vertex = glm::normalize(vertex);
        }

        return vertices;
    }

    /**
     * @brief Calculates the number of vertices in a sphere after subdivision.
     * Replicates the getNbVerticesSphere function from Sphere.cpp for testing.
     */
    unsigned int testGetNbVerticesSphere(const unsigned int nbSubdivision)
    {
        return 10 * static_cast<unsigned int>(std::pow(4, nbSubdivision)) + 2;
    }

    /**
     * @brief Generates cylinder vertices.
     * Replicates the static generateCylinderVertices function from Cylinder.cpp for testing.
     */
    std::vector<glm::vec3> testGenerateCylinderVertices(const unsigned int nbSegment)
    {
        constexpr float CYLINDER_HEIGHT = 1.0f;
        std::vector<glm::vec3> vertices{};
        vertices.reserve(nbSegment * 4);

        unsigned int i = 0;
        for (unsigned int k = nbSegment - 1; i < nbSegment; ++i, --k)
        {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);
            vertices.emplace_back(x, CYLINDER_HEIGHT, z);
        }
        for (unsigned int k = nbSegment - 1; i < nbSegment * 2; ++i, --k)
        {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);
            vertices.emplace_back(x, -CYLINDER_HEIGHT, z);
        }
        for (int k = 0; i < nbSegment * 3; ++i, ++k)
        {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);
            vertices.emplace_back(x, CYLINDER_HEIGHT, z);
        }
        for (int k = 0; i < nbSegment * 4; ++i, ++k)
        {
            const float angle = static_cast<float>(k) / static_cast<float>(nbSegment) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);
            vertices.emplace_back(x, -CYLINDER_HEIGHT, z);
        }
        return vertices;
    }

    // ============================================================================
    // Cube Mesh Tests
    // ============================================================================

    TEST(PrimitiveGeometryTest, CubeVertexCount)
    {
        std::array<glm::vec3, 36> vertices{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};

        testGenCubeMesh(vertices, texCoords, normals);

        // Cube should have exactly 36 vertices (6 faces * 2 triangles * 3 vertices)
        EXPECT_EQ(vertices.size(), 36);
        EXPECT_EQ(texCoords.size(), 36);
        EXPECT_EQ(normals.size(), 36);
    }

    TEST(PrimitiveGeometryTest, CubeVertexPositions)
    {
        std::array<glm::vec3, 36> vertices{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};

        testGenCubeMesh(vertices, texCoords, normals);

        // Check that all vertices are within the expected bounds [-0.5, 0.5]
        for (const auto& vertex : vertices)
        {
            EXPECT_GE(vertex.x, -0.5f);
            EXPECT_LE(vertex.x, 0.5f);
            EXPECT_GE(vertex.y, -0.5f);
            EXPECT_LE(vertex.y, 0.5f);
            EXPECT_GE(vertex.z, -0.5f);
            EXPECT_LE(vertex.z, 0.5f);
        }

        // Verify that corners exist at expected positions
        // Check first triangle of front face
        EXPECT_FLOAT_EQ(vertices[0].x, 0.5f);
        EXPECT_FLOAT_EQ(vertices[0].y, 0.5f);
        EXPECT_FLOAT_EQ(vertices[0].z, 0.5f);
    }

    TEST(PrimitiveGeometryTest, CubeNormalsUnitLength)
    {
        std::array<glm::vec3, 36> vertices{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};

        testGenCubeMesh(vertices, texCoords, normals);

        // All normals should be unit length
        for (const auto& normal : normals)
        {
            float length = glm::length(normal);
            EXPECT_NEAR(length, 1.0f, 1e-5f);
        }
    }

    TEST(PrimitiveGeometryTest, CubeUVCoordinates)
    {
        std::array<glm::vec3, 36> vertices{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};

        testGenCubeMesh(vertices, texCoords, normals);

        // All UV coordinates should be in range [0, 1]
        for (const auto& texCoord : texCoords)
        {
            EXPECT_GE(texCoord.x, 0.0f);
            EXPECT_LE(texCoord.x, 1.0f);
            EXPECT_GE(texCoord.y, 0.0f);
            EXPECT_LE(texCoord.y, 1.0f);
        }
    }

    TEST(PrimitiveGeometryTest, CubeFaceNormals)
    {
        std::array<glm::vec3, 36> vertices{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};

        testGenCubeMesh(vertices, texCoords, normals);

        // Front face (first 6 vertices) should have normals pointing in +Z direction
        for (int i = 0; i < 6; ++i)
        {
            EXPECT_NEAR(normals[i].z, 1.0f, 1e-5f);
            EXPECT_NEAR(normals[i].x, 0.0f, 1e-5f);
            EXPECT_NEAR(normals[i].y, 0.0f, 1e-5f);
        }
    }

    // ============================================================================
    // Pyramid Mesh Tests
    // ============================================================================

    TEST(PrimitiveGeometryTest, PyramidVertexCount)
    {
        std::array<glm::vec3, 18> vertices{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};

        testGenPyramidMesh(vertices, texCoords, normals);

        // Pyramid should have exactly 18 vertices (4 side faces + 1 base = 6 triangles * 3 vertices)
        EXPECT_EQ(vertices.size(), 18);
        EXPECT_EQ(texCoords.size(), 18);
        EXPECT_EQ(normals.size(), 18);
    }

    TEST(PrimitiveGeometryTest, PyramidApexPosition)
    {
        std::array<glm::vec3, 18> vertices{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};

        testGenPyramidMesh(vertices, texCoords, normals);

        // The apex should be at (0, 1, 0)
        // Check vertices 6, 9, 12, 15 which are the apex vertices in each side face
        for (int i = 6; i < 18; i += 3)
        {
            EXPECT_FLOAT_EQ(vertices[i].x, 0.0f);
            EXPECT_FLOAT_EQ(vertices[i].y, 1.0f);
            EXPECT_FLOAT_EQ(vertices[i].z, 0.0f);
        }
    }

    TEST(PrimitiveGeometryTest, PyramidBaseVertices)
    {
        std::array<glm::vec3, 18> vertices{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};

        testGenPyramidMesh(vertices, texCoords, normals);

        // All base vertices should have y = -1
        for (int i = 0; i < 6; ++i)
        {
            EXPECT_FLOAT_EQ(vertices[i].y, -1.0f);
        }
    }

    TEST(PrimitiveGeometryTest, PyramidNormalsUnitLength)
    {
        std::array<glm::vec3, 18> vertices{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};

        testGenPyramidMesh(vertices, texCoords, normals);

        // All normals should be unit length
        for (const auto& normal : normals)
        {
            float length = glm::length(normal);
            EXPECT_NEAR(length, 1.0f, 1e-5f);
        }
    }

    TEST(PrimitiveGeometryTest, PyramidUVCoordinates)
    {
        std::array<glm::vec3, 18> vertices{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};

        testGenPyramidMesh(vertices, texCoords, normals);

        // All UV coordinates should be in range [0, 1]
        for (const auto& texCoord : texCoords)
        {
            EXPECT_GE(texCoord.x, 0.0f);
            EXPECT_LE(texCoord.x, 1.0f);
            EXPECT_GE(texCoord.y, 0.0f);
            EXPECT_LE(texCoord.y, 1.0f);
        }
    }

    // ============================================================================
    // Sphere Mesh Tests
    // ============================================================================

    TEST(PrimitiveGeometryTest, SphereInitialVertexCount)
    {
        auto vertices = testGenerateSphereVertices();

        // Icosahedron has exactly 12 vertices
        EXPECT_EQ(vertices.size(), 12);
    }

    TEST(PrimitiveGeometryTest, SphereVerticesNormalized)
    {
        auto vertices = testGenerateSphereVertices();

        // All vertices should lie on unit sphere (length = 1)
        for (const auto& vertex : vertices)
        {
            float length = glm::length(vertex);
            EXPECT_NEAR(length, 1.0f, 1e-5f);
        }
    }

    TEST(PrimitiveGeometryTest, SphereVertexCountFormula)
    {
        // Test the vertex count formula for different subdivision levels
        EXPECT_EQ(testGetNbVerticesSphere(0), 12u); // 10 * 4^0 + 2 = 12
        EXPECT_EQ(testGetNbVerticesSphere(1), 42u); // 10 * 4^1 + 2 = 42
        EXPECT_EQ(testGetNbVerticesSphere(2), 162u); // 10 * 4^2 + 2 = 162
        EXPECT_EQ(testGetNbVerticesSphere(3), 642u); // 10 * 4^3 + 2 = 642
    }

    TEST(PrimitiveGeometryTest, SphereGoldenRatioVertices)
    {
        auto vertices = testGenerateSphereVertices();

        // Verify that vertices follow the icosahedron pattern
        // Check that we have vertices with golden ratio proportions
        const float phi = (1.0f + sqrtf(5.0f)) * 0.5f;
        bool foundGoldenRatioVertex = false;

        for (const auto& vertex : vertices)
        {
            // Check if any component is close to the normalized golden ratio value
            float normalizedPhi = phi / sqrtf(1.0f + phi * phi);
            if (std::abs(std::abs(vertex.x) - normalizedPhi) < 1e-5f ||
                std::abs(std::abs(vertex.y) - normalizedPhi) < 1e-5f ||
                std::abs(std::abs(vertex.z) - normalizedPhi) < 1e-5f)
            {
                foundGoldenRatioVertex = true;
                break;
            }
        }

        EXPECT_TRUE(foundGoldenRatioVertex);
    }

    // ============================================================================
    // Cylinder Mesh Tests
    // ============================================================================

    TEST(PrimitiveGeometryTest, CylinderVertexCount)
    {
        const unsigned int segments = 8;
        auto vertices = testGenerateCylinderVertices(segments);

        // Cylinder should have 4 * segments vertices (top cap, bottom cap, top side, bottom side)
        EXPECT_EQ(vertices.size(), segments * 4);
    }

    TEST(PrimitiveGeometryTest, CylinderHeightPositions)
    {
        const unsigned int segments = 8;
        auto vertices = testGenerateCylinderVertices(segments);

        // First segment * 1 vertices should be at y = 1.0 (top cap)
        for (unsigned int i = 0; i < segments; ++i)
        {
            EXPECT_FLOAT_EQ(vertices[i].y, 1.0f);
        }

        // Second segment vertices should be at y = -1.0 (bottom cap)
        for (unsigned int i = segments; i < segments * 2; ++i)
        {
            EXPECT_FLOAT_EQ(vertices[i].y, -1.0f);
        }

        // Third segment vertices should be at y = 1.0 (top side)
        for (unsigned int i = segments * 2; i < segments * 3; ++i)
        {
            EXPECT_FLOAT_EQ(vertices[i].y, 1.0f);
        }

        // Fourth segment vertices should be at y = -1.0 (bottom side)
        for (unsigned int i = segments * 3; i < segments * 4; ++i)
        {
            EXPECT_FLOAT_EQ(vertices[i].y, -1.0f);
        }
    }

    TEST(PrimitiveGeometryTest, CylinderRadialPositions)
    {
        const unsigned int segments = 16;
        auto vertices = testGenerateCylinderVertices(segments);

        // All vertices should lie on a circle of radius 1 in the XZ plane
        for (const auto& vertex : vertices)
        {
            float radialDistance = sqrtf(vertex.x * vertex.x + vertex.z * vertex.z);
            EXPECT_NEAR(radialDistance, 1.0f, 1e-5f);
        }
    }

    TEST(PrimitiveGeometryTest, CylinderCircularDistribution)
    {
        const unsigned int segments = 8;
        auto vertices = testGenerateCylinderVertices(segments);

        // Verify all vertices in first segment have radius 1
        for (unsigned int i = 0; i < segments; ++i)
        {
            float radius = sqrtf(vertices[i].x * vertices[i].x + vertices[i].z * vertices[i].z);
            EXPECT_NEAR(radius, 1.0f, 1e-5f);
        }
    }

    TEST(PrimitiveGeometryTest, CylinderMinimumSegments)
    {
        // Test with minimum reasonable segments (triangle)
        const unsigned int segments = 3;
        auto vertices = testGenerateCylinderVertices(segments);

        EXPECT_EQ(vertices.size(), segments * 4);

        // All vertices should still be on unit circle
        for (const auto& vertex : vertices)
        {
            float radialDistance = sqrtf(vertex.x * vertex.x + vertex.z * vertex.z);
            EXPECT_NEAR(radialDistance, 1.0f, 1e-5f);
        }
    }

    // ============================================================================
    // Integration Tests - Geometric Properties
    // ============================================================================

    TEST(PrimitiveGeometryTest, CubeSymmetry)
    {
        std::array<glm::vec3, 36> vertices{};
        std::array<glm::vec2, 36> texCoords{};
        std::array<glm::vec3, 36> normals{};

        testGenCubeMesh(vertices, texCoords, normals);

        // Check that cube has vertices at all 8 corners
        std::set<std::tuple<int, int, int>> corners;
        for (const auto& v : vertices)
        {
            int x = (v.x > 0) ? 1 : -1;
            int y = (v.y > 0) ? 1 : -1;
            int z = (v.z > 0) ? 1 : -1;
            corners.insert(std::make_tuple(x, y, z));
        }

        // Should have all 8 corner combinations
        EXPECT_EQ(corners.size(), 8);
    }

    TEST(PrimitiveGeometryTest, PyramidBaseSquare)
    {
        std::array<glm::vec3, 18> vertices{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};

        testGenPyramidMesh(vertices, texCoords, normals);

        // The base should form a square at y = -1
        std::set<std::pair<float, float>> baseVertices;
        for (const auto& v : vertices)
        {
            if (std::abs(v.y - (-1.0f)) < 1e-5f)
            {
                baseVertices.insert(std::make_pair(v.x, v.z));
            }
        }

        // Should have 4 unique base vertices forming a square
        EXPECT_EQ(baseVertices.size(), 4);
    }

} // namespace nexo::renderer
