#pragma once

#include "Parent.hpp"
#include "save/Serialization.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<components::ParentComponent> { static constexpr uint32_t value = 0; };

    template<>
    struct Serializer<components::ParentComponent, 0> {
        static void serialize(json& j, const components::ParentComponent& v, const SerializationContext& /*ctx*/ = {}) {
            j = json{{"parent", v.parent}};
        }

        static void deserialize(const json& j, components::ParentComponent& v, const SerializationContext& /*ctx*/ = {}) {
            v.parent = j.at("parent").get<ecs::Entity>();
        }

        static void migrate_from_previous(json& /*j*/) {}
    };

    template<>
    struct CurrentVersion<components::RootComponent> { static constexpr uint32_t value = 0; };

    template<>
    struct Serializer<components::RootComponent, 0> {
        static void serialize(json& j, const components::RootComponent& v, const SerializationContext& /*ctx*/ = {}) {
            j = json{{"name", v.name}, {"model", v.modelRef}, {"childCount", v.childCount}};
        }

        static void deserialize(const json& j, components::RootComponent& v, const SerializationContext& /*ctx*/ = {}) {
            v.name = j.at("name").get<std::string>();
            v.modelRef = j.at("model").get<assets::GenericAssetRef>().as<assets::Model>();
            v.childCount = j.at("childCount").get<int>();
        }

        static void migrate_from_previous(json& /*j*/) {}
    };

} // namespace nexo::save

