/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** GameEngine.cpp
*/

#include "game_engine/GameEngine.hpp"
#include <Math.hpp>
#include <EntityHandling.hpp>

#include "game_engine/ecs/components/Behaviour.hpp"
#include "game_engine/ecs/components/Shader.hpp"
#include "raymath.h"
#include <rlgl.h>

std::shared_ptr<ecs::Coordinator> ecs::components::behaviour::Behaviour::_coord = nullptr;
std::shared_ptr<ecs::Coordinator> ecs::system::System::_coord = nullptr;
namespace engine
{

    Engine *Engine::engine = nullptr;
    std::mutex Engine::_mutex;
    void Engine::init(bool disableRender)
    {
        _disableRender = disableRender;
        if (!_disableRender) {
            _window = std::make_shared<core::Window>();
            _window->setFPS(60);
        }
        _coordinator = std::make_shared<ecs::Coordinator>();
        _coordinator->init();
        _collisionResponseSystem = std::make_shared<ecs::system::CollisionResponse>(*_coordinator);
        ecs::components::behaviour::Behaviour::_coord = _coordinator;
        ecs::system::System::_coord = _coordinator;

        _coordinator->registerComponent<ecs::components::physics::transform_t>();
        if (!_disableRender)
            _coordinator->registerComponent<ecs::components::render::render_t>();
        _coordinator->registerComponent<std::shared_ptr<ecs::components::behaviour::Behaviour>>();
        _coordinator->registerComponent<ecs::components::physics::collider_t>();
        _coordinator->registerComponent<ecs::components::physics::rigidBody_t>();
        _coordinator->registerComponent<ecs::components::animations::animation_t>();
        _coordinator->registerComponent<ecs::components::network::network_t>();
        _coordinator->registerComponent<ecs::components::health::health_t>();
        _coordinator->registerComponent<ecs::components::direction::direction_t>();
        _coordinator->registerComponent<ecs::components::sound::AudioSource>();
        _coordinator->registerComponent<ecs::components::sound::MusicSource>();
        ecs::components::input::Input input;
        _coordinator->registerSingletonComponent<ecs::components::input::Input>(input);

        ecs::Signature signaturePhysics;
        signaturePhysics.set(_coordinator->getComponentType<ecs::components::physics::transform_t>());
        signaturePhysics.set(_coordinator->getComponentType<ecs::components::physics::rigidBody_t>());
        ecs::Signature signatureRender;
        if (!_disableRender)
        {
            signatureRender.set(_coordinator->getComponentType<ecs::components::physics::transform_t>());
            signatureRender.set(_coordinator->getComponentType<ecs::components::render::render_t>());
        }
        ecs::Signature signatureBehaviour;
        signatureBehaviour.set(_coordinator->getComponentType<std::shared_ptr<ecs::components::behaviour::Behaviour>>());
        ecs::Signature signatureCollider;
        signatureCollider.set(_coordinator->getComponentType<ecs::components::physics::transform_t>());
        signatureCollider.set(_coordinator->getComponentType<ecs::components::physics::collider_t>());
        ecs::Signature signatureAnimations;
        signatureAnimations.set(_coordinator->getComponentType<ecs::components::animations::animation_t>());
        ecs::Signature signatureAudioSystem;
        signatureAudioSystem.set(_coordinator->getComponentType<ecs::components::sound::AudioSource>());
        ecs::Signature signatureMusicSystem;
        signatureMusicSystem.set(_coordinator->getComponentType<ecs::components::sound::MusicSource>());

        _physicSystem = _coordinator->registerSystem<ecs::system::PhysicsSystem>();
        _coordinator->setSystemSignature<ecs::system::PhysicsSystem>(signaturePhysics);

        if (!_disableRender)
        {
            _renderSystem = _coordinator->registerSystem<ecs::system::RenderSystem>();
            _coordinator->setSystemSignature<ecs::system::RenderSystem>(signatureRender);
        }

        _behaviourSystem = _coordinator->registerSystem<ecs::system::BehaviourSystem>();
        _coordinator->setSystemSignature<ecs::system::BehaviourSystem>(signatureBehaviour);

        _collisionDetectionSystem = _coordinator->registerSystem<ecs::system::ColisionDetectionSystem>();
        _coordinator->setSystemSignature<ecs::system::ColisionDetectionSystem>(signatureCollider);

        _animationSystem = _coordinator->registerSystem<ecs::system::AnimationSystem>();
        _coordinator->setSystemSignature<ecs::system::AnimationSystem>(signatureAnimations);

        _inputSystem = _coordinator->registerSystem<ecs::system::InputSystem>();

        InitAudioDevice();
        _audioSystem = _coordinator->registerSystem<ecs::system::AudioSystem>();
        _coordinator->setSystemSignature<ecs::system::AudioSystem>(signatureAudioSystem);
        _musicSystem = _coordinator->registerSystem<ecs::system::MusicSystem>();
        _coordinator->setSystemSignature<ecs::system::MusicSystem>(signatureMusicSystem);

        ecs::components::shader::defaultLightingShader = LoadShader(DEFAULT_LIGHT_VS, DEFAULT_LIGHT_FS);
        ecs::components::shader::defaultLightingShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(ecs::components::shader::defaultLightingShader, "viewPos");

        float col[4] = { 0.8f, 0.1f, 0.1f, 1.0f };
        int ambientLoc = GetShaderLocation(ecs::components::shader::defaultLightingShader, "ambient");
        SetShaderValue(ecs::components::shader::defaultLightingShader, ambientLoc, col, SHADER_UNIFORM_VEC4);
    }

