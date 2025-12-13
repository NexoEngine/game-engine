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
#include "components/Render.hpp"
#include "components/Uuid.hpp"
#include "components/Editor.hpp"
#include "components/Name.hpp"
#include "systems/ScriptingSystem.hpp"
#include "renderer/RendererExceptions.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Renderer3D.hpp"
#include "systems/TransformSystem.hpp"

#include <memory>

namespace nexo {
    void Application::registerAllDebugListeners()
    {
        m_eventManager->registerListener<event::EventKey>(this);
        m_eventManager->registerListener<event::EventWindowResize>(this);
        m_eventManager->registerListener<event::EventWindowClose>(this);
        m_eventManager->registerListener<event::EventMouseClick>(this);
        m_eventManager->registerListener<event::EventMouseScroll>(this);
        m_eventManager->registerListener<event::EventMouseMove>(this);
        LOG(NEXO_DEV, "Debug listeners registered");
    }

    void Application::registerSignalListeners()
    {
        m_eventManager->registerListener<event::EventAnySignal>(this);
        m_eventManager->registerListener<event::EventSignalTerminate>(this);
        m_eventManager->registerListener<event::EventSignalInterrupt>(this);
        LOG(NEXO_DEV, "Signal listeners registered");
    }

    void Application::registerEcsComponents()
    {
        m_coordinator->registerComponent<components::TransformComponent>();
        m_coordinator->registerComponent<components::RootComponent>();
        m_coordinator->registerComponent<components::RenderComponent>();
        m_coordinator->setRestoreComponent<components::RenderComponent>();
        m_coordinator->registerComponent<components::SceneTag>();
        m_coordinator->setRestoreComponent<components::SceneTag>();
        m_coordinator->registerComponent<components::CameraComponent>();
        m_coordinator->setRestoreComponent<components::CameraComponent>();
        m_coordinator->registerComponent<components::AmbientLightComponent>();
        m_coordinator->setRestoreComponent<components::AmbientLightComponent>();
        m_coordinator->registerComponent<components::PointLightComponent>();
        m_coordinator->setRestoreComponent<components::PointLightComponent>();
        m_coordinator->registerComponent<components::DirectionalLightComponent>();
        m_coordinator->setRestoreComponent<components::DirectionalLightComponent>();
        m_coordinator->registerComponent<components::SpotLightComponent>();
        m_coordinator->setRestoreComponent<components::SpotLightComponent>();
        m_coordinator->registerComponent<components::UuidComponent>();
        m_coordinator->setRestoreComponent<components::UuidComponent>();
        m_coordinator->registerComponent<components::PerspectiveCameraController>("Perspective Camera Controller");
        m_coordinator->setRestoreComponent<components::PerspectiveCameraController>();
        m_coordinator->registerComponent<components::PerspectiveCameraTarget>("Perspective Camera Target");
        m_coordinator->setRestoreComponent<components::PerspectiveCameraTarget>();
        m_coordinator->registerComponent<components::EditorCameraTag>();
        m_coordinator->setRestoreComponent<components::EditorCameraTag>();
        m_coordinator->registerComponent<components::SelectedTag>();
        m_coordinator->registerComponent<components::StaticMeshComponent>();
        m_coordinator->registerComponent<components::ParentComponent>();
        m_coordinator->registerComponent<components::ModelComponent>();
        m_coordinator->registerComponent<components::BillboardComponent>();
        m_coordinator->registerComponent<components::VideoComponent>("Video");
        m_coordinator->registerComponent<components::MaterialComponent>("Material");
        m_coordinator->registerComponent<components::NameComponent>();
        m_coordinator->registerSingletonComponent<components::RenderContext>();
        m_coordinator->registerComponent<components::PhysicsBodyComponent>("Physic Body");
        m_coordinator->registerComponent<components::EditorTag>();
    }

