//// Renderer2D.cpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for renderer 2D class
//
///////////////////////////////////////////////////////////////////////////////
#include "Renderer2D.hpp"
#include "Shader.hpp"
#include "VertexArray.hpp"
#include "RenderCommand.hpp"
#include "Logger.hpp"
#include "renderer/RendererExceptions.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <array>
#include <Path.hpp>

namespace nexo::renderer {
    void Renderer2D::init()
    {
        m_storage = std::make_shared<Renderer2DStorage>();

        // Initialize vertex array and buffer
        m_storage->vertexArray = createVertexArray();
        m_storage->vertexBuffer = createVertexBuffer(m_storage->maxVertices * sizeof(QuadVertex));

        // Define layout for the vertex buffer
        const BufferLayout quadVertexBufferLayout = {
            {ShaderDataType::FLOAT3, "aPos"}, // Position
            {ShaderDataType::FLOAT4, "aColor"}, // Color
            {ShaderDataType::FLOAT2, "aTexCoord"}, // Texture Coordinates
            {ShaderDataType::FLOAT, "aTexIndex"} // Texture Index
        };
        m_storage->vertexBuffer->setLayout(quadVertexBufferLayout);
        m_storage->vertexArray->addVertexBuffer(m_storage->vertexBuffer);

        // Initialize vertex data storage
        m_storage->vertexBufferPtr = m_storage->vertexBufferBase.data();

        // Initialize index data storage
        m_storage->indexBufferPtr = m_storage->indexBufferBase.data();

        // Create index buffer and attach it to the vertex array
        m_storage->indexBuffer = createIndexBuffer();
        m_storage->vertexArray->setIndexBuffer(m_storage->indexBuffer);

        // Initialize white texture
        m_storage->whiteTexture = Texture2D::create(1, 1);
        unsigned int whiteTextureData = 0xffffffff;
        m_storage->whiteTexture->setData(&whiteTextureData, sizeof(unsigned int));

        // Setup texture samplers
        std::array<int, Renderer2DStorage::maxTextureSlots> samplers{};
        for (int i = 0; i < static_cast<int>(Renderer2DStorage::maxTextureSlots); ++i)
            samplers[i] = i;

        try
        {
            m_storage->textureShader = Shader::create(
                Path::resolvePathRelativeToExe("../assets/shaders/texture.glsl").string());
            m_storage->textureShader->bind();
            m_storage->textureShader->setUniformIntArray("uTexture", samplers.data(), Renderer2DStorage::maxTextureSlots);
        } catch (const Exception &e)
        {
            LOG_EXCEPTION(e);
        }

        m_storage->textureSlots[0] = m_storage->whiteTexture;

        // Predefine quad vertex positions
        m_storage->quadVertexPositions[0] = {-0.5f, -0.5f, 0.0f, 1.0f};
        m_storage->quadVertexPositions[1] = {0.5f, -0.5f, 0.0f, 1.0f};
        m_storage->quadVertexPositions[2] = {0.5f, 0.5f, 0.0f, 1.0f};
        m_storage->quadVertexPositions[3] = {-0.5f, 0.5f, 0.0f, 1.0f};

        LOG(NEXO_DEV, "Renderer2D quad renderer initialized");
        LOG(NEXO_INFO, "Renderer2D initialized");
    }


