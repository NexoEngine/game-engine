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
            /**
 * @brief Default constructor for TextureImporter.
 *
 * Initializes a new instance of the TextureImporter class using default settings.
 */
TextureImporter() = default;
            /**
 * @brief Default destructor for TextureImporter.
 *
 * Overrides the base class destructor. Uses the compiler-generated default implementation to handle cleanup.
 */
~TextureImporter() override = default;

            /**
             * @brief Determines if the provided input variant is a readable texture asset.
             *
             * Checks whether the given input variant holds file-based or memory-based texture data and validates it using the
             * corresponding helper function. Returns false if the input type is unsupported.
             *
             * @param inputVariant Variant containing texture input data.
             * @return true if the texture data can be read successfully, false otherwise.
             */
            bool canRead(const ImporterInputVariant& inputVariant) override
            {
                if (std::holds_alternative<ImporterFileInput>(inputVariant))
                    return canReadFile(std::get<ImporterFileInput>(inputVariant));
                if (std::holds_alternative<ImporterMemoryInput>(inputVariant)) {
                    return canReadMemory(std::get<ImporterMemoryInput>(inputVariant));
                }
                return false;
            }

            /**
             * @brief Imports a texture asset using the provided importer context.
             *
             * This method creates a new texture asset by processing the input variant from the context.
             * If the input contains a file path, it loads the texture from file; if it contains memory data,
             * it creates the texture from the provided data buffer. The resulting renderer texture is wrapped
             * in a TextureData object, associated with a new Texture asset, and assigned a unique identifier.
             * Finally, the texture asset is set as the main asset in the importer context.
             *
             * @param ctx The asset importer context containing the input variant and used to store the imported asset.
             */
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

            /**
             * @brief Determines if the input memory data contains valid texture information.
             *
             * This function uses stb_image's stbi_info_from_memory to verify whether the provided memory data
             * can be interpreted as a valid texture. It checks the memory header without loading the full image.
             *
             * @param input Memory input encapsulating the raw texture data.
             * @return true if the texture information could be successfully read from the memory data, false otherwise.
             */
            bool canReadMemory(const ImporterMemoryInput& input)
            {
                const int ok = stbi_info_from_memory(input.memoryData.data(), input.memoryData.size(), nullptr, nullptr, nullptr);
                return ok;
            }

            /**
             * @brief Checks if the file specified in the provided input can be read as a texture.
             *
             * This function uses stb_image's stbi_info to determine if the file, indicated by the input's file path,
             * contains valid image header information suitable for texture assets.
             *
             * @param input File input containing the texture file path.
             * @return true if the file is readable as a texture, false otherwise.
             */
            bool canReadFile(const ImporterFileInput& input)
            {
                const int ok = stbi_info(input.filePath.string().c_str(), nullptr, nullptr, nullptr);
                return ok;
            }

    };

} // namespace nexo::assets
