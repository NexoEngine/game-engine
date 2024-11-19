//// Application.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the main application
//
///////////////////////////////////////////////////////////////////////////////

#include "Application.hpp"

#include <glad/glad.h>

#include "components/Components.hpp"
#include "core/event/Input.hpp"

std::unique_ptr<nexo::Application> nexo::Application::_instance = nullptr;
std::shared_ptr<nexo::ecs::Coordinator> nexo::Application::m_coordinator = nullptr;
std::shared_ptr<nexo::ecs::Coordinator> nexo::ecs::System::coord = nullptr;

namespace nexo {

    void Application::registerAllDebugListeners()
    {
        m_eventManager->registerListener<event::EventKey>(this);
        m_eventManager->registerListener<event::EventWindowResize>(this);
        m_eventManager->registerListener<event::EventWindowClose>(this);
        m_eventManager->registerListener<event::EventMouseClick>(this);
        m_eventManager->registerListener<event::EventMouseScroll>(this);
        m_eventManager->registerListener<event::EventMouseMove>(this);
    }

    void Application::registerEcsComponents()
    {
        m_coordinator->registerComponent<components::TransformComponent>();
        m_coordinator->registerComponent<components::RenderComponent>();
        m_coordinator->registerComponent<components::InActiveScene>();
    }

    void Application::registerWindowCallbacks()
    {
        m_window->setResizeCallback([this](int width, int height) {
            auto event = event::EventWindowResize(width, height);
            m_eventManager->emitEvent<event::EventWindowResize>(std::make_shared<event::EventWindowResize>(event));
        });

        m_window->setCloseCallback([this]() {
            event::EventWindowClose event;
            m_eventManager->emitEvent<event::EventWindowClose>(std::make_shared<event::EventWindowClose>(event));
        });

        m_window->setKeyCallback([this](int key, int action, int mods) {
            event::EventKey eventKey;
            eventKey.keycode = key;
            eventKey.mods = mods;
            switch (action)
            {
                case GLFW_PRESS: {
                    eventKey.action = event::KeyAction::PRESSED;
                    break;
                }
                case GLFW_RELEASE: {
                    eventKey.action = event::KeyAction::RELEASED;
                    break;
                }
                case GLFW_REPEAT: {
                    eventKey.action = event::KeyAction::REPEAT;
                    break;
                }
                default: return;
            }
            m_eventManager->emitEvent<event::EventKey>(std::make_shared<event::EventKey>(eventKey));
        });

        m_window->setKeyCallback([this](int key, int action, int mods) {
            event::EventKey eventKey;
            eventKey.keycode = key;
            eventKey.mods = mods;
            switch (action)
            {
                case GLFW_PRESS: {
                    eventKey.action = event::KeyAction::PRESSED;
                    break;
                }
                case GLFW_RELEASE: {
                    eventKey.action = event::KeyAction::RELEASED;
                    break;
                }
                case GLFW_REPEAT: {
                    eventKey.action = event::KeyAction::REPEAT;
                    break;
                }
                default: return;
            }
            m_eventManager->emitEvent<event::EventKey>(std::make_shared<event::EventKey>(eventKey));
        });

        m_window->setMouseClickCallback([this](int button, int action, int mods) {
            event::EventMouseClick event;
            event.button = static_cast<nexo::event::MouseButton>(button);
            event.mods = mods;
            switch (action)
            {
                case GLFW_PRESS: {
                    event.action = event::KeyAction::PRESSED;
                    break;
                }
                case GLFW_RELEASE:
                    event.action = event::KeyAction::RELEASED;
                    break;
                default: return;
            }
            m_eventManager->emitEvent<event::EventMouseClick>(std::make_shared<event::EventMouseClick>(event));
        });

        m_window->setMouseScrollCallback([this](double xOffset, double yOffset) {
            event::EventMouseScroll event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            m_eventManager->emitEvent<event::EventMouseScroll>(std::make_shared<event::EventMouseScroll>(event));
        });

        m_window->setMouseMoveCallback([this](double xpos, double ypos) {
            event::EventMouseMove event(xpos, ypos);
            m_eventManager->emitEvent<event::EventMouseMove>(std::make_shared<event::EventMouseMove>(event));
        });
    }

    void Application::registerSystems()
    {
        m_onSceneDeleteSystem = m_coordinator->registerSystem<system::OnSceneDeleted>();
        ecs::Signature signatureOnSceneDelete;
        signatureOnSceneDelete.set(m_coordinator->getComponentType<components::InActiveScene>());
        m_coordinator->setSystemSignature<system::OnSceneDeleted>(signatureOnSceneDelete);
    }

