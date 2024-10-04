
#include "protoVR/core/game.hpp"
#include "common/game/entities/EntityFactory.hpp"
#include <SceneHandling.hpp>
#include <CameraHandling.hpp>
#include <EventHandling.hpp>
#include <EntityHandling.hpp>

void Game::run()
{
    engine::initEngine();
    ecs::SceneID sceneID = engine::createScene();
    engine::core::EngineCamera camera = engine::createCamera({-5, 0, 0});
    engine::attachCamera(sceneID, camera);
    engine::activateScene(sceneID);

    common::game::EntityFactory factory;
    ecs::Entity cube = engine::createCube({0, 0, 0}, 1.0, 1.0, 1.0, Color{255, 0, 0, 255});
    engine::addEntityToScene(sceneID, cube);

    while (engine::isWindowOpen()) {
        engine::update(sceneID);
        engine::render(sceneID, camera.getCameraID());
    } 
}