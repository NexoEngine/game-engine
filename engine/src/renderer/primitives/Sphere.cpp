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
    // unsigned int VERTEX_NUMBER = 12;
    // Number of vertex for the sphere min 12 then VERTEX_NUMBER += (old) VERTEX_NUMBER + (old) nb of faces - 2

    static void normalizeVertices(std::vector<glm::vec3>& vertices)
    {
        for (auto& vertex : vertices)
        {
            vertex = glm::normalize(vertex);
        }
    }

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

    static std::vector<glm::vec2> generateTextureCoords(const std::vector<glm::vec3>& vertices)
    {
        std::vector<glm::vec2> texCoords{};

        for (int i = 0; i < vertices.size(); ++i)
        {
            const glm::vec3 p = vertices[i];

            float u = (atan2(p.z, p.x) + M_PI) / (2 * M_PI);
            float v = acos(p.y) / M_PI;

            texCoords.emplace_back(u, v);
        }
        return texCoords;
    }

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

    unsigned int getNbVerticesSphere(const unsigned int nbSubdivision)
    {
        if (nbSubdivision == 0)
            return 12; // Base case with 12 vertices
        return 12 * static_cast<unsigned int>(std::pow(4, nbSubdivision));
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
        }

        vertexBuffer->setData(vertexData.data(), vertexData.size() * sizeof(NxVertex));
        sphereVaoMap[nbSubdivision]->addVertexBuffer(vertexBuffer);

        const auto indexBuffer = createIndexBuffer();
        indexBuffer->setData(indices.data(), indices.size());
        sphereVaoMap[nbSubdivision]->setIndexBuffer(indexBuffer);

        return sphereVaoMap[nbSubdivision];
    }
}
