/// AssetRef.serializer.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        18/11/2025
//  Description: Header file for the AssetRef serializer.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Asset.hpp"
#include "AssetRef.hpp"
#include "save/Serializer.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<assets::GenericAssetRef> {
        static constexpr uint32_t value = 0;
    };

    template<>
    struct Serializer<assets::GenericAssetRef, 0> {
        static void serialize(json& j, const assets::GenericAssetRef& value,
                              const SerializationContext& ctx = SerializationContext{})
        {
            // Serialize as a boolean indicating validity
            auto lock = value.lock();
            if (!lock) {
                j = json{
                        {"uuid", nullptr},
                    };
                return;
            }
            j = json{
                    {"uuid", lock->getID()},
                };
        }

        static void deserialize(const json& j, assets::GenericAssetRef& value,
                                const SerializationContext& ctx = SerializationContext{})
        {
            // Deserialize from a boolean indicating validity
            if (j.at("uuid").is_null()) {
                value = assets::GenericAssetRef{};
                return;
            }
            const std::string uuid = j.at("uuid").get<std::string>();
            auto asset = ctx.assetCatalog->getAssetByID(uuid);
            if (!asset) {
                Logger::error("AssetRef deserialization failed: asset with UUID {} not found", uuid);
                value = assets::GenericAssetRef{};
                return;
            }
            value = assets::GenericAssetRef{asset};
        }

        static void migrate_from_previous(const json& j)
        {
            (void)j; // No previous version
        }

    };



} // namespace nexo::save
