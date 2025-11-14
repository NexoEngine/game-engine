//// Serialization.hpp ////////////////////////////////////////////////////////
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
//  Date:        05/11/2025
//  Description: Implementation of the serialization utilities
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <cstdint>
#include <stdexcept>
#include <string>

#include "Json.hpp"
#include "Serializer.hpp"
#include "SerializationConcepts.hpp"
#include "SerializationContext.hpp"

namespace nexo::save {

    namespace detail {
        // Runtime migration dispatcher - uses compile-time recursion up to a limit
        template<typename T, uint32_t Version>
        struct MigrationDispatcher {
            static void migrate(json& j, uint32_t from_version)
            {
                if (from_version == Version - 1) {
                    // This is the version we need to migrate from
                    Serializer<T, Version>::migrate_from_previous(j);
                    j["version"] = Version;
                } else if (from_version < Version - 1) {
                    // Keep going backwards
                    MigrationDispatcher<T, Version - 1>::migrate(j, from_version);
                    // Then migrate this version
                    Serializer<T, Version>::migrate_from_previous(j);
                    j["version"] = Version;
                }
                // If from_version >= Version, do nothing (already at or past this version)
            }
        };

        // Specialization for version 0 (base case)
        template<typename T>
        struct MigrationDispatcher<T, 0> {
            static void migrate(json& j, uint32_t from_version)
            {
                (void)j;
                (void)from_version;
            }
        };

        // Specialization for version 1 (base case)
        template<typename T>
        struct MigrationDispatcher<T, 1> {
            static void migrate(json& j, uint32_t from_version)
            {
                if (from_version == 0) {
                    // Migrate from version 0 to version 1
                    Serializer<T, 1>::migrate_from_previous(j);
                    j["version"] = 1;
                }
            }
        };

        // Main migration function
        template<typename T>
        void migrate_to_current(json& j, uint32_t from_version)
        {
            constexpr uint32_t current = current_version_v<T>;

            if (from_version >= current) {
                return; // Already at current version or newer
            }

            // Use the dispatcher to migrate from from_version to current
            MigrationDispatcher<T, current>::migrate(j, from_version);
        }

        template<typename T>
        void deserialize_with_migration(const json& j, T& value, const uint32_t data_version,
                                        const SerializationContext& ctx)
        {
            constexpr uint32_t current = current_version_v<T>;

            if (data_version > current) {
                throw std::runtime_error("Cannot load data from future version " + std::to_string(data_version) +
                                         " (current version: " + std::to_string(current) + ")");
            }

            if (data_version == current) {
                // Already at current version - deserialize directly
                Serializer<T, current>::deserialize(j, value, ctx);
            } else {
                // Migrate JSON to current version first
                json migrated = j;
                migrate_to_current<T>(migrated, data_version);

                // Deserialize using current version deserializer
                Serializer<T, current>::deserialize(migrated, value, ctx);
            }
        }

        struct VersionedJson {
            uint32_t version = 0;
            json     data;
        };

        inline void to_json(nexo::json& j, const detail::VersionedJson& vj) {
            if (vj.version == 0) {
                j = vj.data;
                return;
            }
            j = vj.data.is_object() ? vj.data : json{{"_data", vj.data}};
            j["_version"] = vj.version;
        }
        inline void from_json(const nexo::json& j, detail::VersionedJson& vj)
        {
            if (!j.is_object() || !j.contains("_version")) {
                vj.version = 0;
                vj.data    = j;
                return;
            }

            vj.version = j.at("_version").get<uint32_t>();

            if (j.contains("_data")) {
                vj.data = j.at("_data");
            } else {
                vj.data = j;
                vj.data.erase("_version");
            }
        }

    } // namespace detail

    // Public API
    template<typename T>
        requires HasSerializer<T> // Compiler will show "HasSerializer<glm::vec3>" failed
    void serialize(json& j, const T& value, const SerializationContext& ctx = SerializationContext{})
    {
        constexpr uint32_t current = current_version_v<T>;
        detail::VersionedJson vj;
        vj.version = current;
        Serializer<T, current>::serialize(vj.data, value, ctx);
        j = vj;
    }

