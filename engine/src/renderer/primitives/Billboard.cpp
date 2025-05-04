//// Billboard.cpp ///////////////////////////////////////////////////////////////
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
#include "renderer/RendererExceptions.hpp"

#include <algorithm>
#include <array>
#include <glm/fwd.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

namespace nexo::renderer {
    // Quad vertices for a 1x1 billboard centered at origin
    constexpr glm::vec3 billboardPositions[4] = {
        {-0.5f, -0.5f, 0.0f},  // Bottom left
        {0.5f, -0.5f, 0.0f},   // Bottom right
        {0.5f, 0.5f, 0.0f},    // Top right
        {-0.5f, 0.5f, 0.0f}    // Top left
    };

    constexpr unsigned int billboardIndices[6] = {
        0, 1, 2, 2, 3, 0  // Two triangles forming a quad
    };

    constexpr glm::vec2 billboardTexCoords[4] = {
        {0.0f, 1.0f},  // Bottom left (flipped Y)
        {1.0f, 1.0f},  // Bottom right (flipped Y)
        {1.0f, 0.0f},  // Top right (flipped Y)
        {0.0f, 0.0f}   // Top left (flipped Y)
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
    static void genBillboardMesh(std::array<glm::vec3, 6> &vertices, std::array<glm::vec2, 6> &texCoords, std::array<glm::vec3, 6> &normals)
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
        for (int i = 0; i < 6; ++i) {
            normals[i] = {0.0f, 0.0f, 1.0f};
        }
    }

    /**
    * @brief Calculates a billboard rotation matrix that makes the quad face the camera.
    *
    * @param billboardPosition The position of the billboard in world space.
    * @param cameraPosition The position of the camera in world space.
    * @param cameraUp The up vector of the camera (usually {0,1,0}).
    * @param constrainToY Whether to only rotate around Y-axis (true) or do full rotation (false).
    * @return glm::mat4 The rotation matrix for the billboard.
    */
    static glm::mat4 calculateBillboardRotation(
        const glm::vec3& billboardPosition,
        const glm::vec3& cameraPosition,
        const glm::vec3& cameraUp = glm::vec3(0.0f, 1.0f, 0.0f),
        bool constrainToY = false)
    {
        glm::vec3 look = glm::normalize(cameraPosition - billboardPosition);

        if (constrainToY) {
            look.y = 0.0f;
            look = glm::normalize(look);

            glm::vec3 right = glm::normalize(glm::cross(cameraUp, look));
            glm::vec3 up = glm::cross(look, right);

            return glm::mat4(
            glm::vec4(right, 0.0f),
            glm::vec4(up, 0.0f),
            glm::vec4(-look, 0.0f), // Negative look preserves winding
            glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
            );
        } else {
            glm::vec3 right = glm::normalize(glm::cross(cameraUp, look));
            glm::vec3 up = glm::cross(look, right);

            return glm::mat4(
                glm::vec4(right, 0.0f),
                glm::vec4(up, 0.0f),
                glm::vec4(-look, 0.0f), // Negative look preserves winding
                glm::vec4(0.0f, 0.0f, 0.0f, 1.0f)
            );
        }
    }

    void Renderer3D::drawBillboard(
        const glm::vec3& position,
        const glm::vec2& size,
        const glm::vec4& color,
        int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        }

        glm::vec3 cameraPos = m_storage->cameraPosition;

        glm::mat4 billboardRotation = calculateBillboardRotation(position, cameraPos);

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                    billboardRotation *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        renderer::Material mat;
        mat.albedoColor = color;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 6> verts{};
        std::array<glm::vec2, 6> texCoords{};
        std::array<glm::vec3, 6> normals{};
        std::array<unsigned int, 6> indices{};

        genBillboardMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 6; ++i)
            indices[i] = i;

        for (unsigned int i = 0; i < 6; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            m_storage->vertexBufferPtr->texCoord = texCoords[i];
            m_storage->vertexBufferPtr->normal = normals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        std::ranges::for_each(indices, [this](unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });
    }

    void Renderer3D::drawBillboard(
        const glm::vec3& position,
        const glm::vec2& size,
        const components::Material& material,
        int entityID) const
    {
        if (!m_renderingScene)
        {
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        }

        glm::vec3 cameraPos = m_storage->cameraPosition;

        glm::mat4 billboardRotation = calculateBillboardRotation(position, cameraPos);

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                    billboardRotation *
                                    glm::scale(glm::mat4(1.0f), glm::vec3(size.x, size.y, 1.0f));

        m_storage->currentSceneShader->setUniformMatrix("uMatModel", transform);

        renderer::Material mat;
        mat.albedoColor = material.albedoColor;
        mat.albedoTexIndex = material.albedoTexture ? getTextureIndex(material.albedoTexture) : 0;
        mat.specularColor = material.specularColor;
        mat.specularTexIndex = material.metallicMap ? getTextureIndex(material.metallicMap) : 0;
        setMaterialUniforms(mat);

        std::array<glm::vec3, 6> verts{};
        std::array<glm::vec2, 6> texCoords{};
        std::array<glm::vec3, 6> normals{};
        std::array<unsigned int, 6> indices{};

        genBillboardMesh(verts, texCoords, normals);
        for (unsigned int i = 0; i < 6; ++i)
            indices[i] = i;

        for (unsigned int i = 0; i < 6; ++i)
        {
            m_storage->vertexBufferPtr->position = glm::vec4(verts[i], 1.0f);
            m_storage->vertexBufferPtr->texCoord = texCoords[i];
            m_storage->vertexBufferPtr->normal = normals[i];
            m_storage->vertexBufferPtr->entityID = entityID;
            m_storage->vertexBufferPtr++;
        }

        std::ranges::for_each(indices, [this](unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index;
        });
    }
}
