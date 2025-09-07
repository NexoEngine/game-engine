//// Billboard.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/04/2025
//  Description: Source file for the billboard render function
//
///////////////////////////////////////////////////////////////////////////////

#include "renderer/Renderer3D.hpp"
#include <array>
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>

namespace nexo::renderer
{
    // Quad vertices for a 1x1 billboard centered at origin
    constexpr glm::vec3 billboardPositions[4] = {
        {-0.5f, -0.5f, 0.0f}, // Bottom left
        {0.5f, -0.5f, 0.0f}, // Bottom right
        {0.5f, 0.5f, 0.0f}, // Top right
        {-0.5f, 0.5f, 0.0f} // Top left
    };

    constexpr glm::vec2 billboardTexCoords[4] = {
        {0.0f, 0.0f}, // Bottom left
        {1.0f, 0.0f}, // Bottom right
        {1.0f, 1.0f}, // Top right
        {0.0f, 1.0f}, // Top left
    };

    /**
     * @brief Generates the vertex, texture coordinate, and normal data for a billboard mesh.
     *
     * Fills the provided arrays with 6 vertices, texture coordinates, and normals for a billboard quad.
     *
     * @param vertices Array to store generated vertex positions.
     * @param texCoords Array to store generated texture coordinates.
     * @param normals Array to store generated normals.
     */
    static void genBillboardMesh(std::array<glm::vec3, 6>& vertices, std::array<glm::vec2, 6>& texCoords,
                                 std::array<glm::vec3, 6>& normals)
    {
        // Vertex positions
        vertices[0] = billboardPositions[0]; // Bottom left
        vertices[1] = billboardPositions[1]; // Bottom right
        vertices[2] = billboardPositions[2]; // Top right
        vertices[3] = billboardPositions[2]; // Top right
        vertices[4] = billboardPositions[3]; // Top left
        vertices[5] = billboardPositions[0]; // Bottom left

        // Texture coordinates
        texCoords[0] = billboardTexCoords[0]; // Bottom left
        texCoords[1] = billboardTexCoords[1]; // Bottom right
        texCoords[2] = billboardTexCoords[2]; // Top right
        texCoords[3] = billboardTexCoords[2]; // Top right
        texCoords[4] = billboardTexCoords[3]; // Top left
        texCoords[5] = billboardTexCoords[0]; // Bottom left

        // All normals point forward for billboard (will be transformed to face camera)
        for (int i = 0; i < 6; ++i)
        {
            normals[i] = {0.0f, 0.0f, 1.0f};
        }
    }

    std::shared_ptr<NxVertexArray> NxRenderer3D::getBillboardVAO()
    {
        constexpr unsigned int nbVerticesBillboard = 6;
        static std::shared_ptr<NxVertexArray> billboardVao = nullptr;
        if (billboardVao)
            return billboardVao;

        billboardVao = createVertexArray();
        const auto vertexBuffer = createVertexBuffer(nbVerticesBillboard * sizeof(NxVertex));
        const NxBufferLayout cubeVertexBufferLayout = {
            {NxShaderDataType::FLOAT3, "aPos"},
            {NxShaderDataType::FLOAT2, "aTexCoord"},
            {NxShaderDataType::FLOAT3, "aNormal"},
            {NxShaderDataType::FLOAT3, "aTangent"},
            {NxShaderDataType::FLOAT3, "aBiTangent"},
            {NxShaderDataType::INT, "aEntityID"}
        };
        vertexBuffer->setLayout(cubeVertexBufferLayout);

        std::array<glm::vec3, nbVerticesBillboard> vertices{};
        std::array<glm::vec2, nbVerticesBillboard> texCoords{};
        std::array<glm::vec3, nbVerticesBillboard> normals{};
        genBillboardMesh(vertices, texCoords, normals);

        std::vector<NxVertex> vertexData(nbVerticesBillboard);
        for (unsigned int i = 0; i < nbVerticesBillboard; ++i)
        {
            vertexData[i].position = glm::vec4(vertices[i], 1.0f);
            vertexData[i].texCoord = texCoords[i];
            vertexData[i].normal = normals[i];
            vertexData[i].tangent = glm::vec3(0.0f, 0.0f, 0.0f); // Default tangent
            vertexData[i].bitangent = glm::vec3(0.0f, 0.0f, 0.0f); // Default bi tangent
            vertexData[i].entityID = 0; // Default entity ID
        }

        vertexBuffer->setData(vertexData.data(), static_cast<unsigned int>(vertexData.size() * sizeof(NxVertex)));
        billboardVao->addVertexBuffer(vertexBuffer);

        std::vector<unsigned int> indices(nbVerticesBillboard);
        for (uint32_t i = 0; i < nbVerticesBillboard; ++i)
            indices[i] = i;

        const auto indexBuffer = createIndexBuffer();
        indexBuffer->setData(indices.data(), static_cast<unsigned int>(indices.size()));
        billboardVao->setIndexBuffer(indexBuffer);

        return billboardVao;
    }
}
