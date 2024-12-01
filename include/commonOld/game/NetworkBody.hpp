/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** NetworkBody
*/

#pragma once

#include "my_raylib.h"
#include "common/game/entities/Objects.hpp"
#include <string>

namespace boost::serialization {

    template<class Archive>
    void serialize(Archive & ar, Vector3 & v, const unsigned int version) {
        ar & v.x;
        ar & v.y;
        ar & v.z;
    }

} // namespace boost

namespace common::game::netbody {

    // ServerAccept,
    // ServerDeny,
    // ServerPing,
    // MessageAll,
    // ServerMessage,

    // clientConnect,
    // clientUpdatePlayerDirection,
    // clientPlayerFireBullet,

    // serverCreatePlayerShip,
    // serverPlayerTakeDamage,
    // serverPlayerDestroy,

    // serverAllyConnect,
    // serverAllyDisconnect,
    // serverAllyTakeDamage,
    // serverAllyDestroy,

    // serverFireBullet,
    // serverDestroyBullet,

    // serverUpdateShipVelocity,
    // serverUpdateShipPosition,

    // serverCreateEnemy,
    // serverUpdateEnemyVelocity,
    // serverEnemyTakeDamage,
    // serverDestroyEnemy,

    struct PingServer {
        double timeStart;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & timeStart;
        }
    };

    struct ClientConnect {
        std::string name;
        common::game::ObjectName shipName;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & name;
            ar & shipName;
        }
    };

    struct ClientUpdatePlayerDirection {
        Vector3 direction;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & direction;
        }
    };

    struct ClientPlayerFireBullet {
        Vector3 direction;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & direction;
        }
    };

    struct ServerCreatePlayerShip {
        uint32_t entityNetId;
        common::game::ObjectName shipName;
        Vector3 pos;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
            ar & shipName;
            ar & pos;
        }
    };

    struct ServerPlayerTakeDamage {
        uint32_t damage;
        uint32_t entityNetId;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & damage;
            ar & entityNetId;
        }
    };

    struct ServerPlayerDestroy {
        uint32_t entityNetId;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
        }
    };

    struct ServerAllyConnect {
        uint32_t entityNetId;
        std::string name;
        common::game::ObjectName shipName;
        Vector3 pos;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
            ar & name;
            ar & shipName;
            ar & pos;
        }
    };

    struct ServerAllyDisconnect {
        uint32_t entityNetId;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
        }
    };

    struct ServerAllyTakeDamage {
        uint32_t entityNetId;
        uint32_t damage;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
            ar & damage;
        }
    };

    struct ServerAllyDestroy {
        uint32_t entityNetId;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
        }
    };

    struct ServerFireBullet {
        uint32_t entityNetId;
        uint32_t sender;
        Vector3 pos;
        Vector3 direction;
        double speed;
        Vector3 rotation;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
            ar & pos;
            ar & direction;
            ar & speed;
            ar & rotation;
        }
    };

    struct ServerDestroyBullet {
        uint32_t entityNetId;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
        }
    };

    struct ServerUpdateBulletPosition {
        uint32_t entityNetId;
        Vector3 pos;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
            ar & pos;
        }
    };

    struct ServerUpdateShipVelocity {
        uint32_t entityNetId;
        Vector3 velocity;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
            ar & velocity;
        }
    };

    struct ServerUpdateShipPosition {
        uint32_t entityNetId;
        Vector3 pos;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
            ar & pos;
        }
    };

    struct ServerCreateEnemy {
        uint32_t entityNetId;
        std::string name;
        common::game::ObjectName shipName;
        Vector3 pos;
        Vector3 rotation;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
            ar & name;
            ar & shipName;
            ar & pos;
            ar & rotation;
        }
    };

    struct ServerUpdateEnemyVelocity {
        uint32_t entityNetId;
        Vector3 velocity;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
            ar & velocity;
        }
    };

    struct ServerEnemyTakeDamage {
        uint32_t entityNetId;
        uint32_t damage;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
            ar & damage;
        }
    };

    struct ServerDestroyEnemy {
        uint32_t entityNetId;

        template<class Archive>
        void serialize(Archive & ar, [[maybe_unused]] const unsigned int version) {
            ar & entityNetId;
        }
    };

}
