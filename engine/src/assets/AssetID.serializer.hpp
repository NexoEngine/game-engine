//// AssetSerializer.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        01/12/2025
//  Description: Header file for the AssetID serializer.
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AssetID.hpp"

#include "save/Serialization.hpp"

#include <boost/uuid/string_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

#include "Exception.hpp"
#include "Logger.hpp"
#include "save/SerializationException.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<assets::AssetID> {
        static constexpr uint32_t value = 0;
    };

    template<>
    struct Serializer<assets::AssetID, 0> {
        static void serialize(json& j, const assets::AssetID& value,
                              const SerializationContext& ctx = {})
        {
            switch (ctx.mode) {
                case SerializationMode::TEXT:
                    j = json{boost::uuids::to_string(value)};
                    break;
                case SerializationMode::BINARY:
                    std::vector<uint8_t> v(value.size());
                    std::ranges::copy(value, v.begin());
                    j = json{ v};
                    break;
            }
        }

        static void deserialize(const json& j, assets::AssetID& value,
                                const SerializationContext& /*ctx*/ = {})
        {
            // First try to get binary format
            if (j.is_array()) {
                const std::vector<uint8_t> v = j.get<std::vector<uint8_t>>();
                if (v.size() != value.size())
                    THROW_EXCEPTION(DeserializationException, "Invalid UUID size during deserialization");
                boost::uuids::uuid uuid;
                std::ranges::copy(v, uuid.begin());
                value = uuid;
                return;
            }
            // Then try text format
            if (j.is_string()) {
                const std::string uuidStr = j.get<std::string>();
                boost::uuids::string_generator gen;
                try {
                    value = gen(uuidStr);
                } catch (const std::exception& e) {
                    THROW_EXCEPTION(DeserializationException, "Invalid UUID string during deserialization: " + std::string(e.what()));
                }
                return;
            }
            LOG(NEXO_ERROR, "Did not find a valid format for UUID deserialization");
        }

        static void migrate_from_previous(const json& j)
        {
            (void)j; // No previous version
        }
    };

} // namespace nexo::assets
