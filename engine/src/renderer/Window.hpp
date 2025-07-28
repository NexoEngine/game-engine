//// Window.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for window abstraction
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <filesystem>
#include <memory>
#include <functional>
#include <utility>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace nexo::renderer {

    using ResizeCallback = std::function<void(int, int)>;
    using CloseCallback = std::function<void()>;
    using KeyCallback = std::function<void(int, int, int)>;
    using MouseClickCallback = std::function<void(int, int, int)>;
    using MouseScrollCallback = std::function<void(double, double)>;
    using MouseMoveCallback = std::function<void(double, double)>;
    using FileDropCallback = std::function<void(int, const char**)>;

    struct NxWindowProperty
    {
        unsigned int width;
        unsigned int height;
        std::string title;
        bool vsync = true;
        bool isDarkMode = false;

        ResizeCallback resizeCallback;
        CloseCallback closeCallback;
        KeyCallback keyCallback;
        MouseClickCallback mouseClickCallback;
        MouseScrollCallback mouseScrollCallback;
        MouseMoveCallback mouseMoveCallback;
        FileDropCallback fileDropCallback;

        NxWindowProperty(const unsigned int w, const unsigned h, std::string t) : width(w), height(h), title(std::move(t)) {}
    };

    /**
    * @class NxWindow
    * @brief Abstract class for managing window operations in the rendering system.
    *
    * The `NxWindow` class provides an interface for creating, configuring, and
    * managing a window. It includes support for events like resizing, closing,
    * keyboard input, and mouse interactions.
    *
    * Responsibilities:
    * - Initialize and manage the window lifecycle.
    * - Handle window properties such as size, title, and VSync.
    * - Provide event handling through callbacks.
    *
    * Derived classes (e.g., `NxOpenGlWindow`) implement platform-specific behavior
    * for managing windows.
    */
    class NxWindow {
        public:
            NxWindow() = default;

            virtual ~NxWindow() = default;

            virtual void init() = 0;
            virtual void shutdown() = 0;
            virtual void onUpdate() = 0;

            [[nodiscard]] virtual unsigned int getWidth() const = 0;
            [[nodiscard]] virtual unsigned int getHeight() const = 0;

            virtual void getDpiScale(float *x, float *y) const = 0;

            virtual void setWindowIcon(const std::filesystem::path& iconPath) = 0;

            virtual void setTitle(const std::string& title) = 0;
            [[nodiscard]] virtual const std::string& getTitle() const = 0;

            virtual void setDarkMode(bool enabled) = 0;
            [[nodiscard]] virtual bool isDarkMode() const = 0;

            virtual void setVsync(bool enabled) = 0;
            [[nodiscard]] virtual bool isVsync() const = 0;


            [[nodiscard]] virtual bool isOpen() const = 0;
            virtual void close() = 0;

            [[nodiscard]] virtual void *window() const = 0;

            /**
            * @brief Factory function to create a platform-specific window.
            *
            * Depending on the graphics API (e.g., OpenGL), this function creates an
            * instance of the corresponding `NxWindow` implementation.
            *
            * @param width Initial width of the window.
            * @param height Initial height of the window.
            * @param title Title of the window.
            * @return A shared pointer to the created `NxWindow` instance.
            */
            static std::shared_ptr<NxWindow> create(int width = 1920, int height = 1080, const char *title = "Nexo window");

            virtual void setErrorCallback(void *fctPtr) = 0;
            virtual void setResizeCallback(ResizeCallback callback) = 0;
            virtual void setCloseCallback(CloseCallback callback) = 0;
            virtual void setKeyCallback(KeyCallback callback) = 0;
            virtual void setMouseClickCallback(MouseClickCallback callback) = 0;
            virtual void setMouseScrollCallback(MouseScrollCallback callback) = 0;
            virtual void setMouseMoveCallback(MouseMoveCallback callback) = 0;
            virtual void setFileDropCallback(FileDropCallback callback) = 0;

            // Linux specific methods
#ifdef __linux__
            virtual void setWaylandAppId(const char *appId) = 0;
            virtual void setWmClass(const char *className, const char *instanceName) = 0;
#endif
    };
}
