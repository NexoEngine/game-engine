//// AssetName.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        07/12/2024
//  Description: Source file for the AssetName class
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetName.hpp"

namespace nexo::assets {

    void AssetName::checkName(const std::string_view name)
    {
        if (name.empty())
            throw InvalidAssetName(name, "Asset name cannot be empty.");
        if (name.size() > 255)
            throw InvalidAssetName(name, "Asset name cannot exceed 255 characters.");
        if (name.find_first_of(InvalidCharList) != std::string::npos)
            throw InvalidAssetName(name, std::format("Asset name cannot contain any of the following characters: {}", InvalidCharList));
    }

    bool AssetName::isValid(const std::string_view name)
    {
        try {
            checkName(name);
            return true;
        } catch (const InvalidAssetName&) {
            return false;
        }
    }

} // namespace nexo::assets
