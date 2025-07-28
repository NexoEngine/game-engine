//// OpenGlWindow.cpp /////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file the opengl window class
//
///////////////////////////////////////////////////////////////////////////////

#include "OpenGlWindow.hpp"

#include <stb_image.h>

#include "renderer/Renderer.hpp"
#include "renderer/RendererExceptions.hpp"
#include "Logger.hpp"

#if defined(_WIN32) || defined(_WIN64)
    #include <dwmapi.h>
    #define GLFW_EXPOSE_NATIVE_WIN32
    #define GLFW_EXPOSE_NATIVE_WGL
    #define GLFW_NATIVE_INCLUDE_NONE
    #include <GLFW/glfw3native.h>
    #pragma comment (lib, "Dwmapi")
#endif

namespace nexo::renderer {
    static void glfwErrorCallback(const int errorCode, const char *errorStr)
    {
        std::cerr << "[GLFW ERROR] Code : " << errorCode << " / Description : " << errorStr << std::endl;
    }

    void NxOpenGlWindow::setupCallback() const
    {
        // Resize event
        glfwSetWindowSizeCallback(_openGlWindow, [](GLFWwindow *window, const int width, const int height)
        {
            if (width <= 0 || height <= 0)
                return;
            auto *props = static_cast<NxWindowProperty *>(glfwGetWindowUserPointer(window));
            props->width = width;
            props->height = height;
            NxRenderer::onWindowResize(width, height);
            if (props->resizeCallback)
                props->resizeCallback(width, height);
        });

        // Close event
        glfwSetWindowCloseCallback(_openGlWindow, [](GLFWwindow *window)
        {
            const auto *props = static_cast<NxWindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->closeCallback)
                props->closeCallback();
        });

