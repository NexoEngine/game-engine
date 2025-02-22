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

namespace nexo::renderer {

    void Renderer3D::init()
    {
        m_storage = std::make_shared<Renderer3DStorage>();

        m_storage->vertexArray = createVertexArray();
        m_storage->vertexBuffer = createVertexBuffer(m_storage->maxVertices * sizeof(Vertex));

        // Layout
        const BufferLayout cubeVertexBufferLayout = {
            {ShaderDataType::FLOAT3, "aPos"},
            {ShaderDataType::FLOAT2, "aTexCoord"},
            {ShaderDataType::FLOAT3, "aNormal"},
            {ShaderDataType::FLOAT3, "aTangent"},
            {ShaderDataType::FLOAT3, "aBiTangent"},
            {ShaderDataType::INT, "aEntityID"}
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
        m_storage->textureShader->unbind();
        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            m_storage->textureSlots[i]->unbind(i);
        }
    }

    void Renderer3D::flushAndReset() const
    {
        flush();
        m_storage->indexCount = 0;
        m_storage->vertexBufferPtr = m_storage->vertexBufferBase.data();
        m_storage->indexBufferPtr = m_storage->indexBufferBase.data();
        m_storage->textureSlotIndex = 1;
    }

    float Renderer3D::getTextureIndex(const std::shared_ptr<Texture2D> &texture) const
    {
        float textureIndex = 0.0f;

        if (!texture)
            return textureIndex;

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

    void Renderer3D::setMaterialUniforms(const renderer::Material& material) const
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_3D);

        m_storage->textureShader->setUniformFloat4("material.albedoColor", material.albedoColor);
        m_storage->textureShader->setUniformInt("material.albedoTexIndex", material.albedoTexIndex);
        m_storage->textureShader->setUniformFloat4("material.specularColor", material.specularColor);
        m_storage->textureShader->setUniformInt("material.specularTexIndex", material.specularTexIndex);
        m_storage->textureShader->setUniformFloat3("material.emissiveColor", material.emissiveColor);
        m_storage->textureShader->setUniformInt("material.emissiveTexIndex", material.emissiveTexIndex);
        m_storage->textureShader->setUniformFloat("material.roughness", material.roughness);
        m_storage->textureShader->setUniformInt("material.roughnessTexIndex", material.roughnessTexIndex);
        m_storage->textureShader->setUniformFloat("material.metallic", material.metallic);
        m_storage->textureShader->setUniformInt("material.metallicTexIndex", material.metallicTexIndex);
        m_storage->textureShader->setUniformFloat("material.opacity", material.opacity);
        m_storage->textureShader->setUniformInt("material.opacityTexIndex", material.opacityTexIndex);
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
