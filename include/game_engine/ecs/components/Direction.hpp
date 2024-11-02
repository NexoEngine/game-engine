/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Health.hpp
*/

#pragma once

#include "raymath.h"

namespace ecs {
    namespace components {
        namespace direction {
            typedef struct direction_s {
                Vector3 direction;

                NLOHMANN_DEFINE_TYPE_INTRUSIVE(direction_s, direction)
            } direction_t;
        }
    }
}
