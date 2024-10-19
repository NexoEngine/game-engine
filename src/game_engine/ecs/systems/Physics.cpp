/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Physics.cpp
*/

#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/systems/Physics.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/core/event/CollisionEvent.hpp"
#include "game_engine/GameEngine.hpp"

#include <algorithm>
#include <iostream>

namespace ecs {
    namespace system {
        void PhysicsSystem::updatePosition() {
            for (auto const &entity : _entities) {
                auto& transf = _coord->getComponent<components::physics::transform_t>(entity);
                auto& body = _coord->getComponent<components::physics::rigidBody_t>(entity);
                auto &collider = _coord->getComponent<components::physics::collider_t>(entity);

                double now = engine::Engine::getInstance()->getElapsedTime() / 1000;
                double elapsedTime = now - body.velocityLastUpdate;

                transf.pos.x += body.velocity.x * elapsedTime;
                transf.pos.y += body.velocity.y * elapsedTime;
                transf.pos.z += body.velocity.z * elapsedTime;

                Matrix translate = MatrixTranslate(transf.pos.x, transf.pos.y, transf.pos.z);
                collider.matTranslate = translate;
                CollisionResponse::updateColliderGlobalVerts(collider);
                body.velocityLastUpdate = now;
            }
        }

        void ColisionDetectionSystem::detectSphereCollision(ecs::components::physics::collider_t data1,
                                                            ecs::components::physics::collider_t data2) {

        }

        void ColisionDetectionSystem::detectSphereBoxCollision(ecs::components::physics::collider_t collider1,
                                                            ecs::components::physics::collider_t collider2)
        {

        }

        void ColisionDetectionSystem::detectCollision() {
            for (auto it1 = _entities.begin(); it1 != _entities.end(); it1++) {
                auto& transf1 = _coord->getComponent<components::physics::transform_t>(*it1);
                auto& collider1 = _coord->getComponent<components::physics::collider_t>(*it1);
                auto it2 = it1;
                it2++;
                for (; it2 != _entities.end(); it2++) {
                    if (it2 == _entities.end()) break;
                    auto& transf2 = _coord->getComponent<components::physics::transform_t>(*it2);
                    auto& collider2 = _coord->getComponent<components::physics::collider_t>(*it2);
                    if (collider1.collisionType == ecs::components::physics::CollisionType::NON_COLLIDE ||
                            collider2.collisionType == ecs::components::physics::CollisionType::NON_COLLIDE) {
                        continue;
                    }
                    if (collider1.shapeType == ecs::components::ShapeType::SPHERE || collider2.shapeType == ecs::components::ShapeType::SPHERE) {
                    } else {
                        // // BoundingBox box1 = collider1.data->getBoundingBox(collider1);
                        // // BoundingBox box2 = collider2.data->getBoundingBox(collider2);
                        // bool colliding = CheckCollisionBoxes(box1, box2);
                        // if (colliding) {
                        //     CollisionEvent event(*it1, box1, collider1.data->getModel().transform, *it2, box2, collider2.data->getModel().transform);
                        //     _coord->emitEvent<CollisionEvent>(event);
                        // }
                    }
                }
            }
        }

        CollisionResponse::CollisionResponse(ecs::Coordinator &coord) : _coord(coord)
        {
            auto lambda = [this](const CollisionEvent &event) {
                auto &transf = _coord.getComponent<ecs::components::physics::transform_t>(event.entity1);
                auto &collider = _coord.getComponent<ecs::components::physics::collider_t>(event.entity1);
                Vector3 displacementVector = getCollisionResponse(event);
                transf.pos.x += displacementVector.x;
                transf.pos.y += displacementVector.y;
                transf.pos.z += displacementVector.z;
                Matrix translate = MatrixTranslate(displacementVector.x, displacementVector.y, displacementVector.z);
                collider.matTranslate = MatrixMultiply(collider.matTranslate, translate);
                CollisionResponse::updateColliderGlobalVerts(collider);
            };

            auto shared = std::make_shared<std::function<void(CollisionEvent&)>>(lambda);

            _coord.registerListener<CollisionEvent>(shared);
        }

        float CollisionResponse::getOverlap(Vector2 a, Vector2 b)
        {
            if (a.x > b.y)
                return 0;
            if (b.x > a.y)
                return 0;
            if (a.x > b.x)
                return b.y - a.x;
            return b.x - a.y;
        }

