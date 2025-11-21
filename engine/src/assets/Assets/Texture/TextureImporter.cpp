//// TextureImporter.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        13/04/2025
//  Description: Implementation file for the TextureImporter class
//
///////////////////////////////////////////////////////////////////////////////

#include "TextureImporter.hpp"
#include "TextureParameters.hpp"

#include <array>
#include <boost/uuid/random_generator.hpp>
#include <stb_image.h>
#include "assets/AssetImporterBase.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include "assets/AssetImporterContext.hpp"

namespace nexo::assets {

    bool TextureImporter::canRead(const ImporterInputVariant& inputVariant)
    {
        if (std::holds_alternative<ImporterFileInput>(inputVariant))
            return canReadFile(std::get<ImporterFileInput>(inputVariant));
        if (std::holds_alternative<ImporterMemoryInput>(inputVariant)) {
            return canReadMemory(std::get<ImporterMemoryInput>(inputVariant));
        }
        return false;
    }

    void TextureImporter::importImpl(AssetImporterContext& ctx) {
        // Get import parameters
        const auto params = ctx.getParameters<TextureImportParameters>();

        auto asset = std::make_unique<Texture>();
        std::shared_ptr<renderer::NxTexture2D> rendererTexture;

        if (std::holds_alternative<ImporterFileInput>(ctx.input)) {
            const auto& fileInput = std::get<ImporterFileInput>(ctx.input);
            // Use PBR-aware texture creation
            rendererTexture = renderer::NxTexture2D::create(
                fileInput.filePath.string(),
                params.textureType,
                params.generateMipmaps
            );
        } else {
            const auto& memInput = std::get<ImporterMemoryInput>(ctx.input);
            // rendererTexture = renderer::NxTexture2D::create(
            //     memInput.memoryData.data(),
            //     static_cast<unsigned int>(memInput.memoryData.size()),
            //     params.textureType,
            //     params.generateMipmaps
            // );
        }

        auto assetData = std::make_unique<TextureData>();
        assetData->texture = rendererTexture;
        assetData->textureType = params.textureType;  // Store type info

        asset->m_metadata.id = boost::uuids::random_generator()();
        asset->setData(std::move(assetData));
        ctx.setMainAsset(std::move(asset));
    }

    bool TextureImporter::canReadMemory(const ImporterMemoryInput& input) {
        // Handle special PBR texture formats
        if (input.formatHint == "ARGB8888" ||
            input.formatHint == "HDR" ||
            input.formatHint == "EXR" ||
            input.formatHint == "RGBE") {
            return true;
        }

        // Check for HDR formats
        if (stbi_is_hdr_from_memory(input.memoryData.data(), static_cast<int>(input.memoryData.size()))) {
            return true;
        }

        const int infoResult = stbi_info_from_memory(
            input.memoryData.data(),
            static_cast<int>(input.memoryData.size()),
            nullptr, nullptr, nullptr
        );
        return infoResult;
    }

    bool TextureImporter::canReadFile(const ImporterFileInput& input) {
        const std::string pathStr = input.filePath.string();

        // Check for HDR formats first
        if (stbi_is_hdr(pathStr.c_str())) {
            return true;
        }

        // Check standard formats
        const int infoResult = stbi_info(pathStr.c_str(), nullptr, nullptr, nullptr);
        return infoResult;
    }

} // namespace nexo::assets
