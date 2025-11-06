//// glm.hpp //////////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Guillaume HEIN
//  Date:        06/11/2025
//  Description: GLM serialization helpers
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

#include "save/Serialization.hpp"

namespace nexo::save {
    // glm::vec2
    template<>
    struct CurrentVersion<glm::vec2> {
        static constexpr uint32_t value = 0;
    };

    template<>
    struct Serializer<glm::vec2, 0> {
        static void serialize(json& j, const glm::vec2& value, const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            j = {
                {"x", value.x},
                {"y", value.y}
            };
        }

        static void deserialize(const json& j, glm::vec2& value, const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            value.x = j.at("x");
            value.y = j.at("y");
        }

        static void migrate_from_previous(const json& j)
        {
            (void)j; // No previous version
        }
    };

    // glm::vec3
    template<>
    struct Serializer<glm::vec3, 0> {
        static void serialize(json& j, const glm::vec3& value, const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            j = {
                {"x", value.x},
                {"y", value.y},
                {"z", value.z}
            };
        }

        static void deserialize(const json& j, glm::vec3& value, const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            value.x = j.at("x");
            value.y = j.at("y");
            value.z = j.at("z");
        }

        static void migrate_from_previous(const json& j)
        {
            (void)j; // No previous version
        }
    };

    // glm::quat
    template<>
    struct Serializer<glm::quat, 0> {
        static void serialize(json& j, const glm::quat& value, const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            j = {
                {"w", value.w},
                {"x", value.x},
                {"y", value.y},
                {"z", value.z}
            };
        }

        static void deserialize(const json& j, glm::quat& value, const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            value.w = j.at("w");
            value.x = j.at("x");
            value.y = j.at("y");
            value.z = j.at("z");
        }

        static void migrate_from_previous(const json& j)
        {
            (void)j; // No previous version
        }
    };

    // glm::mat4
    template<>
    struct Serializer<glm::mat4, 0> {
        static void serialize(json& j, const glm::mat4& value, const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            j = {
                value[0][0], value[0][1], value[0][2], value[0][3],
                value[1][0], value[1][1], value[1][2], value[1][3],
                value[2][0], value[2][1], value[2][2], value[2][3],
                value[3][0], value[3][1], value[3][2], value[3][3]
            };
        }

        static void deserialize(const json& j, glm::mat4& value, const SerializationContext& ctx = SerializationContext{})
        {
            (void)ctx;
            value[0][0] = j.at(0);  value[0][1] = j.at(1);  value[0][2] = j.at(2);  value[0][3] = j.at(3);
            value[1][0] = j.at(4);  value[1][1] = j.at(5);  value[1][2] = j.at(6);  value[1][3] = j.at(7);
            value[2][0] = j.at(8);  value[2][1] = j.at(9);  value[2][2] = j.at(10); value[2][3] = j.at(11);
            value[3][0] = j.at(12); value[3][1] = j.at(13); value[3][2] = j.at(14); value[3][3] = j.at(15);
        }

        static void migrate_from_previous(const json& j)
        {
            (void)j; // No previous version
        }
    };

} // namespace nexo::save
