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

#include <memory>

#include "core/event/Event.hpp"
#include "core/event/WindowEvent.hpp"

namespace nexo::renderer {

    struct WindowProperty
    {
        unsigned int width;
        unsigned int height;
        const char *title;
        bool vsync{};

        std::shared_ptr<event::EventManager> eventManager;

        WindowProperty(const unsigned int w, const unsigned h, const char * t) : width(w), height(h), title(t) {}
    };

    class Window {
        public:
            Window() = default;

            virtual ~Window() = default;

            virtual void init(std::shared_ptr<nexo::event::EventManager> eventManager) = 0;
            virtual void shutdown() = 0;
            virtual void onUpdate() = 0;

            [[nodiscard]] virtual unsigned int getWidth() const = 0;
            [[nodiscard]] virtual unsigned int getHeight() const = 0;

            virtual void getDpiScale(float *x, float *y) const = 0;

            virtual void setVsync(bool enabled) = 0;
            [[nodiscard]] virtual bool isVsync() const = 0;


            [[nodiscard]] virtual bool isOpen() const = 0;

            [[nodiscard]] virtual void *window() const = 0;

            static std::shared_ptr<Window> create(int width = 1920, int height = 1080, const char *title = "Nexo window");
    };
}
