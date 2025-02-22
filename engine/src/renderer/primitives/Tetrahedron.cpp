//// Tetrahedron  be.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                 z  zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  zzz                zzzz  zzzz      zzzz           zzzz
//  zzz      z  zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        21/02/2025
//  Description: Source file for the tetrahedron  primitive
//
///////////////////////////////////////////////////////////////////////////////


#include "renderer/Renderer3D.hpp"
#include "renderer/RendererExceptions.hpp"

#include <algorithm>
#include <array>
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


namespace nexo::renderer
{
    // 4 unique vertices for a tetrahedron (one per corner)
    constexpr glm::vec3 tetrahedronPositions[4] = {
        {0.0f, 0.5f, 0.0f}, // Top vertex 0
        {-0.5f, -0.5f, -0.5f}, // Bottom-left-back vertex 1
        {0.5f, -0.5f, -0.5f}, // Bottom-right-back vertex 2
        {0.0f, -0.5f, 0.5f} // Bottom-front vertex 3
    };

    constexpr unsigned int tetrahedronIndices[12] = {
        // Front face
        0, 1, 2,
        // Right face
        0, 2, 3,
        // Left face
        0, 3, 1,
        // Bottom face
        1, 3, 2
    };

    static const glm::vec3 vertexNormals[4] = {
        glm::normalize(glm::vec3(0.0f, 1.0f, 0.0f)), // Vertex 0
        glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)), // Vertex 1
        glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)), // Vertex 2
        glm::normalize(glm::vec3(0.0f, -1.0f, 1.0f)), // Vertex 3
    };

    constexpr glm::vec2 textureCoords[12] = {
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Front face
        {0.5f, 1.0f}, {1.0f, 0.0f}, {0.5f, 0.5f}, // Right face
        {0.5f, 1.0f}, {0.5f, 0.5f}, {0.0f, 0.0f}, // Left face
        {0.0f, 0.0f}, {1.0f, 0.0f}, {0.5f, 0.5f} // Bottom face
    };

    static void genTetrahedronMesh(std::array<glm::vec3, 12>& vertices,
                                   std::array<glm::vec2, 12>& texCoords,
                                   std::array<glm::vec3, 12>& normals)
    {
        float size = 0.5f;

        // Define the four vertices of the tetrahedron
        glm::vec3 v0 = glm::vec3(0.0f, size, 0.0f); // Top vertex
        glm::vec3 v1 = glm::vec3(-size, -size, -size); // Bottom-left-back
        glm::vec3 v2 = glm::vec3(size, -size, -size); // Bottom-right-back
        glm::vec3 v3 = glm::vec3(0.0f, -size, size); // Bottom-front

        // Define the 4 triangular faces (each has 3 vertices)
        glm::vec3 verts[] = {
            v0, v1, v2, // Front face
            v0, v2, v3, // Right face
            v0, v3, v1, // Left face
            v1, v3, v2 // Bottom face
        };

        std::copy(std::begin(verts), std::end(verts), vertices.begin());

        // Basic UV mapping for each face
        glm::vec2 texc[] = {
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Front face
            {0.5f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, // Right face
            {0.5f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, // Left face
            {0.0f, 0.0f}, {1.0f, 0.0f}, {0.5f, 1.0f} // Bottom face
        };

        std::copy(std::begin(texc), std::end(texc), texCoords.begin());

        // Compute normals for each face
        glm::vec3 norm[12];

        std::cout << "PRINTING NORMALS" << std::endl;
        for (int i = 0; i < 12; i += 3)
        {
            glm::vec3 normal = glm::normalize(
                glm::cross(
                    verts[i + 1] - verts[i],
                    verts[i + 2] - verts[i]));

            norm[i] = normal;
            norm[i + 1] = normal;
            norm[i + 2] = normal;

            std::cout << "Normal for face " << i / 3 << " { "
                << norm[i].x << ", " << norm[i].y << ", " << norm[i].z
                << " }" << std::endl;
        }

        std::copy(std::begin(norm), std::end(norm), normals.begin());
    }

    void Renderer3D::drawTetrahedron(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color,
                                     int entityID) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");

        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 12> verts;
        std::array<glm::vec2, 12> texCoords;
        std::array<glm::vec3, 12> normals;
        std::array<unsigned int, 12> indices;

        genTetrahedronMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 12; ++i)
            indices[i] = i;

        // Vertex data
        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());
        for (unsigned int i = 0; i < 12; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            m_storage->vertexBufferPtr->texCoord = texCoords[i];
            m_storage->vertexBufferPtr->normal = normals[i];
            //m_storage->vertexBufferPtr->tangent = cubeTangents[i];
            //m_storage->vertexBufferPtr->bitangent = cubeBitangents[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(indices, [this, vertexOffset](unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
        const auto vertexDataSize = static_cast<unsigned int>(
            reinterpret_cast<std::byte*>(m_storage->vertexBufferPtr) -
            reinterpret_cast<std::byte*>(m_storage->vertexBufferBase.data())
        );


        m_storage->vertexBuffer->setData(m_storage->vertexBufferBase.data(), vertexDataSize);

        m_storage->indexBuffer->setData(m_storage->indexBufferBase.data(), m_storage->indexCount);
        flushAndReset();
    }

    void Renderer3D::drawTetrahedron(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
                                     const glm::vec4& color, int entityID) const
    {
    }

    void Renderer3D::drawTetrahedron(const glm::mat4& transform, const glm::vec4& color, int entityID) const
    {
    }

    void Renderer3D::drawTetrahedron(const glm::vec3& position, const glm::vec3& size,
                                     const components::Material& material, int entityID) const
    {
    }

    void Renderer3D::drawTetrahedron(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
                                     const components::Material& material, int entityID) const
    {
    }

    void Renderer3D::drawTetrahedron(const glm::mat4& transform, const components::Material& material,
                                     int entityID) const
    {
    }
}
