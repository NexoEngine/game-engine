//// json.hpp /////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        06/03/2025
//  Description: JSON utils for the engine
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <nlohmann/json.hpp>

namespace nexo {

    using json = nlohmann::json;

    /**
     * @brief Concept that checks if a type can be serialized to and deserialized from JSON
     *
     * To make a type JSONSerializable, implement the following non-member functions:
     * @code{.cpp}
     * void to_json(nexo::json& j, const YourType& obj);
     * void from_json(const nexo::json& j, YourType& obj);
     * @endcode
     *
     * @note See nlohmann::json documentation for detailed implementation guidelines.
     */
    template <typename T>
    concept JSONSerializable = requires(T obj, json& j) {
        { to_json(j, obj) } -> std::same_as<void>;
        { from_json(j, obj) } -> std::same_as<void>;
    };

} // namespace nexo
