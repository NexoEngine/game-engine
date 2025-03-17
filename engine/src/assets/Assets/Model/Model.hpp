//// Model.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        24/02/2025
//  Description: Header file for the Model class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "assets/Asset.hpp"

#include <assimp/scene.h>
#include <assimp/Importer.hpp>

namespace nexo::assets {

    struct ModelData {
        // Model data
        // TODO: Implement model data
        const aiScene *scene;
    };

    class ModelImporter;

    /**
     * @class Model
     *
     * @brief Represents a 3D model asset.
     */
    class Model final : public Asset<ModelData, AssetType::MODEL> {
        public:
            Model() = default;

            ~Model() override = default;
    };

}
