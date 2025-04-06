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
 * @brief Default constructor for the Texture class.
 *
 * Initializes a Texture object with default settings.
 */
Texture() = default;

            /**
 * @brief Default destructor for the Texture asset.
 *
 * Explicitly overrides the base class destructor, ensuring proper resource cleanup.
 */
~Texture() override = default;
    };

}
