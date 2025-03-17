//// ModelParameters.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        11/03/2025
//  Description: Header file for Import parameters for 3D Models
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>

#include "json.hpp"

#include "../Texture/TextureParameters.hpp"

namespace nexo::assets {

    /**
     * @brief Import parameters for 3D models
     */
    struct ModelImportParameters {
        std::vector<TextureImportParameters> textureParameters;

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ModelImportParameters,
            textureParameters
        );
    };

    /**
     * @brief Post-process import parameters for 3D Models
     */
    struct ModelImportPostProcessParameters {
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

        NLOHMANN_DEFINE_TYPE_INTRUSIVE(ModelImportPostProcessParameters,
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


} // namespace nexo::assets
