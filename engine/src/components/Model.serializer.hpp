#pragma once

#include "Model.hpp"

#include "save/Serialization.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<components::ModelComponent> { static constexpr uint32_t value = 0; };

    template<>
    struct Serializer<components::ModelComponent, 0> {
        static void serialize(json& j, const components::ModelComponent& v, const SerializationContext& /*ctx*/ = {}) {
            j = json{{"model", v.model}};
        }

        static void deserialize(const json& j, components::ModelComponent& v, const SerializationContext& /*ctx*/ = {}) {
            v.model = j.at("model").get<assets::GenericAssetRef>().as<assets::Model>();
        }

        static void migrate_from_previous(json& /*j*/) {}
    };

} // namespace nexo::save
