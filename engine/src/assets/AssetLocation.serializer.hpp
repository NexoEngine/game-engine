//// AssetLocationSerializer.hpp //////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        01/12/2025
//  Description: Header file for the AssetLocation serializer.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AssetLocation.hpp"

#include "save/Serialization.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<assets::AssetLocation> {
        static constexpr uint32_t value = 0;
    };

    template<>
    struct Serializer<assets::AssetLocation, 0> {
        static void serialize(json& j, const assets::AssetLocation& value,
                              const SerializationContext& ctx = {})
        {
            (void)ctx;
            j = value.getFullLocation();
        }

        static void deserialize(const json& j, assets::AssetLocation& value,
                                const SerializationContext& /*ctx*/ = {})
        {
            const std::string location = j.get<std::string>();
            value.setLocation(location);
        }
    };

} // namespace nexo::assets
