//// OpenGlWindow.hpp /////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
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
            explicit NxOpenGlWindow(const int width = 1920,
                         const int height = 1080,
                         const std::string &title = "Nexo window") :
                    _props(width, height, title) {}

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

            [[nodiscard]] unsigned int getWidth() const override { return _props.width; };
            [[nodiscard]] unsigned int getHeight() const override {return _props.height; };

            void getDpiScale(float *x, float *y) const override;

            void setWindowIcon(const std::filesystem::path& iconPath) override;

            void setTitle(const std::string& title) override;
            [[nodiscard]] const std::string& getTitle() const override;

            void setDarkMode(bool enabled) override;
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
            [[nodiscard]] bool isVsync() const override;


            [[nodiscard]] bool isOpen() const override { return !glfwWindowShouldClose(_openGlWindow);};
            void close() override { glfwSetWindowShouldClose(_openGlWindow, GLFW_TRUE); };

            [[nodiscard]] void *window() const override { return _openGlWindow; };
            void setErrorCallback(void *fctPtr) override;
            void setResizeCallback(ResizeCallback callback) override { _props.resizeCallback = std::move(callback); }
            void setCloseCallback(CloseCallback callback) override { _props.closeCallback = std::move(callback); }
            void setKeyCallback(KeyCallback callback) override { _props.keyCallback = std::move(callback); }
            void setMouseClickCallback(MouseClickCallback callback) override { _props.mouseClickCallback = std::move(callback); }
            void setMouseScrollCallback(MouseScrollCallback callback) override { _props.mouseScrollCallback = std::move(callback); }
            void setMouseMoveCallback(MouseMoveCallback callback) override { _props.mouseMoveCallback = std::move(callback); }
            void setFileDropCallback(FileDropCallback callback) override { _props.fileDropCallback = std::move(callback); }

            // Linux specific method
#ifdef __linux__
            void setWaylandAppId(const char* appId) override;
            void setWmClass(const char* className, const char* instanceName) override;
        private:
            std::string _waylandAppId = "nexo";
            std::string _x11ClassName = "nexo";
            std::string _x11InstanceName = "nexo";
#endif
        private:
            GLFWwindow *_openGlWindow{};
            NxWindowProperty _props;

            void setupCallback() const;
    };
}
