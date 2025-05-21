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
#include <Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>


namespace nexo::renderer
{
    static void genTetrahedronMesh(std::array<glm::vec3, 12>& vertices,
                                   std::array<glm::vec2, 12>& texCoords,
                                   std::array<glm::vec3, 12>& normals)
    {
        constexpr float size = 0.5f;

        // Define the four vertices of the tetrahedron
        constexpr auto v0 = glm::vec3(0.0f, size, 0.0f); // Top vertex
        constexpr auto v1 = glm::vec3(-size, -size, -size); // Bottom-left-back
        constexpr auto v2 = glm::vec3(size, -size, -size); // Bottom-right-back
        constexpr auto v3 = glm::vec3(0.0f, -size, size); // Bottom-front

        // Define the 4 triangular faces (each has 3 vertices)
        glm::vec3 verts[] = {
            v0, v1, v2, // Front face
            v0, v2, v3, // Right face
            v0, v3, v1, // Left face
            v1, v3, v2 // Bottom face
        };

        std::ranges::copy(verts, vertices.begin());

        // Basic UV mapping for each face
        glm::vec2 texc[] = {
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Front face
            {1.0f, 0.5f}, {1.0f, 1.0f}, {0.0f, 0.0f}, // Right face
            {0.0f, 0.5f}, {1.0f, 0.0f}, {1.0f, 1.0f}, // Left face
            {0.0f, 1.0f}, {1.0f, 1.0f}, {0.5f, 0.0f} // Bottom face
        };

        std::copy(std::begin(texc), std::end(texc), texCoords.begin());

        // Compute normals for each face
        glm::vec3 norm[12];

        for (int i = 0; i < 12; i += 3)
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

    void Renderer3D::drawTetrahedron(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color, const int entityID) const
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

        std::array<glm::vec3, 12> verts{};
        std::array<glm::vec2, 12> texCoords{};
        std::array<glm::vec3, 12> normals{};
        std::array<unsigned int, 12> indices{};

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
    }

    void Renderer3D::drawTetrahedron(const glm::vec3& position, const glm::vec3& size, const glm::vec3 &rotation, const glm::vec4& color, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        }

        const glm::quat rotationQuat = glm::radians(rotation);
        const glm::mat4 rotationMat = glm::toMat4(rotationQuat);

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                    rotationMat *
                                    glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 12> verts{};
        std::array<glm::vec2, 12> texCoords{};
        std::array<glm::vec3, 12> normals{};
        std::array<unsigned int, 12> indices{};

        genTetrahedronMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 12; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawTetrahedron(const glm::mat4& transform, const glm::vec4& color, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 12> verts{};
        std::array<glm::vec2, 12> texCoords{};
        std::array<glm::vec3, 12> normals{};
        std::array<unsigned int, 12> indices{};
        genTetrahedronMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 12; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawTetrahedron(const glm::vec3& position, const glm::vec3& size, const components::Material& material, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 12> verts{};
        std::array<glm::vec2, 12> texCoords{};
        std::array<glm::vec3, 12> normals{};
        std::array<unsigned int, 12> indices{};

        genTetrahedronMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 12; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });


        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawTetrahedron(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation, const components::Material& material, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        const glm::quat rotationQuat = glm::radians(rotation);
        const glm::mat4 rotationMat = glm::toMat4(rotationQuat);
        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            rotationMat *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 12> verts{};
        std::array<glm::vec2, 12> texCoords{};
        std::array<glm::vec3, 12> normals{};
        std::array<unsigned int, 12> indices{};

        genTetrahedronMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 12; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawTetrahedron(const glm::vec3& position, const glm::vec3& size, const glm::quat& rotation, const components::Material& material, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        const glm::mat4 rotationMat = glm::toMat4(rotation);
        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            rotationMat *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 12> verts{};
        std::array<glm::vec2, 12> texCoords{};
        std::array<glm::vec3, 12> normals{};
        std::array<unsigned int, 12> indices{};

        genTetrahedronMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 12; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawTetrahedron(const glm::mat4& transform, const components::Material& material, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                            "Renderer not rendering a scene, make sure to call beginScene first");
        }

        m_storage->textureShader->setUniformMatrix("matModel", transform);

        renderer::Material mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 12> verts{};
        std::array<glm::vec2, 12> texCoords{};
        std::array<glm::vec3, 12> normals{};
        std::array<unsigned int, 12> indices{};

        genTetrahedronMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 12; ++i)
            indices[i] = i;

        // Vertex data
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
        std::ranges::for_each(indices, [this](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }
}
