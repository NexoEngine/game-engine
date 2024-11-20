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
    class OpenGlWindow final : public Window {
        public:
            explicit OpenGlWindow(const int width = 1920,
                         const int height = 1080,
                         const char *title = "Nexo window") :
                    _props(width, height, title) {}

            void init() override;
            void shutdown() override;
            void onUpdate() override;

            [[nodiscard]] unsigned int getWidth() const override { return _props.width; };
            [[nodiscard]] unsigned int getHeight() const override {return _props.height; };

            void getDpiScale(float *x, float *y) const override;

            void setVsync(bool enabled) override;
            [[nodiscard]] bool isVsync() const override;


            [[nodiscard]] bool isOpen() const override { return !glfwWindowShouldClose(_openGlWindow);};

            [[nodiscard]] void *window() const override { return _openGlWindow; };
            void setErrorCallback(void *fctPtr) override;
            void setResizeCallback(ResizeCallback callback) override { _props.resizeCallback = std::move(callback); }
            void setCloseCallback(CloseCallback callback) override { _props.closeCallback = std::move(callback); }
            void setKeyCallback(KeyCallback callback) override { _props.keyCallback = std::move(callback); }
            void setMouseClickCallback(MouseClickCallback callback) override { _props.mouseClickCallback = std::move(callback); }
            void setMouseScrollCallback(MouseScrollCallback callback) override { _props.mouseScrollCallback = std::move(callback); }
            void setMouseMoveCallback(MouseMoveCallback callback) override { _props.mouseMoveCallback = std::move(callback); }
        private:
            GLFWwindow *_openGlWindow{};
            WindowProperty _props;

            void setupCallback() const;
    };
}
