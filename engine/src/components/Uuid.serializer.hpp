#pragma once

#include "Uuid.hpp"

#include "save/Serialization.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<components::UuidComponent> { static constexpr uint32_t value = 0; };

    template<>
    struct Serializer<components::UuidComponent, 0> {
        static void serialize(json& j, const components::UuidComponent& v, const SerializationContext& /*ctx*/ = {}) {
            j = json{{"uuid", v.uuid}};
        }

        static void deserialize(const json& j, components::UuidComponent& v, const SerializationContext& /*ctx*/ = {}) {
            v.uuid = j.at("uuid").get<std::string>();
        }

        static void migrate_from_previous(json& /*j*/) {}
    };

} // namespace nexo::save

