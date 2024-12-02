//// Renderer3D.cpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Source file for the renderer3D class
//
///////////////////////////////////////////////////////////////////////////////

#include "Renderer3D.hpp"
#include "RenderCommand.hpp"
#include "Logger.hpp"
#include "renderer/RendererExceptions.hpp"

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <array>
#include <Path.hpp>
#include <algorithm>

namespace nexo::renderer {

    void Renderer3D::init()
    {
        m_storage = std::make_shared<Renderer3DStorage>();

        m_storage->vertexArray = createVertexArray();
        m_storage->vertexBuffer = createVertexBuffer(m_storage->maxVertices * sizeof(Vertex));

        // Layout
        const BufferLayout cubeVertexBufferLayout = {
            {ShaderDataType::FLOAT3, "aPos"},
            {ShaderDataType::FLOAT4, "aColor"},
            {ShaderDataType::FLOAT2, "aTexCoord"},
            {ShaderDataType::FLOAT, "aTexIndex"},
            {ShaderDataType::FLOAT3, "aNormal"}
        };
        m_storage->vertexBuffer->setLayout(cubeVertexBufferLayout);
        m_storage->vertexArray->addVertexBuffer(m_storage->vertexBuffer);

        m_storage->indexBuffer = createIndexBuffer();
        m_storage->vertexArray->setIndexBuffer(m_storage->indexBuffer);

        // Texture
        m_storage->whiteTexture = Texture2D::create(1, 1);
        unsigned int whiteTextureData = 0xffffffff;
        m_storage->whiteTexture->setData(&whiteTextureData, sizeof(unsigned int));

        // Shader
        std::array<int, Renderer3DStorage::maxTextureSlots> samplers{};
        for (int i = 0; i < static_cast<int>(Renderer3DStorage::maxTextureSlots); ++i)
            samplers[i] = i;

        m_storage->textureShader = Shader::create(Path::resolvePathRelativeToExe(
            "../assets/shaders/texture.glsl").string());
        m_storage->textureShader->bind();
        m_storage->textureShader->setUniformIntArray("uTexture", samplers.data(), Renderer3DStorage::maxTextureSlots);

        m_storage->textureSlots[0] = m_storage->whiteTexture;

        LOG(NEXO_DEV, "Renderer3D initialized");
    }

