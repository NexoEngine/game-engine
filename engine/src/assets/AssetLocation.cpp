//// AssetLocation.cpp ////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        07/12/2024
//  Description: Source file for the AssetLocation class
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetLocation.hpp"
#include "Path.hpp"

namespace nexo::assets {

    void AssetLocation::setLocation(
        const AssetName& name,
        const std::string& path,
        const std::optional<std::reference_wrapper<const AssetPackName>>& packName
    )
    {
        _name = name;
        _path = normalizePathAndRemovePrefixSlash(path);
        _packName = packName;
    }
} // namespace nexo::assets
