//// Cylinder.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                 z  zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  zzz                zzzz  zzzz      zzzz           zzzz
//  zzz      z  zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        05/04/2025
//  Description: Source file for the cylinder primitive
//
///////////////////////////////////////////////////////////////////////////////

#include "renderer/Renderer3D.hpp"
#include "renderer/RendererExceptions.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <gmock/gmock-matchers.h>

namespace nexo::renderer
{
    constexpr int CYLINDER_SEGMENTS = 8; // Number of segments for the cylinder
    constexpr float CYLINDER_HEIGHT = 1.0f; // Height of the cylinder should be 1.0f

    static std::vector<glm::vec3> generateCylinderVertices()
    {
        std::vector<glm::vec3> vertices{};

        int i = 0;
        for (int k = CYLINDER_SEGMENTS-1; i < CYLINDER_SEGMENTS; ++i, --k) {
            const float angle = static_cast<float>(k) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, CYLINDER_HEIGHT, z);
        }
        for (int k = CYLINDER_SEGMENTS-1; i < CYLINDER_SEGMENTS*2; ++i, --k) {
            const float angle = static_cast<float>(k) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, -CYLINDER_HEIGHT, z);
        }
        for (int k = 0; i < CYLINDER_SEGMENTS*3; ++i, ++k) {
            const float angle = static_cast<float>(k) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, CYLINDER_HEIGHT, z);
        }
        for (int k = 0; i < CYLINDER_SEGMENTS*4; ++i, ++k) {
            const float angle = static_cast<float>(k) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, -CYLINDER_HEIGHT, z);
        }
        return vertices;
    }

    // unique vertices for a cylinder
    const std::vector<glm::vec3> cylinderPositions = generateCylinderVertices();

    static void capIndices(std::vector<unsigned int> &indices, const int transformer)
    {
        std::function<bool(int)> recurFun;
        std::function<void(const int, const int)> capIndicesRec;

        capIndicesRec = [&indices, &transformer, &capIndicesRec](const int start, const int nbSegment) {
            if (const int step = ceil(static_cast<double>(nbSegment) / 3.0); step == 1) {
                const int tmp = (start + 2 < CYLINDER_SEGMENTS) ? (start + 2) : 0;
                indices.push_back(start + transformer); indices.push_back(start + 1 + transformer); indices.push_back(tmp + transformer);
            } else {
                capIndicesRec(start, step + 1);
                if (start + 2 * step < start + nbSegment - 1) {
                    int tmp = 0;
                    if (start + 2 * step < start + nbSegment - 1) {
                        tmp = start + nbSegment - 1;
                        capIndicesRec(start + step, tmp - (start + step) + 1);
                    } else if (start + 2 * step < CYLINDER_SEGMENTS) {
                        tmp = start + 2 * step;
                        capIndicesRec(start + step, step + 1);
                    }
                    tmp = tmp > CYLINDER_SEGMENTS-1 ? 0 : tmp;
                    indices.push_back(start + transformer); indices.push_back(start + step + transformer); indices.push_back(tmp + transformer);
                } else {
                    const int tmp = (start + nbSegment - 1 < CYLINDER_SEGMENTS) ? (start + nbSegment - 1) : 0;
                    indices.push_back(start + transformer); indices.push_back(start + step + transformer); indices.push_back(tmp + transformer);
                    if ((start + nbSegment - 1) - (start + step) > 1) {
                        capIndicesRec(start + step, step + 1);
                    }
                }
            }
        };

        constexpr int start = 0;
        const int step = ceil(static_cast<double>(CYLINDER_SEGMENTS) / 3.0); //3
        indices.push_back(start + transformer); indices.push_back(start + step + transformer); indices.push_back(start + 2 * step + transformer);
        if constexpr (CYLINDER_SEGMENTS > 3) {
            capIndicesRec(start, step + 1);
            capIndicesRec(start + step, step + 1);
        }
        if constexpr (CYLINDER_SEGMENTS > 5)
            capIndicesRec(start + 2 * step, CYLINDER_SEGMENTS - 2 * step + 1);
    }

    static std::vector<unsigned int> generateCylinderIndices()
    {
        std::vector<unsigned int> indices{};

        int i = 0;
        for (; i < CYLINDER_SEGMENTS-1; ++i) {
            indices.push_back(i); indices.push_back(i + 1); indices.push_back(i + CYLINDER_SEGMENTS);
            indices.push_back(i + 1); indices.push_back(i + CYLINDER_SEGMENTS + 1); indices.push_back(i + CYLINDER_SEGMENTS);
        }
        indices.push_back(i); indices.push_back(0); indices.push_back(i + CYLINDER_SEGMENTS);
        indices.push_back(0); indices.push_back(CYLINDER_SEGMENTS); indices.push_back(i + CYLINDER_SEGMENTS);

        capIndices(indices, CYLINDER_SEGMENTS*2);
        capIndices(indices, CYLINDER_SEGMENTS*3);

        return indices;
    }

    const std::vector<unsigned int> cylinderIndices = generateCylinderIndices();

    static std::vector<glm::vec2> generateTextureCoords()
    {
        std::vector<glm::vec2> texCoords{};
        for (int i = 0; i < CYLINDER_SEGMENTS*4; ++i) {
            const float angle = static_cast<float>(i) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);

            const float u = angle / (2.0f * static_cast<float>(M_PI));
            constexpr float v = CYLINDER_HEIGHT;

            texCoords.emplace_back(u, v);
        }
        return texCoords;
    }

    const std::vector<glm::vec2> textureCoords = generateTextureCoords();

    /**
    * @brief Generates the vertex, texture coordinate, and normal data for a cylinder mesh.
    *
    * Fills the provided arrays with CYLINDER_SEGMENTS*2 vertices, texture coordinates, and normals for a cylinder.
    *
    * @param vertices Array to store generated vertex positions.
    * @param texCoords Array to store generated texture coordinates.
    * @param normals Array to store generated normals.
    */
    static void genCylinderMesh(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &normals)
    {
        vertices = generateCylinderVertices();
        texCoords = generateTextureCoords();

        int i = 0;
        for (; i < CYLINDER_SEGMENTS*1; ++i) {
            const glm::vec3 vector1 = vertices[i] - glm::vec3(0, 1, 0);
            normals.emplace_back(vector1);
        }
        for (; i < CYLINDER_SEGMENTS*2; ++i) {
            const glm::vec3 vector2 = vertices[i] - glm::vec3(0, -1, 0);
            normals.emplace_back(vector2);
        }
        for (; i < CYLINDER_SEGMENTS*3; ++i) {
            normals.emplace_back(0,1,0);
        }
        for (; i < CYLINDER_SEGMENTS*4; ++i) {
            normals.emplace_back(0,-1,0);
        }
        std::ranges::copy(normals, normals.begin());

        LOG(NEXO_INFO, "{}", cylinderPositions.size());
        LOG(NEXO_INFO, "========================================================");

        for (auto cylinder_position : cylinderPositions)
        {
            LOG(NEXO_INFO, "{}f, {}f, {}f", cylinder_position.x, cylinder_position.y, cylinder_position.z);
        }
        // for (int i = 0; i < cylinderIndices.size(); i+=3)
        // {
        //     LOG(NEXO_INFO, "{} {} {}", cylinderIndices[i], cylinderIndices[i+1], cylinderIndices[i+2]);
        // }
    }

    void Renderer3D::drawCylinder(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color,
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

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices = generateCylinderIndices();

        genCylinderMesh(verts, texCoords, normals);

        // Vertex data
        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*4; ++i)
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
        const auto vertexDataSize = static_cast<unsigned int>(
            reinterpret_cast<std::byte*>(m_storage->vertexBufferPtr) -
            reinterpret_cast<std::byte*>(m_storage->vertexBufferBase.data())
        );

        m_storage->vertexBuffer->setData(m_storage->vertexBufferBase.data(), vertexDataSize);
        m_storage->indexBuffer->setData(m_storage->indexBufferBase.data(), m_storage->indexCount);
        flushAndReset();
    }

    void Renderer3D::drawCylinder(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
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

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices = generateCylinderIndices();

        genCylinderMesh(verts, texCoords, normals);

        // Vertex data
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*4; ++i)
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

    void Renderer3D::drawCylinder(const glm::mat4& transform, const glm::vec4& color, const int entityID) const
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

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices = generateCylinderIndices();

        genCylinderMesh(verts, texCoords, normals);

        // Vertex data
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*4; ++i)
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

    void Renderer3D::drawCylinder(const glm::vec3& position, const glm::vec3& size,
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

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices = generateCylinderIndices();

        genCylinderMesh(verts, texCoords, normals);

        // Vertex data
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*4; ++i)
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

    void Renderer3D::drawCylinder(const glm::vec3& position, const glm::vec3& size, const glm::vec3& rotation,
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

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices = generateCylinderIndices();

        genCylinderMesh(verts, texCoords, normals);

        // Vertex data
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*4; ++i)
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

    void Renderer3D::drawCylinder(const glm::vec3& position, const glm::vec3& size, const glm::quat& rotation,
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

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices = generateCylinderIndices();

        genCylinderMesh(verts, texCoords, normals);

        // Vertex data
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*4; ++i)
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

    void Renderer3D::drawCylinder(const glm::mat4& transform, const components::Material& material,
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

        std::vector<glm::vec3> verts{};
        std::vector<glm::vec2> texCoords{};
        std::vector<glm::vec3> normals{};
        std::vector<unsigned int> indices = generateCylinderIndices();

        genCylinderMesh(verts, texCoords, normals);

        // Vertex data
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*4; ++i)
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
