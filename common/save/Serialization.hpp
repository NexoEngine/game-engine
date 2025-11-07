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
#include "SerializationContext.hpp"
#include "SerializationException.hpp"

namespace nexo::save {

    // Version tracking
    template<typename T>
    struct CurrentVersion {
        static constexpr uint32_t value = 0;
    };

    template<typename T>
    inline constexpr uint32_t current_version_v = CurrentVersion<T>::value;

    // Version-specific serializer
    template<typename T, uint32_t Version>
    struct Serializer {
        // static_assert(sizeof(T) == 0,
        //     "Serializer not implemented for this type and version");

        // Only current version needs these
        static void serialize(json& j, const T& value, const SerializationContext& ctx = SerializationContext{}) = delete;
        static void deserialize(const json& j, T& value, const SerializationContext& ctx = SerializationContext{}) = delete;

        static void migrate_from_previous(const json& j) = delete;
    };

    namespace detail {
        // Runtime migration dispatcher - uses compile-time recursion up to a limit
        template<typename T, uint32_t Version>
        struct MigrationDispatcher {
            static void migrate(json& j, uint32_t from_version) {
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
            static void migrate(json& j, uint32_t from_version) {
                (void)j;
                (void)from_version;
            }
        };

        // Specialization for version 1 (base case)
        template<typename T>
        struct MigrationDispatcher<T, 1> {
            static void migrate(json& j, uint32_t from_version) {
                if (from_version == 0) {
                    // Migrate from version 0 to version 1
                    Serializer<T, 1>::migrate_from_previous(j);
                    j["version"] = 1;
                }
            }
        };

        // Main migration function
        template<typename T>
        void migrate_to_current(json& j, uint32_t from_version) {
            constexpr uint32_t current = current_version_v<T>;

            if (from_version >= current) {
                return; // Already at current version or newer
            }

            // Use the dispatcher to migrate from from_version to current
            MigrationDispatcher<T, current>::migrate(j, from_version);
        }

        template<typename T>
        void deserialize_with_migration(const json& j, T& value,
                                       const uint32_t data_version, const SerializationContext& ctx) {
            constexpr uint32_t current = current_version_v<T>;

            if (data_version > current) {
                throw std::runtime_error(
                    "Cannot load data from future version " +
                    std::to_string(data_version) +
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
    }

    // Public API
    template<typename T>
    void serialize(json& j, const T& value, const SerializationContext& ctx = SerializationContext{}) {
        constexpr uint32_t current = current_version_v<T>;
        Serializer<T, current>::serialize(j, value, ctx);
    }

    template<typename T>
    void deserialize(const json& j, T& value, const uint32_t data_version, const SerializationContext& ctx = SerializationContext{}) {
        detail::deserialize_with_migration<T>(j, value, data_version, ctx);
    }

    template<typename T>
    void deserialize(const json& j, T& value, const SerializationContext& ctx = SerializationContext{}) {
        const uint32_t data_version = j.is_object() ? j.value("version", 0u) : 0u;
        deserialize<T>(j, value, data_version, ctx);
    }

} // namespace nexo::save
