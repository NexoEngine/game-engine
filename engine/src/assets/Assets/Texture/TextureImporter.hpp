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
 * Constructs a TextureImporter object using default initialization.
 */
TextureImporter() = default;
            /**
 * @brief Default destructor for TextureImporter.
 *
 * Overrides the base class destructor to ensure proper cleanup of the TextureImporter instance.
 */
~TextureImporter() override = default;

            /**
             * @brief Determines if the importer can read the provided input variant.
             *
             * Evaluates the input variant to check whether it contains either a file or memory input for texture data.
             * - If the variant holds a file input, the method delegates validation to the file-based checker.
             * - If the variant holds a memory input, it delegates to the memory-based checker.
             * Returns false if the input type is unsupported.
             *
             * @param inputVariant Variant containing texture input data, either as a file or from memory.
             * @return true if the input can be read as a texture; false otherwise.
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
             * @brief Imports a texture asset based on the input type.
             *
             * This function creates a new Texture asset and loads texture data either from a file or from memory,
             * depending on the type of input provided in the asset importer context. When the input is a file, it
             * initializes the texture using the file path; if the input is memory-based, it uses the memory data.
             * The texture is then wrapped in a TextureData object, assigned a unique identifier, and registered as
             * the main asset in the context.
             *
             * @param ctx The asset importer context containing input data and methods for setting the main asset.
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
             * @brief Determines whether the provided memory data contains a valid image for texture import.
             *
             * This function checks if the memory data within the given input can be interpreted as a valid image by
             * invoking stb_image's `stbi_info_from_memory`. It returns true if the image header information is recognized,
             * indicating that the memory data represents a valid texture.
             *
             * @param input Memory input containing raw image data to be validated.
             * @return true if the image data is valid; false otherwise.
             */
            bool canReadMemory(const ImporterMemoryInput& input)
            {
                const int ok = stbi_info_from_memory(input.memoryData.data(), input.memoryData.size(), nullptr, nullptr, nullptr);
                return ok;
            }

            /**
             * @brief Checks whether the file specified in the input can be read as a texture asset.
             *
             * This function uses stb_image's stbi_info function to validate that the file at the given path
             * is accessible and in a supported image format.
             *
             * @param input The file input containing the path of the texture asset.
             * @return True if the file is a valid and readable texture file; false otherwise.
             */
            bool canReadFile(const ImporterFileInput& input)
            {
                const int ok = stbi_info(input.filePath.string().c_str(), nullptr, nullptr, nullptr);
                return ok;
            }

    };

} // namespace nexo::assets
