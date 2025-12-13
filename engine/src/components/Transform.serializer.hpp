//// Transform.serializer.hpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        05/11/2025
//  Description: Header for Transform component serialization
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "Transform.hpp"

#include "save/Serialization.hpp"
#include "save/glm/glm.hpp"

namespace nexo::save {

    template<>
    struct Serializer<components::TransformComponent, 0> {
        static void serialize(json& j, const components::TransformComponent& value, const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            j = json{
                {"pos", value.pos},
                {"size", value.size},
                {"quat", value.quat},
                {"localMatrix", value.localMatrix},
                {"localCenter", value.localCenter},
                {"children", value.children}
            };
        }

        static void deserialize(const json& j, components::TransformComponent& value, const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            value.pos = j.at("pos").get<glm::vec3>();
            value.size = j.at("size").get<glm::vec3>();
            value.quat = j.at("quat").get<glm::quat>();
            value.localMatrix = j.at("localMatrix").get<glm::mat4>();
            value.localCenter = j.at("localCenter").get<glm::vec3>();
            value.children = j.at("children").get<std::vector<ecs::Entity>>();
        }

        static void migrate_from_previous(const json& j)
        {
            (void)j; // No previous version
        }

    };

} // namespace nexo::save
