/*
** EPITECH PROJECT, 2023
** server
** File description:
** EnemySpawner.hpp
*/

#pragma once

#include "game_engine/ecs/components/NetworkBehaviour.hpp"
#include "common/game/NetworkBody.hpp"
#include "server/core/NetServer.hpp"
#include "common/game/entities/EntityFactory.hpp"
#include "server/entities/Enemy/EnemyNetwork.hpp"

#include "game_engine/core/event/EnemyDestroyEvent.hpp"

#include <vector>

namespace ecs::components::behaviour
{
    class EnemySpawner: public ecs::components::behaviour::NetworkBehaviour<server::NetServer> {
        public:
            explicit EnemySpawner(server::NetServer& networkManager, uint32_t entityNetId = 0, uint32_t connectionId = 0)
                : NetworkBehaviour(networkManager, entityNetId, connectionId)
                {
                    _spawnPoints = {
                        {
                            {
                                {0, 0, 10},
                                100
                            }
                        },
                        {
                            {
                                {0, -5, 10},
                                100
                            },
                            {
                                {0, 5, 10},
                                100
                            },
                        },
                        {
                            {
                                {0, -5, 10},
                                100
                            },
                            {
                                {0, 0, 10},
                                100
                            },
                            {
                                {0, 5, 10},
                                100
                            }
                        },
                        {
                            {
                                {0, -5, 10},
                                200
                            },
                            {
                                {0, -5, 10},
                                200
                            },
                        },
                        {
                            {
                                {0, 0, 10},
                                300
                            },
                        }
                    };
                }

            void onAttach(ecs::Entity entity) override
            {
                // addListener<EnemyDestroyEvent>(
                //     [this](EnemyDestroyEvent& event) {
                //     auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(event.entity);
                //     if (metadata.type != server::entities::EntityType::ENEMY)
                //         return;

                //     removeEnemy(event.entity);
                // });
            }

            void addEnemy(Vector3 position, uint32_t healthPoints)
            {
                common::game::EntityFactory factory;
                ecs::Entity entity = factory.createEntity(common::game::ObjectType::Model3D, common::game::ObjectName::Transtellar, {
                    position,
                    0,
                    0,
                    0,
                    WHITE,
                    false,
                    WHITE,
                    {0, 180, 0},
                    {1, 1, 1}
                }, common::game::ObjectFormat::OBJ);
                auto behave = engine::createBehavior<server::EnemyNetwork>(_networkManager, entity, 0, _sceneID);
                engine::attachBehavior(entity, behave);
                engine::addEntityToScene(entity, _sceneID);
                auto &health = engine::Engine::getInstance()->getComponent<ecs::components::health::health_t>(entity);
                health.healthPoints = healthPoints;
                // auto &metadata = engine::Engine::getInstance()->getComponent<ecs::components::metadata::metadata_t>(entity);
                // metadata.type = server::entities::EntityType::ENEMY;

                rtype::net::Message<common::NetworkMessage> resMsg;
                resMsg.header.id = common::NetworkMessage::serverCreateEnemy;
                common::game::netbody::ServerCreateEnemy body = {
                    .entityNetId = entity,
                    .name = "Transtellar",
                    .shipName = common::game::ObjectName::Transtellar,
                    .pos = position,
                    .rotation = {0, 135, 0},
                };
               resMsg << body;

                _networkManager.messageAllClients(resMsg);

                _ennemies.emplace(entity);
            }

            void removeEnemy(Entity entity)
            {
                _ennemies.erase(entity);
            }

            void update() override
            {
                if (_ennemies.size() == 0 && _waveIndex < _spawnPoints.size() - 1) {
                    for (auto &spawnPoint : _spawnPoints[_waveIndex]) {
                        addEnemy(spawnPoint.position, spawnPoint.healthPoints);
                    }
                    _waveIndex++;
                }
            }

            typedef struct enemy_spawn_point_s {
                Vector3 position;
                uint32_t healthPoints;
            } enemy_spawn_point_t;

        private:
            std::set<Entity> _ennemies;
            std::vector<std::vector<enemy_spawn_point_t>> _spawnPoints;
            uint32_t _waveIndex = 0;
    };
}
