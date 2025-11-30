#pragma once

#include "SceneComponents.hpp"
#include "save/Serializer.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<components::SceneTag> { static constexpr uint32_t value = 0; };

    template<>
    struct Serializer<components::SceneTag, 0> {
        static void serialize(json& j, const components::SceneTag& v, const SerializationContext& /*ctx*/ = {}) {
            j = json{{"id", v.id}, {"isActive", v.isActive}, {"isRendered", v.isRendered}};
        }

        static void deserialize(const json& j, components::SceneTag& v, const SerializationContext& /*ctx*/ = {}) {
            v.id = j.at("id").get<unsigned int>();
            v.isActive = j.at("isActive").get<bool>();
            v.isRendered = j.at("isRendered").get<bool>();
        }

        static void migrate_from_previous(json& /*j*/) {}
    };

} // namespace nexo::save

