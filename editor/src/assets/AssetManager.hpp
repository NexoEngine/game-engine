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

namespace nexo::editor {

    class AssetManager {
        public:
            AssetManager() = default;
            ~AssetManager() = default;

            void addAsset(const std::shared_ptr<Asset>& asset);
            void removeAsset(AssetUUID uuid);
            std::shared_ptr<Asset> getAsset(AssetUUID uuid);

        private:
            std::unordered_map<AssetUUID, std::shared_ptr<Asset>> m_assetMap;
            boost::uuids::random_generator m_uuidGenerator;

    };
} // namespace nexo::editor
