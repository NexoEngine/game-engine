//// AssetImporterRegistry.cpp ////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/03/2025
//  Description: Registry for asset importers implementation
//
///////////////////////////////////////////////////////////////////////////////

#include "AssetImporterRegistry.hpp"

namespace nexo::assets {

    const std::vector<std::shared_ptr<AssetImporter>>& AssetImporterRegistry::
    getImportersForType(const std::type_index& typeIdx) const
    {
        if (const auto it = m_importers.find(typeIdx) ; it == m_importers.end()) {
            static constexpr std::vector<std::shared_ptr<AssetImporter>> empty = {};
            return empty;
        }
        return m_importers.at(typeIdx);
    }

    bool AssetImporterRegistry::hasImportersForType(const std::type_index& typeIdx) const
    {
        const auto it = m_importers.find(typeIdx);
        return (it != m_importers.end() && !it->second.empty());
    }

    void AssetImporterRegistry::unregisterAllImportersForType(const std::type_index& typeIdx)
    {
        m_importers.erase(typeIdx);
        m_importersDetails.erase(typeIdx);
    }

} // namespace nexo::assets
