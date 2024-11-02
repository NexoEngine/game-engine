/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Health.hpp
*/

#pragma once

namespace ecs {
    namespace components {
        namespace health {
            typedef struct health_s {
                uint32_t healthPoints;

                NLOHMANN_DEFINE_TYPE_INTRUSIVE(health_s, healthPoints)
            } health_t;
        }
    }
}