    void Renderer2D::shutdown()
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_2D);
        m_storage.reset();
    }

    void Renderer2D::beginScene(const glm::mat4 &viewProjection)
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_2D);
        m_storage->textureShader->bind();
        m_storage->vertexArray->bind();
        m_storage->vertexBuffer->bind();
        m_storage->textureShader->setUniformMatrix("viewProjection", viewProjection);
        m_storage->indexCount = 0;
        m_storage->vertexBufferPtr = m_storage->vertexBufferBase.data();
        m_storage->indexBufferPtr = m_storage->indexBufferBase.data();
        m_storage->textureSlotIndex = 1;
        m_renderingScene = true;
    }

    void Renderer2D::flush() const
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

    void Renderer2D::flushAndReset() const
    {
        flush();
        m_storage->indexCount = 0;
        m_storage->vertexBufferPtr = m_storage->vertexBufferBase.data();
        m_storage->indexBufferPtr = m_storage->indexBufferBase.data();
        m_storage->textureSlotIndex = 1;
    }

    void Renderer2D::endScene() const
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_2D);
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        const auto vertexDataSize = static_cast<unsigned int>(
            reinterpret_cast<std::byte*>(m_storage->vertexBufferPtr) -
            reinterpret_cast<std::byte*>(m_storage->vertexBufferBase.data())
        );

        m_storage->vertexBuffer->setData(m_storage->vertexBufferBase.data(), vertexDataSize);
        m_storage->indexBuffer->setData(m_storage->indexBufferBase.data(), m_storage->indexCount);

        flushAndReset();
    }


    void Renderer2D::generateQuadVertices(const glm::mat4 &transform, const glm::vec4 color, const float textureIndex,
                                          const glm::vec2 *textureCoords) const
    {
        constexpr unsigned int quadVertexCount = 4;
        constexpr unsigned int quadIndexCount = 6;

        // Ensure we don't overflow the buffers
        if ((m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data()) + quadVertexCount > m_storage->maxVertices ||
            m_storage->indexCount + quadIndexCount > m_storage->maxIndices)
        {
            flushAndReset();
        }

        // Calculate vertex offset for this quad
        auto vertexOffset = static_cast<unsigned int>(m_storage->vertexBufferPtr - m_storage->vertexBufferBase.data());

        // Generate quad vertices
        for (unsigned int i = 0; i < quadVertexCount; ++i)
        {
            m_storage->vertexBufferPtr->position = transform * m_storage->quadVertexPositions[i];
            m_storage->vertexBufferPtr->color = color;
            m_storage->vertexBufferPtr->texCoord = textureCoords[i];
            m_storage->vertexBufferPtr->texIndex = textureIndex;
            m_storage->vertexBufferPtr++;
        }

        // Generate quad indices dynamically
        m_storage->indexBufferPtr[0] = vertexOffset + 0;
        m_storage->indexBufferPtr[1] = vertexOffset + 1;
        m_storage->indexBufferPtr[2] = vertexOffset + 2;

        m_storage->indexBufferPtr[3] = vertexOffset + 2;
        m_storage->indexBufferPtr[4] = vertexOffset + 3;
        m_storage->indexBufferPtr[5] = vertexOffset + 0;

        m_storage->indexBufferPtr += quadIndexCount;
        m_storage->indexCount += quadIndexCount; // Increment index count
    }


    float Renderer2D::getTextureIndex(const std::shared_ptr<Texture2D> &texture) const
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

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        drawQuad({pos.x, pos.y, 0.0f}, size, color);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec4 &color) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        if (m_storage->indexCount >= m_storage->maxIndices)
            flushAndReset();
        constexpr glm::vec2 textureCoords[4] = {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
        };

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                                    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        constexpr float textureIndex = 0.0f;

        generateQuadVertices(transform, color, textureIndex, textureCoords);

        m_storage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size,
                              const std::shared_ptr<Texture2D> &texture) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        drawQuad({pos.x, pos.y, 0.0f}, size, texture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size,
                              const std::shared_ptr<Texture2D> &texture) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        if (m_storage->indexCount >= m_storage->maxIndices)
            flushAndReset();
        constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        constexpr glm::vec2 textureCoords[4] = {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
        };

        const float textureIndex = getTextureIndex(texture);

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                                    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        generateQuadVertices(transform, color, textureIndex, textureCoords);

        m_storage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size,
                              const std::shared_ptr<SubTexture2D> &subTexture) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        drawQuad({pos.x, pos.y, 0.0f}, size, subTexture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size,
                              const std::shared_ptr<SubTexture2D> &subTexture) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        if (m_storage->indexCount >= m_storage->maxIndices)
            flushAndReset();
        constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        const glm::vec2 *textureCoords = subTexture->getTextureCoords();

        const float textureIndex = getTextureIndex(subTexture->getTexture());

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                                    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        generateQuadVertices(transform, color, textureIndex, textureCoords);

        m_storage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const float rotation,
                              const glm::vec4 &color) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        drawQuad({pos.x, pos.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        if (m_storage->indexCount >= m_storage->maxIndices)
            flushAndReset();
        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                                    glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                                    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});
        constexpr float textureIndex = 0.0f;
        constexpr glm::vec2 textureCoords[4] = {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
        };

        generateQuadVertices(transform, color, textureIndex, textureCoords);

        m_storage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const float rotation,
                              const std::shared_ptr<Texture2D> &texture) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        drawQuad({pos.x, pos.y, 0.0f}, size, rotation, texture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const float rotation,
                              const std::shared_ptr<Texture2D> &texture) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        if (m_storage->indexCount >= m_storage->maxIndices)
            flushAndReset();
        constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        constexpr glm::vec2 textureCoords[4] = {
            glm::vec2(0.0f, 0.0f),
            glm::vec2(1.0f, 0.0f),
            glm::vec2(1.0f, 1.0f),
            glm::vec2(0.0f, 1.0f),
        };

        const float textureIndex = getTextureIndex(texture);

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                                    glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                                    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        generateQuadVertices(transform, color, textureIndex, textureCoords);

        m_storage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const float rotation,
                              const std::shared_ptr<SubTexture2D> &subTexture) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        drawQuad({pos.x, pos.y, 0.0f}, size, rotation, subTexture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const float rotation,
                              const std::shared_ptr<SubTexture2D> &subTexture) const
    {
        if (!m_renderingScene)
            THROW_EXCEPTION(RendererSceneLifeCycleFailure, RendererType::RENDERER_2D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        if (m_storage->indexCount >= m_storage->maxIndices)
            flushAndReset();
        constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        const glm::vec2 *textureCoords = subTexture->getTextureCoords();

        const float textureIndex = getTextureIndex(subTexture->getTexture());

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                                    glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                                    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        generateQuadVertices(transform, color, textureIndex, textureCoords);

        m_storage->stats.quadCount++;
    }

    void Renderer2D::resetStats() const
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_2D);
        m_storage->stats.drawCalls = 0;
        m_storage->stats.quadCount = 0;
    }

    RendererStats Renderer2D::getStats() const
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_2D);
        return m_storage->stats;
    }

}