    void Engine::update(ecs::SceneID sceneId)
    {
        if (_coordinator->isScenePaused(sceneId) || !_coordinator->isSceneActive(sceneId))
            return;
        //activateScene(sceneId);
        _inputSystem->handleInputs();
        _behaviourSystem->handleBehaviours();
        //_physicSystem->updatePosition();
        _animationSystem->handleAnimations();
        //_collisionDetectionSystem->detectCollision();
        _audioSystem->update();
        _musicSystem->update();
        _coordinator->dispatchEvents();
        // pop and use front to destroy entity is entitiestodestroy
        while (!_entitiesToDestroy.empty()) {
            _coordinator->destroyEntity(_entitiesToDestroy.front());
            _entitiesToDestroy.pop();
        }
        //deactivateScene(sceneId);
    }

    void Engine::render(ecs::SceneID sceneId, engine::core::CameraID cameraId)
    {
        bool isSceneActive = _coordinator->isSceneActive(sceneId);
        if (!isSceneActive)
            activateScene(sceneId);
        auto &camera = _coordinator->getCamera(sceneId, cameraId);
        camera->update();
        BeginDrawing();
        _window->clear(Color{41, 41, 41, 255});
        BeginMode3D(_coordinator->getCamera(sceneId, cameraId)->getCamera());
        _renderSystem->render();
        //DrawGrid(10000, 1.0f);
        EndMode3D();
        EndDrawing();
        if (!isSceneActive)
            deactivateScene(sceneId);
    }

    void renderGridInternal(std::shared_ptr<engine::core::EngineCamera> engineCamera)
    {
        Camera camera = engineCamera->getCamera();
        Vector2 screenSize = engineCamera->getRenderTextureSize();
        float screenWidth = screenSize.x;
        float screenHeight = screenSize.y;
        Shader shader = LoadShader("src/game_engine/ressources/shaders/grid_fading.vs", "src/game_engine/ressources/shaders/grid_fading.fs");
        int matViewLoc = GetShaderLocation(shader, "matView");
	    int matProjectionLoc = GetShaderLocation(shader, "matProjection");
        Matrix view = GetCameraMatrix(camera);
        Matrix projection = MatrixPerspective(camera.fovy * DEG2RAD, (float)screenWidth / (float)screenHeight, 0.01f, 1000.0f);
        SetShaderValueMatrix(shader, matViewLoc, view);
	    SetShaderValueMatrix(shader, matProjectionLoc, projection);
        rlDisableDepthTest();
            BeginShaderMode(shader);
                rlPushMatrix();
                    rlTranslatef(0.0f, 0.0f, 0.0f);
                    rlScalef(1000.0f, 1.0f, 1000.0f);

                    rlBegin(RL_TRIANGLES);
                        rlVertex3f(-1, 1, -0);
                        rlVertex3f(-1, -1, 0);
                        rlVertex3f(1, -1, 0);
                        rlVertex3f(1, 1, 0);
                        rlVertex3f(-1, 1, 0);
                        rlVertex3f(1, -1, 0);
					rlEnd();
				rlPopMatrix();
            EndShaderMode();
		rlEnableDepthTest();
    }

