//// TextureParameters.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        11/03/2025
//  Description: Header file for Import parameters for Textures
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "json.hpp"

namespace nexo::assets {

    /**
     * @brief Import parameters for textures
     */
    struct TextureImportParameters {
        bool generateMipmaps = true;
        bool convertToSRGB = true;
        bool flipVertically = true;

        enum class Format {
            Preserve,    // Keep original format
            RGB,         // Convert to RGB
            RGBA,        // Convert to RGBA with alpha
            BC1,         // Block compression (DXT1)
            BC3,         // Block compression (DXT5)
            BC7          // High quality block compression
        };
        Format format = Format::Preserve;

        int maxSize = 4096;         // Max texture dimension
        float compressionQuality = 0.9f;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(TextureImportParameters,
            generateMipmaps,
            convertToSRGB,
            flipVertically,
            format,
            maxSize,
            compressionQuality
        );
    };

    /**
     * @brief Post-process import parameters for textures
     */
    struct TexturesImportPostProcessParameters {
    };


} // namespace nexo::assets
