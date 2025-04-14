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

#include <core/event/SignalEvent.hpp>
#include <glad/glad.h>
#include <sys/types.h>

#include "components/Components.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/Transform.hpp"
#include "components/Editor.hpp"
#include "components/Uuid.hpp"
#include "core/event/Input.hpp"
#include "Timestep.hpp"
#include "renderer/RendererExceptions.hpp"
#include "systems/CameraSystem.hpp"
#include "systems/RenderSystem.hpp"
#include "systems/lights/DirectionalLightsSystem.hpp"
#include "systems/lights/PointLightsSystem.hpp"

std::unique_ptr<nexo::Application> nexo::Application::_instance = nullptr;
std::shared_ptr<nexo::ecs::Coordinator> nexo::Application::m_coordinator = nullptr;

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

    void Application::registerEcsComponents() const
    {
        m_coordinator->registerComponent<components::TransformComponent>();
        m_coordinator->registerComponent<components::RenderComponent>();
        m_coordinator->registerComponent<components::SceneTag>();
        m_coordinator->registerComponent<components::CameraComponent>();
        m_coordinator->registerComponent<components::AmbientLightComponent>();
        m_coordinator->registerComponent<components::PointLightComponent>();
        m_coordinator->registerComponent<components::DirectionalLightComponent>();
        m_coordinator->registerComponent<components::SpotLightComponent>();
        m_coordinator->registerComponent<components::UuidComponent>();
        m_coordinator->registerComponent<components::PerspectiveCameraController>();
        m_coordinator->registerComponent<components::PerspectiveCameraTarget>();
        m_coordinator->registerComponent<components::EditorCameraTag>();
        m_coordinator->registerComponent<components::SelectedTag>();
        m_coordinator->registerSingletonComponent<components::RenderContext>();

        m_coordinator->registerComponent<components::InActiveScene>();
        m_coordinator->registerComponent<components::PhysicsBodyComponent>();
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

        m_window->setKeyCallback([this](const int key, const int action, const int mods) {
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

        m_window->setMouseClickCallback([this](const int button, const int action, const int mods) {
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

        m_window->setMouseScrollCallback([this](const double xOffset, const double yOffset) {
            m_eventManager->emitEvent<event::EventMouseScroll>(
                std::make_shared<event::EventMouseScroll>(static_cast<float>(xOffset), static_cast<float>(yOffset)));
        });

        m_window->setMouseMoveCallback([this](const double xpos, const double ypos) {
            m_eventManager->emitEvent<event::EventMouseMove>(
                std::make_shared<event::EventMouseMove>(static_cast<float>(xpos), static_cast<float>(ypos)));
        });
    }

    void Application::registerSystems()
    {
        m_cameraContextSystem = m_coordinator->registerGroupSystem<system::CameraContextSystem>();
        m_perspectiveCameraControllerSystem = m_coordinator->registerQuerySystem<system::PerspectiveCameraControllerSystem>();
        m_perspectiveCameraTargetSystem = m_coordinator->registerQuerySystem<system::PerspectiveCameraTargetSystem>();

        m_renderSystem = m_coordinator->registerGroupSystem<system::RenderSystem>();

        auto pointLightSystem = m_coordinator->registerGroupSystem<system::PointLightsSystem>();
        auto directionalLightSystem = m_coordinator->registerGroupSystem<system::DirectionalLightsSystem>();
        auto spotLightSystem = m_coordinator->registerGroupSystem<system::SpotLightsSystem>();
        auto ambientLightSystem = m_coordinator->registerGroupSystem<system::AmbientLightSystem>();
        m_lightSystem = std::make_shared<system::LightSystem>(ambientLightSystem, directionalLightSystem, pointLightSystem, spotLightSystem);
    }

    Application::Application()
    {
        m_window = renderer::NxWindow::create();
        m_eventManager = std::make_shared<event::EventManager>();
        registerAllDebugListeners();
        registerSignalListeners();

        // Debug flags
        //m_eventDebugFlags |= DEBUG_LOG_KEYBOARD_EVENT;

        m_coordinator = std::make_shared<ecs::Coordinator>();

        ecs::System::coord = m_coordinator;

        LOG(NEXO_DEV, "Application created");
    }

    void Application::displayProfileResults() const
    {
        for (auto &result: m_profilesResults)
        {
            std::ostringstream stream;
            stream << std::fixed << std::setprecision(3) << result.time;
            std::string label = stream.str() + "ms" + " " + result.name;
            LOG(NEXO_DEV, "{}", label);
        }
    }

    void Application::init()
    {
        event::Input::init(m_window);
        event::SignalHandler::getInstance()->registerEventManager(m_eventManager);

        // Window and glad init
        m_window->init();
        registerWindowCallbacks();
        m_window->setVsync(false);

#ifdef NX_GRAPHICS_API_OPENGL
        if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress))
        {
            THROW_EXCEPTION(renderer::NxGraphicsApiInitFailure, "Failed to initialize OpenGL context with glad");
        }
        LOG(NEXO_INFO, "OpenGL context initialized with glad");
        glViewport(0, 0, static_cast<int>(m_window->getWidth()), static_cast<int>(m_window->getHeight()));
#endif

        renderer::NxRenderer::init();

        m_coordinator->init();
        registerEcsComponents();
        physicsSystem.Init(*m_coordinator);
        registerSystems();
        m_SceneManager.setCoordinator(m_coordinator);

        LOG(NEXO_DEV, "Application initialized");
    }

    void Application::beginFrame()
    {
	    const auto time = static_cast<float>(glfwGetTime());
	    m_currentTimestep = time - m_lastFrameTime;
	    m_lastFrameTime = time;
    }

    void Application::run(const SceneInfo &sceneInfo)
    {
        const auto time = static_cast<float>(glfwGetTime());
        const Timestep timestep = time - m_lastFrameTime;
        m_lastFrameTime = time;
        auto scenesIds = m_sceneManager.getSceneIDs();
        physicsSystem.Update(timestep.getSeconds(), *m_coordinator);
       	auto &renderContext = m_coordinator->getSingletonComponent<components::RenderContext>();

        if (!m_isMinimized)
        {
         	renderContext.sceneRendered = sceneInfo.id;
            renderContext.sceneType = sceneInfo.sceneType;
            if (sceneInfo.isChildWindow) {
                renderContext.isChildWindow = true;
                renderContext.viewportBounds[0] = sceneInfo.viewportBounds[0];
                renderContext.viewportBounds[1] = sceneInfo.viewportBounds[1];
            }
        	if (m_SceneManager.getScene(sceneInfo.id).isRendered())
			{
				m_cameraContextSystem->update();
				m_lightSystem->update();
				m_renderSystem->update();
			}
			if (m_SceneManager.getScene(sceneInfo.id).isActive())
			{
				m_perspectiveCameraControllerSystem->update(m_currentTimestep);
			}
        }

        // Update (swap buffers and poll events)
        if (sceneInfo.renderingType == RenderingType::WINDOW)
            m_window->onUpdate();
        m_eventManager->dispatchEvents();
        renderContext.reset();
        if (m_displayProfileResult)
            displayProfileResults();
    }

    void Application::endFrame()
    {
    	m_eventManager->clearEvents();
    }

    ecs::Entity Application::createEntity() const
    {
        return m_coordinator->createEntity();
    }

    void Application::deleteEntity(ecs::Entity entity)
    {
    	const auto tag = m_coordinator->tryGetComponent<components::SceneTag>(entity);
     	if (tag)
		{
			unsigned int sceneId = tag->get().id;
			m_SceneManager.getScene(sceneId).removeEntity(entity);
		}
		m_coordinator->destroyEntity(entity);
    }
}
