//// TextureImporter.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/02/2025
//  Description: Header file for the TextureImporter class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <array>
#include <stb_image.h>
#include "assets/AssetImporterBase.hpp"
#include "assets/Assets/Texture/Texture.hpp"
#include "TextureParameters.hpp"

namespace nexo::assets {

    class TextureImporter final : public AssetImporterBase {
        public:
            TextureImporter() = default;
            ~TextureImporter() override = default;

            bool canRead(const ImporterInputVariant& inputVariant) override
            {
                if (std::holds_alternative<ImporterFileInput>(inputVariant))
                    return canReadFile(std::get<ImporterFileInput>(inputVariant));
                if (std::holds_alternative<ImporterMemoryInput>(inputVariant)) {
                    return canReadMemory(std::get<ImporterMemoryInput>(inputVariant));
                }
                return false;
            }

            void importImpl(AssetImporterContext& ctx) override
            {
                auto asset = new Texture();
                std::shared_ptr<renderer::Texture> rendererTexture;
                if (std::holds_alternative<ImporterFileInput>(ctx.input))
                    rendererTexture = renderer::Texture2D::create(std::get<ImporterFileInput>(ctx.input).filePath.string());
                else {
                    auto data = std::get<ImporterMemoryInput>(ctx.input).memoryData;
                    rendererTexture = renderer::Texture2D::create(data.data(), data.size());
                }
                auto assetData = new TextureData();
                assetData->texture = rendererTexture;
                asset->setData(assetData);
                asset->m_metadata.id = boost::uuids::random_generator()();

                ctx.setMainAsset(asset);
            }

        protected:

            bool canReadMemory(const ImporterMemoryInput& input)
            {
                const int ok = stbi_info_from_memory(input.memoryData.data(), input.memoryData.size(), nullptr, nullptr, nullptr);
                return ok;
            }

            bool canReadFile(const ImporterFileInput& input)
            {
                const int ok = stbi_info(input.filePath.string().c_str(), nullptr, nullptr, nullptr);
                return ok;
            }

    };

} // namespace nexo::assets
