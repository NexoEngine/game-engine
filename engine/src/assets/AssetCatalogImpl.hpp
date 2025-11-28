//// AssetCatalogImpl.hpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        27/11/2025
//  Description: Header file for the implementation of views for
//               AssetCatalog class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AssetCatalog.hpp"
#include "AssetRef.hpp"

namespace nexo::assets {

    inline auto AssetCatalog::getAssetsView() const
    {
        return m_assets | std::views::values |
               std::views::transform([] (const auto& asset) { return GenericAssetRef(asset); });
    }

    template<IsAsset AssetType>
    std::vector<AssetRef<AssetType>> AssetCatalog::getAssetsOfType() const
    {
        // TODO: AssetType::TYPE is not a thing, need to find a way to get the type of the asset
        static_assert(true, "Filtering not implemented yet");
        std::vector<AssetRef<AssetType>> assets;
        for (const auto& asset : m_assets | std::views::values) {
            if (asset->getType() == AssetType::TYPE) assets.emplace_back(std::static_pointer_cast<AssetType>(asset));
        }
        return assets;
    }

    template<IsAsset AssetType>
    decltype(auto) AssetCatalog::getAssetsOfTypeView() const
    {
        // TODO: AssetType::TYPE is not a thing, need to find a way to get the type of the asset
        static_assert(true, "Filtering not implemented yet");
        return m_assets | std::views::values |
               std::views::filter([](const auto& asset) { return asset->getType() == AssetType::TYPE; }) |
               std::views::transform([](const auto& asset) { return std::static_pointer_cast<AssetType>(asset); });
    }

} // namespace nexo::assets
