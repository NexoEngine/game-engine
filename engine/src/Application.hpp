//// Application.hpp //////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the main application
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glad/glad.h>
#include <iostream>
#include <memory>
#include <vector>

#include "Logger.hpp"
#include "Timer.hpp"
#include "Types.hpp"
#include "WorldState.hpp"
// #include "components/Light.hpp"
#include "components/PhysicsBodyComponent.hpp"
#include "core/event/SignalEvent.hpp"
#include "core/event/WindowEvent.hpp"
#include "core/scene/SceneManager.hpp"
#include "ecs/Coordinator.hpp"
#include "renderer/Window.hpp"

#include "systems/CameraSystem.hpp"
#include "systems/LightSystem.hpp"
#include "systems/PhysicsSystem.hpp"
#include "systems/RenderBillboardSystem.hpp"
#include "systems/RenderCommandSystem.hpp"
#include "systems/RenderVideoSystem.hpp"
#include "systems/TransformHierarchySystem.hpp"
#include "systems/TransformMatrixSystem.hpp"

#define NEXO_PROFILE(name) \
    nexo::Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_profileResults.push_back(profileResult); })

namespace nexo {

    namespace system {
        class ScriptingSystem;
    }

    enum class GameState { EDITOR_MODE, PLAY_MODE };

    enum EventDebugFlags {
        DEBUG_LOG_RESIZE_EVENT       = 1 << 0,
        DEBUG_LOG_KEYBOARD_EVENT     = 1 << 1,
        DEBUG_LOG_MOUSE_CLICK_EVENT  = 1 << 2,
        DEBUG_LOG_MOUSE_SCROLL_EVENT = 1 << 3,
        DEBUG_LOG_MOUSE_MOVE_EVENT   = 1 << 4
    };

