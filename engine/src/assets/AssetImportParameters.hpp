//// AssetImportParameters.hpp ////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        03/03/2025
//  Description: Import parameter definitions for asset importers
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

namespace nexo::assets {

    /**
     * @brief Base class for all import parameter structs
     */
    struct ImportParametersBase {
        /**
         * @brief Convert parameters to JSON
         * @return JSON representation of parameters
         */
        virtual nlohmann::json toJson() const = 0;

        /**
         * @brief Apply JSON values to parameters
         * @param j JSON object to apply
         */
        virtual void fromJson(const nlohmann::json& j) = 0;

        virtual ~ImportParametersBase() = default;
    };

    /**
     * @brief Import parameters for 3D models
     */
    struct ModelImportParameters : public ImportParametersBase {
        // Mesh processing options
        bool calculateTangentSpace = false;
        bool joinIdenticalVertices = true;
        bool generateSmoothNormals = false;
        bool optimizeMeshes = true;
        int maxBones = 60;

        // Scene options
        bool importAnimations = true;
        bool importMaterials = true;
        bool importTextures = true;
        float globalScale = 1.0f;

        // Texture options
        enum class TextureQuality { Low, Medium, High };
        TextureQuality textureQuality = TextureQuality::Medium;
        bool convertToUncompressed = false;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ModelImportParameters,
            calculateTangentSpace,
            joinIdenticalVertices,
            generateSmoothNormals,
            optimizeMeshes,
            maxBones,
            importAnimations,
            importMaterials,
            importTextures,
            globalScale,
            textureQuality,
            convertToUncompressed
        );
    };

    /**
     * @brief Import parameters for textures
     */
    struct TextureImportParameters : public ImportParametersBase {
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

    // You can add more parameter structs for other asset types

} // namespace nexo::assets
