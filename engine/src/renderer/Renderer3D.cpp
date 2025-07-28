//// Renderer3D.cpp ///////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/11/2024
//  Description: Source file for the renderer3D class
//
///////////////////////////////////////////////////////////////////////////////

#include "ShaderLibrary.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>
#include <array>

#include "Renderer3D.hpp"
#include "RenderCommand.hpp"
#include "Logger.hpp"
#include "Shader.hpp"
#include "renderer/RendererExceptions.hpp"
#include <glad/glad.h>
#include "Path.hpp"

namespace nexo::renderer {

    void NxRenderer3D::init()
    {
        m_storage = std::make_shared<NxRenderer3DStorage>();

        m_storage->vertexArray = createVertexArray();
        m_storage->vertexBuffer = createVertexBuffer(m_storage->maxVertices * sizeof(NxVertex));

        // Layout
        const NxBufferLayout cubeVertexBufferLayout = {
            {NxShaderDataType::FLOAT3, "aPos"},
            {NxShaderDataType::FLOAT2, "aTexCoord"},
            {NxShaderDataType::FLOAT3, "aNormal"},
            {NxShaderDataType::FLOAT3, "aTangent"},
            {NxShaderDataType::FLOAT3, "aBiTangent"},
            {NxShaderDataType::INT, "aEntityID"}
        };
        m_storage->vertexBuffer->setLayout(cubeVertexBufferLayout);
        m_storage->vertexArray->addVertexBuffer(m_storage->vertexBuffer);

        m_storage->indexBuffer = createIndexBuffer();
        m_storage->vertexArray->setIndexBuffer(m_storage->indexBuffer);

        // Texture
        m_storage->whiteTexture = NxTexture2D::create(1, 1);
        unsigned int whiteTextureData = 0xffffffff;
        m_storage->whiteTexture->setData(&whiteTextureData, sizeof(unsigned int));

        // Shader
        std::array<int, NxRenderer3DStorage::maxTextureSlots> samplers{};
        for (int i = 0; i < static_cast<int>(NxRenderer3DStorage::maxTextureSlots); ++i)
            samplers[i] = i;

        const auto phong = ShaderLibrary::getInstance().get("Phong");
        const auto outlinePulseTransparentFlat = ShaderLibrary::getInstance().get("Outline pulse transparent flat");
        const auto albedoUnshadedTransparent = ShaderLibrary::getInstance().get("Albedo unshaded transparent");
        phong->bind();
        phong->setUniformIntArray(NxShaderUniforms::TEXTURE_SAMPLER, samplers.data(), NxRenderer3DStorage::maxTextureSlots);
        phong->unbind();
        outlinePulseTransparentFlat->bind();
        outlinePulseTransparentFlat->setUniformIntArray(NxShaderUniforms::TEXTURE_SAMPLER, samplers.data(), NxRenderer3DStorage::maxTextureSlots);
        outlinePulseTransparentFlat->unbind();
        albedoUnshadedTransparent->bind();
        albedoUnshadedTransparent->setUniformIntArray(NxShaderUniforms::TEXTURE_SAMPLER, samplers.data(), NxRenderer3DStorage::maxTextureSlots);
        albedoUnshadedTransparent->unbind();

        m_storage->textureSlots[0] = m_storage->whiteTexture;

        LOG(NEXO_DEV, "NxRenderer3D initialized");
    }

    void NxRenderer3D::shutdown()
    {
        if (!m_storage)
            THROW_EXCEPTION(NxRendererNotInitialized, NxRendererType::RENDERER_3D);
        m_storage.reset();
    }

