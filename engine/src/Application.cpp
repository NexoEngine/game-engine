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


#include "core/event/Input.hpp"

std::unique_ptr<nexo::Application> nexo::Application::_instance = nullptr;

namespace nexo {

    Application::Application()
    {
        m_window = renderer::Window::create();
        m_eventManager = std::make_shared<event::EventManager>();
        event::Input::init(m_window);

        // Window and glad init
        m_window->init(m_eventManager);
        m_window->setVsync(false);

        #ifdef GRAPHICS_API_OPENGL
            gladLoadGL();
            glViewport(0, 0, m_window->getWidth(), m_window->getHeight());
        #endif

        renderer::Renderer::init();

        // Debug flags
        m_eventDebugFlags |= DEBUG_LOG_KEYBOARD_EVENT;

        m_coordinator = std::make_shared<ecs::Coordinator>();
        m_coordinator->init();
    }

    void Application::registerAllDebugListeners()
    {
        m_eventManager->registerListener<event::EventKey>(this);
        m_eventManager->registerListener<event::EventWindowResize>(this);
        m_eventManager->registerListener<event::EventWindowClose>(this);
        m_eventManager->registerListener<event::EventMouseClick>(this);
        m_eventManager->registerListener<event::EventMouseScroll>(this);
        m_eventManager->registerListener<event::EventMouseMove>(this);
    }

    void Application::run()
    {
        while (m_isRunning)
        {
            const auto time = static_cast<float>(glfwGetTime());
            const core::Timestep timestep = time - m_lastFrameTime;
            m_lastFrameTime = time;

            if (!m_isMinimized)
            {
                // Clear
                renderer::RenderCommand::setClearColor({0.1f, 0.1f, 0.1f, 1.0f});
                renderer::RenderCommand::clear();

                if (m_actualScene != -1)
                {
                    m_scenes[m_actualScene].onUpdate(timestep);
                    m_scenes[m_actualScene].onRender();
                }
            }

            // Update (swap buffers and poll events)
            m_window->onUpdate();
            m_eventManager->dispatchEvents(&m_scenes[m_actualScene]);
        }
    }

    void Application::addScene(scene::Scene &scene)
    {
        const auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
                                     [&scene](const scene::Scene& existingScene) {
                                         return existingScene.id == scene.id;
                                     });

        if (it == m_scenes.end()) {
            m_scenes.push_back(scene);
            LOG(NEXO_DEBUG, "Added new scene with id: {}", scene.id);
        } else
            LOG(NEXO_WARN, "Scene with id: {} already exists", scene.id);
    }

    void Application::removeScene(scene::Scene &scene)
    {
        const auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
                                     [&scene](const scene::Scene& existingScene) {
                                         return existingScene.id == scene.id;
                                     });

        if (it == m_scenes.end()) {
            m_scenes.erase(it, m_scenes.end());
            LOG(NEXO_DEBUG, "Removed new scene with id: {}", scene.id);
        } else
            LOG(NEXO_WARN, "Scene with id: {} not found", scene.id);
    }

    void Application::switchScene(scene::Scene &scene)
    {
        const auto it = std::find_if(m_scenes.begin(), m_scenes.end(),
                                     [scene](const scene::Scene& existingScene) {
                                         return existingScene.id == scene.id;
                                     });

        if (it != m_scenes.end()) {
            m_actualScene = std::distance(m_scenes.begin(), it);
            LOG(NEXO_DEBUG, "Switched to scene: {}", scene.id);
        } else
            LOG(NEXO_WARN, "Scene with id: {} not found", scene.id);
    }
}
