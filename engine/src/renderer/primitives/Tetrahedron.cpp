//// Tetrahedron.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz               zzz  zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  zzzz               zzzz  zzzz      zzzz           zzzz
//  zzz      z  zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        21/02/2025
//  Description: Source file for the tetrahedron primitive
//
///////////////////////////////////////////////////////////////////////////////


#include "renderer/Renderer3D.hpp"
#include "renderer/RendererExceptions.hpp"

#include <algorithm>
#include <array>
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


namespace nexo::renderer
{
    /**
    * @brief Generates the vertex, texture coordinate, and normal data for a tetrahedron mesh.
    *
    * Fills the provided arrays with 12 vertices, texture coordinates, and normals for a tetrahedron.
    *
    * @param vertices Array to store generated vertex positions.
    * @param texCoords Array to store generated texture coordinates.
    * @param normals Array to store generated normals.
    */
    static void genTetrahedronMesh(std::array<glm::vec3, 12>& vertices,
                                   std::array<glm::vec2, 12>& texCoords,
                                   std::array<glm::vec3, 12>& normals)
    {
        constexpr float size = 1.0f;

        // Define the four vertices of the tetrahedron
        constexpr auto v0 = glm::vec3(-size, -size, -size);
        constexpr auto v1 = glm::vec3(size, -size, size);
        constexpr auto v2 = glm::vec3(-size, size, size);
        constexpr auto v3 = glm::vec3(size, size, -size);

        // Define the 4 triangular faces (each has 3 vertices)
        std::array<glm::vec3, 12> verts = {
            v0, v1, v2,
            v0, v2, v3,
            v0, v3, v1,
            v1, v3, v2
        };

        std::ranges::copy(verts, vertices.begin());

        // Basic UV mapping for each face
        glm::vec2 texturesCoord[] = {
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Front face
            {1.0f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f}, // Right face
            {0.0f, 0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f}, // Left face
            {0.0f, 1.0f}, {1.0f, 1.0f}, {0.5f, 0.0f} // Bottom face
        };

        vertices = verts;

        // Compute normals for each face
        glm::vec3 norm[12];

        for (int i = 0; i < 12; i += 3)
        {
            const glm::vec3 normal = glm::normalize(
                glm::cross(
                    vertices[i + 1] - vertices[i],
                    vertices[i + 2] - vertices[i]));

            norm[i] = normal;
            norm[i + 1] = normal;
            norm[i + 2] = normal;
        }

        std::ranges::copy(norm, normals.begin());
    }


    /**
     * @brief Creates a vertex array object (VAO) for a tetrahedron mesh.
     *
     * @return A shared pointer to a vertex array object containing the tetrahedron mesh data.
     */
    std::shared_ptr<NxVertexArray> NxRenderer3D::getTetrahedronVAO()
    {
        constexpr unsigned int nbVerticesTetrahedron = 12;
        static std::shared_ptr<NxVertexArray> tetrahedronVao = nullptr;
        if (tetrahedronVao)
            return tetrahedronVao;

        tetrahedronVao = createVertexArray();
        const auto vertexBuffer = createVertexBuffer(nbVerticesTetrahedron * sizeof(NxVertex));
        const NxBufferLayout tetrahedronVertexBufferLayout = {
            {NxShaderDataType::FLOAT3, "aPos"},
            {NxShaderDataType::FLOAT2, "aTexCoord"},
            {NxShaderDataType::FLOAT3, "aNormal"},
            {NxShaderDataType::FLOAT3, "aTangent"},
            {NxShaderDataType::FLOAT3, "aBiTangent"},
            {NxShaderDataType::INT, "aEntityID"}
        };
        vertexBuffer->setLayout(tetrahedronVertexBufferLayout);

        std::array<glm::vec3, nbVerticesTetrahedron> vertices{};
        std::array<glm::vec2, nbVerticesTetrahedron> texCoords{};
        std::array<glm::vec3, nbVerticesTetrahedron> normals{};
        genTetrahedronMesh(vertices, texCoords, normals);

        std::vector<NxVertex> vertexData(nbVerticesTetrahedron);
        for (unsigned int i = 0; i < nbVerticesTetrahedron; ++i) {
            vertexData[i].position = glm::vec4(vertices[i], 1.0f);
            vertexData[i].texCoord = texCoords[i];
            vertexData[i].normal = normals[i];
            vertexData[i].tangent = glm::vec3(0.0f); // Default tangent
            vertexData[i].bitangent = glm::vec3(0.0f); // Default bi tangent
            vertexData[i].entityID = 0; // Default entity ID
        }

        vertexBuffer->setData(vertexData.data(), vertexData.size() * sizeof(NxVertex));
        tetrahedronVao->addVertexBuffer(vertexBuffer);

        std::vector<unsigned int> indices(nbVerticesTetrahedron);
        for (uint32_t i = 0; i < nbVerticesTetrahedron; ++i)
            indices[i] = i;

        const auto indexBuffer = createIndexBuffer();
        indexBuffer->setData(indices.data(), indices.size());
        tetrahedronVao->setIndexBuffer(indexBuffer);

        return tetrahedronVao;
    }
}
