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

#include <glm/gtc/matrix_transform.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <array>

namespace nexo::renderer {

    void Renderer3D::init()
    {
        m_storage = new Renderer3DStorage();

        m_storage->cubeVertexArray = createVertexArray();
        m_storage->cubeVertexBuffer = createVertexBuffer(m_storage->maxVertices * sizeof(CubeVertex));

        // Layout
        const BufferLayout cubeVertexBufferLayout = {
            {ShaderDataType::FLOAT3, "aPos"},
            {ShaderDataType::FLOAT4, "aColor"},
            {ShaderDataType::FLOAT2, "aTexCoord"},
            {ShaderDataType::FLOAT, "aTexIndex"}
        };
        m_storage->cubeVertexBuffer->setLayout(cubeVertexBufferLayout);
        m_storage->cubeVertexArray->addVertexBuffer(m_storage->cubeVertexBuffer);

        m_storage->cubeVertexBufferBase = new CubeVertex[m_storage->maxVertices];

        // Indices
        unsigned int offset = 0;
        auto* cubeIndices = new unsigned int[m_storage->maxIndices];
        for (unsigned int i = 0; i < m_storage->maxIndices; i += 36)
        {
            // Define the cube's indices (two triangles per face, six faces)
            unsigned int indices[36] = {
                0, 1, 2, 2, 3, 0, // Front face
                4, 5, 6, 6, 7, 4, // Back face
                0, 1, 5, 5, 4, 0, // Bottom face
                3, 2, 6, 6, 7, 3, // Top face
                0, 3, 7, 7, 4, 0, // Left face
                1, 2, 6, 6, 5, 1  // Right face
            };

            for (unsigned int j = 0; j < 36; ++j)
                cubeIndices[i + j] = indices[j] + offset;

            offset += 8;
        }
        const auto cubeIndexBuffer = createIndexBuffer(cubeIndices, m_storage->maxIndices);
        m_storage->cubeVertexArray->setIndexBuffer(cubeIndexBuffer);
        delete[] cubeIndices;

        // Texture
        m_storage->whiteTexture = Texture2D::create(1, 1);
        unsigned int whiteTextureData = 0xffffffff;
        m_storage->whiteTexture->setData(&whiteTextureData, sizeof(unsigned int));

        // Shader
        int samplers[Renderer3DStorage::maxTextureSlots];
        for (int i = 0; i < static_cast<int>(Renderer3DStorage::maxTextureSlots); ++i)
            samplers[i] = i;

        m_storage->textureShader = Shader::create("../assets/shaders/texture.glsl");
        m_storage->textureShader->bind();
        m_storage->textureShader->setUniformIntArray("uTexture", samplers, Renderer3DStorage::maxTextureSlots);

        m_storage->textureSlots[0] = m_storage->whiteTexture;

        LOG(NEXO_DEV, "Renderer3D initialized");
    }

    void Renderer3D::shutdown()
    {
        delete[] m_storage->cubeVertexBufferBase;
        delete m_storage;
    }

    void Renderer3D::beginScene(const glm::mat4& viewProjection) const
    {
        m_storage->textureShader->bind();
        m_storage->cubeVertexArray->bind();
        m_storage->cubeVertexBuffer->bind();
        m_storage->textureShader->setUniformMatrix("viewProjection", viewProjection);
        m_storage->cubeIndexCount = 0;
        m_storage->cubeVertexBufferPtr = m_storage->cubeVertexBufferBase;
        m_storage->textureSlotIndex = 1;
    }

    void Renderer3D::endScene() const
    {
        const unsigned int dataSize = reinterpret_cast<uint8_t*>(m_storage->cubeVertexBufferPtr) - reinterpret_cast<uint8_t*>(m_storage->cubeVertexBufferBase);

        m_storage->cubeVertexBuffer->setData(m_storage->cubeVertexBufferBase, dataSize);
        flushAndReset();
    }

    void Renderer3D::flush() const
    {
        m_storage->textureShader->bind();
        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            m_storage->textureSlots[i]->bind(i);
        }
        RenderCommand::drawIndexed(m_storage->cubeVertexArray, m_storage->cubeIndexCount);
        m_storage->stats.drawCalls++;
        m_storage->cubeVertexArray->unbind();
        m_storage->cubeVertexBuffer->unbind();
    }

    void Renderer3D::flushAndReset() const
    {
        flush();
        m_storage->cubeIndexCount = 0;
        m_storage->cubeVertexBufferPtr = m_storage->cubeVertexBufferBase;
        m_storage->textureSlotIndex = 1;
    }

    void Renderer3D::generateCubeVertices(const glm::mat4& transform, const glm::vec4& color, float textureIndex, const glm::vec2* textureCoords) const
    {
        constexpr glm::vec3 positions[8] = {
            {-0.5f, -0.5f, -0.5f}, {0.5f, -0.5f, -0.5f},
            {0.5f,  0.5f, -0.5f}, {-0.5f,  0.5f, -0.5f},
            {-0.5f, -0.5f,  0.5f}, {0.5f, -0.5f,  0.5f},
            {0.5f,  0.5f,  0.5f}, {-0.5f,  0.5f,  0.5f}
        };

        for (unsigned int i = 0; i < 8; ++i)
        {
            m_storage->cubeVertexBufferPtr->position = transform * glm::vec4(positions[i], 1.0f);

            m_storage->cubeVertexBufferPtr->color = color;
            m_storage->cubeVertexBufferPtr->texCoord = textureCoords[i % 4];
            m_storage->cubeVertexBufferPtr->texIndex = textureIndex;
            m_storage->cubeVertexBufferPtr++;
        }

        m_storage->cubeIndexCount += 36;
    }

    float Renderer3D::getTextureIndex(const std::shared_ptr<Texture2D>& texture) const
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

    void Renderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, const glm::vec4& color) const
    {
        constexpr glm::vec2 textureCoords[4] = {
            {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 1.0f}, {0.0f, 1.0f}
        };

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                    glm::scale(glm::mat4(1.0f), size);

        generateCubeVertices(transform, color, 0.0f, textureCoords);
    }


    void Renderer3D::drawCube(const glm::vec3& position, const glm::vec3& size, const std::shared_ptr<Texture2D>& texture) const
    {
        constexpr glm::vec2 textureCoords[4] = {
            {0.0f, 0.0f}, {1.0f, 0.0f},
            {1.0f, 1.0f}, {0.0f, 1.0f}
        };

        const glm::mat4 transform = glm::translate(glm::mat4(1.0f), position) *
                                    glm::scale(glm::mat4(1.0f), size);

        const float textureIndex = getTextureIndex(texture);
        generateCubeVertices(transform, {1.0f, 1.0f, 1.0f, 1.0f}, textureIndex, textureCoords);
    }

    void Renderer3D::resetStats() const
    {
        m_storage->stats.drawCalls = 0;
        m_storage->stats.cubeCount = 0;
    }

    Renderer3DStats Renderer3D::getStats() const
    {
        return m_storage->stats;
    }

}