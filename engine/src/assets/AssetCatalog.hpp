//// AssetCatalog.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        03/12/2024
//  Description: Header file for the AssetCatalog class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AssetImporter.hpp"

namespace nexo::assets {


    /**
    * @class AssetCatalog
    *
    * @brief Singleton class that holds all the assets in the engine.
    */
    class AssetCatalog {
        public:
            AssetCatalog() = default;
            ~AssetCatalog() = default;

            /*void addAsset(AssetID id, AssetRef<Asset> asset);
            void removeAsset(AssetID id);
            void removeAsset(AssetRef<Asset> asset);

            [[nodiscard]] AssetRef<Asset> getAsset(AssetID id) const;
            [[nodiscard]] AssetRef<Asset> getAsset(const std::string &name) const;

            [[nodiscard]] bool hasAsset(AssetID id) const;
            [[nodiscard]] bool hasAsset(const std::string &name) const;

            [[nodiscard]] std::size_t size() const;

        private:
            std::unordered_map<AssetID, AssetRef<Asset>> m_assets;*/
    };


} // namespace nexo::assets