    // Diagnostic overload for serialize - triggers when HasSerializer<T> fails
    template<typename T>
        requires(!HasSerializer<T>)
    void serialize(json& j, const T& value, const SerializationContext& ctx = SerializationContext{})
    {
        static_assert(HasSerializer<T>,
                      "ERROR: No Serializer defined for this type!\n"
                      "To fix this, define a specialization:\n"
                      "  template<> struct nexo::save::Serializer<YourType, VERSION> {\n"
                      "    static void serialize(json& j, const YourType& value, const SerializationContext& ctx) {\n"
                      "      // Your serialization code here\n"
                      "    }\n"
                      "  };\n"
                      "Example for glm::vec3:\n"
                      "  template<> struct nexo::save::Serializer<glm::vec3, 0> {\n"
                      "    static void serialize(json& j, const glm::vec3& v, const SerializationContext& ctx) {\n"
                      "      j = {{\"x\", value.x}, {\"y\", value.y}, {\"z\", value.z}};\n"
                      "    }\n"
                      "  };");
    }

    template<typename T>
        requires HasSerializer<T> // Compiler will show "HasSerializer<glm::vec3>" failed
    void deserialize(const json& j, T& value, const uint32_t data_version,
                     const SerializationContext& ctx = SerializationContext{})
    {
        detail::deserialize_with_migration<T>(j, value, data_version, ctx);
    }

    // Diagnostic overload for deserialize (with data_version) - triggers when HasSerializer<T> fails
    template<typename T>
        requires(!HasSerializer<T>)
    void deserialize(const json& j, T& value, const uint32_t data_version,
                     const SerializationContext& ctx = SerializationContext{})
    {
        static_assert(HasSerializer<T>,
                      "ERROR: No Serializer defined for this type!\n"
                      "To fix this, define a specialization:\n"
                      "  template<> struct nexo::save::Serializer<YourType, VERSION> {\n"
                      "    static void deserialize(const json& j, YourType& value, const SerializationContext& ctx) {\n"
                      "      // Your deserialization code here\n"
                      "    }\n"
                      "  };\n"
                      "Example for glm::vec3:\n"
                      "  template<> struct nexo::save::Serializer<glm::vec3, 0> {\n"
                      "    static void deserialize(const json& j, glm::vec3& v, const SerializationContext& ctx) {\n"
                      "      value.x = j.at(\"x\");\n"
                      "      value.y = j.at(\"y\");\n"
                      "      value.z = j.at(\"z\");\n"
                      "    }\n"
                      "  };");
    }

    template<typename T>
        requires HasSerializer<T> // Compiler will show "HasSerializer<glm::vec3>" failed
    void deserialize(const json& j, T& value, const SerializationContext& ctx = SerializationContext{})
    {
        auto [version, data] = j.get<detail::VersionedJson>();
        deserialize<T>(data, value, version, ctx);
    }

    // Diagnostic overload for deserialize (without data_version) - triggers when HasSerializer<T> fails
    template<typename T>
        requires(!HasSerializer<T>)
    void deserialize(const json& j, T& value, const SerializationContext& ctx = SerializationContext{})
    {
        static_assert(HasSerializer<T>,
                      "ERROR: No Serializer defined for this type!\n"
                      "To fix this, define a specialization:\n"
                      "  template<> struct nexo::save::Serializer<YourType, VERSION> {\n"
                      "    static void deserialize(const json& j, YourType& value, const SerializationContext& ctx) {\n"
                      "      // Your deserialization code here\n"
                      "    }\n"
                      "  };\n"
                      "Example for glm::vec3:\n"
                      "  template<> struct nexo::save::Serializer<glm::vec3, 0> {\n"
                      "    static void deserialize(const json& j, glm::vec3& v, const SerializationContext& ctx) {\n"
                      "      value.x = j.at(\"x\");\n"
                      "      value.y = j.at(\"y\");\n"
                      "      value.z = j.at(\"z\");\n"
                      "    }\n"
                      "  };");
    }

} // namespace nexo::save

namespace nlohmann {

    // ADL serializer for types with HasSerializer concept
    template<typename T>
    requires nexo::save::HasSerializer<T> && (!nexo::HasJsonFunctions<T>)
    struct adl_serializer<T> {
        static void to_json(json& j, const T& value) {
            // Use default context (runtime)
            nexo::save::SerializationContext ctx{};
            nexo::save::serialize(j, value, ctx);
        }

        static void from_json(const json& j, T& value) {
            // Use default context (runtime)
            nexo::save::SerializationContext ctx{};
            nexo::save::deserialize(j, value, ctx);
        }
    };

} // namespace nlohmann

