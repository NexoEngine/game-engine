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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace nexo::renderer
{
    static void normalizeVertices(std::vector<glm::vec3>& vertices)
    {
        for (auto& vertex : vertices)
        {
            vertex = normalize(vertex);
        }
    }

    static std::vector<glm::vec3> generateSphereVertices()
    {
        std::vector<glm::vec3> vertices{};
        vertices.reserve(12); // Reserve space for the 12 vertices of the icosahedron

        const float phi = (1.0f + sqrtf(5.0f)) * 0.5f; // golden ratio
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

    static std::vector<unsigned int> generateSphereIndices()
    {
        std::vector<unsigned int> indices = {
            2, 1, 0,
            1, 2, 3,
            5, 4, 3,
            4, 8, 3,
            7, 6, 0,
            6, 9, 0,
            11, 10, 4,
            10, 11, 6,
            9, 5, 2,
            5, 9, 11,
            8, 7, 1,
            7, 8, 10,
            2, 5, 3,
            8, 1, 3,
            9, 2, 0,
            1, 7, 0,
            11, 9, 6,
            7, 10, 6,
            5, 11, 4,
            10, 8, 4
        };

        return indices;
    }

    struct Vec3Comparator
    {
        bool operator()(const glm::vec3& a, const glm::vec3& b) const
        {
            if (a.x != b.x) return a.x < b.x;
            if (a.y != b.y) return a.y < b.y;
            return a.z < b.z;
        }
    };

    void loopSubdivision(std::vector<unsigned int>& indices, std::vector<glm::vec3>& vertices,
                                const unsigned int nbSubdivision)
    {
        for (unsigned int i = 0; i < nbSubdivision; ++i)
        {
            std::vector<unsigned int> newIndices{};
            std::map<glm::vec3, unsigned int, Vec3Comparator> newVertices{};

            for (size_t j = 0; j < indices.size(); j += 3)
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
                    newVertices.insert({m1_pos, static_cast<unsigned int>(vertices.size() - 1)});
                }
                if (!newVertices.contains(m2_pos))
                {
                    vertices.emplace_back(m2_pos);
                    newVertices.insert({m2_pos, static_cast<unsigned int>(vertices.size() - 1)});
                }
                if (!newVertices.contains(m3_pos))
                {
                    vertices.emplace_back(m3_pos);
                    newVertices.insert({m3_pos, static_cast<unsigned int>(vertices.size() - 1)});
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

    static std::vector<glm::vec2> generateTextureCoords(const std::vector<glm::vec3>& vertices)
    {
        std::vector<glm::vec2> texCoords{};
        texCoords.reserve(vertices.size()); // Reserve space for texture coordinates

        for (const auto vertex : vertices)
        {
            auto u = (atan2(vertex.z, vertex.x) + M_PI) / (2 * M_PI);
            auto v = acos(vertex.y) / M_PI;

            texCoords.emplace_back(u, v);
        }
        return texCoords;
    }

    static std::vector<glm::vec3> generateSphereNormals(const std::vector<glm::vec3>& vertices)
    {
        std::vector<glm::vec3> normals{};
        normals.reserve(vertices.size()); // Reserve space for normals

        for (auto vec : vertices)
        {
            const glm::vec3 vector1 = vec - glm::vec3(0, 0, 0);
            normals.emplace_back(vector1);
        }
        return normals;
    }

    unsigned int getNbVerticesSphere(const unsigned int nbSubdivision)
    {
        return 10 * static_cast<unsigned int>(std::pow(4, nbSubdivision)) + 2;
    }

    /**
     * @brief Creates a vertex array object (VAO) for a sphere mesh.
     *
     * @return A shared pointer to a vertex array object containing the sphere mesh data.
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
            vertexData[i].tangent = glm::vec3(0.0f, 0.0f, 0.0f); // Default tangent
            vertexData[i].bitangent = glm::vec3(0.0f, 0.0f, 0.0f); // Default bi tangent
            vertexData[i].entityID = 0; // Default entity ID
        }

        vertexBuffer->setData(vertexData.data(), static_cast<unsigned int>(vertexData.size() * sizeof(NxVertex)));
        sphereVaoMap[nbSubdivision]->addVertexBuffer(vertexBuffer);

        const auto indexBuffer = createIndexBuffer();
        indexBuffer->setData(indices.data(), static_cast<unsigned int>(indices.size()));
        sphereVaoMap[nbSubdivision]->setIndexBuffer(indexBuffer);

        return sphereVaoMap[nbSubdivision];
    }
}