        // Keyboard events
        glfwSetKeyCallback(_openGlWindow, [](GLFWwindow *window, const int key, [[maybe_unused]]int scancode, const int action, const int mods)
        {
            const auto *props = static_cast<NxWindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->keyCallback)
                props->keyCallback(key, action, mods);
        });

        // Mouse click callback
        glfwSetMouseButtonCallback(_openGlWindow, [](GLFWwindow *window, const int button, const int action, const int mods)
        {
            const auto *props = static_cast<NxWindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->mouseClickCallback)
                props->mouseClickCallback(button, action, mods);
        });

        // Mouse scroll event
        glfwSetScrollCallback(_openGlWindow, [](GLFWwindow *window, const double xOffset, const double yOffset)
        {
            const auto *props = static_cast<NxWindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->mouseScrollCallback)
                props->mouseScrollCallback(xOffset, yOffset);
        });

        // Mouse move event
        glfwSetCursorPosCallback(_openGlWindow, [](GLFWwindow *window, const double xpos, const double ypos)
        {
            const auto *props = static_cast<NxWindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->mouseMoveCallback)
                props->mouseMoveCallback(xpos, ypos);
        });

        glfwSetDropCallback(_openGlWindow, [](GLFWwindow *window, const int count, const char **paths)
        {
            const auto *props = static_cast<NxWindowProperty *>(glfwGetWindowUserPointer(window));
            if (props->fileDropCallback)
                props->fileDropCallback(count, paths);
        });
    }

    void NxOpenGlWindow::init()
    {
        if (!glfwInit())
            THROW_EXCEPTION(NxGraphicsApiInitFailure, "OPENGL");
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
        _openGlWindow = glfwCreateWindow(static_cast<int>(_props.width), static_cast<int>(_props.height), _props.title.c_str(), nullptr, nullptr);
        if (!_openGlWindow)
            THROW_EXCEPTION(NxGraphicsApiWindowInitFailure, "OPENGL");
        glfwMakeContextCurrent(_openGlWindow);
        glfwSetWindowUserPointer(_openGlWindow, &_props);
        setVsync(true);
        setDarkMode(false);
        setupCallback();
        LOG(NEXO_DEV, "Opengl window ({}, {}) initialized", _props.width, _props.height);
    }

    void NxOpenGlWindow::shutdown()
    {
        glfwDestroyWindow(_openGlWindow);
        glfwTerminate();
    }

    void NxOpenGlWindow::onUpdate()
    {
        glfwSwapBuffers(_openGlWindow);
        glfwPollEvents();
    }

    void NxOpenGlWindow::setVsync(const bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);
        _props.vsync = enabled;
    }

    bool NxOpenGlWindow::isVsync() const
    {
        return _props.vsync;
    }

    void NxOpenGlWindow::getDpiScale(float *x, float *y) const
    {
        glfwGetWindowContentScale(_openGlWindow, x, y);
    }

    void NxOpenGlWindow::setWindowIcon(const std::filesystem::path &iconPath)
    {
        GLFWimage icon;
        const auto iconStringPath = iconPath.string();
        icon.pixels               = stbi_load(iconStringPath.c_str(), &icon.width, &icon.height, nullptr, 4);
        if (!icon.pixels) {
            THROW_EXCEPTION(NxStbiLoadException,
                            std::format("Failed to load icon '{}': {}", iconStringPath, stbi_failure_reason()));
        }
        if (icon.width == 0 || icon.height == 0) {
            LOG(NEXO_WARN, "Icon '{}' has a size of 0x0", iconStringPath);
        }
        LOG(NEXO_DEV, "Window icon loaded from '{}', size {}x{}", iconStringPath, icon.width, icon.height);
        glfwSetWindowIcon(_openGlWindow, 1, &icon);
        stbi_image_free(icon.pixels);
    }

    void NxOpenGlWindow::setTitle(const std::string &title)
    {
        _props.title = title;
        glfwSetWindowTitle(_openGlWindow, _props.title.c_str());
        LOG(NEXO_DEV, "Window title set to '{}'", _props.title);
    }

    const std::string &NxOpenGlWindow::getTitle() const
    {
        return _props.title;
    }

    void NxOpenGlWindow::setDarkMode(const bool enabled)
    {
#if defined(_WIN32) || defined(_WIN64)
        HWND hWnd = glfwGetWin32Window(_openGlWindow);
        if (hWnd == nullptr) {
            LOG(NEXO_ERROR, "[GLFW ERROR] Failed to get Win32 window handle for dark mode setting");
            return;
        }

        const BOOL useDarkMode                 = enabled ? TRUE : FALSE;
        const BOOL setImmersiveDarkModeSuccess = SUCCEEDED(DwmSetWindowAttribute(
            hWnd, DWMWINDOWATTRIBUTE::DWMWA_USE_IMMERSIVE_DARK_MODE, &useDarkMode, sizeof(useDarkMode)));
        if (!setImmersiveDarkModeSuccess) {
            LOG(NEXO_ERROR, "[GLFW ERROR] Failed to set enable/disable immersive dark mode for window: {}",
                GetLastError());
            return;
        }
#endif
        LOG(NEXO_DEV, "Setting dark mode to {}", enabled ? "enabled" : "disabled");
        _props.isDarkMode = enabled;
    }

    bool NxOpenGlWindow::isDarkMode() const
    {
        return _props.isDarkMode;
    }

    void NxOpenGlWindow::setErrorCallback(void *fctPtr)
    {
        glfwSetErrorCallback(reinterpret_cast<GLFWerrorfun>(fctPtr));
    }

    // Linux specific method
#ifdef __linux__
    void NxOpenGlWindow::setWaylandAppId(const char* appId)
    {
        _waylandAppId = appId;
        LOG(NEXO_DEV, "Wayland app id set to '{}'", appId);
    }

    void NxOpenGlWindow::setWmClass(const char* className, const char* instanceName)
    {
        _x11ClassName = className;
        _x11InstanceName = instanceName;
        LOG(NEXO_DEV, "X11 class name set to '{}' and instance name set to '{}'", className, instanceName);
    }
#endif
}
