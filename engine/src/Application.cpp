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

#include "Renderer3D.hpp"
#include "components/BillboardMesh.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "components/Model.hpp"
#include "components/Name.hpp"
#include "components/Parent.hpp"
#include "components/Render3D.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/StaticMesh.hpp"
#include "components/Transform.hpp"
#include "components/Editor.hpp"
#include "components/Uuid.hpp"
#include "components/Render.hpp"
#include "components/MaterialComponent.hpp"
#include "core/event/Input.hpp"
#include "Timestep.hpp"
#include "exceptions/Exceptions.hpp"
#include "renderer/RendererExceptions.hpp"
#include "renderer/Renderer.hpp"
#include "scripting/native/Scripting.hpp"
#include "systems/CameraSystem.hpp"
#include "systems/RenderBillboardSystem.hpp"
#include "systems/RenderCommandSystem.hpp"
#include "systems/TransformHierarchySystem.hpp"
#include "systems/TransformMatrixSystem.hpp"
#include "systems/ScriptingSystem.hpp"
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
        m_coordinator->registerComponent<components::PerspectiveCameraController>();
        m_coordinator->setRestoreComponent<components::PerspectiveCameraController>();
        m_coordinator->registerComponent<components::PerspectiveCameraTarget>();
        m_coordinator->setRestoreComponent<components::PerspectiveCameraTarget>();
        m_coordinator->registerComponent<components::EditorCameraTag>();
        m_coordinator->setRestoreComponent<components::EditorCameraTag>();
        m_coordinator->registerComponent<components::SelectedTag>();
        m_coordinator->registerComponent<components::StaticMeshComponent>();
        m_coordinator->registerComponent<components::ParentComponent>();
        m_coordinator->registerComponent<components::ModelComponent>();
        m_coordinator->registerComponent<components::BillboardComponent>();
        m_coordinator->registerComponent<components::MaterialComponent>();
        m_coordinator->registerComponent<components::NameComponent>();
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
        m_renderCommandSystem = m_coordinator->registerGroupSystem<system::RenderCommandSystem>();
        m_renderBillboardSystem = m_coordinator->registerGroupSystem<system::RenderBillboardSystem>();
        m_transformHierarchySystem = m_coordinator->registerGroupSystem<system::TransformHierarchySystem>();
        m_transformMatrixSystem = m_coordinator->registerQuerySystem<system::TransformMatrixSystem>();

        auto pointLightSystem = m_coordinator->registerGroupSystem<system::PointLightsSystem>();
        auto directionalLightSystem = m_coordinator->registerGroupSystem<system::DirectionalLightsSystem>();
        auto spotLightSystem = m_coordinator->registerGroupSystem<system::SpotLightsSystem>();
        auto ambientLightSystem = m_coordinator->registerGroupSystem<system::AmbientLightSystem>();
        m_lightSystem = std::make_shared<system::LightSystem>(ambientLightSystem, directionalLightSystem, pointLightSystem, spotLightSystem);

        m_scriptingSystem = std::make_shared<system::ScriptingSystem>();
    }

    int Application::initScripting() const
    {
        return m_scriptingSystem->init();
    }

    int Application::shutdownScripting() const
    {
        return m_scriptingSystem->shutdown();
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
        renderer::NxRenderer3D::get().init();
        physicsSystem.Init(*m_coordinator);
        registerSystems();
        m_SceneManager.setCoordinator(m_coordinator);

        LOG(NEXO_DEV, "Application initialized");
    }

    void Application::beginFrame()
    {
	    const auto time = glfwGetTime();
        m_worldState.time.deltaTime = time - m_worldState.time.totalTime;
        m_worldState.time.totalTime = time;
        m_worldState.stats.frameCount += 1;
    }

    void Application::run(const SceneInfo &sceneInfo)
    {
        const auto time = static_cast<float>(glfwGetTime());
        const Timestep timestep = time - m_lastFrameTime;
        m_lastFrameTime = time;
       	auto &renderContext = m_coordinator->getSingletonComponent<components::RenderContext>();

        m_scriptingSystem->update();

        if (!m_isMinimized)
        {
         	renderContext.sceneRendered = static_cast<int>(sceneInfo.id);
            renderContext.sceneType = sceneInfo.sceneType;
            if (sceneInfo.isChildWindow) {
                renderContext.isChildWindow = true;
                renderContext.viewportBounds[0] = sceneInfo.viewportBounds[0];
                renderContext.viewportBounds[1] = sceneInfo.viewportBounds[1];
            }
        	if (m_SceneManager.getScene(sceneInfo.id).isRendered())
			{
                m_transformMatrixSystem->update();
                m_transformHierarchySystem->update();
				m_cameraContextSystem->update();
				m_lightSystem->update();
				m_renderCommandSystem->update();
				m_renderBillboardSystem->update();
				for (auto &camera : renderContext.cameras)
				    camera.pipeline.execute();
				// We have to unbind after the whole pipeline since multiple passes can use the same textures
				// but we cant bind everything beforehand since a resize can be triggered and invalidate the whole state
                renderer::NxRenderer3D::get().unbindTextures();
        	    physicsSystem.Update(timestep, *m_coordinator);
			}
			if (m_SceneManager.getScene(sceneInfo.id).isActive())
			{
				m_perspectiveCameraControllerSystem->update(m_worldState.time.deltaTime);
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

    void Application::deleteEntity(const ecs::Entity entity)
    {
        // First, recursively delete all children of this entity
        deleteEntityChildren(entity);

        // Then, remove this entity from its parent's children list (if it has a parent)
        removeEntityFromParent(entity);

        // Finally, handle the scene tag and destroy the entity as in the original code
        const auto tag = m_coordinator->tryGetComponent<components::SceneTag>(entity);
        if (tag) {
            const unsigned int sceneId = tag->get().id;
            m_SceneManager.getScene(sceneId).removeEntity(entity);
        }
        m_coordinator->destroyEntity(entity);
    }

    void Application::removeEntityFromParent(const ecs::Entity entity) const
    {
        // Get the parent component to find the parent entity
        auto parentComponent = m_coordinator->tryGetComponent<components::ParentComponent>(entity);
        if (!parentComponent || parentComponent->get().parent == ecs::INVALID_ENTITY)
            return;

        ecs::Entity parentEntity = parentComponent->get().parent;

        // Get the parent's transform component which now stores children
        auto parentTransform = m_coordinator->tryGetComponent<components::TransformComponent>(parentEntity);
        if (parentTransform) {
            // Remove this entity from parent's children vector
            parentTransform->get().removeChild(entity);
        }
    }

    void Application::deleteEntityChildren(const ecs::Entity entity)
    {
        // Check if this entity has a transform component with children
        auto transform = m_coordinator->tryGetComponent<components::TransformComponent>(entity);
        if (!transform || transform->get().children.empty())
            return;

        // Create a copy of the children vector since we'll be modifying it during iteration
        std::vector<ecs::Entity> childrenCopy = transform->get().children;

        // Delete each child entity recursively
        for (const auto& childEntity : childrenCopy)
        {
            if (childEntity != ecs::INVALID_ENTITY && childEntity != entity) // Avoid circular references
                deleteEntity(childEntity);
        }

        // Clear the children list to avoid dangling references
        transform->get().children.clear();
    }
}
