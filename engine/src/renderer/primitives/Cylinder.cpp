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
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <Logger.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace nexo::renderer
{
    constexpr int CYLINDER_SEGMENTS = 16; // Number of segments for the cylinder

    static std::vector<glm::vec3> generateCylinderVertices()
    {
        std::vector<glm::vec3> vertices{};

        // Generate vertices for the top and bottom circles
        // Set vertex top circle
        for (int i = 0; i < CYLINDER_SEGMENTS; ++i) {
            const float angle = static_cast<float>(i) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, 0.5f, z);
        }
        // Set vertex bottom circle
        for (int i = 0; i < CYLINDER_SEGMENTS; ++i) {
            const float angle = static_cast<float>(i) / static_cast<float>(CYLINDER_SEGMENTS) * 2.0f * static_cast<float>(M_PI);
            const float x = std::cos(angle);
            const float z = std::sin(angle);

            vertices.emplace_back(x, -0.5f, z);
        }

        // Center points for top and bottom circles
        vertices.emplace_back(0.0f, 0.5f, 0.0f);
        vertices.emplace_back(0.0f, -0.5f, 0.0f);

        return vertices;
    }

    // unique vertices for a cylinder
    const std::vector<glm::vec3> cylinderPositions = generateCylinderVertices();

    static std::vector<unsigned int> generateCylinderIndices()
    {
        std::vector<unsigned int> indices{};
        for (int i = 0; i < CYLINDER_SEGMENTS; ++i)
        {
            // top faces
            indices.push_back(i);
            indices.push_back( CYLINDER_SEGMENTS*2);
            indices.push_back((i + 1) % CYLINDER_SEGMENTS);

            // bottom faces
            indices.push_back(i + CYLINDER_SEGMENTS);
            indices.push_back( CYLINDER_SEGMENTS*2+1);
            indices.push_back((i + 1) % CYLINDER_SEGMENTS + CYLINDER_SEGMENTS);

            // sides faces first triangle
            indices.push_back(i);
            indices.push_back(i + CYLINDER_SEGMENTS);
            indices.push_back((i + 1) % CYLINDER_SEGMENTS);
            // sides faces second triangle
            indices.push_back((i + 1) % CYLINDER_SEGMENTS);
            indices.push_back((i + 1) % CYLINDER_SEGMENTS + CYLINDER_SEGMENTS);
            indices.push_back(i + CYLINDER_SEGMENTS);
        }
        return indices;
    }

    const std::vector<unsigned int> cylinderIndices = generateCylinderIndices();

    static std::vector<glm::vec2> generateTextureCoords()
    {
        std::vector<glm::vec2> texCoords{};
        for (int i = 0; i < CYLINDER_SEGMENTS; ++i) {
            const float u = static_cast<float>(i) / CYLINDER_SEGMENTS;
            texCoords.emplace_back(u, 1.0f);
            texCoords.emplace_back(u, 0.0f);
        }
        // Center points for top and bottom circles
        texCoords.emplace_back(0.5f, 0.5f);
        texCoords.emplace_back(0.5f, 0.5f);

        return texCoords;
    }

    const std::vector<glm::vec2> textureCoords = generateTextureCoords();

    /**
    * @brief Generates the vertex, texture coordinate, and normal data for a cylinder mesh.
    *
    * Fills the provided arrays with CYLINDER_SEGMENTS*2+2 vertices, texture coordinates, and normals for a cylinder.
    *
    * @param vertices Array to store generated vertex positions.
    * @param texCoords Array to store generated texture coordinates.
    * @param normals Array to store generated normals.
    */
    static void genCylinderMesh(std::vector<glm::vec3> &vertices, std::vector<glm::vec2> &texCoords, std::vector<glm::vec3> &normals)
    {
        vertices = generateCylinderVertices();
        texCoords = generateTextureCoords();

        // Side + caps
        for (int i = 0; i < CYLINDER_SEGMENTS; ++i) {
            const unsigned int idx0 = i * 2;
            const unsigned int idx1 = idx0 + 1;
            const unsigned int idx2 = ((i + 1) % (CYLINDER_SEGMENTS + 1)) * 2;
            const unsigned int idx3 = idx2 + 1;

            normals.push_back(glm::normalize(glm::vec3(vertices[idx0].x, 0.0f, vertices[idx0].z)));
            normals.push_back(glm::normalize(glm::vec3(vertices[idx1].x, 0.0f, vertices[idx1].z)));
            normals.push_back(glm::normalize(glm::vec3(vertices[idx2].x, 0.0f, vertices[idx2].z)));

            normals.push_back(glm::normalize(glm::vec3(vertices[idx2].x, 0.0f, vertices[idx2].z)));
            normals.push_back(glm::normalize(glm::vec3(vertices[idx1].x, 0.0f, vertices[idx1].z)));
            normals.push_back(glm::normalize(glm::vec3(vertices[idx3].x, 0.0f, vertices[idx3].z)));
        }
        for (int i = 0; i <= CYLINDER_SEGMENTS; ++i) {
            normals.emplace_back(0.0f, 1.0f, 0.0f);
            normals.emplace_back(0.0f, -1.0f, 0.0f);
        }
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
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*2+2; ++i)
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
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*2+2; ++i)
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
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*2+2; ++i)
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
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*2+2; ++i)
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
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*2+2; ++i)
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
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*2+2; ++i)
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
        for (unsigned int i = 0; i < CYLINDER_SEGMENTS*2+2; ++i)
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
