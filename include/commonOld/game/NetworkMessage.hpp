/*
** EPITECH PROJECT, 2023
** client
** File description:
** NetworkMessage.hpp
*/

#pragma once

#include <cstdint>

namespace common {

    enum class NetworkMessage : uint32_t {
        ServerAccept,
        ServerDeny,
        ServerPing,
        MessageAll,
        ServerMessage,

        clientConnect,
        clientUpdatePlayerDirection,
        clientPlayerFireBullet,

        serverCreatePlayerShip,
        serverPlayerTakeDamage,
        serverPlayerDestroy,

        serverAllyConnect,
        serverAllyDisconnect,
        serverAllyTakeDamage,
        serverAllyDestroy,

        serverFireBullet,
        serverDestroyBullet,
        serverUpdateBulletPosition,

        serverUpdateShipVelocity,
        serverUpdateShipPosition,

        serverCreateEnemy,
        serverUpdateEnemyVelocity,
        serverEnemyTakeDamage,
        serverDestroyEnemy,
    };

}
