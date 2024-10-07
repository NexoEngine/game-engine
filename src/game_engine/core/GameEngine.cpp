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

    std::vector<std::pair<std::type_index, std::any>> Engine::getAllComponents(ecs::Entity entity)
    {
        return _coordinator->getAllComponents(entity);
    }

    ecs::Entity Engine::addEntity(ecs::components::physics::transform_t transf, ecs::components::render::render_t render)
    {
        ecs::Entity entity = _coordinator->createEntity();
        _coordinator->addComponent<ecs::components::physics::transform_t>(entity, transf);
        _coordinator->addComponent<ecs::components::render::render_t>(entity, render);
        return entity;
    }

    ecs::Entity Engine::addInvisibleEntity(void)
    {
        return _coordinator->createEntity();
    }

    void Engine::stopMusic(ecs::Entity musicSource)
    {
        auto &music = _coordinator->getComponent<ecs::components::sound::MusicSource>(musicSource);
        music.stop = true;
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
        _renderSystem->render();
        DrawGrid(10000, 1.0f);
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

    void destroyEntity(ecs::Entity entity)
    {
        Engine::getInstance()->destroyEntity(entity);
    }

    void triggerAudio(Sound sound)
    {
        Engine::getInstance()->triggerAudio(sound);
    }

    ecs::Entity playMusic(const std::string &audioPath, bool looping)
    {
        return Engine::getInstance()->playMusic(audioPath, looping);
    }

    void pauseMusic(ecs::Entity musicSource)
    {
        Engine::getInstance()->pauseMusic(musicSource);
    }

    void resumeMusic(ecs::Entity musicSource)
    {
        Engine::getInstance()->resumeMusic(musicSource);
    }

    ecs::SceneID createScene()
    {
        return Engine::getInstance()->createScene();
    }

    void deleteScene(ecs::SceneID id)
    {
        Engine::getInstance()->deleteScene(id);
    }

    void activateScene(ecs::SceneID id)
    {
        Engine::getInstance()->activateScene(id);
    }

    void deactivateScene(ecs::SceneID id)
    {
        Engine::getInstance()->deactivateScene(id);
    }

    void pauseScene(ecs::SceneID id)
    {
        Engine::getInstance()->pauseScene(id);
    }

    void resumeScene(ecs::SceneID id)
    {
        Engine::getInstance()->resumeScene(id);
    }

    bool isScenePaused(ecs::SceneID id)
    {
        return Engine::getInstance()->isScenePaused(id);
    }

    std::shared_ptr<engine::core::EngineCamera> createCamera(Vector3 pos, Vector3 target, Vector3 up, int mode, float fov)
    {
        return Engine::getInstance()->createCamera(pos, target, up, mode, fov);
    }

    std::shared_ptr<engine::core::EngineCamera> createMovableCamera(Vector3 pos, Vector3 target, Vector3 up, int mode, float fov)
    {
        return Engine::getInstance()->createMovableCamera(pos, target, up, mode, fov);
    }

    void attachCamera(ecs::SceneID sceneID, std::shared_ptr<engine::core::EngineCamera> camera)
    {
        Engine::getInstance()->attachCamera(sceneID, camera);
    }

    void detachCamera(ecs::SceneID sceneID, std::shared_ptr<engine::core::EngineCamera> camera)
    {
        Engine::getInstance()->detachCamera(sceneID, camera);
    }

    engine::core::LightId Engine::createLight(engine::core::LightType type, Vector3 position, Vector3 target, Color color, Shader lightingShader)
    {
        if (engine::core::lights.size() >= MAX_LIGHTS) {
            std::cout << "Max number of lights reached!" << std::endl;
            return -1;
        }

        engine::core::Light newLight;
        newLight.id = engine::core::nextLightID;
        std::cout << engine::core::nextLightID << std::endl;
        newLight.enabled = 1;
        newLight.type = type;
        newLight.position = position;
        newLight.target = target;
        newLight.color = color;

        engine::core::lights.push_back(newLight);

        newLight.enabledLoc = GetShaderLocation(lightingShader, TextFormat("lights[%i].enabled", engine::core::nextLightID));
        newLight.typeLoc = GetShaderLocation(lightingShader, TextFormat("lights[%i].type", engine::core::nextLightID));
        newLight.positionLoc = GetShaderLocation(lightingShader, TextFormat("lights[%i].position", engine::core::nextLightID));
        newLight.targetLoc = GetShaderLocation(lightingShader, TextFormat("lights[%i].target", engine::core::nextLightID));
        newLight.colorLoc = GetShaderLocation(lightingShader, TextFormat("lights[%i].color", engine::core::nextLightID));

        int enabled = (newLight.enabled) ? 1 : 0;
        int lightType = (newLight.type == engine::core::DIRECTIONAL) ? 0 : 1;
        SetShaderValue(lightingShader, newLight.enabledLoc, &enabled, SHADER_UNIFORM_INT);
        SetShaderValue(lightingShader, newLight.typeLoc, &lightType, SHADER_UNIFORM_INT);

        float pos[3] = { newLight.position.x, newLight.position.y, newLight.position.z };
        SetShaderValue(lightingShader, newLight.positionLoc, pos, SHADER_UNIFORM_VEC3);

        float tar[3] = { newLight.target.x, newLight.target.y, newLight.target.z };
        SetShaderValue(lightingShader, newLight.targetLoc, tar, SHADER_UNIFORM_VEC3);

        float col[4] = { (float)newLight.color.r/(float)255, (float)newLight.color.g/(float)255, 
                        (float)newLight.color.b/(float)255, (float)newLight.color.a/(float)255 };
        SetShaderValue(lightingShader, newLight.colorLoc, col, SHADER_UNIFORM_VEC4);

        engine::core::nextLightID++;
        return newLight.id; 
    }

    void Engine::setLightEnabled(engine::core::LightId lightID, bool enabled, Shader lightingShader)
    {
        for (engine::core::Light& light : engine::core::lights) {
            if (light.id == lightID) {
                light.enabled = enabled ? 1 : 0;
                for (int i = 0; i < engine::core::lights.size(); i++) {
                    std::string enabledStr = "lights[" + std::to_string(i) + "].enabled";
                    std::string typeStr = "lights[" + std::to_string(i) + "].type";
                    std::string posStr = "lights[" + std::to_string(i) + "].position";
                    std::string colorStr = "lights[" + std::to_string(i) + "].color";

                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, enabledStr.c_str()), &engine::core::lights[i].enabled, SHADER_UNIFORM_INT);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, typeStr.c_str()), &engine::core::lights[i].type, SHADER_UNIFORM_INT);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, posStr.c_str()), &engine::core::lights[i].position, SHADER_UNIFORM_VEC3);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, colorStr.c_str()), &engine::core::lights[i].color, SHADER_UNIFORM_VEC4);
                }
                break;
            }
        }
    }

    void Engine::setLightColor(engine::core::LightId lightID, Color newColor, Shader lightingShader)
    {
        for (engine::core::Light& light : engine::core::lights) {
            if (light.id == lightID) {
                light.color = newColor;
                for (int i = 0; i < engine::core::lights.size(); i++) {
                    std::string enabledStr = "lights[" + std::to_string(i) + "].enabled";
                    std::string typeStr = "lights[" + std::to_string(i) + "].type";
                    std::string posStr = "lights[" + std::to_string(i) + "].position";
                    std::string colorStr = "lights[" + std::to_string(i) + "].color";

                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, enabledStr.c_str()), &engine::core::lights[i].enabled, SHADER_UNIFORM_INT);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, typeStr.c_str()), &engine::core::lights[i].type, SHADER_UNIFORM_INT);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, posStr.c_str()), &engine::core::lights[i].position, SHADER_UNIFORM_VEC3);
                    SetShaderValue(lightingShader, GetShaderLocation(lightingShader, colorStr.c_str()), &engine::core::lights[i].color, SHADER_UNIFORM_VEC4);
                }
                break;
            }
        }
    }
}
