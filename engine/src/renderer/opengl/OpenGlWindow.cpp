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
#include "core/exceptions/Exceptions.hpp"
#include "Logger.hpp"


namespace nexo::renderer {
    static void glfwErrorCallback(const int errorCode, const char *errorStr)
    {
        std::cerr << "[GLFW ERROR] Code : " << errorCode << " / Description : " << errorStr << std::endl;
    }

    void OpenGlWindow::setupCallback() const
    {
        // Resize event
        glfwSetWindowSizeCallback(_openGlWindow, [](GLFWwindow *window, int width, int height)
        {
            auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
            props->width = width;
            props->height = height;
            Renderer::onWindowResize(width, height);
            if (props->resizeCallback)
                props->resizeCallback(width, height);
        });

        // Close event
        glfwSetWindowCloseCallback(_openGlWindow, [](GLFWwindow *window)
        {
            auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->closeCallback)
                props->closeCallback();
        });

        // Keyboard events
        glfwSetKeyCallback(_openGlWindow, [](GLFWwindow *window, const int key, [[maybe_unused]]int scancode, const int action, const int mods)
        {
            const auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->keyCallback)
                props->keyCallback(key, action, mods);
        });

        // Mouse click callback
        glfwSetMouseButtonCallback(_openGlWindow, [](GLFWwindow *window, int button, const int action, const int mods)
        {
            const auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->mouseClickCallback)
                props->mouseClickCallback(button, action, mods);
        });

        // Mouse scroll event
        glfwSetScrollCallback(_openGlWindow, [](GLFWwindow *window, const double xOffset, const double yOffset)
        {
            auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->mouseScrollCallback)
                props->mouseScrollCallback(xOffset, yOffset);
        });

        // Mouse move event
        glfwSetCursorPosCallback(_openGlWindow, [](GLFWwindow *window, const double xpos, const double ypos)
        {
            const auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->mouseMoveCallback)
                props->mouseMoveCallback(xpos, ypos);
        });
    }

    void OpenGlWindow::init()
    {
        if (!glfwInit())
            THROW_EXCEPTION(core::GraphicsApiInitFailure, "OPENGL");
        glfwSetErrorCallback(glfwErrorCallback);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        _openGlWindow = glfwCreateWindow(_props.width, _props.height, _props.title, nullptr, nullptr);
        if (!_openGlWindow)
            THROW_EXCEPTION(core::GraphicsApiWindowInitFailure, "OPENGL");
        glfwMakeContextCurrent(_openGlWindow);
        glfwSetWindowUserPointer(_openGlWindow, &_props);
        setVsync(true);
        setupCallback();
        LOG(NEXO_DEV, "Opengl window ({}, {}) initialized", _props.width, _props.height);
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

    void OpenGlWindow::setErrorCallback(void *fctPtr)
    {
        glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(fctPtr));
    }
}
