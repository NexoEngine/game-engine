//// AssetLocation.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        07/12/2024
//  Description: Source file for the AssetLocation class
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetLocation.hpp"

namespace nexo::assets {

    void AssetLocation::setLocation(
        const AssetName& name,
        const std::string& path,
        const std::optional<std::reference_wrapper<const AssetPackName>>& packName
    )
    {
        _name = name;
        _path = path;
        _packName = packName;
    }
} // namespace nexo::assets
