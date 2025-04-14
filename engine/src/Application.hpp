//// Application.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the main application
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#include <memory>
#include <vector>
#include <glad/glad.h>
#include <systems/PhysicsSystemWrapper.hpp>

#include "Types.hpp"
#include "renderer/Window.hpp"
#include "core/event/WindowEvent.hpp"
#include "core/event/SignalEvent.hpp"
#include "ecs/Coordinator.hpp"
#include "core/scene/SceneManager.hpp"
#include "Logger.hpp"
#include "Timer.hpp"
#include "WorldState.hpp"
#include "components/Light.hpp"
#include "components/PhysicsBodyComponent.hpp"

#include "systems/CameraSystem.hpp"
#include "systems/LightSystem.hpp"
#include "systems/PhysicsSystem.hpp"
#include "systems/RenderCommandSystem.hpp"
#include "systems/RenderBillboardSystem.hpp"
#include "systems/TransformHierarchySystem.hpp"
#include "systems/TransformMatrixSystem.hpp"

#define NEXO_PROFILE(name) nexo::Timer timer##__LINE__(name, [&](ProfileResult profileResult) {m_profileResults.push_back(profileResult); })

namespace nexo {

    namespace system {
        class ScriptingSystem;
    }

    enum EventDebugFlags {
        DEBUG_LOG_RESIZE_EVENT = 1 << 0,
        DEBUG_LOG_KEYBOARD_EVENT = 1 << 1,
        DEBUG_LOG_MOUSE_CLICK_EVENT = 1 << 2,
        DEBUG_LOG_MOUSE_SCROLL_EVENT = 1 << 3,
        DEBUG_LOG_MOUSE_MOVE_EVENT = 1 << 4
    };

