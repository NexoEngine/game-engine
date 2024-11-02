/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Network.hpp
*/

#pragma once

namespace ecs {
    namespace components {
        namespace network {
            typedef struct network_s {
                uint32_t entityNetId;
                uint32_t connectionId;

                NLOHMANN_DEFINE_TYPE_INTRUSIVE(network_s, entityNetId, connectionId)
            } network_t;
        }
    }
}
