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

#include <glm/gtc/matrix_transform.hpp>
#include <array>

namespace nexo::renderer {

    struct QuadVertex {
        glm::vec3 position;
        glm::vec4 color;
        glm::vec2 texCoord;

        float texIndex;
    };

    struct Renderer2DStorage {
        const unsigned int maxQuads = 10000;
        const unsigned int maxVertices = maxQuads * 4;
        const unsigned int maxIndices = maxQuads * 6;
        static const unsigned int maxTextureSlots = 32;

        std::shared_ptr<Shader> textureShader;
        std::shared_ptr<VertexArray> quadVertexArray;
        std::shared_ptr<VertexBuffer> quadVertexBuffer;
        std::shared_ptr<Texture2D> whiteTexture;

        unsigned int quadIndexCount = 0;
        QuadVertex *quadVertexBufferBase = nullptr;
        QuadVertex *quadVertexBufferPtr = nullptr;

        std::array<std::shared_ptr<Texture2D>, maxTextureSlots> textureSlots;
        unsigned int textureSlotIndex = 1;

        glm::vec4 quadVertexPositions[4];

        Renderer2D::RendererStats stats;
    };


    static Renderer2DStorage *s_Renderer2DStorage;

    void Renderer2D::init()
    {
        s_Renderer2DStorage = new Renderer2DStorage();

        s_Renderer2DStorage->quadVertexArray = createVertexArray();
        // Vertex buffer init
        s_Renderer2DStorage->quadVertexBuffer = createVertexBuffer(s_Renderer2DStorage->maxVertices * sizeof(QuadVertex));

        // Layout init
        const BufferLayout quadVertexBufferLayout = {
            {ShaderDataType::FLOAT3, "aPos"},
            {ShaderDataType::FLOAT4, "aColor"},
            {ShaderDataType::FLOAT2, "aTexCoord"},
            {ShaderDataType::FLOAT, "aTexIndex"}
        };
        s_Renderer2DStorage->quadVertexBuffer->setLayout(quadVertexBufferLayout);
        s_Renderer2DStorage->quadVertexArray->addVertexBuffer(s_Renderer2DStorage->quadVertexBuffer);

        s_Renderer2DStorage->quadVertexBufferBase = new QuadVertex[s_Renderer2DStorage->maxVertices];

        // Index buffer init
        unsigned int offset = 0;
        unsigned int *quadIndices = new unsigned int[s_Renderer2DStorage->maxIndices];
        for (unsigned int i = 0; i < s_Renderer2DStorage->maxIndices; i += 6)
        {
            quadIndices[i + 0] = offset + 0;
            quadIndices[i + 1] = offset + 1;
            quadIndices[i + 2] = offset + 2;

            quadIndices[i + 3] = offset + 2;
            quadIndices[i + 4] = offset + 3;
            quadIndices[i + 5] = offset + 0;

            offset += 4;
        }
        const std::shared_ptr<IndexBuffer> quadIndexBuffer = createIndexBuffer(quadIndices, s_Renderer2DStorage->maxIndices);
        s_Renderer2DStorage->quadVertexArray->setIndexBuffer(quadIndexBuffer);
        delete[] quadIndices;

        s_Renderer2DStorage->whiteTexture = Texture2D::create(1, 1);
        unsigned int whiteTextureData = 0xffffffff;
        s_Renderer2DStorage->whiteTexture->setData(&whiteTextureData, sizeof(unsigned int));

        int samplers[Renderer2DStorage::maxTextureSlots];
        for (unsigned int i = 0; i < Renderer2DStorage::maxTextureSlots; ++i)
            samplers[i] = i;

        s_Renderer2DStorage->textureShader = Shader::create("../assets/shaders/texture.glsl");
        s_Renderer2DStorage->textureShader->bind();
        s_Renderer2DStorage->textureShader->setUniformIntArray("uTexture", samplers, Renderer2DStorage::maxTextureSlots);

        s_Renderer2DStorage->textureSlots[0] = s_Renderer2DStorage->whiteTexture;

        s_Renderer2DStorage->quadVertexPositions[0] = {-0.5, -0.5f, 0.0f, 1.0f};
        s_Renderer2DStorage->quadVertexPositions[1] = {0.5, -0.5f, 0.0f, 1.0f};
        s_Renderer2DStorage->quadVertexPositions[2] = {0.5, 0.5f, 0.0f, 1.0f};
        s_Renderer2DStorage->quadVertexPositions[3] = {-0.5, 0.5f, 0.0f, 1.0f};
    }

    void Renderer2D::shutdown()
    {
        delete s_Renderer2DStorage;
    }

    void Renderer2D::beginScene(const camera::OrthographicCamera &camera)
    {
        s_Renderer2DStorage->textureShader->bind();
        s_Renderer2DStorage->textureShader->setUniformMatrix("viewProjection", camera.getViewProjectionMatrix());
        s_Renderer2DStorage->quadIndexCount = 0;
        s_Renderer2DStorage->quadVertexBufferPtr = s_Renderer2DStorage->quadVertexBufferBase;

        s_Renderer2DStorage->textureSlotIndex = 1;
    }

    void Renderer2D::flush()
    {
        for (unsigned int i = 0; i < s_Renderer2DStorage->textureSlotIndex; ++i)
        {
            s_Renderer2DStorage->textureSlots[i]->bind(i);
        }
        RenderCommand::drawIndexed(s_Renderer2DStorage->quadVertexArray, s_Renderer2DStorage->quadIndexCount);
        s_Renderer2DStorage->stats.drawCalls++;
    }

