//// OpenGlWindow.cpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file the opengl window class
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGlWindow.hpp"
#include "renderer/Renderer.hpp"


namespace nexo::renderer {
        static void glfwErrorCallback(const int errorCode, const char *errorStr)
    {
        std::cerr << "[GLFW ERROR] Code : " << errorCode << " / Description : " << errorStr << std::endl;
    }

    void OpenGlWindow::setupCallback() const {
        // Resize event
        glfwSetWindowSizeCallback(_openGlWindow, [](GLFWwindow *window, int width, int height)
        {
            auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
            props->width = width;
            props->height = height;
            Renderer::onWindowResize(width, height);

            auto event = event::EventWindowResize(width, height);
            props->eventManager->emitEvent<nexo::event::EventWindowResize>(std::make_shared<nexo::event::EventWindowResize>(event));
        });

        // Close event
        glfwSetWindowCloseCallback(_openGlWindow, [](GLFWwindow *window)
        {
            auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
            nexo::event::EventWindowClose event;
            props->eventManager->emitEvent<nexo::event::EventWindowClose>(std::make_shared<nexo::event::EventWindowClose>(event));
        });

        // Keyboard events
        glfwSetKeyCallback(_openGlWindow, [](GLFWwindow *window, const int key, int scancode, const int action, const int mods)
        {
            const auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));

            event::EventKey eventKey;
            eventKey.keycode = key;
            eventKey.mods = mods;
            switch (action)
            {
                case GLFW_PRESS:
                {
                    eventKey.action = event::KeyAction::PRESSED;
                    break;
                }
                case GLFW_RELEASE:
                {
                    eventKey.action = event::KeyAction::RELEASED;
                    break;
                }
                case GLFW_REPEAT:
                {
                    eventKey.action = event::KeyAction::REPEAT;
                    break;
                }
                default: return;
            }
            props->eventManager->emitEvent<event::EventKey>(std::make_shared<event::EventKey>(eventKey));
        });

        // Mouse click callback
        glfwSetMouseButtonCallback(_openGlWindow, [](GLFWwindow *window, int button, const int action, const int mods)
        {
            const auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));

            event::EventMouseClick event;
            event.button = static_cast<nexo::event::MouseButton>(button);
            event.mods = mods;
            switch (action)
            {
                case GLFW_PRESS:
                {
                    event.action = event::KeyAction::PRESSED;
                    break;
                }
                case GLFW_RELEASE:
                    event.action = event::KeyAction::RELEASED;
                    break;
                default: return;
            }
            props->eventManager->emitEvent<event::EventMouseClick>(std::make_shared<event::EventMouseClick>(event));
        });

        // Mouse scroll event
        glfwSetScrollCallback(_openGlWindow, [](GLFWwindow *window, const double xOffset, const double yOffset)
        {
            auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));

            event::EventMouseScroll event(static_cast<float>(xOffset), static_cast<float>(yOffset));
            props->eventManager->emitEvent<event::EventMouseScroll>(std::make_shared<event::EventMouseScroll>(event));
        });

        // Mouse move event
        glfwSetCursorPosCallback(_openGlWindow, [](GLFWwindow *window, const double xpos, const double ypos)
        {
            const auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));

            event::EventMouseMove event(xpos, ypos);
            props->eventManager->emitEvent<event::EventMouseMove>(std::make_shared<event::EventMouseMove>(event));
        });
    }

    void OpenGlWindow::init(const std::shared_ptr<event::EventManager> eventManager)
    {
        if (!glfwInit()) {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return;
        }
        glfwSetErrorCallback(glfwErrorCallback);
        // Telling openGL what we are using
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        _openGlWindow = glfwCreateWindow(_props.width, _props.height, _props.title, nullptr, nullptr);
        glfwMakeContextCurrent(_openGlWindow);
        glfwSetWindowUserPointer(_openGlWindow, &_props);
        setVsync(true);
        _props.eventManager = eventManager;
        setupCallback();
    }
    void OpenGlWindow::shutdown()
    {
        glfwDestroyWindow(_openGlWindow);
        glfwTerminate();
    }

    void OpenGlWindow::onUpdate()
    {
        glfwSwapBuffers(_openGlWindow);
        glfwPollEvents();
    }

    void OpenGlWindow::setVsync(const bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        _props.vsync = enabled;
    }

    bool OpenGlWindow::isVsync() const
    {
        return _props.vsync;
    }

    void OpenGlWindow::getDpiScale(float *x, float *y) const
    {
        glfwGetWindowContentScale(_openGlWindow, x, y);
    }

}
