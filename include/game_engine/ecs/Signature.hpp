/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Signature.hpp
*/

#pragma once

#include <bitset>
#include "Components.hpp"
#include <nlohmann/json.hpp>

namespace ecs {
    using Signature = std::bitset<components::MAX_COMPONENT_TYPE>;

    inline void from_json(const nlohmann::json& j, Signature& signature)
    {
        signature = Signature(j.get<std::string>());
    }

    inline void to_json(nlohmann::json& j, const Signature& signature)
    {
        j = signature.to_string();
    }
}