    class Application final
        : LISTENS_TO(event::EventKey, event::EventWindowClose, event::EventWindowResize, event::EventMouseClick,
                     event::EventMouseScroll, event::EventMouseMove, event::EventAnySignal, event::EventSignal<SIGTERM>,
                     event::EventSignalInterrupt) {
       public:
        ~Application() override                     = default;
        Application(const Application &)            = delete;
        Application &operator=(const Application &) = delete;
        Application(Application &&)                 = delete;
        Application &operator=(Application &&)      = delete;

        /**
         * @brief Initializes the application, setting up the window, event manager, and ECS coordinator.
         *
         * This function performs the following steps:
         *  - Initializes the input system with the application window.
         *  - Registers signal handlers with the event manager.
         *  - Creates and configures the ECS coordinator.
         *  - Registers all necessary components and systems with the ECS coordinator.
         *  - Initializes various systems including rendering, physics, and scripting.
         *
         * @throws RendererException if there is an error initializing the rendering context.
         */
        void init();

        /**
         * @brief Begins a new frame by updating the timestep.
         *
         * Calculates the time elapsed since the last frame using glfwGetTime()
         * and updates the current timestep. Also updates the last frame time.
         */
        void beginFrame();

        struct SceneInfo {
            scene::SceneId id;
            RenderingType renderingType = RenderingType::WINDOW;
            SceneType sceneType         = SceneType::GAME;
            bool isChildWindow          = false; //<< Is the current scene embedded in a sub window ?
            glm::vec2 viewportBounds[2]{};       //<< Viewport bounds in absolute coordinates (if the window viewport is
                                                 // embedded in the window), this is used for mouse coordinates
        };

        /**
         * @brief Runs the application for the specified scene and rendering type.
         *
         * This function performs the following steps:
         *  - Retrieves the RenderContext singleton and sets the current scene to be rendered.
         *  - If the application window is not minimized:
         *      - If the scene is marked as rendered, it updates the camera context, light, and render systems.
         *      - If the scene is active, it updates the perspective camera controller system.
         *  - Depending on the rendering type, it triggers a window update (swaps buffers and polls events).
         *  - Dispatches events via the EventManager.
         *  - Resets the RenderContext for the next frame.
         *  - If profiling is enabled, displays the profiling results.
         *
         * @param sceneInfo
         */
        void run(const SceneInfo &sceneInfo);

        /**
         * @brief Ends the current frame by clearing processed events.
         *
         * Clears all the events that have been dispatched during the frame,
         * preparing the EventManager for the next frame.
         */
        void endFrame() const;

        /**
         * @brief Handles keyboard input events
         * @param event The keyboard event to process
         * Logs the keyboard event if DEBUG_LOG_KEYBOARD_EVENT flag is set
         */
        void handleEvent(event::EventKey &event) override
        {
            if (this->m_eventDebugFlags & DEBUG_LOG_KEYBOARD_EVENT) std::cout << event << std::endl;
        }

        /**
         * @brief Handles window close events
         * @param event The window close event to process
         * Sets the application running state to false, initiating shutdown
         */
        void handleEvent([[maybe_unused]] event::EventWindowClose &event) override
        {
            m_isRunning = false;
        }

        /**
         * @brief Handles window resize events
         * @param event The resize event containing new width and height
         * Updates minimized state based on window dimensions and logs if DEBUG_LOG_RESIZE_EVENT is set
         */
        void handleEvent(event::EventWindowResize &event) override
        {
            if (event.height == 0 || event.width == 0) m_isMinimized = true;
            if (m_isMinimized && event.width != 0 && event.height != 0) m_isMinimized = false;
            if (this->m_eventDebugFlags & DEBUG_LOG_RESIZE_EVENT) std::cout << event << std::endl;
        }

        /**
         * @brief Handles mouse click events
         * @param event The mouse click event to process
         * Logs the mouse click if DEBUG_LOG_MOUSE_CLICK_EVENT flag is set
         */
        void handleEvent(event::EventMouseClick &event) override
        {
            if (this->m_eventDebugFlags & DEBUG_LOG_MOUSE_CLICK_EVENT) std::cout << event << std::endl;
        }

        /**
         * @brief Handles mouse scroll events
         * @param event The mouse scroll event to process
         * Logs the scroll event if DEBUG_LOG_MOUSE_SCROLL_EVENT flag is set
         */
        void handleEvent(event::EventMouseScroll &event) override
        {
            if (this->m_eventDebugFlags & DEBUG_LOG_MOUSE_SCROLL_EVENT) std::cout << event << std::endl;
        }

        /**
         * @brief Handles mouse movement events
         * @param event The mouse move event to process
         * Logs the movement if DEBUG_LOG_MOUSE_MOVE_EVENT flag is set
         */
        void handleEvent(event::EventMouseMove &event) override
        {
            if (this->m_eventDebugFlags & DEBUG_LOG_MOUSE_MOVE_EVENT) std::cout << event << std::endl;
        }

        /**
         * @brief Handles any signal events
         * @param event The signal event to process
         * Logs reception of the signal event
         */
        void handleEvent(event::EventAnySignal &event) override
        {
            LOG(NEXO_INFO, "Received signal via {}", event);
        }

        /**
         * @brief Handles terminate signal events
         * @param event The terminate event to process
         * Logs reception and sets application running state to false
         */
        void handleEvent(event::EventSignal<SIGTERM> &) override
        {
            LOG(NEXO_INFO, "Received terminate signal");
            m_isRunning = false;
        }

        /**
         * @brief Handles interrupt signal events
         * @param event The interrupt event to process
         * Logs reception and sets application running state to false
         */
        void handleEvent(event::EventSignalInterrupt &) override
        {
            LOG(NEXO_INFO, "Received interrupt signal");
            m_isRunning = false;
        }

        /**
         * @brief Gets the event manager instance
         * @return Shared pointer to the event manager
         */
        [[nodiscard]] std::shared_ptr<event::EventManager> getEventManager() const
        {
            return m_eventManager;
        }

        /**
         * @brief Sets debug flags for event logging
         * @param flags Bitmask of debug flags to set
         */
        void setEventDebugFlags(const int flags)
        {
            m_eventDebugFlags = flags;
        }

        /**
         * @brief Removes specified debug flags
         * @param flag Debug flag to remove
         */
        void removeEventDebugFlags(const int flag)
        {
            m_eventDebugFlags &= flag;
        }

        /**
         * @brief Adds a debug flag to the current flags
         * @param flag Debug flag to add
         */
        void addEventDebugFlag(const int flag)
        {
            m_eventDebugFlags |= flag;
        }

        /**
         * @brief Resets all debug flags to 0
         */
        void resetEventDebugFlags()
        {
            m_eventDebugFlags = 0;
        }

        /**
         * @brief Checks if application is running
         * @return True if application is running, false otherwise
         */
        [[nodiscard]] bool isRunning() const
        {
            return m_isRunning;
        }

        /**
         * @brief Creates a new entity.
         *
         * Delegates the creation to the ECS coordinator.
         *
         * @return ecs::Entity The newly created entity.
         */
        static ecs::Entity createEntity();

        /**
         * @brief Gets the camera context system.
         *
         * @return std::shared_ptr<system::CameraContextSystem> Shared pointer to the camera context system.
         */
        [[nodiscard]] std::shared_ptr<system::CameraContextSystem> getCameraContextSystem() const
        {
            return m_cameraContextSystem;
        }

        /**
         * @brief Gets the light system.
         *
         * @return std::shared_ptr<system::LightSystem> Shared pointer to the light system.
         */
        [[nodiscard]] std::shared_ptr<system::PhysicsSystem> getPhysicsSystem() const
        {
            return m_physicsSystem;
        }

        /**
         * @brief Deletes an existing entity.
         *
         * If the entity has a SceneTag component, it is first removed from the corresponding scene,
         * and then destroyed by the ECS coordinator.
         *
         * @param entity The entity to delete.
         */
        void deleteEntity(ecs::Entity entity);

        /**
         * @brief Removes the entity from its parent in the hierarchy
         * @param entity The entity to remove from its parent
         */
        static void removeEntityFromParent(ecs::Entity entity);

        /**
         * @brief Recursively deletes all children of the specified entity
         * @param entity The parent entity whose children should be deleted
         */
        void deleteEntityChildren(ecs::Entity entity);

        /**
         * @brief Gets the singleton instance of the Application class
         * @return Reference to the Application instance
         * Creates the instance if it doesn't exist yet
         */
        static Application &getInstance()
        {
            if (!_instance) _instance.reset(new Application());
            return *_instance;
        }

        /**
         * @brief Sets a custom derived Application instance as the singleton
         * @tparam DerivedApp Type of the derived Application class
         * @tparam Args Parameter pack for constructor arguments
         * @param args Constructor arguments for the derived class
         */
        template<typename DerivedApp, typename... Args>
        static void setInstance(Args &&...args)
        {
            _instance = std::make_unique<DerivedApp>(std::forward<Args>(args)...);
        }

        /**
         * @brief Gets a component of type T for the specified entity
         * @tparam T Type of component to retrieve
         * @param entity Entity to get the component from
         * @return Reference to the component
         */
        template<typename T>
        static T &getEntityComponent(const ecs::Entity entity)
        {
            return m_coordinator->getComponent<T>(entity);
        }

        /**
         * @brief Gets all component types attached to the specified entity
         * @param entity Entity to get components from
         * @return Vector of component types
         */
        static std::vector<ecs::ComponentType> getAllEntityComponentTypes(const ecs::Entity entity)
        {
            return m_coordinator->getAllComponentTypes(entity);
        }

        /**
         * @brief Gets the SceneManager instance
         * @return Reference to the SceneManager
         */
        scene::SceneManager &getSceneManager()
        {
            return m_SceneManager;
        }

        /**
         * @brief Gets the window instance
         * @return Const reference to the shared window pointer
         */
        [[nodiscard]] const std::shared_ptr<renderer::NxWindow> &getWindow() const
        {
            return m_window;
        }

        /**
         * @brief Checks if the application window is open
         * @return True if window is open, false otherwise
         */
        [[nodiscard]] bool isWindowOpen() const
        {
            return m_window->isOpen();
        }

        /**
         * @brief Gets the current world state
         * @return Reference to the WorldState instance
         */
        [[nodiscard]] WorldState &getWorldState()
        {
            return m_worldState;
        }

        /**
         * @brief Initializes the scripting subsystem
         * @return Status code indicating success (0) or failure (non-zero)
         */
        [[nodiscard]] int initScripting() const;

        /**
         * @brief Shuts down the scripting subsystem
         * @return Status code indicating success (0) or failure (non-zero)
         */
        [[nodiscard]] int shutdownScripting() const;

        /**
         * @brief Gets the current game state
         * @return Current GameState enum value
         */
        [[nodiscard]] GameState getGameState() const
        {
            return m_gameState;
        }

        /**
         * @brief Sets the game state
         * @param state New GameState to set
         * Resets video system when entering play mode
         */
        void setGameState(GameState state)
        {
            m_gameState = state;
            if (state == GameState::PLAY_MODE) {
                m_renderVideoSystem->reset();
            }
        }

        /**
         * @brief Checks if the game is in play mode
         * @return True if in play mode, false otherwise
         */
        [[nodiscard]] bool isInPlayMode() const
        {
            return m_gameState == GameState::PLAY_MODE;
        }

        /**
         * @brief Checks if the game is in editor mode
         * @return True if in editor mode, false otherwise
         */
        [[nodiscard]] bool isInEditorMode() const
        {
            return m_gameState == GameState::EDITOR_MODE;
        }
        static std::shared_ptr<ecs::Coordinator> m_coordinator;

       protected:
        Application();
        std::shared_ptr<event::EventManager> m_eventManager;

       private:
        /**
         * @brief Registers all debug listeners for the application.
         *
         * This function sets up listeners for debugging purposes, allowing the application
         * to handle and log various debug events.
         */
        void registerAllDebugListeners();

        /**
         * @brief Registers signal listeners for the application.
         *
         * This function sets up listeners to handle system signals such as termination
         * or interruption, ensuring the application can respond appropriately.
         */
        void registerSignalListeners();

        /**
         * @brief Registers all ECS components with the coordinator.
         *
         * This static function initializes and registers all necessary components
         * for the Entity Component System (ECS) used in the application.
         */
        static void registerEcsComponents();

        /**
         * @brief Registers window-related callbacks.
         *
         * This function sets up callbacks for window events such as resizing,
         * closing, and input handling.
         */
        void registerWindowCallbacks() const;

        /**
         * @brief Registers all systems used in the application.
         *
         * This function initializes and registers all systems, such as rendering,
         * physics, and scripting, with the ECS coordinator.
         */
        void registerSystems();

        /**
         * @brief Initializes the scripting system.
         *
         * This function sets up the scripting environment, allowing the application
         * to execute scripts and interact with the scripting API.
         */
        void initScripting();

        /**
         * @brief Displays profiling results.
         *
         * This function outputs the profiling results collected during the application's
         * execution, providing insights into performance metrics.
         */
        void displayProfileResults() const;

        static std::unique_ptr<Application> _instance;

        scene::SceneId m_nextSceneId = 0;
        scene::SceneManager m_SceneManager;

        bool m_isRunning            = true;
        bool m_isMinimized          = false;
        bool m_displayProfileResult = true;
        std::shared_ptr<renderer::NxWindow> m_window;

        WorldState m_worldState;
        GameState m_gameState = GameState::EDITOR_MODE;

        int m_eventDebugFlags{};

        std::shared_ptr<system::CameraContextSystem> m_cameraContextSystem;
        std::shared_ptr<system::LightSystem> m_lightSystem;
        std::shared_ptr<system::TransformMatrixSystem> m_transformMatrixSystem;
        std::shared_ptr<system::TransformHierarchySystem> m_transformHierarchySystem;
        std::shared_ptr<system::PerspectiveCameraControllerSystem> m_perspectiveCameraControllerSystem;
        std::shared_ptr<system::PerspectiveCameraTargetSystem> m_perspectiveCameraTargetSystem;
        std::shared_ptr<system::ScriptingSystem> m_scriptingSystem;
        std::shared_ptr<system::RenderCommandSystem> m_renderCommandSystem;
        std::shared_ptr<system::RenderBillboardSystem> m_renderBillboardSystem;
        std::shared_ptr<system::RenderVideoSystem> m_renderVideoSystem;
        std::shared_ptr<system::PhysicsSystem> m_physicsSystem;

        std::vector<ProfileResult> m_profilesResults;
    };
} // namespace nexo