    void Renderer3D::shutdown()
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_3D);
        m_storage.reset();
    }

    void Renderer3D::beginScene(const glm::mat4 &viewProjection, const glm::vec3 &cameraPos)
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_3D);
        m_storage->textureShader->bind();
        m_storage->vertexArray->bind();
        m_storage->vertexBuffer->bind();
        m_storage->textureShader->setUniformMatrix("viewProjection", viewProjection);
        m_storage->textureShader->setUniformFloat3("camPos", cameraPos);
        m_storage->indexCount = 0;
        m_storage->vertexBufferPtr = m_storage->vertexBufferBase.data();
        m_storage->indexBufferPtr = m_storage->indexBufferBase.data();
        m_storage->textureSlotIndex = 1;
        m_renderingScene = true;
    }

    void Renderer3D::endScene() const
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_3D);
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        const auto vertexDataSize = static_cast<unsigned int>(
            reinterpret_cast<std::byte*>(m_storage->vertexBufferPtr) -
            reinterpret_cast<std::byte*>(m_storage->vertexBufferBase.data())
        );


        m_storage->vertexBuffer->setData(m_storage->vertexBufferBase.data(), vertexDataSize);

        m_storage->indexBuffer->setData(m_storage->indexBufferBase.data(), m_storage->indexCount);

        flushAndReset();
    }

    void Renderer3D::flush() const
    {
        m_storage->textureShader->bind();
        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            m_storage->textureSlots[i]->bind(i);
        }
        RenderCommand::drawIndexed(m_storage->vertexArray, m_storage->indexCount);
        m_storage->stats.drawCalls++;
        m_storage->vertexArray->unbind();
        m_storage->vertexBuffer->unbind();
    }

    void Renderer3D::flushAndReset() const
    {
        flush();
        m_storage->indexCount = 0;
        m_storage->vertexBufferPtr = m_storage->vertexBufferBase.data();
        m_storage->indexBufferPtr = m_storage->indexBufferBase.data();
        m_storage->textureSlotIndex = 1;
    }

    void Renderer3D::generateCubeVertices(const glm::mat4 &transform, const glm::vec4 &color, float textureIndex,
                                          const glm::vec2 *textureCoords) const
    {
        constexpr glm::vec3 positions[8] = {
            {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}
        };

        for (unsigned int i = 0; i < 8; ++i)
        {
            m_storage->vertexBufferPtr->position = transform * glm::vec4(positions[i], 1.0f);

            m_storage->vertexBufferPtr->color = color;
            m_storage->vertexBufferPtr->texCoord = textureCoords[i % 4];
            m_storage->vertexBufferPtr->texIndex = textureIndex;
            m_storage->vertexBufferPtr++;
        }

        m_storage->indexCount += 36;
    }

    float Renderer3D::getTextureIndex(const std::shared_ptr<Texture2D> &texture) const
    {
        float textureIndex = 0.0f;

        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            if (*m_storage->textureSlots[i].get() == *texture)
            {
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        if (textureIndex == 0)
        {
            textureIndex = static_cast<float>(m_storage->textureSlotIndex);
            m_storage->textureSlots[m_storage->textureSlotIndex] = texture;
            m_storage->textureSlotIndex++;
        }

        return textureIndex;
    }

    void Renderer3D::drawCube(const glm::vec3 &position, const glm::vec3 &size, const glm::vec4 &color) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        constexpr glm::vec3 cubePositions[8] = {
            {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}
        };

        constexpr unsigned int cubeIndices[36] = {
            // Front face
            0, 1, 2, 2, 3, 0,
            // Back face
            4, 5, 6, 6, 7, 4,
            // Bottom face
            0, 1, 5, 5, 4, 0,
            // Top face
            3, 2, 6, 6, 7, 3,
            // Left face
            0, 3, 7, 7, 4, 0,
            // Right face
            1, 2, 6, 6, 5, 1
        };

        static const glm::vec3 vertexNormals[8] = {
            glm::normalize(glm::vec3(-1.0f, -1.0f, -1.0f)), // Vertex 0
            glm::normalize(glm::vec3(1.0f, -1.0f, -1.0f)),  // Vertex 1
            glm::normalize(glm::vec3(1.0f, 1.0f, -1.0f)),   // Vertex 2
            glm::normalize(glm::vec3(-1.0f, 1.0f, -1.0f)),  // Vertex 3
            glm::normalize(glm::vec3(-1.0f, -1.0f, 1.0f)),  // Vertex 4
            glm::normalize(glm::vec3(1.0f, -1.0f, 1.0f)),   // Vertex 5
            glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f)),    // Vertex 6
            glm::normalize(glm::vec3(-1.0f, 1.0f, 1.0f))    // Vertex 7
        };

        constexpr glm::vec2 textureCoords[4] = {
            {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 1.0f}, {0.0f, 1.0f}
        };

        // Transform matrix
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                    glm::scale(glm::mat4(1.0f), size);

        // Check buffer limits
        if ((m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data()) + 8 > m_storage->maxVertices ||
            m_storage->indexCount + 36 > m_storage->maxIndices)
        {
            flushAndReset();
        }

        // Vertex data
        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());
        for (unsigned int i = 0; i < 8; ++i)
        {
            m_storage->vertexBufferPtr->position = transform * glm::vec4(cubePositions[i], 1.0f);
            m_storage->vertexBufferPtr->color = color;
            m_storage->vertexBufferPtr->texCoord = textureCoords[i % 4];
            m_storage->vertexBufferPtr->texIndex = 0.0f; // White texture
            m_storage->vertexBufferPtr->normal = vertexNormals[i];
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(cubeIndices, [this, vertexOffset](unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index + vertexOffset;
        });


        // Update stats
        m_storage->stats.cubeCount++;
    }


    void Renderer3D::drawCube(const glm::vec3 &position, const glm::vec3 &size,
                              const std::shared_ptr<Texture2D> &texture) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        constexpr glm::vec3 cubePositions[8] = {
            {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
            {0.5f, 0.5f, -0.5f}, {-0.5f, 0.5f, -0.5f},
            {-0.5f, -0.5f, 0.5f}, {0.5f, -0.5f, 0.5f},
            {0.5f, 0.5f, 0.5f}, {-0.5f, 0.5f, 0.5f}
        };

        constexpr unsigned int cubeIndices[36] = {
            0, 1, 2, 2, 3, 0, 4, 5, 6, 6, 7, 4,
            0, 1, 5, 5, 4, 0, 3, 2, 6, 6, 7, 3,
            0, 3, 7, 7, 4, 0, 1, 2, 6, 6, 5, 1
        };

        constexpr glm::vec2 textureCoords[4] = {
            {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 1.0f}, {0.0f, 1.0f}
        };

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                    glm::scale(glm::mat4(1.0f), size);

        // Check buffer limits
        if ((m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data()) + 8 > m_storage->maxVertices ||
            m_storage->indexCount + 36 > m_storage->maxIndices)
        {
            flushAndReset();
        }

        const float textureIndex = getTextureIndex(texture);

        // Vertex data
        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());
        for (unsigned int i = 0; i < 8; ++i)
        {
            m_storage->vertexBufferPtr->position = transform * glm::vec4(cubePositions[i], 1.0f);
            m_storage->vertexBufferPtr->color = {1.0f, 1.0f, 1.0f, 1.0f};
            m_storage->vertexBufferPtr->texCoord = textureCoords[i % 4];
            m_storage->vertexBufferPtr->texIndex = textureIndex;
            m_storage->vertexBufferPtr->normal = glm::normalize(cubePositions[i]);
            m_storage->vertexBufferPtr++;
        }

        // Index data
        std::ranges::for_each(cubeIndices, [this, vertexOffset](unsigned int index) {
            m_storage->indexBufferBase[m_storage->indexCount++] = index + vertexOffset;
        });


        m_storage->stats.cubeCount++;
    }

    void Renderer3D::drawMesh(const std::vector<Vertex> &vertices, const std::vector<unsigned int> &indices,
                              const std::shared_ptr<Texture2D> &texture) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        if ((m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data()) + vertices.size() > m_storage->maxVertices ||
            m_storage->indexCount + indices.size() > m_storage->maxIndices)
        {
            //TODO: Implement the batch rendering for meshes
            LOG(NEXO_INFO, "Max number attained");
        }
        const float textureIndex = getTextureIndex(texture);

        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());


        for (const auto &vertex: vertices)
        {
            m_storage->vertexBufferPtr->position = vertex.position;
            m_storage->vertexBufferPtr->color = {1.0f, 1.0f, 1.0f, 1.0f};
            m_storage->vertexBufferPtr->texCoord = vertex.texCoord;
            m_storage->vertexBufferPtr->texIndex = textureIndex;
            m_storage->vertexBufferPtr->normal = vertex.normal;
            m_storage->vertexBufferPtr++;
        }

        for (const auto &index : indices)
        {
            m_storage->indexBufferBase[m_storage->indexCount++] = index + vertexOffset;
        }
    }

    void Renderer3D::resetStats() const
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_3D);
        m_storage->stats.drawCalls = 0;
        m_storage->stats.cubeCount = 0;
    }

    Renderer3DStats Renderer3D::getStats() const
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_3D);
        return m_storage->stats;
    }

}
