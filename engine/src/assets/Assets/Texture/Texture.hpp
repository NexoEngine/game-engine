//// Texture.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/02/2025
//  Description: Header file for the Texture class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "assets/Asset.hpp"

namespace nexo::assets {

    struct TextureData {
        std::shared_ptr<renderer::NxTexture2D> texture;
    };

    /**
     * @class Texture
     *
     * @brief Represents a texture asset.
     */
    class Texture final : public Asset<TextureData, AssetType::TEXTURE> {
        public:
            Texture() = default;

            /**
             * @brief Constructs a Texture object from a file path.
             *
             * @param path The path to the texture file.
             */
            explicit Texture(const std::filesystem::path &path)
                : Asset()
            {
                auto texture = renderer::NxTexture2D::create(path.string());
                auto textureData = new TextureData();
                textureData->texture = texture;
                setData(textureData);
            }

            ~Texture() override = default;
    };

}
