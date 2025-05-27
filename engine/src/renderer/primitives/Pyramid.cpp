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
#include <Renderer3D.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace nexo::renderer
{
    // 5 unique vertices for a pyramid (one per corner)
    constexpr glm::vec3 pyramidPositions[5] = {
        {0.0f, 1.0f, 0.0f}, // Top vertex 0
        {-1.0f, -1.0f, -1.0f}, // Bottom-left-back vertex 1
        {1.0f, -1.0f, -1.0f}, // Bottom-right-back vertex 2
        {1.0f, -1.0f, 1.0f}, // Bottom-right-front vertex 3
        {-1.0f, -1.0f, 1.0f}, // Bottom-left-front vertex 4
    };

    constexpr unsigned int pyramidIndices[18] = {
        // Base face
        1, 2, 3, 1, 3, 4,
        // Side faces
        0, 2, 1,
        0, 3, 2,
        0, 4, 3,
        0, 1, 4
    };

    constexpr glm::vec2 textureCoords[18] = {
        // Base face
        {0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
        {0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
        // Side faces
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
        {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}
    };

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
        glm::vec2 texc[] = {
            // Base face
            {0.5f, 0.0f}, {0.0f, 1.0f}, {1.0f, 1.0f},
            {0.5f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f},
            // Side faces
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f},
            {0.5f, 1.0f}, {0.0f, 0.0f}, {1.0f, 0.0f}
        };

        std::ranges::copy(texc, texCoords.begin());

        // Compute normals for each face of the pyramid
        glm::vec3 norm[18];
        for (int i = 0; i < 18; i += 3) {
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

    void NxRenderer3D::drawPyramid(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color,
                                 const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        }
        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
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
        std::ranges::for_each(indices, [this, vertexOffset](const unsigned int index)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });

        // Update stats
        m_storage->stats.cubeCount++;
    }

    void NxRenderer3D::drawPyramid(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
                                     const glm::vec4& color, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        }

        const glm::quat rotationQuat = glm::radians(rotation);
        const glm::mat4 rotationMat = glm::toMat4(rotationQuat);

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            rotationMat *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
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

    void NxRenderer3D::drawPyramid(const glm::mat4& transform, const glm::vec4& color, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        }

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
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

    void NxRenderer3D::drawPyramid(const glm::vec3& position, const glm::vec3& size,
                                     const NxMaterial& material, const int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        }

        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
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

    void NxRenderer3D::drawPyramid(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
                                     const NxMaterial& material, const int entityID) const
    {
        if (!m_renderingScene)
	    {
	        THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
	    }

        const glm::quat rotationQuat = glm::radians(rotation);
        const glm::mat4 rotationMat = glm::toMat4(rotationQuat);
        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            rotationMat *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
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

    void NxRenderer3D::drawPyramid(const glm::vec3& position, const glm::vec3& size, const glm::quat& rotation,
                                     const NxMaterial& material, const int entityID) const
    {
        if (!m_renderingScene)
	    {
	        THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
	    }

        const glm::mat4 rotationMat = glm::toMat4(rotation);
        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
            rotationMat *
            glm::scale(glm::mat4(1.0f), size);

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
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

    void NxRenderer3D::drawPyramid(const glm::mat4& transform, const NxMaterial& material,
                                     const int entityID) const
    {
        if (!m_renderingScene)
	    {
	        THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
	                    "Renderer not rendering a scene, make sure to call beginScene first");
	    }

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        NxIndexedMaterial mat;
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
