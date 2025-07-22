//// Cube.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2025
//  Description: Source file for the cube primitive
//
///////////////////////////////////////////////////////////////////////////////

#include "VertexArray.hpp"
#include "renderer/Renderer3D.hpp"

#include <algorithm>
#include <array>
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace nexo::renderer
{
    /**
    * @brief Generates the vertex, texture coordinate, and normal data for a cube mesh.
    *
    * Fills the provided arrays with 36 vertices, texture coordinates, and normals for a cube.
    *
    * @param vertices Array to store generated vertex positions.
    * @param texCoords Array to store generated texture coordinates.
    * @param normals Array to store generated normals.
    */
    static void genCubeMesh(std::array<glm::vec3, 36>& vertices, std::array<glm::vec2, 36>& texCoords,
                            std::array<glm::vec3, 36>& normals)
    {
        float x = 0.5f;
        float y = 0.5f;
        float z = 0.5f;

        glm::vec3 a0 = {+x, +y, +z}; // 0.5, 0.5, 0.5 -> Front face top right
        glm::vec3 a1 = {-x, +y, +z}; // -0.5, 0.5, 0.5 -> Front face top left
        glm::vec3 a2 = {-x, -y, +z}; // -0.5, -0.5, 0.5 -> Front face bottom left
        glm::vec3 a3 = {+x, -y, +z}; // 0.5, -0.5, 0.5 -> Front face bottom right
        glm::vec3 a4 = {+x, +y, -z}; // 0.5, 0.5, -0.5 -> Back face top right
        glm::vec3 a5 = {-x, +y, -z}; // -0.5, 0.5, -0.5 -> Back face top left
        glm::vec3 a6 = {-x, -y, -z}; // -0.5, -0.5, -0.5 -> Back face bottom left
        glm::vec3 a7 = {+x, -y, -z}; // 0.5, -0.5, -0.5 -> Back face bottom right

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

    std::shared_ptr<NxVertexArray> NxRenderer3D::getCubeVAO()
    {
        constexpr unsigned int nbVerticesCube = 36;
        static std::shared_ptr<NxVertexArray> cubeVao = nullptr;
        if (cubeVao)
            return cubeVao;

        cubeVao = createVertexArray();
        const auto vertexBuffer = createVertexBuffer(nbVerticesCube * sizeof(NxVertex));
        const NxBufferLayout cubeVertexBufferLayout = {
            {NxShaderDataType::FLOAT3, "aPos"},
            {NxShaderDataType::FLOAT2, "aTexCoord"},
            {NxShaderDataType::FLOAT3, "aNormal"},
            {NxShaderDataType::FLOAT3, "aTangent"},
            {NxShaderDataType::FLOAT3, "aBiTangent"},
            {NxShaderDataType::INT, "aEntityID"}
        };
        vertexBuffer->setLayout(cubeVertexBufferLayout);

        std::array<glm::vec3, nbVerticesCube> vertices{};
        std::array<glm::vec2, nbVerticesCube> texCoords{};
        std::array<glm::vec3, nbVerticesCube> normals{};
        genCubeMesh(vertices, texCoords, normals);

        std::vector<NxVertex> vertexData(nbVerticesCube);
        for (unsigned int i = 0; i < nbVerticesCube; ++i)
        {
            vertexData[i].position = glm::vec4(vertices[i], 1.0f);
            vertexData[i].texCoord = texCoords[i];
            vertexData[i].normal = normals[i];
            vertexData[i].tangent = glm::vec3(0.0f, 0.0f, 0.0f); // Default tangent
            vertexData[i].bitangent = glm::vec3(0.0f, 0.0f, 0.0f); // Default bi tangent
            vertexData[i].entityID = 0; // Default entity ID
        }

        vertexBuffer->setData(vertexData.data(), static_cast<unsigned int>(vertexData.size() * sizeof(NxVertex)));
        cubeVao->addVertexBuffer(vertexBuffer);

        std::vector<unsigned int> indices(nbVerticesCube);
        for (uint32_t i = 0; i < nbVerticesCube; ++i)
            indices[i] = i;

        auto indexBuffer = createIndexBuffer();
        indexBuffer->setData(indices.data(), static_cast<unsigned int>(indices.size()));
        cubeVao->setIndexBuffer(indexBuffer);

        return cubeVao;
    }
}
