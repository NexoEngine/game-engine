//// AssetManager.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        18/11/2024
//  Description: Source file for the AssetManager class
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetManager.hpp"

namespace nexo::editor {

    void AssetManager::addAsset(const std::shared_ptr<Asset>& asset)
    {
        auto uuid = m_uuidGenerator();
        asset->setID(uuid);
        m_assetMap[uuid] = asset;
    }

    void AssetManager::removeAsset(const AssetUUID uuid)
    {
        m_assetMap.erase(uuid);
    }

    std::shared_ptr<Asset> AssetManager::getAsset(const AssetUUID uuid)
    {
        return m_assetMap[uuid];
    }

} // namespace nexo::editor
