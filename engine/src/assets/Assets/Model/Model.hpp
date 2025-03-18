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

    /**
     * @class Model
     *
     * @brief Represents a 3D model asset.
     */
    class Model final : public Asset<ModelData, AssetType::MODEL> {
        public:
            /**
 * @brief Default constructor for the Model class.
 *
 * Constructs a Model asset with default initialization. The Model class encapsulates
 * 3D model data via the associated ModelData structure, and this constructor allows
 * for straightforward instantiation without additional configuration.
 */
Model() = default;

            /**
 * @brief Default destructor.
 *
 * Overrides the base class destructor. No additional cleanup is required beyond the default behavior.
 */
~Model() override = default;
    };

}
