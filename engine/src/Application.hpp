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
#include "core/event/SignalEvent.hpp"
#include "renderer/Buffer.hpp"
#include "renderer/Renderer.hpp"
#include "ecs/Coordinator.hpp"
#include "core/scene/SceneManager.hpp"
#include "core/scene/NewSceneManager.hpp"
#include "Logger.hpp"
#include "Timer.hpp"
#include "components/Light.hpp"

#include "systems/OnSceneDeletedSystem.hpp"
#include "systems/CameraSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/LightSystem.hpp"

#define NEXO_PROFILE(name) nexo::Timer timer##__LINE__(name, [&](ProfileResult profileResult) {m_profileResults.push_back(profileResult); })

namespace nexo {

    enum class RenderingType {
        WINDOW,
        FRAMEBUFFER
    };

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

            void run(scene::SceneId sceneId, RenderingType renderingType);

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

            ecs::Entity createEntity() const;
            void destroyEntity(ecs::Entity entity);

            scene::SceneId createScene(const std::string &sceneName, bool active = true);
            void deleteScene(scene::SceneId sceneId);
            scene::LayerId addNewLayer(scene::SceneId sceneId, const std::string &layerName = "Default Layer");
            scene::LayerId addNewOverlay(scene::SceneId sceneId, const std::string &overlayName = "Default Overlay");
            void removeLayer(scene::SceneId sceneId, scene::LayerId id);
            void removeOverlay(scene::SceneId sceneId, scene::LayerId id);
            void activateScene(scene::SceneId sceneId);
            void activateLayer(scene::SceneId, scene::LayerId id);
            void deactivateScene(scene::SceneId sceneId);
            void deactivateLayer(scene::SceneId sceneId, scene::LayerId id);
            void setSceneRenderStatus(scene::SceneId sceneId, bool status);
            void setLayerRenderStatus(scene::SceneId sceneId, scene::LayerId id, bool status);
            bool isSceneActive(const scene::SceneId sceneId) { return m_sceneManager.isSceneActive(sceneId); };
            bool isSceneRendered(const scene::SceneId sceneId) { return m_sceneManager.isSceneRendered(sceneId); };
            void addEntityToScene(ecs::Entity entity, scene::SceneId sceneId, int layerId = -1);
            void removeEntityFromScene(ecs::Entity entity, scene::SceneId sceneId, int layerId = -1);
            void attachCamera(scene::SceneId sceneId, const std::shared_ptr<camera::Camera> &camera, scene::LayerId id);
            void detachCamera(scene::SceneId sceneId, scene::LayerId id);
            std::shared_ptr<camera::Camera> getCamera(scene::SceneId sceneId, scene::LayerId id);
            unsigned int addLightToScene(scene::SceneId sceneId, const std::shared_ptr<components::Light> &light);
            std::shared_ptr<components::Light> getLight(scene::SceneId sceneId, unsigned int index);
            unsigned int getLightCount(scene::SceneId sceneId);
            void removeLightFromScene(scene::SceneId sceneId, unsigned int index);
            void setAmbientLightValue(scene::SceneId sceneId, glm::vec3 value);
            glm::vec3 getAmbientLightValue(scene::SceneId sceneId);


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

            static std::vector<std::type_index> getAllEntityComponentTypes(const ecs::Entity entity)
            {
                return m_coordinator->getAllComponentTypes(entity);
            }

            static std::vector<std::pair<std::type_index, std::any>> getAllEntityComponents(const ecs::Entity entity)
            {
                return m_coordinator->getAllComponents(entity);
            }

            scene::SceneManager &getSceneManager() { return m_sceneManager; };
            scene::NewSceneManager &getNewSceneManager() { return m_newSceneManager; }

            [[nodiscard]] const std::shared_ptr<renderer::Window> &getWindow() const { return m_window; };
            [[nodiscard]] bool isWindowOpen() const { return m_window->isOpen(); };


            void genAssetPreview(ecs::Entity entity);

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
            scene::SceneManager m_sceneManager;
            scene::NewSceneManager m_newSceneManager;

            bool m_isRunning = true;
            bool m_isMinimized = false;
            bool m_displayProfileResult = true;
            std::shared_ptr<renderer::Window> m_window;

            float m_lastFrameTime = 0.0f;

            int m_eventDebugFlags{};

            std::shared_ptr<system::OnSceneDeleted> m_onSceneDeleteSystem;
            std::shared_ptr<system::CameraContextSystem> m_cameraContextSystem;
            std::shared_ptr<system::RenderSystem> m_renderSystem;
            std::shared_ptr<system::LightSystem> m_lightSystem;

            std::vector<ProfileResult> m_profilesResults;
    };
}
