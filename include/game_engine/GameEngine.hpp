/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** GameEngine.hpp
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"
#include "core/Window.hpp"
#include "./ecs/systems/Physics.hpp"
#include "game_engine/ecs/systems/Behaviour.hpp"
#include "game_engine/ecs/components/Physics.hpp"
#include "game_engine/ecs/components/Render.hpp"
#include "game_engine/ecs/components/Behaviour.hpp"
#include "game_engine/ecs/components/Network.hpp"
#include "game_engine/ecs/components/Health.hpp"
#include "game_engine/ecs/components/Direction.hpp"
#include "game_engine/ecs/components/Input.hpp"
#include "game_engine/ecs/components/Audio.hpp"
#include "game_engine/ecs/systems/Render.hpp"
#include "game_engine/ecs/Entity.hpp"
#include "game_engine/ecs/components/Animations.hpp"
#include "game_engine/ecs/systems/Animations.hpp"
#include "game_engine/ecs/systems/Input.hpp"
#include "game_engine/ecs/Scene.hpp"
#include "game_engine/ecs/systems/Audio.hpp"
#include "common/utils/Chrono.hpp"
#include "game_engine/core/Camera.hpp"
#include "game_engine/core/Light.hpp"
#include <memory>
#include <mutex>
#include <functional>
#include <utility>

namespace engine {
    /**
     * @class Engine
     * @brief Main engine class responsible for initializing and managing game components.
     *
     * The Engine class is the core of the game engine, handling the initialization and management
     * of various systems like physics, rendering, behaviors, and animations. It also manages entities
     * and provides interfaces to interact with the ECS Coordinator.
     */
    class Engine {
        public:
            /**
             * @brief Initializes the engine, setting up necessary components and systems.
             * @param disableRender Flag to disable rendering, useful for non-graphical applications.
             */
            void init(bool disableRender = false);

            /**
             * @brief Adds a new entity to the game with optional physics and render components.
             * @param transf Physics transformation component.
             * @param render Render component.
             * @return The created entity.
             */
            ecs::Entity addEntity(ecs::components::physics::transform_t transf = {{0, 1, 0}, {0}, {0}},
                                    ecs::components::render::render_t render = {ecs::components::ShapeType::CUBE, true, std::make_shared<ecs::components::Cube>()}
            );

            ecs::Entity addInvisibleEntity(void);

            void destroyEntity(ecs::Entity entity);

            /**
             * @brief Adds a component to an entity.
             * @tparam T Type of the component.
             * @param entity The entity to which the component will be added.
             * @param component The component to add.
             */
            template<typename T>
            void addComponent(ecs::Entity entity, T component) {
                _coordinator->addComponent<T>(entity, component);
            }

            /**
             * @brief Retrieves a component from an entity.
             * @tparam T Type of the component.
             * @param entity The entity from which the component will be retrieved.
             * @return Reference to the component of type T.
             */
            template<typename T>
            T &getComponent(ecs::Entity entity) {
                return _coordinator->getComponent<T>(entity);
            }

            template<typename T>
            T &getSingletonComponent() {
                return _coordinator->getSingletonComponent<T>();
            }

            std::vector<std::pair<std::type_index, std::any>> getAllComponents(ecs::Entity entity);

            /**
             * @brief Registers an event listener.
             * @tparam T Type of the event.
             * @param listener Function to handle the event.
             */
            template<typename T>
            void registerListener(std::function<void(T&)> listener)
            {
                auto shared = std::make_shared<std::function<void(T&)>>(listener);
                _coordinator->registerListener<T>(shared);
            }

            [[nodiscard]] ecs::SceneManager& getSceneManager() const
            {
                return _coordinator->getSceneManager();
            }

            /**
             * @brief Get the window object.
             */
            [[nodiscard]] std::shared_ptr<engine::core::Window> getWindow() const
            {
                return _window;
            }



            /**
             * @brief Checks if the window of the game engine is open.
             * @return True if the window is open, false otherwise.
             */
            bool isWindowOpen(void)
            {
                if (_disableRender)
                    return false;
                return _window->isOpen();
            }

            /*/**
             * @brief Starts and runs the game engine.
             #1#
            void run();*/

            void update(ecs::SceneID id);
            void render(ecs::SceneID sceneId, engine::core::CameraID cameraId);
            void renderTextureMode(ecs::SceneID sceneId, engine::core::CameraID cameraId);

            /*void runTextureMode(RenderTexture& ViewTexture);*/

            /**
             * @brief Gets the elapsed time since the engine started.
             * @return Elapsed time in seconds.
             */
            [[nodiscard]] double getElapsedTime() const
            {
                return _chrono.getElapsedTime();
            }

            /**
             * @brief Allows to trigger an audio sound, creates a new audioEntity and adds
             * it to the ecs
             * @param sound The sound to play
             */
            void triggerAudio(Sound sound);

            /**
             * @brief Add a new audio that plays a music
             *
             * @param audioPath
             * @return ecs::Entity
             */
            ecs::Entity playMusic(const std::string &audioPath, bool looping = false);

            /**
             * @brief Stops a music
             *
             * @param audioSource The entity corresponding to the music source
             */
            void stopMusic(ecs::Entity musicSource);

            /**
             * @brief Pause a music
             *
             * @param musicSource The entity corresponding to the music source
             */
            void pauseMusic(ecs::Entity musicSource);

