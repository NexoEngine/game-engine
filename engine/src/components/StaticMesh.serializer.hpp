#pragma once

#include "StaticMesh.hpp"

#include "save/Serialization.hpp"

namespace nexo::save {

    template<>
    struct CurrentVersion<components::StaticMeshComponent> { static constexpr uint32_t value = 0; };

    template<>
    struct Serializer<components::StaticMeshComponent, 0> {
        static void serialize(json& j, const components::StaticMeshComponent& v, const SerializationContext& /*ctx*/ = {}) {
            // VertexArray and GPU handles aren't serializable here; store a placeholder
            j = json{{"hasVao", static_cast<bool>(v.vao != nullptr)}};
        }

        static void deserialize(const json& j, components::StaticMeshComponent& v, const SerializationContext& /*ctx*/ = {}) {
            // Cannot reconstruct GPU resources during load here; leave vao empty
            (void)j;
            v.vao = nullptr;
        }

        static void migrate_from_previous(json& /*j*/) {}
    };

} // namespace nexo::save