    Application::Application()
    {
        m_window = renderer::Window::create();
        m_eventManager = std::make_shared<event::EventManager>();
        registerAllDebugListeners();
        LOG(NEXO_DEV, "Debug listeners registered");
        event::Input::init(m_window);

        // Window and glad init
        m_window->init();
        registerWindowCallbacks();
        m_window->setVsync(false);

#ifdef GRAPHICS_API_OPENGL
        gladLoadGL();
        glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
#endif

        renderer::Renderer::init();

        // Debug flags
        //m_eventDebugFlags |= DEBUG_LOG_KEYBOARD_EVENT;

        m_coordinator = std::make_shared<ecs::Coordinator>();
        m_coordinator->init();
        ecs::System::coord = m_coordinator;
        registerEcsComponents();
        registerSystems();
        m_sceneManager.setCoordinator(m_coordinator);
    }

    void Application::run(scene::SceneId sceneId, const RenderingType renderingType)
    {
        const auto time = static_cast<float>(glfwGetTime());
        const core::Timestep timestep = time - m_lastFrameTime;
        m_lastFrameTime = time;
        auto scenesIds = m_sceneManager.getSceneIDs();

        if (!m_isMinimized)
        {
            // Clear
             renderer::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
            renderer::RenderCommand::clear();

            if (m_sceneManager.isSceneActive(sceneId))
                m_sceneManager.getScene(sceneId).onUpdate(timestep);
            if (m_sceneManager.isSceneRendered(sceneId))
                m_sceneManager.getScene(sceneId).onRender();
        }

        // Update (swap buffers and poll events)
        if (renderingType == RenderingType::WINDOW)
            m_window->onUpdate();
        // Dispatch events to active scenes
        m_eventManager->dispatchEvents(m_sceneManager.getScene(sceneId), m_sceneManager.isSceneActive(sceneId));
    }

    ecs::Entity Application::createEntity()
    {
        return m_coordinator->createEntity();
    }

    void Application::destroyEntity(const ecs::Entity entity)
    {
        LOG(NEXO_DEV, "Entity {} destroyed", entity);
        m_coordinator->destroyEntity(entity);
        m_sceneManager.entityDestroyed(entity);
    }

    scene::SceneId Application::createScene(const std::string &sceneName, const bool active)
    {
        return m_sceneManager.createScene(sceneName, active);
    }

    void Application::deleteScene(const scene::SceneId sceneId)
    {
        m_sceneManager.deleteScene(sceneId);
        m_onSceneDeleteSystem->onSceneDelete(sceneId);
    }

    scene::LayerId Application::addNewLayer(const scene::SceneId sceneId, const std::string &layerName)
    {
        return m_sceneManager.addLayer(sceneId, layerName);
    }

    scene::LayerId Application::addNewOverlay(scene::SceneId sceneId, const std::string &overlayName)
    {
        return m_sceneManager.addOverlay(sceneId, overlayName);
    }

    void Application::removeLayer(scene::SceneId sceneId, scene::LayerId id)
    {
        std::set<ecs::Entity> layerEntities = m_sceneManager.getLayerEntities(sceneId, id);
        for (auto entity : layerEntities)
            removeEntityFromScene(entity, sceneId, id);
        m_sceneManager.removeLayer(sceneId, id);
    }

    void Application::removeOverlay(scene::SceneId sceneId, scene::LayerId id)
    {
        std::set<ecs::Entity> layerEntities = m_sceneManager.getLayerEntities(sceneId, id);
        for (auto entity : layerEntities)
            removeEntityFromScene(entity, sceneId, id);
        m_sceneManager.removeOverlay(sceneId, id);
    }

    void Application::activateScene(scene::SceneId sceneId)
    {
        if (m_sceneManager.isSceneActive(sceneId))
            return;
        m_sceneManager.setSceneActiveStatus(sceneId, true);
        auto sceneEntities = m_sceneManager.getAllSceneEntities(sceneId);
        for (auto entity : sceneEntities)
        {
            auto activeSceneComponent = m_coordinator->tryGetComponent<components::InActiveScene>(entity);
            if (activeSceneComponent)
            {
                activeSceneComponent->get().sceneIds.insert(sceneId);
            }
            else
            {
                components::InActiveScene newActiveSceneComponent;
                newActiveSceneComponent.sceneIds.insert(sceneId);
                m_coordinator->addComponent<components::InActiveScene>(entity, newActiveSceneComponent);
            }
        }
    }

