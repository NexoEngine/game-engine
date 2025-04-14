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

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <array>

#include "Renderer3D.hpp"
#include "RenderCommand.hpp"
#include "Logger.hpp"
#include "Shader.hpp"
#include "renderer/RendererExceptions.hpp"
#include "Path.hpp"

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

        auto phong = m_storage->shaderLibrary.load("Phong", Path::resolvePathRelativeToExe(
            "../resources/shaders/phong.glsl").string());
        m_storage->shaderLibrary.load("Outline pulse flat", Path::resolvePathRelativeToExe(
            "../resources/shaders/outline_pulse_flat.glsl").string());
        auto outlinePulseTransparentFlat = m_storage->shaderLibrary.load("Outline pulse transparent flat", Path::resolvePathRelativeToExe(
            "../resources/shaders/outline_pulse_transparent_flat.glsl").string());
        auto albedoUnshadedTransparent = m_storage->shaderLibrary.load("Albedo unshaded transparent", Path::resolvePathRelativeToExe(
            "../resources/shaders/albedo_unshaded_transparent.glsl").string());
        m_storage->shaderLibrary.load("Grid shader", Path::resolvePathRelativeToExe(
            "../resources/shaders/grid_shader.glsl").string());
        m_storage->shaderLibrary.load("Flat color", Path::resolvePathRelativeToExe(
            "../resources/shaders/flat_color.glsl").string());
        phong->bind();
        phong->setUniformIntArray(ShaderUniforms::TEXTURE_SAMPLER, samplers.data(), Renderer3DStorage::maxTextureSlots);
        phong->unbind();
        outlinePulseTransparentFlat->bind();
        outlinePulseTransparentFlat->setUniformIntArray(ShaderUniforms::TEXTURE_SAMPLER, samplers.data(), Renderer3DStorage::maxTextureSlots);
        outlinePulseTransparentFlat->unbind();
        albedoUnshadedTransparent->bind();
        albedoUnshadedTransparent->setUniformIntArray(ShaderUniforms::TEXTURE_SAMPLER, samplers.data(), Renderer3DStorage::maxTextureSlots);
        albedoUnshadedTransparent->unbind();

        m_storage->textureSlots[0] = m_storage->whiteTexture;

        LOG(NEXO_DEV, "Renderer3D initialized");
    }

    void Renderer3D::shutdown()
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_3D);
        m_storage.reset();
    }

    void Renderer3D::beginScene(const glm::mat4 &viewProjection, const glm::vec3 &cameraPos, const std::string &shader)
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_3D);
        if (shader.empty())
            m_storage->currentSceneShader = m_storage->shaderLibrary.get("Phong");
        else
            m_storage->currentSceneShader = m_storage->shaderLibrary.get(shader);
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
        m_storage->currentSceneShader->bind();
        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            m_storage->textureSlots[i]->bind(i);
        }
        RenderCommand::drawIndexed(m_storage->vertexArray, m_storage->indexCount);
        m_storage->stats.drawCalls++;
        m_storage->vertexArray->unbind();
        m_storage->vertexBuffer->unbind();
        m_storage->currentSceneShader->unbind();
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

    int Renderer3D::getTextureIndex(const std::shared_ptr<Texture2D> &texture) const
    {
        int textureIndex = 0.0f;

        if (!texture)
            return textureIndex;

        for (unsigned int i = 0; i < m_storage->textureSlotIndex; ++i)
        {
            if (*m_storage->textureSlots[i].get() == *texture)
            {
                textureIndex = i;
                break;
            }
        }

        if (textureIndex == 0)
        {
            textureIndex = m_storage->textureSlotIndex;
            m_storage->textureSlots[m_storage->textureSlotIndex] = texture;
            m_storage->textureSlotIndex++;
        }

        return textureIndex;
    }

    void Renderer3D::setMaterialUniforms(const renderer::InternalMaterial& material) const
    {
        if (!m_storage)
            THROW_EXCEPTION(RendererNotInitialized, RendererType::RENDERER_3D);

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
