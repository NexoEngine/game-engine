#pragma once

#include "MaterialComponent.hpp"

#include "save/Serialization.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<components::MaterialComponent> { static constexpr uint32_t value = 0; };

    template<>
    struct Serializer<components::MaterialComponent, 0> {
        static void serialize(json& j, const components::MaterialComponent& v, const SerializationContext& /*ctx*/ = {}) {
            j = json{{"material", v.material}};
        }

        static void deserialize(const json& j, components::MaterialComponent& v, const SerializationContext& /*ctx*/ = {}) {
            v.material = j.at("material").get<assets::GenericAssetRef>().as<assets::Material>();
        }

        static void migrate_from_previous(json& /*j*/) {}
    };

} // namespace nexo::save

