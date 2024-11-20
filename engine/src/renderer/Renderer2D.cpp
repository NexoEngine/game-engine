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
#include "core/exceptions/Exceptions.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <array>

namespace nexo::renderer {
    void Renderer2D::init()
    {
        m_storage = new Renderer2DStorage();

        m_storage->quadVertexArray = createVertexArray();
        // Vertex buffer init
        m_storage->quadVertexBuffer = createVertexBuffer(m_storage->maxVertices * sizeof(QuadVertex));

        // Layout init
        const BufferLayout quadVertexBufferLayout = {
            {ShaderDataType::FLOAT3, "aPos"},
            {ShaderDataType::FLOAT4, "aColor"},
            {ShaderDataType::FLOAT2, "aTexCoord"},
            {ShaderDataType::FLOAT, "aTexIndex"}
        };
        m_storage->quadVertexBuffer->setLayout(quadVertexBufferLayout);
        m_storage->quadVertexArray->addVertexBuffer(m_storage->quadVertexBuffer);

        m_storage->quadVertexBufferBase = new QuadVertex[m_storage->maxVertices];

        // Index buffer init
        unsigned int offset = 0;
        unsigned int *quadIndices = new unsigned int[m_storage->maxIndices];
        for (unsigned int i = 0; i < m_storage->maxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }
        const std::shared_ptr<IndexBuffer> quadIndexBuffer = createIndexBuffer(quadIndices, m_storage->maxIndices);
        m_storage->quadVertexArray->setIndexBuffer(quadIndexBuffer);
        delete[] quadIndices;

        m_storage->whiteTexture = Texture2D::create(1, 1);
        unsigned int whiteTextureData = 0xffffffff;
        m_storage->whiteTexture->setData(&whiteTextureData, sizeof(unsigned int));

        int samplers[Renderer2DStorage::maxTextureSlots];
        for (unsigned int i = 0; i < Renderer2DStorage::maxTextureSlots; ++i)
            samplers[i] = i;

        try {
            m_storage->textureShader = Shader::create("../assets/shaders/texture.glsl");
            m_storage->textureShader->bind();
            m_storage->textureShader->setUniformIntArray("uTexture", samplers, Renderer2DStorage::maxTextureSlots);
        } catch (const Exception &e) {
            LOG_EXCEPTION(e);
        }

        m_storage->textureSlots[0] = m_storage->whiteTexture;

        m_storage->quadVertexPositions[0] = {-0.5, -0.5f, 0.0f, 1.0f};
        m_storage->quadVertexPositions[1] = {0.5, -0.5f, 0.0f, 1.0f};
        m_storage->quadVertexPositions[2] = {0.5, 0.5f, 0.0f, 1.0f};
        m_storage->quadVertexPositions[3] = {-0.5, 0.5f, 0.0f, 1.0f};
        LOG(NEXO_DEV, "Renderer2D quad renderer initialized");
        LOG(NEXO_INFO, "Renderer2D initialized");

    }

    void Renderer2D::shutdown()
    {
        delete m_storage->quadVertexBufferBase;
        delete m_storage;
    }

    void Renderer2D::beginScene(const glm::mat4 &viewProjection)
    {
        m_storage->textureShader->bind();
        m_storage->quadVertexArray->bind();
        m_storage->quadVertexBuffer->bind();
        m_storage->textureShader->setUniformMatrix("viewProjection", viewProjection);
        m_storage->quadIndexCount = 0;
        m_storage->quadVertexBufferPtr = m_storage->quadVertexBufferBase;

        m_storage->textureSlotIndex = 1;
    }

    void Renderer2D::flush()
    {
        m_storage->textureShader->bind();
        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            m_storage->textureSlots[i]->bind(i);
        }
        RenderCommand::drawIndexed(m_storage->quadVertexArray, m_storage->quadIndexCount);
        m_storage->stats.drawCalls++;
        m_storage->quadVertexArray->unbind();
        m_storage->quadVertexBuffer->unbind();
    }

    void Renderer2D::flushAndReset()
    {
        endScene();

        m_storage->quadIndexCount = 0;
        m_storage->quadVertexBufferPtr = m_storage->quadVertexBufferBase;

        m_storage->textureSlotIndex = 1;
    }

    void Renderer2D::endScene()
    {
        unsigned int dataSize = (uint8_t*)m_storage->quadVertexBufferPtr - (uint8_t*)m_storage->quadVertexBufferBase;
        m_storage->quadVertexBuffer->setData(m_storage->quadVertexBufferBase, dataSize);
        flush();
    }

    void Renderer2D::generateQuadVertices(const glm::mat4 &transform, const glm::vec4 color, const float textureIndex, const glm::vec2 *textureCoords)
    {
        constexpr unsigned int quadVertexCount = 4;

        for (unsigned int i = 0; i < quadVertexCount; ++i)
        {
            m_storage->quadVertexBufferPtr->position = transform * m_storage->quadVertexPositions[i];
            m_storage->quadVertexBufferPtr->color = color;
            m_storage->quadVertexBufferPtr->texCoord = textureCoords[i];
            m_storage->quadVertexBufferPtr->texIndex = textureIndex;
            m_storage->quadVertexBufferPtr++;
        }

        m_storage->quadIndexCount += 6;
    }

    float Renderer2D::getTextureIndex(const std::shared_ptr<Texture2D> &texture)
    {
        float textureIndex = 0.0f;

        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            if (*m_storage->textureSlots[i].get() == *texture.get())
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

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, color);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec4 &color)
    {
        if (m_storage->quadIndexCount >= m_storage->maxIndices)
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

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, texture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture)
    {
        if (m_storage->quadIndexCount >= m_storage->maxIndices)
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

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subTexture)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, subTexture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subTexture)
    {
        if (m_storage->quadIndexCount >= m_storage->maxIndices)
            flushAndReset();
        constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        const glm::vec2 *textureCoords = subTexture->getTextureCoords();

        const float textureIndex = getTextureIndex(subTexture->getTexture());

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                            glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        generateQuadVertices(transform, color, textureIndex, textureCoords);

        m_storage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const float rotation, const glm::vec4 &color)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color)
    {
        if (m_storage->quadIndexCount >= m_storage->maxIndices)
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
        const std::shared_ptr<Texture2D> &texture)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, rotation, texture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const float rotation,
        const std::shared_ptr<Texture2D> &texture)
    {
        if (m_storage->quadIndexCount >= m_storage->maxIndices)
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
    const std::shared_ptr<SubTexture2D> &subTexture)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, rotation, subTexture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const float rotation,
        const std::shared_ptr<SubTexture2D> &subTexture)
    {
        if (m_storage->quadIndexCount >= m_storage->maxIndices)
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

    void Renderer2D::resetStats()
    {
        m_storage->stats.drawCalls = 0;
        m_storage->stats.quadCount = 0;
    }

    RendererStats Renderer2D::getStats()
    {
        return m_storage->stats;
    }

}
