//// AssetManager.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        18/11/2024
//  Description: Header file for the AssetManager class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <memory>
#include <vector>
#include <unordered_map>
#include <boost/uuid/uuid_generators.hpp>

#include "Asset.hpp"
#include "AssetRef.hpp"
#include "AssetConcepts.hpp"
#include "AssetIndex.hpp"

namespace nexo::assets {

    class AssetManager {
        public:
            // to construct an asset manager you should provide via template the type of the AssetRefBase
            AssetManager()
            {

            }
            ~AssetManager() = default;

            template<DerivedFromAsset TAssetData, typename... Args>
            AssetRef<TAssetData> createAsset(Args&&... args) {
                auto asset = new TAssetData(std::forward<Args>(args)...);
                auto assetRef = AssetRef<Asset>(asset);

                auto& metadata = assetRef->getMetadata();
                auto assetId = m_uuidGenerator();
                metadata.id = assetId;
                metadata.manager = this;

                m_assetMap[assetId] = assetRef;

                return assetRef;
            }

            void loadAsset(AssetID id);
            void loadAsset(AssetRef<Asset> asset);

            void unloadAsset(AssetID id);
            void unloadAsset(AssetRef<Asset> asset);

            void removeAsset(AssetID uuid);
            void removeAsset(AssetRef<Asset> asset);

            AssetRef<Asset> getAsset(AssetID id);
        private:
            std::unordered_map<AssetID, AssetRef<Asset>> m_assetMap;
            boost::uuids::random_generator m_uuidGenerator;

    };
} // namespace nexo::editor
