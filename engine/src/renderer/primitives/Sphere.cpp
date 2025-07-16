//// Sphere.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzz  zzzzz
//  zzz zzz     zzz  zzzz               zzz  zzzz       zzzz          zzzz
//  zzz   zzz   zzz  zzzzzzzzz             zzzz        zzzz            zzzz
//  zzz    zzz  zzz  zzzz               zzzz  zzzz      zzzz          zzzz
//  zzz      zz zzz  zzzzzzzzzzzzz    zzzz       zzzz      zzzzz  zzzzz
//
//  Author:      Marie Giacomel
//  Date:        06/05/2025
//  Description: Source file for the sphere primitive
//
///////////////////////////////////////////////////////////////////////////////

#include "renderer/Renderer3D.hpp"

#include <algorithm>
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif
#ifndef M_PI_2
#define M_PI_2 1.57079632679489661923 // π/2
#endif
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <Logger.hpp>
#include <map>
#include <set>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace nexo::renderer
{
    /**
     * @brief Normalizes each vertex in the input vector to unit length.
     *
     * Modifies the input vector in place so that all vertices lie on the unit sphere.
     */
    static void normalizeVertices(std::vector<glm::vec3>& vertices)
    {
        for (auto& vertex : vertices)
        {
            vertex = glm::normalize(vertex);
        }
    }

    /**
     * @brief Generates the 12 vertices of an icosahedron normalized to form a unit sphere.
     *
     * @return std::vector<glm::vec3> List of 12 normalized vertex positions representing the initial sphere mesh.
     */
    static std::vector<glm::vec3> generateSphereVertices()
    {
        std::vector<glm::vec3> vertices{};
        const float phi = (1.0f + sqrt(5.0f)) * 0.5f; // golden ratio
        float a = 1.0f;
        float b = 1.0f / phi;

        vertices.emplace_back(0, b, -a); // 0
        vertices.emplace_back(b, a, 0); // 1
        vertices.emplace_back(-b, a, 0); // 2
        vertices.emplace_back(0, b, a); // 3
        vertices.emplace_back(0, -b, a); // 4
        vertices.emplace_back(-a, 0, b); // 5
        vertices.emplace_back(0, -b, -a); // 6
        vertices.emplace_back(a, 0, -b); // 7
        vertices.emplace_back(a, 0, b); // 8
        vertices.emplace_back(-a, 0, -b); // 9
        vertices.emplace_back(b, -a, 0); // 10
        vertices.emplace_back(-b, -a, 0); // 11

        // Normalize the vertices to create a unit sphere
        normalizeVertices(vertices);

        return vertices;
    }

    /**
     * @brief Returns the indices defining the 20 triangular faces of an icosahedron.
     *
     * The indices correspond to the initial faces of a sphere mesh before subdivision, using a fixed vertex ordering.
     *
     * @return std::vector<unsigned int> List of indices for the icosahedron's triangular faces.
     */
    static std::vector<unsigned int> generateSphereIndices()
    {
        std::vector<unsigned int> indices{};

        indices.push_back(2);
        indices.push_back(1);
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(2);
        indices.push_back(3);
        indices.push_back(5);
        indices.push_back(4);
        indices.push_back(3);
        indices.push_back(4);
        indices.push_back(8);
        indices.push_back(3);
        indices.push_back(7);
        indices.push_back(6);
        indices.push_back(0);
        indices.push_back(6);
        indices.push_back(9);
        indices.push_back(0);
        indices.push_back(11);
        indices.push_back(10);
        indices.push_back(4);
        indices.push_back(10);
        indices.push_back(11);
        indices.push_back(6);
        indices.push_back(9);
        indices.push_back(5);
        indices.push_back(2);
        indices.push_back(5);
        indices.push_back(9);
        indices.push_back(11);
        indices.push_back(8);
        indices.push_back(7);
        indices.push_back(1);
        indices.push_back(7);
        indices.push_back(8);
        indices.push_back(10);
        indices.push_back(2);
        indices.push_back(5);
        indices.push_back(3);
        indices.push_back(8);
        indices.push_back(1);
        indices.push_back(3);
        indices.push_back(9);
        indices.push_back(2);
        indices.push_back(0);
        indices.push_back(1);
        indices.push_back(7);
        indices.push_back(0);
        indices.push_back(11);
        indices.push_back(9);
        indices.push_back(6);
        indices.push_back(7);
        indices.push_back(10);
        indices.push_back(6);
        indices.push_back(5);
        indices.push_back(11);
        indices.push_back(4);
        indices.push_back(10);
        indices.push_back(8);
        indices.push_back(4);

        return indices;
    }

    struct Vec3Comparator
    {
        /**
         * @brief Compares two glm::vec3 objects for strict weak ordering.
         *
         * Returns true if vector a is less than vector b, comparing x, then y, then z components in order.
         * Useful for using glm::vec3 as keys in ordered containers such as std::map.
         *
         * @param a First vector to compare.
         * @param b Second vector to compare.
         * @return true if a is less than b; false otherwise.
         */
        bool operator()(const glm::vec3& a, const glm::vec3& b) const
        {
            if (a.x != b.x) return a.x < b.x;
            if (a.y != b.y) return a.y < b.y;
            return a.z < b.z;
        }
    };

    /**
     * @brief Refines a triangular mesh using Loop subdivision for a specified number of iterations.
     *
     * For each subdivision step, splits each triangle into four by inserting midpoints on each edge, adds new vertices as needed, and normalizes all vertices to maintain a spherical shape. Updates the indices to reflect the subdivided mesh.
     *
     * @param indices Reference to the triangle index list, updated in-place.
     * @param vertices Reference to the vertex position list, updated in-place.
     * @param nbSubdivision Number of subdivision iterations to perform.
     */
    void loopSubdivision(std::vector<unsigned int>& indices, std::vector<glm::vec3>& vertices,
                                const unsigned int nbSubdivision)
    {
        for (unsigned int i = 0; i < nbSubdivision; ++i)
        {
            std::vector<unsigned int> newIndices{};
            std::map<glm::vec3, unsigned int, Vec3Comparator> newVertices{};

            for (int j = 0; j < indices.size(); j += 3)
            {
                const unsigned int v1 = indices[j];
                const unsigned int v2 = indices[j + 1];
                const unsigned int v3 = indices[j + 2];

                const glm::vec3 m1_pos = (vertices[v1] + vertices[v2]) / 2.0f;
                const glm::vec3 m2_pos = (vertices[v2] + vertices[v3]) / 2.0f;
                const glm::vec3 m3_pos = (vertices[v1] + vertices[v3]) / 2.0f;

                if (!newVertices.contains(m1_pos))
                {
                    vertices.emplace_back(m1_pos);
                    newVertices.insert({m1_pos, vertices.size() - 1});
                }
                if (!newVertices.contains(m2_pos))
                {
                    vertices.emplace_back(m2_pos);
                    newVertices.insert({m2_pos, vertices.size() - 1});
                }
                if (!newVertices.contains(m3_pos))
                {
                    vertices.emplace_back(m3_pos);
                    newVertices.insert({m3_pos, vertices.size() - 1});
                }

                auto m1 = newVertices[m1_pos];
                auto m2 = newVertices[m2_pos];
                auto m3 = newVertices[m3_pos];

                // Triangle 1: v1, m1, m3
                newIndices.push_back(v1);
                newIndices.push_back(m1);
                newIndices.push_back(m3);
                // Triangle 2: m1, v2, m2
                newIndices.push_back(m1);
                newIndices.push_back(v2);
                newIndices.push_back(m2);
                // Triangle 3: m3, m2, v3
                newIndices.push_back(m3);
                newIndices.push_back(m2);
                newIndices.push_back(v3);
                // Triangle 4: m1, m2, m3
                newIndices.push_back(m1);
                newIndices.push_back(m2);
                newIndices.push_back(m3);
            }

            normalizeVertices(vertices);
            indices = std::move(newIndices);
        }
    }

    /**
     * @brief Generates spherical texture coordinates for a set of 3D vertices.
     *
     * Computes (u, v) coordinates for each vertex based on its position on the unit sphere, using longitude and latitude mapping.
     *
     * @param vertices List of 3D positions representing points on a sphere.
     * @return std::vector<glm::vec2> Texture coordinates corresponding to each input vertex.
     */
    static std::vector<glm::vec2> generateTextureCoords(const std::vector<glm::vec3>& vertices)
    {
        std::vector<glm::vec2> texCoords{};

        for (const auto vertex : vertices)
        {
            float u = (atan2(vertex.z, vertex.x) + M_PI) / (2 * M_PI);
            float v = acos(vertex.y) / M_PI;

            texCoords.emplace_back(u, v);
        }
        return texCoords;
    }

    /**
     * @brief Generates normal vectors for a sphere mesh based on vertex positions.
     *
     * Each normal is computed as the vector from the origin to the corresponding vertex, matching the normalized vertex direction for a unit sphere.
     *
     * @param vertices The positions of the sphere's vertices.
     * @return std::vector<glm::vec3> The normal vectors for each vertex.
     */
    static std::vector<glm::vec3> generateSphereNormals(const std::vector<glm::vec3>& vertices)
    {
        std::vector<glm::vec3> normals{};
        for (auto vec : vertices)
        {
            const glm::vec3 vector1 = vec - glm::vec3(0, 0, 0);
            normals.emplace_back(vector1);
        }
        return normals;
    }

    /**
     * @brief Calculates the number of vertices in a subdivided icosahedron-based sphere mesh.
     *
     * @param nbSubdivision The number of Loop subdivision steps applied to the initial icosahedron.
     * @return unsigned int The total number of vertices after the specified number of subdivisions.
     */
    unsigned int getNbVerticesSphere(const unsigned int nbSubdivision)
    {
        return 10 * static_cast<unsigned int>(std::pow(4, nbSubdivision)) + 2;
    }

    /**
     * @brief Returns a vertex array object (VAO) representing a unit sphere mesh with the specified subdivision level.
     *
     * Generates or retrieves a cached VAO containing vertex and index buffers for a sphere mesh refined using Loop subdivision. The mesh includes positions, texture coordinates, normals, tangents, bitangents, and entity IDs. The VAO is suitable for rendering a smooth sphere in 3D scenes.
     *
     * @param nbSubdivision Number of Loop subdivision iterations to refine the sphere mesh.
     * @return Shared pointer to the VAO containing the sphere mesh data.
     */
    std::shared_ptr<NxVertexArray> NxRenderer3D::getSphereVAO(const unsigned int nbSubdivision)
    {
        static std::map <unsigned int, std::shared_ptr<NxVertexArray>> sphereVaoMap;
        if (sphereVaoMap.contains(nbSubdivision))
            return sphereVaoMap[nbSubdivision];

        const unsigned int nbVertices = getNbVerticesSphere(nbSubdivision);
        sphereVaoMap[nbSubdivision] = createVertexArray();
        const auto vertexBuffer = createVertexBuffer(nbVertices * sizeof(NxVertex));
        const NxBufferLayout sphereVertexBufferLayout = {
            {NxShaderDataType::FLOAT3, "aPos"},
            {NxShaderDataType::FLOAT2, "aTexCoord"},
            {NxShaderDataType::FLOAT3, "aNormal"},
            {NxShaderDataType::FLOAT3, "aTangent"},
            {NxShaderDataType::FLOAT3, "aBiTangent"},
            {NxShaderDataType::INT, "aEntityID"}
        };
        vertexBuffer->setLayout(sphereVertexBufferLayout);

        std::vector<glm::vec3> vertices = generateSphereVertices();
        std::vector<unsigned int> indices = generateSphereIndices();

        loopSubdivision(indices, vertices, nbSubdivision);

        const std::vector<glm::vec3> normals = generateSphereNormals(vertices);
        const std::vector<glm::vec2> texCoords = generateTextureCoords(vertices);

        std::vector<NxVertex> vertexData(nbVertices);
        for (unsigned int i = 0; i < nbVertices; ++i) {
            vertexData[i].position = glm::vec4(vertices[i], 1.0f);
            vertexData[i].texCoord = texCoords[i];
            vertexData[i].normal = normals[i];
        }

        vertexBuffer->setData(vertexData.data(), vertexData.size() * sizeof(NxVertex));
        sphereVaoMap[nbSubdivision]->addVertexBuffer(vertexBuffer);

        const auto indexBuffer = createIndexBuffer();
        indexBuffer->setData(indices.data(), indices.size());
        sphereVaoMap[nbSubdivision]->setIndexBuffer(indexBuffer);

        return sphereVaoMap[nbSubdivision];
    }
}