        Vector2 CollisionResponse::getColliderProjectionBounds(
            ecs::components::physics::collider_t &collider,
            Vector3 vec)
        {
            Vector2 bounds = {0};
            float proj = Vector3DotProduct(collider.vertsGlobal[0], vec);
            bounds.x = bounds.y = proj;
            for (int i = 1; i < 8; ++i) {
                proj = Vector3DotProduct(collider.vertsGlobal[i], vec);
                bounds.x = fmin(bounds.x, proj);
                bounds.y = fmax(bounds.y, proj);
            }
            return bounds;
        }

        void CollisionResponse::getCollisionVectors(
            ecs::components::physics::collider_t &collider1,
            ecs::components::physics::collider_t &collider2,
            Vector3 *vecs
        ) {
            Vector3 x = { 1.f, 0.f, 0.f };
	        Vector3 y = { 0.f, 1.f, 0.f };
	        Vector3 z = { 0.f, 0.f, 1.f };

            vecs[0] = Vector3Transform(x, collider1.matRotate);
            vecs[1] = Vector3Transform(y, collider1.matRotate);
            vecs[2] = Vector3Transform(z, collider1.matRotate);
            vecs[3] = Vector3Transform(x, collider2.matRotate);
            vecs[4] = Vector3Transform(y, collider2.matRotate);
            vecs[5] = Vector3Transform(z, collider2.matRotate);

            int i = 6;
            for (int j = 0; j < 3; ++j)
                for (int k = 3; k < 6; ++k)
                    if (Vector3Equals(vecs[j], vecs[k]))
                        vecs[i++] = x;
                    else
                        vecs[i++] = Vector3Normalize(Vector3CrossProduct(vecs[j], vecs[k]));
        }

        Vector3 CollisionResponse::getCollisionResponse(const CollisionEvent &event)
        {
            float overlapMin = 50.f;
            Vector3 overlapDir = {0};
            ecs::components::physics::collider_t &collider1 = _coord.getComponent<ecs::components::physics::collider_t>(event.entity1);
            ecs::components::physics::collider_t &collider2 = _coord.getComponent<ecs::components::physics::collider_t>(event.entity2);

            Vector3 testVec[15];
            getCollisionVectors(collider1, collider2, testVec);
            for (int i = 0; i < 15; ++i) {
                Vector2 apro, bpro;
                apro = getColliderProjectionBounds(collider1, testVec[i]);
                bpro = getColliderProjectionBounds(collider2, testVec[i]);

                float overlap = getOverlap(apro, bpro);
                if (overlap == 0)
                    return Vector3Zero();
                if (fabs(overlap) < fabs(overlapMin)) {
                    overlapMin = overlap;
                    overlapDir = testVec[i];
                }
            }
            return Vector3Scale(overlapDir, overlapMin);
        }

        void CollisionResponse::updateColliderGlobalVerts(ecs::components::physics::collider_t &collider)
        {
            Matrix matTemp = MatrixMultiply(MatrixMultiply(collider.matScale, collider.matRotate), collider.matTranslate);
            Vector3 vertsLocal[8];
            vertsLocal[0] = Vector3 { collider.box.min.x, collider.box.min.y, collider.box.min.z };
            vertsLocal[1] = Vector3 { collider.box.min.x, collider.box.min.y, collider.box.max.z };
            vertsLocal[2] = Vector3 { collider.box.min.x, collider.box.max.y, collider.box.min.z };
            vertsLocal[3] = Vector3 { collider.box.min.x, collider.box.max.y, collider.box.max.z };
            vertsLocal[4] = Vector3 { collider.box.max.x, collider.box.min.y, collider.box.min.z };
            vertsLocal[5] = Vector3 { collider.box.max.x, collider.box.min.y, collider.box.max.z };
            vertsLocal[6] = Vector3 { collider.box.max.x, collider.box.max.y, collider.box.min.z };
            vertsLocal[7] = Vector3 { collider.box.max.x, collider.box.max.y, collider.box.max.z };
            for (int i = 0; i < 8; ++i)
                collider.vertsGlobal[i] = Vector3Transform(vertsLocal[i], matTemp);
        }
    }
}