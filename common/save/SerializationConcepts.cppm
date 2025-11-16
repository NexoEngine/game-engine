//// SerializationConcepts.cppm ///////////////////////////////////////////////
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
//  Date:        07/11/2025
//  Description: Module implementation of concepts for JSON serialization
//
///////////////////////////////////////////////////////////////////////////////

module;

#include <concepts>

export module nexo.save.concepts;

import nexo.json;
import nexo.save.serializer;
import nexo.save.context;

export namespace nexo::save {

    namespace detail {
        // Break down the concept into smaller, named pieces for better diagnostics
        template<typename T>
        concept HasCurrentVersion = requires {
            { CurrentVersion<T>::value } -> std::convertible_to<uint32_t>;
        };

        template<typename T>
        concept HasSerializeMethod = HasCurrentVersion<T> && requires(json& j, const T& value, const SerializationContext& ctx) {
            { Serializer<T, CurrentVersion<T>::value>::serialize(j, value, ctx) } -> std::same_as<void>;
        };

        template<typename T>
        concept HasDeserializeMethod = HasCurrentVersion<T> && requires(const json& j, T& value, const SerializationContext& ctx) {
            { Serializer<T, CurrentVersion<T>::value>::deserialize(j, value, ctx) } -> std::same_as<void>;
        };
    }

    /**
     * @brief Concept to check if a type has a complete Serializer implementation
     *
     * A type satisfies HasSerializer if:
     * 1. CurrentVersion<T>::value is defined and convertible to uint32_t
     * 2. Serializer<T, Version>::serialize exists and returns void
     * 3. Serializer<T, Version>::deserialize exists and returns void
     *
     * @note When this concept fails, check the compiler output for which specific
     *       requirement is not satisfied (HasCurrentVersion, HasSerializeMethod, or HasDeserializeMethod)
     */
    template<typename T>
    concept HasSerializer =
        detail::HasCurrentVersion<T> &&
        detail::HasSerializeMethod<T> &&
        detail::HasDeserializeMethod<T>;

} // namespace nexo::save

export namespace nexo {

    /**
     * @brief Concept to check if a type has to_json/from_json free functions
     *
     * A type satisfies HasJsonFunctions if:
     * 1. to_json(json&, const T&) exists and returns void
     * 2. from_json(const json&, T&) exists and returns void
     */
    template<typename T>
    concept HasJsonFunctions = requires(json& j, const T& value, T& out) {
        { to_json(j, value) } -> std::same_as<void>;
        { from_json(j, out) } -> std::same_as<void>;
    };

    /**
     * @brief Concept that checks if a type can be serialized to and deserialized from JSON
     *
     * A type is JSONSerializable if it satisfies ONE of the following:
     *
     * 1. Has a specialized Serializer<T, Version> in nexo::save (HasSerializer)
     * 2. Has to_json/from_json free functions (HasJsonFunctions)
     *
     * @note When compilation fails, the compiler will show which sub-concept failed:
     *       - If HasSerializer failed, check HasCurrentVersion, HasSerializeMethod, HasDeserializeMethod
     *       - If HasJsonFunctions failed, check to_json and from_json implementations
     */
    template<typename T>
    concept JSONSerializable = nexo::save::HasSerializer<T> || HasJsonFunctions<T>;

} // namespace nexo

