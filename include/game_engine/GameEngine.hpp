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

            engine::core::EngineCamera createCamera(Vector3 pos, Vector3 target, Vector3 up, int mode, float fov);

            void attachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera);
            void detachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera);

        private:
            std::shared_ptr<ecs::Coordinator> _coordinator;
            std::shared_ptr<ecs::system::PhysicsSystem> _physicSystem;
            std::shared_ptr<ecs::system::RenderSystem> _renderSystem;
            std::shared_ptr<ecs::system::BehaviourSystem> _behaviourSystem;
            std::shared_ptr<ecs::system::AnimationSystem> _animationSystem;
            std::shared_ptr<ecs::system::CollisionResponse> _collisionResponseSystem;
            std::shared_ptr<ecs::system::ColisionDetectionSystem> _collisionDetectionSystem;
            std::shared_ptr<ecs::system::InputSystem> _inputSystem;
            std::shared_ptr<ecs::system::AudioSystem> _audioSystem;
            std::shared_ptr<ecs::system::MusicSystem> _musicSystem;

            std::shared_ptr<core::Window> _window;
            bool _disableRender = false;

            common::utils::Chrono _chrono;

            std::queue<ecs::Entity> _entitiesToDestroy;

            engine::core::CameraID _nextId = 0;

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
     * @brief Runs the game engine.
     */
    void runEngine();

    void runEngineTextureMode(RenderTexture& ViewTexture);

    /**
     * @brief Creates an entity without transform or render component
     * (useful for entites that should have a behavior but not physical existence
     * like enemy spawner etc...)
     *
     */
    ecs::Entity createEntity(void);
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

    /**
    * @brief Render the entities attached to the scene based on the specified camera
    *
    * @param sceneId id of the scene to be rendered
    * @param cameraId id of the camera to be used
    * @note Renders to a RenderTexture stored in the camera
    * See camera.getRenderTexture()
    */
    void renderTextureMode(ecs::SceneID sceneId, engine::core::CameraID cameraId);


    std::vector<std::pair<std::type_index, std::any>> getAllComponents(ecs::Entity entity);

    /**
     * @brief Creates a cube entity with specified parameters.
     * @param pos Position of the cube.
     * @param width Width of the cube.
     * @param height Height of the cube.
     * @param length Length of the cube.
     * @param color Color of the cube.
     * @param toggleWire Flag to toggle wireframe mode.
     * @param wireColor Color of the wireframe.
     * @return The created cube entity.
     */
    ecs::Entity createCube(Vector3 pos, float width, float height, float length, Color color = RED, bool toggleWire = false, Color wireColor = BLACK);
    /**
     * @brief Creates a 3D model entity from a file.
     * @param filename Path to the model file.
     * @param pos Position of the model.
     * @param color Color to apply to the model.
     * @return The created model entity.
     */
    ecs::Entity createModel3D(const char *filename, Vector3 pos, Color color = WHITE);
    /**
     * @brief Creates a skybox entity from a file.
     * @param filename Path to the skybox file.
     * @param pos Position of the skybox.
     * @param color Color to apply to the skybox.
     * @return The created skybox entity.
     */
    ecs::Entity createSkybox(const char *filename, Vector3 pos, Color color = WHITE);

    void destroyEntity(ecs::Entity entity);

    /**
     * @brief Creates a behavior component.
     * @tparam T Type of the behavior.
     * @tparam Args Variadic template arguments for the behavior's constructor.
     * @param args Arguments to be forwarded to the behavior's constructor.
     * @return Shared pointer to the created behavior component.
     */
    template<typename T, typename... Args>
    std::shared_ptr<T> createBehavior(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }

    /**
     * @brief Attaches a behavior to an entity.
     * @param entity The entity to attach the behavior to.
     * @param behaviour Shared pointer to the behavior component.
     */
    void attachBehavior(ecs::Entity entity, std::shared_ptr<ecs::components::behaviour::Behaviour> behaviour);

    /**
     * @brief Checks if the window of the game engine is open.
     * @return True if the window is open, false otherwise.
     */
    bool isWindowOpen(void);

    /**
     * @brief Returns true if a key has been pressed
     *
     * @param key
     * @return true
     * @return false
     */
    bool isKeyPressed(ecs::components::input::Keys key);

    /**
     * @brief Returns true if a key has been released
     *
     * @param key
     * @return true
     * @return false
     */
    bool isKeyReleased(ecs::components::input::Keys key);

    /**
     * @brief Returns true if a key is down
     *
     * @param key
     * @return true
     * @return false
     */
    bool isKeyDown(ecs::components::input::Keys key);

    /**
     * @brief Returns true if a key is up
     *
     * @param key
     * @return true
     * @return false
     */
    bool isKeyUp(ecs::components::input::Keys key);

    /**
     * @brief Rotates an entity.
     * @param entity The entity to rotate.
     * @param rotation The rotation vector.
     */
    void rotate(ecs::Entity entity, Vector3 rotation);

    /**
    * @brief Set the rotation of an entity.
    * @param entity The entity to rotate.
    * @param rotation The rotation vector.
    */
    void setRotation(ecs::Entity entity, Vector3 rotation);
    /**
     * @brief Scales an entity.
     * @param entity The entity to scale.
     * @param scale The scale vector.
     */
    void scale(ecs::Entity entity, Vector3 scale);

    /**
    * @brief Set the scale of an entity.
    * @param entity The entity to scale.
    * @param scale The scale vector.
    */
    void setScale(ecs::Entity entity, Vector3 scale);
    /**
     * @brief Sets the animation for an entity.
     * @param entity The entity to set the animation for.
     * @param filename Path to the animation file.
     */
    void setAnimation(ecs::Entity entity, const char *filename);

    /**
     * @brief Registers an event listener.
     * @tparam T Type of the event.
     * @param listener Function to handle the event.
     */
    template<typename T>
    void registerListener(std::function<void(T&)> listener)
    {
        Engine::getInstance()->registerListener<T>(listener);
    }

    /**
     * @brief Triggers an audio sound
     *
     * @param sound The sound to play
     */
    void triggerAudio(Sound sound);

    /**
     * @brief Plays a music
     *
     * @param audioPath the path to the music file
     * @return ecs::Entity
     */
    ecs::Entity playMusic(const std::string &audioPath, bool looping = false);

    /**
     * @brief Stops a music
     *
     * @param musicSource The music source entity to which the music is attached
     */
    void stopMusic(ecs::Entity musicSource);

    /**
     * @brief Pauses a music
     *
     * @param musicSource The music source entity to which the music is attached
     */
    void pauseMusic(ecs::Entity musicSource);

    /**
     * @brief Resumes a paused music
     *
     * @param musicSource The music source entity to which the paused music is attached
     */
    void resumeMusic(ecs::Entity musicSource);

    template<typename T>
    void emitEvent(T &event)
    {
        Engine::getInstance()->emitEvent(event);
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

    /**
     * @brief Adds an entity to a scene
     *
     * @param entity
     * @param sceneID
     */
    void addEntityToScene(ecs::Entity entity, ecs::SceneID sceneID);

    /**
     * @brief Removes an entity from a scene
     *
     * @param entity
     * @param sceneID
     */
    void removeEntityFromScene(ecs::Entity entity, ecs::SceneID sceneID);

    /**
     * @brief Create a Camera object
     *
     * @param pos
     * @param target
     * @param up
     * @param mode
     * @param fov
     * @return engine::core::EngineCamera
     */
    engine::core::EngineCamera createCamera(Vector3 pos = {0, 0, 0}, Vector3 target = {0, 0, 0}, Vector3 up = {0, 1, 0}, int mode = CAMERA_PERSPECTIVE, float fov = 90.0f);

    /**
     * @brief Attach a camera to a scene
     *
     * @param sceneID
     * @param camera
     */
    void attachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera);

    /**
     * @brief Detach a camera from a scene
     *
     * @param sceneID
     * @param camera
     */
    void detachCamera(ecs::SceneID sceneID, engine::core::EngineCamera &camera);


    [[nodiscard]] ecs::SceneManager& getSceneManager();


    Matrix matrixFromFloat16(const float16& matrix);

    namespace entity {
        /**
         * @brief Get the transform matrix of the entity.
         * @param entity The entity to get the transform matrix from.
         * @return The transform matrix.
         */
        Matrix getTransformMatrix(ecs::Entity entity);

        /**
         * @brief Get the transform of the entity.
         * @param entity The entity to get the transform from.
         * @return The transform.
         */
        Transform getTransform(ecs::Entity entity);

        /**
        * @brief Retrieves a component from an entity.
        * @tparam T Type of the component.
        * @param entity The entity from which the component will be retrieved.
        * @return Reference to the component of type T.
        */
        template<typename T>
        T &getComponent(ecs::Entity entity)
        {
            return Engine::getInstance()->getComponent<T>(entity);
        }

        /**
         * @brief Set the transform of the entity.
         * @param entity The entity to set the transform to.
         * @param transMatrix The transformation matrix.
         */
        void setTransform(ecs::Entity entity, const Vector3 &position,
            const Vector3 &rotation, const Vector3 &scale);
    }

    namespace math {
          Matrix matrixFromFloat16(const float16& matrix);

          void ExtractCameraViewComponents(Matrix viewMatrix, Vector3& position, Vector3& target, Vector3& up);
    }
}