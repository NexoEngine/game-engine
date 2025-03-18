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
 * @brief Default constructs a TextureImporter object.
 *
 * Initializes a new instance of the TextureImporter class using the default settings.
 */
TextureImporter() = default;
            /**
 * @brief Default destructor for TextureImporter.
 *
 * This defaulted destructor ensures that the TextureImporter is properly cleaned up,
 * delegating resource management to the base class.
 */
~TextureImporter() override = default;

            /**
             * @brief Determines if the provided input variant contains a valid texture source.
             *
             * This function inspects the input variant and checks if it holds either a file-based or 
             * memory-based texture input. It delegates the validation to the corresponding helper 
             * function (canReadFile or canReadMemory) based on the input type. Returns false if the 
             * variant does not match any supported input type.
             *
             * @param inputVariant The variant input encapsulating either a file or memory resource.
             * @return true if the input is valid and readable; otherwise, false.
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
             * This method creates a new texture asset by examining the input contained in the context.
             * If the input is file-based, it loads the texture from the specified file path; otherwise, it
             * creates the texture from in-memory data. A new texture data object is then instantiated,
             * associating the renderer texture with the asset, and a unique identifier is generated for
             * the asset's metadata. Finally, the asset is set as the main asset in the context.
             *
             * @param ctx The importer context containing input data and used to store the main asset.
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
             * @brief Checks if the memory input contains valid texture image data.
             *
             * This function verifies that the memory buffer provided in the input
             * holds valid image information, using stb_image's functionality.
             *
             * @param input Memory input containing the texture data.
             * @return True if valid image information is detected, false otherwise.
             */
            bool canReadMemory(const ImporterMemoryInput& input)
            {
                const int ok = stbi_info_from_memory(input.memoryData.data(), input.memoryData.size(), nullptr, nullptr, nullptr);
                return ok;
            }

            /**
             * @brief Checks if the file specified in the input contains valid image header data.
             *
             * This function uses stb_image's stbi_info to verify that the file at the provided file path
             * can be recognized as an image, meaning it possesses the necessary header information for further processing.
             *
             * @param input File input containing the path to the image asset.
             * @return true if the file is readable as an image; false otherwise.
             */
            bool canReadFile(const ImporterFileInput& input)
            {
                const int ok = stbi_info(input.filePath.string().c_str(), nullptr, nullptr, nullptr);
                return ok;
            }

    };

} // namespace nexo::assets
