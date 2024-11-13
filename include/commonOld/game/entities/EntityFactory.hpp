/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** EntityFactory
*/

#pragma once

#include "game_engine/GameEngine.hpp"
#include "common/game/entities/Objects.hpp"
#include <memory>

namespace common::game {
    class EntityFactory {
        public:
            ecs::Entity createEntity(
                ObjectType type,
                ObjectName name,
                ObjectParams params,
                ObjectFormat format = ObjectFormat::OBJ,
                std::string path = "./"
            );
        private:
            ecs::Entity createCube(
                ObjectName name,
                ObjectParams params,
                ObjectFormat format
            );
            ecs::Entity createObject3D(
                ObjectName name,
                ObjectParams params,
                ObjectFormat format,
                std::string path = "./"
            );
            ecs::Entity createSkybox(
                common::game::ObjectName name,
                common::game::ObjectParams params,
                common::game::ObjectFormat format = common::game::ObjectFormat::PNG,
                std::string path = "./"
            );
            std::string objectNameToString(common::game::ObjectName name);
            std::string objectFormatToString(common::game::ObjectFormat format);
    };
}
