//// Init.cpp ////////////////////////////////////////////////////////////////
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the main application initialization
//
///////////////////////////////////////////////////////////////////////////////

#include "Application.hpp"
#include "renderer/RendererExceptions.hpp"
#include "renderer/Renderer.hpp"
#include "renderer/Renderer3D.hpp"

#include <memory>

namespace nexo {
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