    void Application::activateLayer(scene::SceneId sceneId, scene::LayerId id)
    {
        if (m_sceneManager.isLayerActive(sceneId, id))
            return;
        m_sceneManager.setLayerActiveStatus(sceneId, id, true);
        auto layerEntities = m_sceneManager.getLayerEntities(sceneId, id);
        for (auto entity : layerEntities)
        {
            auto activeSceneComponent = m_coordinator->tryGetComponent<components::InActiveScene>(entity);
            if (activeSceneComponent)
            {
                activeSceneComponent->get().sceneIds.insert(sceneId);
            }
            else
            {
                components::InActiveScene newActiveSceneComponent;
                newActiveSceneComponent.sceneIds.insert(sceneId);
                m_coordinator->addComponent<components::InActiveScene>(entity, newActiveSceneComponent);
            }
        }
    }

    void Application::deactivateScene(scene::SceneId sceneId)
    {
        if (!m_sceneManager.isSceneActive(sceneId))
            return;
        m_sceneManager.setSceneActiveStatus(sceneId, false);
        auto sceneEntities = m_sceneManager.getAllSceneEntities(sceneId);
        for (auto entity : sceneEntities)
        {
            auto activeSceneComponent = m_coordinator->tryGetComponent<components::InActiveScene>(entity);
            if (activeSceneComponent)
            {
                activeSceneComponent->get().sceneIds.erase(sceneId);
                if (activeSceneComponent->get().sceneIds.empty())
                    m_coordinator->removeComponent<components::InActiveScene>(entity);
            }
        }
    }

    void Application::deactivateLayer(scene::SceneId sceneId, scene::LayerId id)
    {
        if (!m_sceneManager.isLayerActive(sceneId, id))
            return;
        m_sceneManager.setLayerActiveStatus(sceneId, id, false);
        auto layerEntities = m_sceneManager.getLayerEntities(sceneId, id);
        for (auto entity : layerEntities)
        {
            auto activeSceneComponent = m_coordinator->tryGetComponent<components::InActiveScene>(entity);
            if (activeSceneComponent)
            {
                activeSceneComponent->get().sceneIds.erase(sceneId);
                if (activeSceneComponent->get().sceneIds.empty())
                    m_coordinator->removeComponent<components::InActiveScene>(entity);
            }
        }
    }

    void Application::setSceneRenderStatus(scene::SceneId sceneId, bool status)
    {
        m_sceneManager.setSceneRenderStatus(sceneId, status);
    }

    void Application::setLayerRenderStatus(scene::SceneId sceneId, scene::LayerId id, bool status)
    {
        m_sceneManager.setLayerRenderStatus(sceneId, id, status);
    }

    void Application::addEntityToScene(ecs::Entity entity, scene::SceneId sceneId, int id)
    {
        LOG(NEXO_DEV, "Added entity {} to scene {}", entity, sceneId);
        if (id != -1)
        {
            m_sceneManager.addEntityToLayer(entity, sceneId, id);
            if (!m_sceneManager.isSceneActive(sceneId) || !m_sceneManager.isLayerActive(sceneId, id))
                return;
        }
        else
        {
            m_sceneManager.addGlobalEntity(entity, sceneId);
            if (!m_sceneManager.isSceneActive(sceneId))
                return;
        }
        auto activeSceneComponent = m_coordinator->tryGetComponent<components::InActiveScene>(entity);
        if (activeSceneComponent)
        {
            activeSceneComponent->get().sceneIds.insert(sceneId);
            return;
        }
        components::InActiveScene newActiveSceneComponent;
        newActiveSceneComponent.sceneIds.insert(sceneId);
        m_coordinator->addComponent<components::InActiveScene>(entity, newActiveSceneComponent);
    }

    void Application::removeEntityFromScene(ecs::Entity entity, scene::SceneId sceneId, int id)
    {
        LOG(NEXO_DEV, "Removed entity {} from scene {}", entity, sceneId);
        if (id != -1)
            m_sceneManager.removeEntityFromLayer(entity, sceneId, id);
        else
            m_sceneManager.removeGlobalEntity(entity, sceneId);
        if (!m_sceneManager.isSceneActive(sceneId))
            return;
        auto activeSceneComponent = m_coordinator->tryGetComponent<components::InActiveScene>(entity);
        if (activeSceneComponent)
        {
            activeSceneComponent->get().sceneIds.erase(sceneId);
            if (activeSceneComponent->get().sceneIds.empty())
                m_coordinator->removeComponent<components::InActiveScene>(entity);
        }
    }

    void Application::attachCamera(scene::SceneId sceneId, const std::shared_ptr<camera::Camera> &camera, scene::LayerId id)
    {
        m_sceneManager.attachCameraToLayer(sceneId, camera, id);
    }

    void Application::detachCamera(scene::SceneId sceneId, scene::LayerId id)
    {
        m_sceneManager.detachCameraFromLayer(sceneId, id);
    }

    std::shared_ptr<camera::Camera> Application::getCamera(scene::SceneId sceneId, scene::LayerId id)
    {
        return m_sceneManager.getCameraLayer(sceneId, id);
    }
}