    void Renderer2D::flushAndReset()
    {
        endScene();

        s_Renderer2DStorage->quadIndexCount = 0;
        s_Renderer2DStorage->quadVertexBufferPtr = s_Renderer2DStorage->quadVertexBufferBase;

        s_Renderer2DStorage->textureSlotIndex = 1;
    }

    void Renderer2D::endScene()
    {
        unsigned int dataSize = (uint8_t*)s_Renderer2DStorage->quadVertexBufferPtr - (uint8_t*)s_Renderer2DStorage->quadVertexBufferBase;
        s_Renderer2DStorage->quadVertexBuffer->setData(s_Renderer2DStorage->quadVertexBufferBase, dataSize);
        flush();
    }

    void Renderer2D::generateQuadVertices(const glm::mat4 &transform, const glm::vec4 color, const float textureIndex, const glm::vec2 *textureCoords)
    {
        constexpr unsigned int quadVertexCount = 4;

        for (unsigned int i = 0; i < quadVertexCount; ++i)
        {
            s_Renderer2DStorage->quadVertexBufferPtr->position = transform * s_Renderer2DStorage->quadVertexPositions[i];
            s_Renderer2DStorage->quadVertexBufferPtr->color = color;
            s_Renderer2DStorage->quadVertexBufferPtr->texCoord = textureCoords[i];
            s_Renderer2DStorage->quadVertexBufferPtr->texIndex = textureIndex;
            s_Renderer2DStorage->quadVertexBufferPtr++;
        }

        s_Renderer2DStorage->quadIndexCount += 6;
    }

    float Renderer2D::getTextureIndex(const std::shared_ptr<Texture2D> &texture)
    {
        float textureIndex = 0.0f;

        for (unsigned int i = 0; i < s_Renderer2DStorage->textureSlotIndex; ++i)
        {
            if (*s_Renderer2DStorage->textureSlots[i].get() == *texture.get())
            {
                textureIndex = static_cast<float>(i);
                break;
            }
        }

        if (textureIndex == 0)
        {
            textureIndex = static_cast<float>(s_Renderer2DStorage->textureSlotIndex);
            s_Renderer2DStorage->textureSlots[s_Renderer2DStorage->textureSlotIndex] = texture;
            s_Renderer2DStorage->textureSlotIndex++;
        }

        return textureIndex;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const glm::vec4 &color)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, color);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const glm::vec4 &color)
    {
        if (s_Renderer2DStorage->quadIndexCount >= s_Renderer2DStorage->maxIndices)
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

        s_Renderer2DStorage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, texture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<Texture2D> &texture)
    {
        if (s_Renderer2DStorage->quadIndexCount >= s_Renderer2DStorage->maxIndices)
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

        s_Renderer2DStorage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subTexture)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, subTexture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const std::shared_ptr<SubTexture2D> &subTexture)
    {
        if (s_Renderer2DStorage->quadIndexCount >= s_Renderer2DStorage->maxIndices)
            flushAndReset();
        constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        const glm::vec2 *textureCoords = subTexture->getTextureCoords();

        const float textureIndex = getTextureIndex(subTexture->getTexture());

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                            glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        generateQuadVertices(transform, color, textureIndex, textureCoords);

        s_Renderer2DStorage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const float rotation, const glm::vec4 &color)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, rotation, color);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, float rotation, const glm::vec4 &color)
    {
        if (s_Renderer2DStorage->quadIndexCount >= s_Renderer2DStorage->maxIndices)
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

        s_Renderer2DStorage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const float rotation,
        const std::shared_ptr<Texture2D> &texture)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, rotation, texture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const float rotation,
        const std::shared_ptr<Texture2D> &texture)
    {
        if (s_Renderer2DStorage->quadIndexCount >= s_Renderer2DStorage->maxIndices)
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

        s_Renderer2DStorage->stats.quadCount++;
    }

    void Renderer2D::drawQuad(const glm::vec2 &pos, const glm::vec2 &size, const float rotation,
    const std::shared_ptr<SubTexture2D> &subTexture)
    {
        drawQuad({pos.x, pos.y, 0.0f}, size, rotation, subTexture);
    }

    void Renderer2D::drawQuad(const glm::vec3 &pos, const glm::vec2 &size, const float rotation,
        const std::shared_ptr<SubTexture2D> &subTexture)
    {
        if (s_Renderer2DStorage->quadIndexCount >= s_Renderer2DStorage->maxIndices)
            flushAndReset();
        constexpr glm::vec4 color = {1.0f, 1.0f, 1.0f, 1.0f};
        const glm::vec2 *textureCoords = subTexture->getTextureCoords();

        const float textureIndex = getTextureIndex(subTexture->getTexture());

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) *
                                    glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f)) *
                                    glm::scale(glm::mat4(1.0f), {size.x, size.y, 1.0f});

        generateQuadVertices(transform, color, textureIndex, textureCoords);

        s_Renderer2DStorage->stats.quadCount++;
    }

    void Renderer2D::resetStats()
    {
        s_Renderer2DStorage->stats.drawCalls = 0;
        s_Renderer2DStorage->stats.quadCount = 0;
    }

    Renderer2D::RendererStats Renderer2D::getStats()
    {
        return s_Renderer2DStorage->stats;
    }

}
