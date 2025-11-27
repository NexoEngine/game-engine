//// Name.serializer.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        18/11/2025
//  Description: Header for Name component serialization
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Name.hpp"
#include "save/Serializer.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<components::NameComponent> {
        static constexpr uint32_t value = 0;
    };

    template<>
    struct Serializer<components::NameComponent, 0> {
        static void serialize(json& j, const components::NameComponent& value,
                              const SerializationContext& ctx = SerializationContext{})
        {
            j = json{
                {"name", value.name},
            };
        }

        static void deserialize(const json& j, components::NameComponent& value,
                                const SerializationContext& ctx = SerializationContext{})
        {
            value.name = j.at("name").get<std::string>();
        }

        static void migrate_from_previous(const json& j)
        {
            (void)j; // No previous version
        }
    };

} // namespace nexo::save
