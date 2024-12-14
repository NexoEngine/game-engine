//// AssetImporter.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/12/2024
//  Description: Header file for the AssetImporter class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Asset.hpp"

namespace nexo::assets {

    /**
     * @class AssetImporter
     *
     * @brief Interface for importing assets into the engine.
     */
    class AssetImporter {
        public:
            AssetImporter() = default;
            virtual ~AssetImporter() = default;

            /*/**
             * @brief Imports an asset from a file.
             *
             * @param path The path to the file to import.
             * @return The imported asset.
             #1#
            virtual AssetRef<Asset> import(std::filesystem::path path) = 0;*/
    };

} // namespace nexo::assets
