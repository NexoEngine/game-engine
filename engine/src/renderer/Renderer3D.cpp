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

#include "ShaderLibrary.hpp"
#define GLM_ENABLE_EXPERIMENTAL
#include <array>
#include <glm/gtx/string_cast.hpp>

#include <glad/glad.h>
#include "Logger.hpp"
#include "Path.hpp"
#include "RenderCommand.hpp"
#include "Renderer3D.hpp"
#include "Shader.hpp"
#include "renderer/RendererExceptions.hpp"

namespace nexo::renderer {

    void NxRenderer3D::init()
    {
        m_storage = std::make_shared<NxRenderer3DStorage>();

        // Texture
        m_storage->whiteTexture       = NxTexture2D::create(1, 1);
        unsigned int whiteTextureData = 0xffffffff;
        m_storage->whiteTexture->setData(&whiteTextureData, sizeof(unsigned int));

        // Shader
        std::array<int, NxRenderer3DStorage::maxTextureSlots> samplers{};
        for (int i = 0; i < static_cast<int>(NxRenderer3DStorage::maxTextureSlots); ++i) samplers[i] = i;

        const auto phong                       = ShaderLibrary::getInstance().get("Phong");
        const auto outlinePulseTransparentFlat = ShaderLibrary::getInstance().get("Outline pulse transparent flat");
        const auto albedoUnshadedTransparent   = ShaderLibrary::getInstance().get("Albedo unshaded transparent");
        phong->bind();
        phong->setUniformIntArray(NxShaderUniforms::TEXTURE_SAMPLER, samplers.data(),
                                  NxRenderer3DStorage::maxTextureSlots);
        phong->unbind();
        outlinePulseTransparentFlat->bind();
        outlinePulseTransparentFlat->setUniformIntArray(NxShaderUniforms::TEXTURE_SAMPLER, samplers.data(),
                                                        NxRenderer3DStorage::maxTextureSlots);
        outlinePulseTransparentFlat->unbind();
        albedoUnshadedTransparent->bind();
        albedoUnshadedTransparent->setUniformIntArray(NxShaderUniforms::TEXTURE_SAMPLER, samplers.data(),
                                                      NxRenderer3DStorage::maxTextureSlots);
        albedoUnshadedTransparent->unbind();

        const auto pbr = ShaderLibrary::getInstance().get("PBR");
        pbr->bind();
        pbr->setUniformIntArray("uTexture", samplers.data(),
                                NxRenderer3DStorage::maxTextureSlots);
        int cubeSamplers[10];
        for (int i = 0; i < 10; ++i) {
            cubeSamplers[i] = 32 + i; // texture units 32..(32+10-1)
        }
        pbr->setUniformIntArray("uPointShadowMaps", cubeSamplers, 10);
        pbr->unbind();

        m_storage->textureSlotsBatch.clear();
        m_storage->textureSlotsBatch.resize(1);
        m_storage->textureSlotsBatch[0][0] = m_storage->whiteTexture;

        m_storage->nbTexturesInBatch.clear();
        m_storage->nbTexturesInBatch.push_back(1);

        m_storage->currentTextureBatchIndex  = 0;
        m_storage->currentTextureBindedIndex = 0;

        LOG(NEXO_DEV, "NxRenderer3D initialized");
    }

    void NxRenderer3D::shutdown()
    {
        if (!m_storage) THROW_EXCEPTION(NxRendererNotInitialized, NxRendererType::RENDERER_3D);
        m_storage.reset();
    }

    void NxRenderer3D::bindTextureBatch(unsigned int batchIndex) const
    {
        if (batchIndex >= m_storage->textureSlotsBatch.size()) {
            LOG(NEXO_ERROR, "Trying to bind invalid texture batch {}", batchIndex);
            return;
        }

        m_storage->currentTextureBindedIndex = batchIndex;
        unsigned int maxTextureSlot = m_storage->nbTexturesInBatch[batchIndex];
        for (unsigned int i = 0; i < maxTextureSlot; ++i) {
            if (m_storage->textureSlotsBatch[batchIndex][i])
                m_storage->textureSlotsBatch[batchIndex][i]->bind(i);
        }
    }

    void NxRenderer3D::bindTextures() const
    {
        bindTextureBatch(0);
    }

    void NxRenderer3D::bindNextTextureBatch() const
    {
        m_storage->currentTextureBindedIndex++;
        bindTextureBatch(m_storage->currentTextureBindedIndex);
    }

    void NxRenderer3D::unbindTextures() const
    {
        for (unsigned int batch = 0; batch < m_storage->textureSlotsBatch.size(); ++batch) {
            unsigned int maxTextureSlot = m_storage->nbTexturesInBatch[batch];
            for (unsigned int i = 0; i < maxTextureSlot; ++i) {
                if (m_storage->textureSlotsBatch[batch][i])
                    m_storage->textureSlotsBatch[batch][i]->unbind(i);
            }
        }

        m_storage->textureSlotsBatch.clear();
        m_storage->textureSlotsBatch.resize(1);
        m_storage->textureSlotsBatch[0][0] = m_storage->whiteTexture;

        m_storage->nbTexturesInBatch.clear();
        m_storage->nbTexturesInBatch.push_back(1);

        m_storage->currentTextureBatchIndex  = 0;
        m_storage->currentTextureBindedIndex = 0;
    }

    int NxRenderer3D::getTextureIndex(const std::shared_ptr<NxTexture2D> &texture) const
    {
        int textureIndex = 0;
        if (!texture) return textureIndex;

        unsigned int batch = m_storage->currentTextureBatchIndex;
        if (batch >= m_storage->textureSlotsBatch.size()) {
            LOG(NEXO_ERROR, "Invalid texture batch index {}", batch);
            return 0;
        }

        unsigned int currentMax = m_storage->nbTexturesInBatch[batch];

        for (unsigned int i = 0; i < currentMax; ++i) {
            if (m_storage->textureSlotsBatch[batch][i] &&
                *m_storage->textureSlotsBatch[batch][i] == *texture) {
                return static_cast<int>(i);
            }
        }

        if (currentMax >= NxRenderer3DStorage::maxTextureSlots) {
            LOG(NEXO_ERROR, "Too many textures in batch {} (>{}), cannot add more",
                batch, NxRenderer3DStorage::maxTextureSlots);
            return 0;
        }

        textureIndex = static_cast<int>(currentMax);
        m_storage->textureSlotsBatch[batch][currentMax] = texture;
        m_storage->nbTexturesInBatch[batch]++;

        return textureIndex;
    }

    void NxRenderer3D::switchToNextTextureBatch()
    {
        m_storage->currentTextureBatchIndex++;

        const auto newIndex = m_storage->currentTextureBatchIndex;

        if (newIndex >= m_storage->textureSlotsBatch.size()) {
            m_storage->textureSlotsBatch.resize(newIndex + 1);
            m_storage->nbTexturesInBatch.resize(newIndex + 1, 0);
        }

        m_storage->textureSlotsBatch[newIndex][0] = m_storage->whiteTexture;
        m_storage->nbTexturesInBatch[newIndex]    = 1;
    }

} // namespace nexo::renderer
