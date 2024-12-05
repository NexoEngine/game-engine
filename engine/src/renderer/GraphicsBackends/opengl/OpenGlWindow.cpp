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

#include <stb_image.h>
#include <core/exceptions/Exceptions.hpp>

#include "renderer/Renderer.hpp"
#include "renderer/RendererExceptions.hpp"
#include "Logger.hpp"


namespace nexo::renderer {
    static void glfwErrorCallback(const int errorCode, const char *errorStr)
    {
        std::cerr << "[GLFW ERROR] Code : " << errorCode << " / Description : " << errorStr << std::endl;
    }

    void OpenGlWindow::setupCallback() const
    {
        // Resize event
        glfwSetWindowSizeCallback(_openGlWindow, [](GLFWwindow *window, const int width, const int height)
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
            const auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
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
        glfwSetMouseButtonCallback(_openGlWindow, [](GLFWwindow *window, const int button, const int action, const int mods)
        {
            const auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->mouseClickCallback)
                props->mouseClickCallback(button, action, mods);
        });

        // Mouse scroll event
        glfwSetScrollCallback(_openGlWindow, [](GLFWwindow *window, const double xOffset, const double yOffset)
        {
            const auto *props = static_cast<WindowProperty *>(glfwGetWindowUserPointer(window));
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
            THROW_EXCEPTION(GraphicsApiInitFailure, "OPENGL");
        LOG(NEXO_DEV, "Initializing opengl window");
        glfwSetErrorCallback(glfwErrorCallback);

#ifdef __linux__
        if (glfwGetPlatform() == GLFW_PLATFORM_WAYLAND) {
            glfwWindowHintString(GLFW_WAYLAND_APP_ID, _waylandAppId.c_str());
        } else if (glfwGetPlatform() == GLFW_PLATFORM_X11) {
            glfwWindowHintString(GLFW_X11_CLASS_NAME, _x11ClassName.c_str());
            glfwWindowHintString(GLFW_X11_INSTANCE_NAME, _x11InstanceName.c_str());
        } else {
            LOG(NEXO_WARN, "[GLFW WARNING] Unsupported platform for specific window hints.");
        }
#endif

		// TODO: add in documentation, if a function of opengl segv, it might be bcs this hints a version older than the function's opengl version
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        _openGlWindow = glfwCreateWindow(static_cast<int>(_props.width), static_cast<int>(_props.height), _props.title, nullptr, nullptr);
        if (!_openGlWindow)
            THROW_EXCEPTION(GraphicsApiWindowInitFailure, "OPENGL");
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

    void OpenGlWindow::setWindowIcon(const std::filesystem::path& iconPath)
    {
        GLFWimage icon;
        const auto iconStringPath = iconPath.string();
        icon.pixels = stbi_load(iconStringPath.c_str(), &icon.width, &icon.height, nullptr, 4);
        if (!icon.pixels) {
            THROW_EXCEPTION(StbiLoadException,
                std::format("Failed to load icon '{}': {}",
                    iconStringPath, stbi_failure_reason()));
        }
        if (icon.width == 0 || icon.height == 0) {
            LOG(NEXO_WARN, "Icon '{}' has a size of 0x0", iconStringPath);
        }
        LOG(NEXO_DEV, "Window icon loaded from '{}', size {}x{}", iconStringPath, icon.width, icon.height);
        glfwSetWindowIcon(_openGlWindow, 1, &icon);
        stbi_image_free(icon.pixels);
    }

    void OpenGlWindow::setErrorCallback(void *fctPtr)
    {
        glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(fctPtr));
    }

    // Linux specific method
#ifdef __linux__
    void OpenGlWindow::setWaylandAppId(const char* appId)
    {
        _waylandAppId = appId;
        LOG(NEXO_DEV, "Wayland app id set to '{}'", appId);
    }

    void OpenGlWindow::setWmClass(const char* className, const char* instanceName)
    {
        _x11ClassName = className;
        _x11InstanceName = instanceName;
        LOG(NEXO_DEV, "X11 class name set to '{}' and instance name set to '{}'", className, instanceName);
    }
#endif
}
