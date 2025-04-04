//// Pyramid.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                 z  zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  zzz                zzzz  zzzz      zzzz           zzzz
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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace nexo::renderer
{
    // 5 unique vertices for a pyramid (one per corner)
    constexpr glm::vec3 pyramidPositions[5] = {
        {0.0f, 0.5f, 0.0f}, // Top vertex 0
        {-0.5f, -0.5f, -0.5f}, // Bottom-left-back vertex 1
        {0.5f, -0.5f, -0.5f}, // Bottom-right-back vertex 2
        {0.5f, -0.5f, 0.5f}, // Bottom-right-front vertex 3
        {-0.5f, -0.5f, 0.5f} // Bottom-left-front vertex 4
    };

    constexpr unsigned int pyramidIndices[18] = {
        // Base face
        1, 2, 3,
        1, 3, 4,
        // Side faces
        0, 1, 2,
        0, 2, 3,
        0, 3, 4,
        0, 4, 1
    };

    constexpr glm::vec2 textureCoords[18] = {
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Base face
        {0.5f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, // Base face
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Side face
        {0.5f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, // Side face
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Side face
        {0.5f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f} // Side face
    };

    static void genPyramidMesh(std::array<glm::vec3, 18>& vertices,
                               std::array<glm::vec2, 18>& texCoords,
                               std::array<glm::vec3, 18>& normals)
    {
        // Define the five vertices of the pyramid
        constexpr glm::vec3 v0 = {0.0f, 0.5f, 0.0f}; // Top vertex
        constexpr glm::vec3 v1 = {-0.5f, -0.5f, -0.5f}; // Bottom-left-back
        constexpr glm::vec3 v2 = {0.5f, -0.5f, -0.5f}; // Bottom-right-back
        constexpr glm::vec3 v3 = {0.5f, -0.5f, 0.5f}; // Bottom-right-front
        constexpr glm::vec3 v4 = {-0.5f, -0.5f, 0.5f}; // Bottom-left-front

        // Define the 6 triangular faces (each has 3 vertices)
        glm::vec3 verts[] = {
            v1, v2, v3, v1, v3, v4, // Base face
            v0, v1, v2, v0, v2, v3, v0, v3, v4, v0, v4, v1 // Side faces
        };

        std::copy(std::begin(verts), std::end(verts), vertices.begin());

        // Basic UV mapping for each face
        glm::vec2 texc[] = {
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Base face
            {0.5f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, // Base face
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Side face
            {0.5f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f}, // Side face
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}, // Side face
            {0.5f, 1.0f}, {1.0f, 0.0f}, {0.0f, 0.0f} // Side face
        };

        std::copy(std::begin(texc), std::end(texc), texCoords.begin());

        // Compute normals for each face
        glm::vec3 norm[18];
        for (int i = 0; i < 18; i += 3)
        {
            glm::vec3 normal = glm::normalize(
                glm::cross(
                    verts[i + 1] - verts[i],
                    verts[i + 2] - verts[i]));

            norm[i] = normal;
            norm[i + 1] = normal;
            norm[i + 2] = normal;
        }

        std::copy(std::begin(norm), std::end(norm), normals.begin());
    }

    void Renderer3D::drawPyramid(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color,
                                 const int entityID) const
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

        std::array<glm::vec3, 18> verts{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};
        std::array<unsigned int, 18> indices{};

        genPyramidMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 18; ++i)
            indices[i] = i;

        // Vertex data
        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());
        for (unsigned int i = 0; i < 18; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            m_storage->vertexBufferPtr->texCoord = texCoords[i];
            m_storage->vertexBufferPtr->normal = normals[i];
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

    void Renderer3D::drawPyramid(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
                                     const glm::vec4& color, const int entityID) const
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

        std::array<glm::vec3, 18> verts{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};
        std::array<unsigned int, 18> indices{};

        genPyramidMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 18; ++i)
            indices[i] = i;

        // Vertex data
        for (unsigned int i = 0; i < 18; ++i)
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

    void Renderer3D::drawPyramid(const glm::mat4& transform, const glm::vec4& color, const int entityID) const
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

        std::array<glm::vec3, 18> verts{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};
        std::array<unsigned int, 18> indices{};
        genPyramidMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 18; ++i)
            indices[i] = i;

        // Vertex data
        for (unsigned int i = 0; i < 18; ++i)
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

    void Renderer3D::drawPyramid(const glm::vec3& position, const glm::vec3& size,
                                     const components::Material& material, const int entityID) const
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

        std::array<glm::vec3, 18> verts{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};
        std::array<unsigned int, 18> indices{};

        genPyramidMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 18; ++i)
            indices[i] = i;

        // Vertex data
        for (unsigned int i = 0; i < 18; ++i)
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

    void Renderer3D::drawPyramid(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
                                     const components::Material& material, const int entityID) const
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

        std::array<glm::vec3, 18> verts{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};
        std::array<unsigned int, 18> indices{};

        genPyramidMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 18; ++i)
            indices[i] = i;

        // Vertex data
        for (unsigned int i = 0; i < 18; ++i)
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

    void Renderer3D::drawPyramid(const glm::vec3& position, const glm::vec3& size, const glm::quat& rotation,
                                     const components::Material& material, const int entityID) const
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

        std::array<glm::vec3, 18> verts{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};
        std::array<unsigned int, 18> indices{};

        genPyramidMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 18; ++i)
            indices[i] = i;

        // Vertex data
        for (unsigned int i = 0; i < 18; ++i)
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

    void Renderer3D::drawPyramid(const glm::mat4& transform, const components::Material& material,
                                     const int entityID) const
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

        std::array<glm::vec3, 18> verts{};
        std::array<glm::vec2, 18> texCoords{};
        std::array<glm::vec3, 18> normals{};
        std::array<unsigned int, 18> indices{};

        genPyramidMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 18; ++i)
            indices[i] = i;

        // Vertex data
        for (unsigned int i = 0; i < 18; ++i)
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