    void Application::registerWindowCallbacks() const
    {
        m_window->setResizeCallback([this](const int width, const int height) {
            m_eventManager->emitEvent<event::EventWindowResize>(
                std::make_shared<event::EventWindowResize>(width, height));
        });

        m_window->setCloseCallback([this]() {
            m_eventManager->emitEvent<event::EventWindowClose>(std::make_shared<event::EventWindowClose>());
        });

        m_window->setKeyCallback([this](const int key, const int action, const int mods) {
            event::EventKey eventKey;
            eventKey.keycode = key;
            eventKey.mods    = mods;
            switch (action) {
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
                default:
                    return;
            }
            m_eventManager->emitEvent<event::EventKey>(std::make_shared<event::EventKey>(eventKey));
        });

        m_window->setKeyCallback([this](const int key, const int action, const int mods) {
            event::EventKey eventKey;
            eventKey.keycode = key;
            eventKey.mods    = mods;
            switch (action) {
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
                default:
                    return;
            }
            m_eventManager->emitEvent<event::EventKey>(std::make_shared<event::EventKey>(eventKey));
        });

        m_window->setMouseClickCallback([this](const int button, const int action, const int mods) {
            event::EventMouseClick event;
            event.button = static_cast<nexo::event::MouseButton>(button);
            event.mods   = mods;
            switch (action) {
                case GLFW_PRESS: {
                    event.action = event::KeyAction::PRESSED;
                    break;
                }
                case GLFW_RELEASE:
                    event.action = event::KeyAction::RELEASED;
                    break;
                default:
                    return;
            }
            m_eventManager->emitEvent<event::EventMouseClick>(std::make_shared<event::EventMouseClick>(event));
        });

        m_window->setMouseScrollCallback([this](const double xOffset, const double yOffset) {
            m_eventManager->emitEvent<event::EventMouseScroll>(
                std::make_shared<event::EventMouseScroll>(static_cast<float>(xOffset), static_cast<float>(yOffset)));
        });

        m_window->setMouseMoveCallback([this](const double xpos, const double ypos) {
            m_eventManager->emitEvent<event::EventMouseMove>(
                std::make_shared<event::EventMouseMove>(static_cast<float>(xpos), static_cast<float>(ypos)));
        });

        m_window->setFileDropCallback([this](const int count, const char **paths) {
            std::vector<std::string> files;
            files.reserve(count);
            for (int i = 0; i < count; ++i) {
                files.emplace_back(paths[i]);
            }
            m_eventManager->emitEvent<event::EventFileDrop>(std::make_shared<event::EventFileDrop>(files));
        });
    }

    void Application::registerSystems()
    {
        m_cameraContextSystem = m_coordinator->registerGroupSystem<system::CameraContextSystem>();
        m_perspectiveCameraControllerSystem =
            m_coordinator->registerQuerySystem<system::PerspectiveCameraControllerSystem>();
        m_perspectiveCameraTargetSystem = m_coordinator->registerQuerySystem<system::PerspectiveCameraTargetSystem>();
        m_renderCommandSystem           = m_coordinator->registerGroupSystem<system::RenderCommandSystem>();
        m_renderBillboardSystem         = m_coordinator->registerGroupSystem<system::RenderBillboardSystem>();
        m_renderVideoSystem             = m_coordinator->registerGroupSystem<system::RenderVideoSystem>();
        m_physicsSystem                 = m_coordinator->registerQuerySystem<system::PhysicsSystem>();
        m_physicsSystem->init();

        auto pointLightSystem       = m_coordinator->registerGroupSystem<system::PointLightsSystem>();
        auto directionalLightSystem = m_coordinator->registerGroupSystem<system::DirectionalLightsSystem>();
        auto spotLightSystem        = m_coordinator->registerGroupSystem<system::SpotLightsSystem>();
        auto ambientLightSystem     = m_coordinator->registerGroupSystem<system::AmbientLightSystem>();
        m_lightSystem               = std::make_shared<system::LightSystem>(ambientLightSystem, directionalLightSystem,
                                                              pointLightSystem, spotLightSystem);

        m_scriptingSystem = std::make_shared<system::ScriptingSystem>();
        m_aabbdebugSystem = m_coordinator->registerQuerySystem<system::AABBDebugSystem>();
        m_transformSystem = m_coordinator->registerGroupSystem<system::TransformSystem>();
    }

    int Application::initScripting() const
    {
        return m_scriptingSystem->init();
    }

    void Application::init()
    {
        event::Input::init(m_window);
        event::SignalHandler::getInstance()->registerEventManager(m_eventManager);

        // Window and glad init
        m_window->init();
        registerWindowCallbacks();
        m_window->setVsync(false);
        m_window->setDarkMode(true);

#ifdef NX_GRAPHICS_API_OPENGL
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
            THROW_EXCEPTION(renderer::NxGraphicsApiInitFailure, "Failed to initialize OpenGL context with glad");
        }
        LOG(NEXO_INFO, "OpenGL context initialized with glad");
        glViewport(0, 0, static_cast<int>(m_window->getWidth()), static_cast<int>(m_window->getHeight()));
#endif

        renderer::NxRenderer::init();

        m_coordinator->init();
        registerEcsComponents();
        renderer::NxRenderer3D::get().init();
        registerSystems();
        m_SceneManager.setCoordinator(m_coordinator);

        LOG(NEXO_DEV, "Application initialized");
    }
}
