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
        // Texture data
        // TODO: Implement texture data
        std::shared_ptr<renderer::Texture> texture;
    };

    /**
     * @class Texture
     *
     * @brief Represents a texture asset.
     */
    class Texture final : public Asset<TextureData, AssetType::TEXTURE> {
        public:
            /**
 * @brief Default constructs a Texture object.
 *
 * This constructor initializes a Texture instance using default initialization.
 */
Texture() = default;

            /**
 * @brief Default destructor for the Texture class.
 *
 * Overrides the base class destructor and ensures proper cleanup of managed resources
 * using the default implementation.
 */
~Texture() override = default;
    };

}
