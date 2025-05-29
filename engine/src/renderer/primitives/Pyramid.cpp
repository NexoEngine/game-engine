//// Pyramid.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                 z  zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  zzzz                zzzz  zzzz      zzzz           zzzz
//  zzz      z  zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        03/04/2025
//  Description: Source file for the pyramid primitive
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
    * @brief Generates the vertex, texture coordinate, and normal data for a pyramid mesh.
    *
    * Fills the provided arrays with 36 vertices, texture coordinates, and normals for a pyramid.
    *
    * @param vertices Array to store generated vertex positions.
    * @param texCoords Array to store generated texture coordinates.
    * @param normals Array to store generated normals.
    */
    static void genPyramidMesh(std::array<glm::vec3, 18>& vertices,
                               std::array<glm::vec2, 18>& texCoords,
                               std::array<glm::vec3, 18>& normals)
    {
        // Define the five vertices of the pyramid
        constexpr glm::vec3 v0 = {0.0f, 1.0f, 0.0f}; // Top vertex
        constexpr glm::vec3 v1 = {-1.0f, -1.0f, -1.0f}; // Bottom-left-back
        constexpr glm::vec3 v2 = {1.0f, -1.0f, -1.0f}; // Bottom-right-back
        constexpr glm::vec3 v3 = {1.0f, -1.0f, 1.0f}; // Bottom-right-front
        constexpr glm::vec3 v4 = {-1.0f, -1.0f, 1.0f}; // Bottom-left-front

        // Define the 4 triangular faces + the base (2 triangles)
        glm::vec3 verts[] = {
            v1, v2, v3, v1, v3, v4, // Base face
            // Side faces
            v0, v2, v1,
            v0, v3, v2,
            v0, v4, v3,
            v0, v1, v4
        };

        std::ranges::copy(verts, vertices.begin());

        // Basic UV mapping for each face
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

        // Compute normals for each face of the pyramid
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
     *
     * @brief Creates a vertex array object (VAO) for a pyramid mesh.
     *
     * @return A shared pointer to a vertex array object containing the pyramid mesh data.
     */
    std::shared_ptr<NxVertexArray> NxRenderer3D::getPyramidVAO()
    {
        constexpr unsigned int nbVerticesPyramid = 18;
        static std::shared_ptr<NxVertexArray> pyramidVao = nullptr;
        if (pyramidVao)
            return pyramidVao;

        pyramidVao = createVertexArray();
        const auto vertexBuffer = createVertexBuffer(nbVerticesPyramid * sizeof(NxVertex));
        const NxBufferLayout pyramidVertexBufferLayout = {
            {NxShaderDataType::FLOAT3, "aPos"},
            {NxShaderDataType::FLOAT2, "aTexCoord"},
            {NxShaderDataType::FLOAT3, "aNormal"},
            {NxShaderDataType::FLOAT3, "aTangent"},
            {NxShaderDataType::FLOAT3, "aBiTangent"},
            {NxShaderDataType::INT, "aEntityID"}
        };
        vertexBuffer->setLayout(pyramidVertexBufferLayout);

        std::array<glm::vec3, nbVerticesPyramid> vertices{};
        std::array<glm::vec2, nbVerticesPyramid> texCoords{};
        std::array<glm::vec3, nbVerticesPyramid> normals{};
        genPyramidMesh(vertices, texCoords, normals);

        std::vector<NxVertex> vertexData(nbVerticesPyramid);
        for (unsigned int i = 0; i < nbVerticesPyramid; ++i)
        {
            vertexData[i].position = glm::vec4(vertices[i], 1.0f);
            vertexData[i].texCoord = texCoords[i];
            vertexData[i].normal = normals[i];
        }

        vertexBuffer->setData(vertexData.data(), vertexData.size() * sizeof(NxVertex));
        pyramidVao->addVertexBuffer(vertexBuffer);

        std::vector<unsigned int> indices(nbVerticesPyramid);
        for (uint32_t i = 0; i < nbVerticesPyramid; ++i)
            indices[i] = i;

        const auto indexBuffer = createIndexBuffer();
        indexBuffer->setData(indices.data(), indices.size());
        pyramidVao->setIndexBuffer(indexBuffer);

        return pyramidVao;
    }
}