    void Engine::renderTextureMode(ecs::SceneID sceneId, engine::core::CameraID cameraId)
    {
        bool isSceneActive = _coordinator->isSceneActive(sceneId);
        if (!isSceneActive)
            activateScene(sceneId);
        auto &camera = _coordinator->getCamera(sceneId, cameraId);
        auto screenTexture = _coordinator->getCamera(sceneId, cameraId)->getRenderTexture();
        camera->update();
        float cameraPos[3] = { camera->getPosition().x, camera->getPosition().y, camera->getPosition().z };
        SetShaderValue(ecs::components::shader::defaultLightingShader, ecs::components::shader::defaultLightingShader.locs[SHADER_LOC_VECTOR_VIEW], cameraPos, SHADER_UNIFORM_VEC3);
        BeginTextureMode(screenTexture);
        _window->clear(Color{41, 41, 41, 255});
        BeginMode3D(_coordinator->getCamera(sceneId, cameraId)->getCamera());
        renderGridInternal(camera);
        _renderSystem->render();
        //DrawGrid(10000, 1.0f);
        EndMode3D();

        //TODO: Add post process shaders
        if (camera->isShaderEnabled(ecs::components::shader::SHADER_HDR))
        {
            //BeginShaderMode()
            // DrawTextureRec(screenTexture.texture, 
            //               (Rectangle){0, 0, screenTexture.texture.width, -screenTexture.texture.height}, 
            //               (Vector2){0, 0}, 
            //               WHITE);
            //EndShaderMode()
        }

        if (camera->isShaderEnabled(ecs::components::shader::SHADER_BLOOM))
        {
            //BeginShaderMode()
            // DrawTextureRec(screenTexture.texture, 
            //               (Rectangle){0, 0, screenTexture.texture.width, -screenTexture.texture.height}, 
            //               (Vector2){0, 0}, 
            //               WHITE);
            //EndShaderMode()
        }

        if (camera->isShaderEnabled(ecs::components::shader::SHADER_DOF))
        {
            //BeginShaderMode()
            // DrawTextureRec(screenTexture.texture, 
            //               (Rectangle){0, 0, screenTexture.texture.width, -screenTexture.texture.height}, 
            //               (Vector2){0, 0}, 
            //               WHITE);
            //EndShaderMode()
        }

        if (camera->isShaderEnabled(ecs::components::shader::SHADER_MOTION_BLUR))
        {
            //BeginShaderMode()
            // DrawTextureRec(screenTexture.texture, 
            //               (Rectangle){0, 0, screenTexture.texture.width, -screenTexture.texture.height}, 
            //               (Vector2){0, 0}, 
            //               WHITE);
            //EndShaderMode()
        }

        if (camera->isShaderEnabled(ecs::components::shader::SHADER_SSAO))
        {
            //BeginShaderMode()
            // DrawTextureRec(screenTexture.texture, 
            //               (Rectangle){0, 0, screenTexture.texture.width, -screenTexture.texture.height}, 
            //               (Vector2){0, 0}, 
            //               WHITE);
            //EndShaderMode()
        }

        if (camera->isShaderEnabled(ecs::components::shader::SHADER_GAMMA_CORRECTION))
        {
            //BeginShaderMode()
            // DrawTextureRec(screenTexture.texture, 
            //               (Rectangle){0, 0, screenTexture.texture.width, -screenTexture.texture.height}, 
            //               (Vector2){0, 0}, 
            //               WHITE);
            //EndShaderMode()
        }

        EndTextureMode();

        if (!isSceneActive)
            deactivateScene(sceneId);
    }

    void initEngine(bool disableRender)
    {
        Engine::getInstance()->init(disableRender);
    }

    void update(ecs::SceneID sceneId)
    {
        Engine::getInstance()->update(sceneId);
    }

    void render(ecs::SceneID sceneId, engine::core::CameraID cameraId)
    {
        Engine::getInstance()->render(sceneId, cameraId);
    }

    void renderTextureMode(ecs::SceneID sceneId, engine::core::CameraID cameraId)
    {
        Engine::getInstance()->renderTextureMode(sceneId, cameraId);
    }
}
