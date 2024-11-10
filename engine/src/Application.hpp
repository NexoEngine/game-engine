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

#include "renderer/Window.hpp"
#include "core/event/WindowEvent.hpp"
#include "renderer/Buffer.hpp"
#include "renderer/Renderer.hpp"
#include "ecs/Coordinator.hpp"
#include "core/scene/SceneManager.hpp"

namespace nexo {

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
        event::EventMouseMove
    ) {
        public:
            ~Application() override = default;

            virtual int init() { return 1; };
            virtual void run();

            void addScene(scene::Scene &scene);
            void removeScene(scene::Scene &scene);
            void switchScene(scene::Scene &scene);

            [[nodiscard]] bool isWindowOpen() const { return m_window->isOpen(); };

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

            void registerAllDebugListeners();

            void setEventDebugFlags(const int flags) {m_eventDebugFlags = flags; };
            void removeEventDebugFlags(const int flag) {m_eventDebugFlags &= flag; };
            void addEventDebugFlag(const int flag) {m_eventDebugFlags |= flag; };
            void resetEventDebugFlags() {m_eventDebugFlags = 0; };

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

            [[nodiscard]] std::shared_ptr<event::EventManager> getEventManager() const { return m_eventManager; };

            [[nodiscard]] const std::shared_ptr<renderer::Window> &getWindow() const { return m_window; };

            scene::SceneManager &getSceneManager() { return m_sceneManager; };

        protected:
            Application();
            std::shared_ptr<event::EventManager> m_eventManager;

        private:
            static std::unique_ptr<Application> _instance;

            bool m_isRunning = true;
            bool m_isMinimized = false;
            std::shared_ptr<renderer::Window> m_window;
            std::shared_ptr<ecs::Coordinator> m_coordinator;
            scene::SceneManager m_sceneManager;

            std::vector<scene::Scene> m_scenes;
            int m_actualScene = -1;

            float m_lastFrameTime = 0.0f;

            int m_eventDebugFlags{};
    };
}
