//// Json.hpp /////////////////////////////////////////////////////////////////
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
//  Date:        06/03/2025
//  Description: JSON utils for the engine
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <nlohmann/json.hpp>

#include "save/SerializationContext.hpp"

// Forward declare serialization system
namespace nexo::save {
    template<typename T>
    struct CurrentVersion;

    template<typename T, uint32_t Version>
    struct Serializer;

    struct SerializationContext;

    template<typename T>
    void serialize(nlohmann::json& j, const T& value, const SerializationContext& ctx);

    template<typename T>
    void deserialize(const nlohmann::json& j, T& value, const SerializationContext& ctx);
}

namespace nexo {

    using json = nlohmann::json;

    namespace detail {
        // Check if CurrentVersion is specialized for T
        template<typename T, typename = void>
        struct has_serializer : std::false_type {};

        template<typename T>
        struct has_serializer<T, std::void_t<
            decltype(save::CurrentVersion<T>::value)
        >> : std::true_type {};

        template<typename T>
        inline constexpr bool has_serializer_v = has_serializer<T>::value;

        // Check if a type has to_json/from_json free functions
        template<typename T, typename = void>
        struct has_json_functions : std::false_type {};

        template<typename T>
        struct has_json_functions<T, std::void_t<
            decltype(to_json(std::declval<json&>(), std::declval<const T&>())),
            decltype(from_json(std::declval<const json&>(), std::declval<T&>()))
        >> : std::true_type {};

        template<typename T>
        inline constexpr bool has_json_functions_v = has_json_functions<T>::value;
    }


    /**
     * @brief Concept that checks if a type can be serialized to and deserialized from JSON
     *
     * A type is JSONSerializable if it satisfies ONE of the following:
     *
     * 1. Has a specialized Serializer<T, Version> in nexo::save
     * @code{.cpp}
     * template<>
     * struct nexo::save::Serializer<YourType, 1> {
     *     static void serialize(json& j, const YourType& obj, const Context& ctx);
     *     static void deserialize(const json& j, YourType& obj, const Context& ctx);
     *     static void migrate_from_previous(json& j);
     * };
     * @endcode
     *
     * 2. Implements the following non-member functions:
     * @code{.cpp}
     * void to_json(nexo::json& j, const YourType& obj);
     * void from_json(const nexo::json& j, YourType& obj);
     * @endcode
     *
     * @note See nlohmann::json documentation for detailed implementation guidelines.
     */
    template<typename T>
    concept JSONSerializable = detail::has_serializer_v<T> || detail::has_json_functions_v<T>;

} // namespace nexo

namespace nlohmann {

    // save: T -> JSON
    template<typename T>
    requires nexo::detail::has_serializer_v<T> && (!nexo::detail::has_json_functions_v<T>)
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