    class Application : LISTENS_TO(
        event::EventKey,
        event::EventWindowClose,
        event::EventWindowResize,
        event::EventMouseClick,
        event::EventMouseScroll,
        event::EventMouseMove,
        event::EventAnySignal,
        event::EventSignal<SIGTERM>,
        event::EventSignalInterrupt
    ) {
        public:
            ~Application() override = default;
            Application(const Application&) = delete;
            Application& operator=(const Application&) = delete;
            Application(Application&&) = delete;
            Application& operator=(Application&&) = delete;

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
                SceneType sceneType = SceneType::GAME;
                bool isChildWindow = false; //<< Is the current scene embedded in a sub window ?
                glm::vec2 viewportBounds[2]{}; //<< Viewport bounds in absolute coordinates (if the window viewport is embedded in the window), this is used for mouse coordinates
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
             * @param sceneId The ID of the scene to render.
             * @param renderingType The rendering mode (e.g., WINDOW or other types).
             * @param sceneType The type of scene to render.
             */
            void run(const SceneInfo &sceneInfo);

            /**
             * @brief Ends the current frame by clearing processed events.
             *
             * Clears all the events that have been dispatched during the frame,
             * preparing the EventManager for the next frame.
             */
            void endFrame();

            void handleEvent(event::EventKey &event) override
            {
                if (this->m_eventDebugFlags & DEBUG_LOG_KEYBOARD_EVENT)
                    std::cout << event << std::endl;
            }

            void handleEvent([[maybe_unused]] event::EventWindowClose &event) override
            {
                m_isRunning = false;
            }

            void handleEvent(event::EventWindowResize &event) override
            {
                if (event.height == 0 || event.width == 0)
                    m_isMinimized = true;
                if (m_isMinimized && event.width != 0 && event.height != 0)
                    m_isMinimized = false;
                if (this->m_eventDebugFlags & DEBUG_LOG_RESIZE_EVENT)
                    std::cout << event << std::endl;
            }

            void handleEvent(event::EventMouseClick &event) override
            {
                if (this->m_eventDebugFlags & DEBUG_LOG_MOUSE_CLICK_EVENT)
                    std::cout << event << std::endl;
            }

            void handleEvent(event::EventMouseScroll &event) override
            {
                if (this->m_eventDebugFlags & DEBUG_LOG_MOUSE_SCROLL_EVENT)
                    std::cout << event << std::endl;
            }

            void handleEvent(event::EventMouseMove &event) override
            {
                if (this->m_eventDebugFlags & DEBUG_LOG_MOUSE_MOVE_EVENT)
                    std::cout << event << std::endl;
            }

            void handleEvent(event::EventAnySignal &event) override
            {
                LOG(NEXO_INFO, "Received signal via {}", event);
            }

            void handleEvent(event::EventSignal<SIGTERM>&) override
            {
                LOG(NEXO_INFO, "Received terminate signal");
                m_isRunning = false;
            }

            void handleEvent(event::EventSignalInterrupt&) override
            {
                LOG(NEXO_INFO, "Received interrupt signal");
                m_isRunning = false;
            }

            [[nodiscard]] std::shared_ptr<event::EventManager> getEventManager() const { return m_eventManager; };
            void setEventDebugFlags(const int flags) {m_eventDebugFlags = flags; };
            void removeEventDebugFlags(const int flag) {m_eventDebugFlags &= flag; };
            void addEventDebugFlag(const int flag) {m_eventDebugFlags |= flag; };
            void resetEventDebugFlags() {m_eventDebugFlags = 0; };

            bool isRunning() const { return m_isRunning; };

            /**
             * @brief Creates a new entity.
             *
             * Delegates the creation to the ECS coordinator.
             *
             * @return ecs::Entity The newly created entity.
             */
            ecs::Entity createEntity() const;

            system::PhysicsSystemWrapper& getPhysicsSystem() { return physicsSystem; }
            /**
             * @brief Deletes an existing entity.
             *
             * If the entity has a SceneTag component, it is first removed from the corresponding scene,
             * and then destroyed by the ECS coordinator.
             *
             * @param entity The entity to delete.
             */
            void deleteEntity(ecs::Entity entity);
            void removeEntityFromParent(const ecs::Entity entity) const;
            void deleteEntityChildren(const ecs::Entity entity);

            static Application &getInstance()
            {
                if (!_instance)
                    _instance.reset(new Application());
                return *_instance;
            }

            template<typename DerivedApp, typename... Args>
            static void setInstance(Args &&... args)
            {
                _instance = std::make_unique<DerivedApp>(std::forward<Args>(args)...);
            }

            template<typename T>
            static T &getEntityComponent(const ecs::Entity entity)
            {
                return m_coordinator->getComponent<T>(entity);
            }

            static std::vector<ecs::ComponentType> getAllEntityComponentTypes(const ecs::Entity entity)
            {
                return m_coordinator->getAllComponentTypes(entity);
            }

            scene::SceneManager &getSceneManager() { return m_SceneManager; }

            [[nodiscard]] const std::shared_ptr<renderer::NxWindow> &getWindow() const { return m_window; }
            [[nodiscard]] bool isWindowOpen() const { return m_window->isOpen(); }
            [[nodiscard]] WorldState &getWorldState() { return m_worldState; }

            int initScripting() const;
            int shutdownScripting() const;

            static std::shared_ptr<ecs::Coordinator> m_coordinator;
        protected:
            Application();
            std::shared_ptr<event::EventManager> m_eventManager;

        private:
            void registerAllDebugListeners();
            void registerSignalListeners();
            void registerEcsComponents() const;
            void registerWindowCallbacks() const;
            void registerSystems();

            void displayProfileResults() const;
            static std::unique_ptr<Application> _instance;

            scene::SceneId m_nextSceneId = 0;
            scene::SceneManager m_SceneManager;

            bool m_isRunning = true;
            bool m_isMinimized = false;
            bool m_displayProfileResult = true;
            std::shared_ptr<renderer::NxWindow> m_window;

            WorldState m_worldState;

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

            std::vector<ProfileResult> m_profilesResults;

            system::PhysicsSystemWrapper physicsSystem;

    };
}
