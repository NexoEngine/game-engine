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
#include "AssetID.hpp"

#include "save/Serialization.hpp"

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
            (void)ctx;

            const auto lock = value.lock();

            switch (ctx.mode) {
                case SerializationMode::TEXT:
                    j = lock ? json{{"location", lock->getLocation()}} : json{{"uuid", nullptr}};
                    break;
                case SerializationMode::BINARY:
                    j= lock ? json{{"uuid", lock->getID()}} : json{{"uuid", nullptr}};
                    break;
            }
        }

        static void deserialize(const json& j, assets::GenericAssetRef& value,
                                const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            if (j.contains("uuid")) {
                // TODO: Fix compilation
                //const assets::AssetID id = j.at("uuid").get<assets::AssetID>();
                // value = assets::GenericAssetRef{id};
                return;
            }

            if (j.contains("location")) {
                const assets::AssetLocation location = j.at("location").get<assets::AssetLocation>();
                value = assets::GenericAssetRef{location};
                return;
            }
            LOG(NEXO_WARN, "AssetRef deserialization: no valid data found (uuid or location)");
        }

        static void migrate_from_previous(const json& j)
        {
            (void)j; // No previous version
        }

    };

    template<typename InputAsset>
    struct CurrentVersion<assets::AssetRef<InputAsset>> {
        static constexpr uint32_t value = 0;
    };

    template<typename InputAsset>
    struct Serializer<assets::AssetRef<InputAsset>, 0> {
        static void serialize(json& j, const assets::AssetRef<InputAsset>& value,
                              const SerializationContext& ctx = SerializationContext{})
        {
            assets::GenericAssetRef genericRef = value;
            j = genericRef;
        }

        static void deserialize(const json& j, assets::AssetRef<InputAsset>& value,
                                const SerializationContext& ctx = SerializationContext{})
        {
            assets::GenericAssetRef genericRef;
            j.get_to(genericRef);
            value = genericRef.as<InputAsset>();
        }

        static void migrate_from_previous(const json& j)
        {
            (void)j; // No previous version
        }
    };

} // namespace nexo::save