    void NxRenderer3D::bindTextures() const
    {
        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            m_storage->textureSlots[i]->bind(i);
        }
    }

    void NxRenderer3D::unbindTextures() const
    {
        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            m_storage->textureSlots[i]->unbind(i);
        }
        m_storage->textureSlotIndex = 1;
    }

    void NxRenderer3D::beginScene(const glm::mat4 &viewProjection, const glm::vec3 &cameraPos, const std::string &shader)
    {
        if (!m_storage)
            THROW_EXCEPTION(NxRendererNotInitialized, NxRendererType::RENDERER_3D);
        if (shader.empty())
            m_storage->currentSceneShader = ShaderLibrary::getInstance().get("Phong");
        else
            m_storage->currentSceneShader = ShaderLibrary::getInstance().get(shader);
        m_storage->currentSceneShader->bind();
        m_storage->vertexArray->bind();
        m_storage->vertexBuffer->bind();
        m_storage->currentSceneShader->setUniformMatrix("uViewProjection", viewProjection);
        m_storage->cameraPosition = cameraPos;
        m_storage->currentSceneShader->setUniformFloat3("uCamPos", cameraPos);
        m_storage->indexCount = 0;
        m_storage->vertexBufferPtr = m_storage->vertexBufferBase.data();
        m_storage->indexBufferPtr = m_storage->indexBufferBase.data();
        m_storage->textureSlotIndex = 1;
        m_renderingScene = true;
    }

    void NxRenderer3D::endScene() const
    {
        if (!m_storage)
            THROW_EXCEPTION(NxRendererNotInitialized, NxRendererType::RENDERER_3D);
        if (!m_renderingScene)
            THROW_EXCEPTION(NxRendererSceneLifeCycleFailure, NxRendererType::RENDERER_3D,
                        "Renderer not rendering a scene, make sure to call beginScene first");
        const auto vertexDataSize = static_cast<unsigned int>(
            reinterpret_cast<std::byte*>(m_storage->vertexBufferPtr) -
            reinterpret_cast<std::byte*>(m_storage->vertexBufferBase.data())
        );


        m_storage->vertexBuffer->setData(m_storage->vertexBufferBase.data(), vertexDataSize);

        m_storage->indexBuffer->setData(m_storage->indexBufferBase.data(), m_storage->indexCount);

        flushAndReset();
    }

    void NxRenderer3D::flush() const
    {
        m_storage->currentSceneShader->bind();
        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            m_storage->textureSlots[i]->bind(i);
        }
        NxRenderCommand::drawIndexed(m_storage->vertexArray, m_storage->indexCount);
        m_storage->stats.drawCalls++;
        m_storage->vertexArray->unbind();
        m_storage->vertexBuffer->unbind();
        m_storage->currentSceneShader->unbind();
        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            m_storage->textureSlots[i]->unbind(i);
        }
    }

    void NxRenderer3D::flushAndReset() const
    {
        flush();
        m_storage->indexCount = 0;
        m_storage->vertexBufferPtr = m_storage->vertexBufferBase.data();
        m_storage->indexBufferPtr = m_storage->indexBufferBase.data();
        m_storage->textureSlotIndex = 1;
    }

    int NxRenderer3D::getTextureIndex(const std::shared_ptr<NxTexture2D> &texture) const
    {
        int textureIndex = 0;

        if (!texture)
            return textureIndex;

        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            if (*m_storage->textureSlots[i].get() == *texture)
            {
                textureIndex = static_cast<int>(i);
                break;
            }
        }

        if (textureIndex == 0)
        {
            textureIndex = static_cast<int>(m_storage->textureSlotIndex);
            m_storage->textureSlots[m_storage->textureSlotIndex] = texture;
            m_storage->textureSlotIndex++;
        }

        return textureIndex;
    }

    void NxRenderer3D::setMaterialUniforms(const NxIndexedMaterial& material) const
    {
        if (!m_storage)
            THROW_EXCEPTION(NxRendererNotInitialized, NxRendererType::RENDERER_3D);

        m_storage->currentSceneShader->setUniformFloat4("uMaterial.albedoColor", material.albedoColor);
        m_storage->currentSceneShader->setUniformInt("uMaterial.albedoTexIndex", material.albedoTexIndex);
        m_storage->currentSceneShader->setUniformFloat4("uMaterial.specularColor", material.specularColor);
        m_storage->currentSceneShader->setUniformInt("uMaterial.specularTexIndex", material.specularTexIndex);
        m_storage->currentSceneShader->setUniformFloat3("uMaterial.emissiveColor", material.emissiveColor);
        m_storage->currentSceneShader->setUniformInt("uMaterial.emissiveTexIndex", material.emissiveTexIndex);
        m_storage->currentSceneShader->setUniformFloat("uMaterial.roughness", material.roughness);
        m_storage->currentSceneShader->setUniformInt("uMaterial.roughnessTexIndex", material.roughnessTexIndex);
        m_storage->currentSceneShader->setUniformFloat("uMaterial.metallic", material.metallic);
        m_storage->currentSceneShader->setUniformInt("uMaterial.metallicTexIndex", material.metallicTexIndex);
        m_storage->currentSceneShader->setUniformFloat("uMaterial.opacity", material.opacity);
        m_storage->currentSceneShader->setUniformInt("uMaterial.opacityTexIndex", material.opacityTexIndex);
    }

    void NxRenderer3D::resetStats() const
    {
        if (!m_storage)
            THROW_EXCEPTION(NxRendererNotInitialized, NxRendererType::RENDERER_3D);
        m_storage->stats.drawCalls = 0;
        m_storage->stats.cubeCount = 0;
    }

    NxRenderer3DStats NxRenderer3D::getStats() const
    {
        if (!m_storage)
            THROW_EXCEPTION(NxRendererNotInitialized, NxRendererType::RENDERER_3D);
        return m_storage->stats;
    }

}
