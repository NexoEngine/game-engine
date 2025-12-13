#pragma once

#include "PhysicsBodyComponent.hpp"
#include "save/glm/glm.hpp"

#include "save/Serialization.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<components::PhysicsBodyComponent> { static constexpr uint32_t value = 0; };

    template<>
    struct Serializer<components::PhysicsBodyComponent, 0> {
        static void serialize(json& j, const components::PhysicsBodyComponent& v, const SerializationContext& /*ctx*/ = {}) {
            j = json{{"bodyID", v.bodyID.GetIndex()}, {"type", static_cast<int>(v.type)}, {"lastPos", v.lastPos}, {"lastRot", v.lastRot}};
        }

        static void deserialize(const json& j, components::PhysicsBodyComponent& v, const SerializationContext& /*ctx*/ = {}) {
            // BodyID deserialization: we cannot reconstruct a live BodyID here; set default and log
            v.bodyID = JPH::BodyID();
            v.type = static_cast<components::PhysicsBodyComponent::Type>(j.at("type").get<int>());
            v.lastPos = j.at("lastPos").get<glm::vec3>();
            v.lastRot = j.at("lastRot").get<glm::quat>();
        }

        static void migrate_from_previous(json& /*j*/) {}
    };

} // namespace nexo::save

