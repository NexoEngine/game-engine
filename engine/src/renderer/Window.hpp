//// Window.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for window abstraction
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <filesystem>
#include <functional>
#include <memory>
#include <utility>

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace nexo::renderer {

    using ResizeCallback      = std::function<void(int, int)>;
    using CloseCallback       = std::function<void()>;
    using KeyCallback         = std::function<void(int, int, int)>;
    using MouseClickCallback  = std::function<void(int, int, int)>;
    using MouseScrollCallback = std::function<void(double, double)>;
    using MouseMoveCallback   = std::function<void(double, double)>;
    using FileDropCallback    = std::function<void(int, const char **)>;

    struct NxWindowProperty {
        unsigned int width;
        unsigned int height;
        std::string title;
        bool vsync      = true;
        bool isDarkMode = false;

        ResizeCallback resizeCallback;
        CloseCallback closeCallback;
        KeyCallback keyCallback;
        MouseClickCallback mouseClickCallback;
        MouseScrollCallback mouseScrollCallback;
        MouseMoveCallback mouseMoveCallback;
        FileDropCallback fileDropCallback;

        NxWindowProperty(const unsigned int w, const unsigned h, std::string t)
            : width(w), height(h), title(std::move(t))
        {}
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

        /**
         * @brief Initializes the window with specified properties.
         *
         * This method sets up the window with the desired width, height, title,
         * and other properties. It must be called before using the window for
         * rendering or event handling.
         *
         * Throws:
         * - Implementation-specific exceptions if initialization fails.
         *
         * Notes:
         * - The actual implementation of this method is platform-specific and should be
         *   provided by derived classes.
         */
        virtual void init() = 0;

        /**
         * @brief Cleans up resources and shuts down the window.
         */
        virtual void shutdown() = 0;

        /**
         * @brief Updates the window state. Should be called each frame.
         */
        virtual void onUpdate() = 0;

        /**
         * @brief Gets the current width of the window.
         * @return The window width in pixels.
         */
        [[nodiscard]] virtual unsigned int getWidth() const = 0;

        /**
         * @brief Gets the current height of the window.
         * @return The window height in pixels.
         */
        [[nodiscard]] virtual unsigned int getHeight() const = 0;

        /**
         * @brief Gets the DPI scaling factors for the window.
         * @param x Pointer to store the horizontal DPI scale.
         * @param y Pointer to store the vertical DPI scale.
         */
        virtual void getDpiScale(float *x, float *y) const = 0;

        /**
         * @brief Sets the window icon from an image file.
         * @param iconPath Path to the icon image file.
         */
        virtual void setWindowIcon(const std::filesystem::path &iconPath) = 0;

        /**
         * @brief Sets the window title.
         * @param title The new window title.
         */
        virtual void setTitle(const std::string &title) = 0;

        /**
         * @brief Gets the current window title.
         * @return The window title string.
         */
        [[nodiscard]] virtual const std::string &getTitle() const = 0;

        /**
         * @brief Enables or disables dark mode for the window.
         * @param enabled True to enable dark mode, false to disable.
         */
        virtual void setDarkMode(bool enabled) = 0;

        /**
         * @brief Checks if dark mode is enabled.
         * @return True if dark mode is enabled, false otherwise.
         */
        [[nodiscard]] virtual bool isDarkMode() const = 0;

        /**
         * @brief Enables or disables vertical synchronization.
         * @param enabled True to enable VSync, false to disable.
         */
        virtual void setVsync(bool enabled) = 0;

        /**
         * @brief Checks if VSync is enabled.
         * @return True if VSync is enabled, false otherwise.
         */
        [[nodiscard]] virtual bool isVsync() const = 0;

        /**
         * @brief Checks if the window is currently open.
         * @return True if the window is open, false otherwise.
         */
        [[nodiscard]] virtual bool isOpen() const = 0;

        /**
         * @brief Closes the window.
         */
        virtual void close() = 0;

        /**
         * @brief Gets the native window handle.
         * @return A pointer to the native window implementation.
         */
        [[nodiscard]] virtual void *window() const = 0;

        virtual void setFullscreen(bool fullscreen) = 0;
        [[nodiscard]] virtual bool isFullscreen() const = 0;
        virtual void toggleFullscreen() = 0;

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
        static std::shared_ptr<NxWindow> create(int width = 1920, int height = 1080,
                                                const std::string &title = "Nexo window");

        /**
         * @brief Sets the callback for error handling.
         * @param fctPtr Pointer to the error callback function.
         */
        virtual void setErrorCallback(void *fctPtr) = 0;

        /**
         * @brief Sets the callback for window resize events.
         * @param callback Function called when the window is resized (width, height).
         */
        virtual void setResizeCallback(ResizeCallback callback) = 0;

        /**
         * @brief Sets the callback for window close events.
         * @param callback Function called when the window is closed.
         */
        virtual void setCloseCallback(CloseCallback callback) = 0;

        /**
         * @brief Sets the callback for keyboard events.
         * @param callback Function called when a key is pressed/released (key, scancode, action).
         */
        virtual void setKeyCallback(KeyCallback callback) = 0;

        /**
         * @brief Sets the callback for mouse button events.
         * @param callback Function called when a mouse button is pressed/released (button, action, mods).
         */
        virtual void setMouseClickCallback(MouseClickCallback callback) = 0;

        /**
         * @brief Sets the callback for mouse scroll events.
         * @param callback Function called when mouse wheel is scrolled (xoffset, yoffset).
         */
        virtual void setMouseScrollCallback(MouseScrollCallback callback) = 0;

        /**
         * @brief Sets the callback for mouse movement events.
         * @param callback Function called when mouse position changes (xpos, ypos).
         */
        virtual void setMouseMoveCallback(MouseMoveCallback callback) = 0;

        /**
         * @brief Sets the callback for file drop events.
         * @param callback Function called when files are dropped onto the window (count, paths).
         */
        virtual void setFileDropCallback(FileDropCallback callback) = 0;
        // Linux specific methods
#ifdef __linux__
        virtual void setWaylandAppId(const char *appId)                          = 0;
        virtual void setWmClass(const char *className, const char *instanceName) = 0;
#endif
    };
} // namespace nexo::renderer
