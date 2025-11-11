//// OpenGlWindow.hpp /////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header for opengl window
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "renderer/Window.hpp"

namespace nexo::renderer {

    /**
     * @class NxOpenGlWindow
     * @brief OpenGL-specific implementation of the `NxWindow` class.
     *
     * The `NxOpenGlWindow` class manages the creation and behavior of a window in
     * an OpenGL context. It integrates with GLFW for window management and event
     * handling.
     *
     * Responsibilities:
     * - Create and manage an OpenGL-compatible window.
     * - Provide event handling for window, keyboard, and mouse events.
     * - Manage OpenGL context initialization and VSync settings.
     */
    class NxOpenGlWindow final : public NxWindow {
       public:
        /**
         * @brief Creates an OpenGL window with the specified properties.
         *
         * Initializes the `NxWindowProperty` structure with the given width, height,
         * and title. The window itself is created during the `init()` call.
         *
         * @param width Initial width of the window.
         * @param height Initial height of the window.
         * @param title Title of the window.
         */
        explicit NxOpenGlWindow(const int width = 1920, const int height = 1080,
                                const std::string& title = "Nexo window")
            : _props(width, height, title)
        {}

        /**
         * @brief Initializes the OpenGL window and its associated resources.
         *
         * Creates the window using GLFW, sets up the OpenGL context, and configures
         * callbacks for handling window events like resizing, closing, and input.
         *
         * @throw NxGraphicsApiInitFailure If GLFW initialization fails.
         * @throw NxGraphicsApiWindowInitFailure If the window creation fails.
         */
        void init() override;

        /**
         * @brief Shuts down the OpenGL window and releases its resources.
         *
         * Destroys the GLFW window and terminates GLFW. Should be called during
         * cleanup to ensure proper resource management.
         */
        void shutdown() override;

        /**
         * @brief Updates the window's state and processes events.
         *
         * Swaps the front and back buffers for rendering and polls for window events.
         */
        void onUpdate() override;

        /**
         * @brief Gets the current width of the window.
         * @return The width in pixels.
         */
        [[nodiscard]] unsigned int getWidth() const override
        {
            return _props.width;
        }

        /**
         * @brief Gets the current height of the window.
         * @return The height in pixels.
         */
        [[nodiscard]] unsigned int getHeight() const override
        {
            return _props.height;
        }

        /**
         * @brief Retrieves the DPI scaling factors for the window.
         * @param x Pointer to store the horizontal DPI scale.
         * @param y Pointer to store the vertical DPI scale.
         */
        void getDpiScale(float* x, float* y) const override;

        /**
         * @brief Sets the window icon from the specified file path.
         * @param iconPath Path to the icon image file.
         */
        void setWindowIcon(const std::filesystem::path& iconPath) override;

        /**
         * @brief Sets the window title.
         * @param title The new title for the window.
         */
        void setTitle(const std::string& title) override;

        /**
         * @brief Gets the current window title.
         * @return The window title as a string.
         */
        [[nodiscard]] const std::string& getTitle() const override;

        /**
         * @brief Enables or disables dark mode for the window.
         * @param enabled True to enable dark mode, false to disable.
         */
        void setDarkMode(bool enabled) override;

        /**
         * @brief Checks if dark mode is enabled.
         * @return True if dark mode is enabled, false otherwise.
         */
        [[nodiscard]] bool isDarkMode() const override;
        /**
         * @brief Enables or disables vertical synchronization (VSync).
         *
         * When VSync is enabled, the frame rate is synchronized with the display's
         * refresh rate to prevent screen tearing.
         *
         * @param enabled True to enable VSync, false to disable it.
         */
        void setVsync(bool enabled) override;

        /**
         * @brief Checks if vertical synchronization (VSync) is enabled.
         *
         * @return True if VSync is enabled, false otherwise.
         */
        [[nodiscard]] bool isVsync() const override;

        /**
         * @brief Checks if the window is currently open.
         *
         * @return True if the window is open, false if it should close.
         */
        [[nodiscard]] bool isOpen() const override
        {
            return !glfwWindowShouldClose(_openGlWindow);
        }

        /**
         * @brief Requests the window to close.
         *
         * Sets the window's close flag, which will be processed in the next update cycle.
         */
        void close() override
        {
            glfwSetWindowShouldClose(_openGlWindow, GLFW_TRUE);
        }

        /**
         * @brief Returns the native window handle.
         *
         * @return Pointer to the underlying GLFWwindow.
         */
        [[nodiscard]] void* window() const override
        {
            return _openGlWindow;
        }

        /**
         * @brief Sets the error callback function for the window.
         *
         * @param fctPtr Pointer to the error callback function.
         */
        void setErrorCallback(void* fctPtr) override;

        /**
         * @brief Sets the callback function for window resize events.
         *
         * @param callback The function to call when the window is resized.
         */
        void setResizeCallback(ResizeCallback callback) override
        {
            _props.resizeCallback = std::move(callback);
        }

        /**
         * @brief Sets the callback function for window close events.
         *
         * @param callback The function to call when the window is requested to close.
         */
        void setCloseCallback(CloseCallback callback) override
        {
            _props.closeCallback = std::move(callback);
        }

        /**
         * @brief Sets the callback function for key events.
         *
         * @param callback The function to call when a key event occurs.
         */
        void setKeyCallback(KeyCallback callback) override
        {
            _props.keyCallback = std::move(callback);
        }

        /**
         * @brief Sets the callback function for mouse click events.
         *
         * @param callback The function to call when a mouse button is clicked.
         */
        void setMouseClickCallback(MouseClickCallback callback) override
        {
            _props.mouseClickCallback = std::move(callback);
        }

        /**
         * @brief Sets the callback function for mouse scroll events.
         *
         * @param callback The function to call when the mouse wheel is scrolled.
         */
        void setMouseScrollCallback(MouseScrollCallback callback) override
        {
            _props.mouseScrollCallback = std::move(callback);
        }

        /**
         * @brief Sets the callback function for mouse movement events.
         *
         * @param callback The function to call when the mouse is moved.
         */
        void setMouseMoveCallback(MouseMoveCallback callback) override
        {
            _props.mouseMoveCallback = std::move(callback);
        }

        /**
         * @brief Sets the callback function for file drop events.
         *
         * @param callback The function to call when files are dropped onto the window.
         */
        void setFileDropCallback(FileDropCallback callback) override
        {
            _props.fileDropCallback = std::move(callback);
        }

        void setFullscreen(bool fullscreen) override;
        [[nodiscard]] bool isFullscreen() const override;
        void toggleFullscreen() override;

        // Linux specific method
#ifdef __linux__
        void setWaylandAppId(const char* appId) override;
        void setWmClass(const char* className, const char* instanceName) override;

       private:
        std::string _waylandAppId    = "nexo";
        std::string _x11ClassName    = "nexo";
        std::string _x11InstanceName = "nexo";
#endif
       private:
        GLFWwindow* _openGlWindow{};
        NxWindowProperty _props;

        int _windowedXPos = 0;
        int _windowedYPos = 0;
        int _windowedWidth = 1920;
        int _windowedHeight = 1080;

        void setupCallback() const;
    };
} // namespace nexo::renderer