            /**
             * @brief Resume a music
             *
             * @param musicSource The entity corresponding to the music source that has been paused
             */
            void resumeMusic(ecs::Entity musicSource);

            template<typename T>
            void emitEvent(T &event)
            {
                _coordinator->emitEvent<T>(event);
            }

            /**
             * @brief Create a Scene object
             *
             * @return ecs::SceneID
             */
            ecs::SceneID createScene();

            /**
             * @brief Deletes a scene
             *
             * @param id
             */
            void deleteScene(ecs::SceneID id);

            /**
             * @brief Activates a scene
             *
             * @param id
             */
            void activateScene(ecs::SceneID id);

            /**
             * @brief Deactivate a scene
             *
             * @param id
             */
            void deactivateScene(ecs::SceneID id);

            void pauseScene(ecs::SceneID id);
            void resumeScene(ecs::SceneID id);
            bool isScenePaused(ecs::SceneID id);

            void addEntityToScene(ecs::Entity entity, ecs::SceneID sceneID)
            {
                _coordinator->addEntityToScene(entity, sceneID);
            }

            void removeEntityFromScene(ecs::Entity entity, ecs::SceneID sceneID)
            {
                _coordinator->removeEntityFromScene(entity, sceneID);
            }

            void setSceneWindowOffset(ecs::SceneID id, Vector2 offset);
            [[nodiscard]] Vector2 getSceneWindowOffset(ecs::SceneID id) const;

            std::shared_ptr<engine::core::EngineCamera> createCamera(Vector3 pos, Vector3 target, Vector3 up, int mode, float fov);
            std::shared_ptr<engine::core::EngineCamera> createMovableCamera(Vector3 pos, Vector3 target, Vector3 up, int mode, float fov);

            void attachCamera(ecs::SceneID sceneID, std::shared_ptr<engine::core::EngineCamera> camera);
            void detachCamera(ecs::SceneID sceneID, std::shared_ptr<engine::core::EngineCamera> camera);

            engine::core::LightId createLight(engine::core::LightType type, Vector3 position, Vector3 target, Color color, Shader lightingShader = ecs::components::shader::defaultLightingShader);
            void setLightEnabled(engine::core::LightId lightID, bool enabled, Shader lightingShader = ecs::components::shader::defaultLightingShader);
            void setLightColor(engine::core::LightId lightID, Color newColor, Shader lightingShader = ecs::components::shader::defaultLightingShader);

            void startRendering(ecs::SceneID sceneID, core::CameraID cameraID);
            void endRendering(ecs::SceneID sceneID);

            void renderAllEntities(ecs::SceneID sceneId, core::CameraID cameraId);
            void renderGrid(ecs::SceneID sceneID, core::CameraID cameraID);
            void enableDebug(void) { _debug = true; };
            void disableDebug(void) {_debug = false; };


        private:
            std::shared_ptr<ecs::Coordinator> _coordinator;
            std::shared_ptr<ecs::system::PhysicsSystem> _physicSystem;
            std::shared_ptr<ecs::system::RenderSystem> _renderSystem;
            std::shared_ptr<ecs::system::RenderSystemDebug> _renderSystemDebug;
            std::shared_ptr<ecs::system::BehaviourSystem> _behaviourSystem;
            std::shared_ptr<ecs::system::AnimationSystem> _animationSystem;
            std::shared_ptr<ecs::system::CollisionResponse> _collisionResponseSystem;
            std::shared_ptr<ecs::system::ColisionDetectionSystem> _collisionDetectionSystem;
            std::shared_ptr<ecs::system::InputSystem> _inputSystem;
            std::shared_ptr<ecs::system::AudioSystem> _audioSystem;
            std::shared_ptr<ecs::system::MusicSystem> _musicSystem;

            std::shared_ptr<core::Window> _window;
            bool _disableRender = false;
            bool _rendering = false;

            common::utils::Chrono _chrono;

            std::queue<ecs::Entity> _entitiesToDestroy;

            engine::core::CameraID _nextId = 0;

            bool _debug = false;

        private:
            static Engine *engine;
            static std::mutex _mutex;

        protected:
            Engine() {};
            ~Engine() {};

        public:
            Engine(Engine &other) = delete;
            void operator=(const Engine &) = delete;

            static Engine *getInstance() {
                std::lock_guard<std::mutex> lock(_mutex);
                if (engine == nullptr)
                    engine = new Engine;
                return engine;
            };
    };

    /**
     * @brief Initializes the game engine.
     * @param disableRender Flag to disable rendering.
     */
    void initEngine(bool disableRender = false);

    /**
     * @brief Update all the systems of the engine of the entites attached to the scene
     *
     * @param sceneId Id of the scene
     */
    void update(ecs::SceneID sceneId);

    /**
     * @brief Render the entities attached to the scene based on the specified camera
     *
     * @param sceneId id of the scene to be rendered
     * @param cameraId id of the camera to be used
     * @note Renders to the window
     */
    void render(ecs::SceneID sceneId, engine::core::CameraID cameraId);

    void enableDebug(void);
    void disableDebug(void);

    /**
    * @brief Render the entities attached to the scene based on the specified camera
    *
    * @param sceneId id of the scene to be rendered
    * @param cameraId id of the camera to be used
    * @note Renders to a RenderTexture stored in the camera
    * See camera.getRenderTexture()
    */
    void renderTextureMode(ecs::SceneID sceneId, engine::core::CameraID cameraId);
}