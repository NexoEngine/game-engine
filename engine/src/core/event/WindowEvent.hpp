//// WindowEvent.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the window event class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <iostream>
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "Event.hpp"

namespace nexo::event {
    class AEvent;

    class EventWindowClose final : public Event
    {
        public:
            LISTENABLE();
    };

    class EventWindowResize final : public Event {
        public:
            EventWindowResize(const int width, const int height) : width(width) , height(height) {};
            int width;
            int height;

            LISTENABLE();
    };
    std::ostream& operator<<(std::ostream& os, const EventWindowResize& event);

    enum KeyAction {
        PRESSED,
        RELEASED,
        REPEAT
    };
    std::ostream& operator<<(std::ostream& os, const KeyAction& action);

    enum class KeyMods {
        NONE = 0,
        SHIFT = GLFW_MOD_SHIFT,
        CONTROL = GLFW_MOD_CONTROL,
        ALT = GLFW_MOD_ALT,
    };
    std::ostream& operator<<(std::ostream& os, const KeyMods& mod);

    class EventKey final : public Event {
        public:
            EventKey() = default;
            EventKey(const int keycode, const KeyAction action, const int mods) : keycode(keycode), action(action), mods(mods) {};


            [[nodiscard]] bool hasMod(KeyMods mod) const
            {
                return (mods & static_cast<int>(mod));
            }
            int keycode{};
            KeyAction action{};
            int mods{};

            LISTENABLE();
    };
    std::ostream& operator<<(std::ostream& os, const EventKey& event);

    enum MouseButton {
        LEFT = 0,
        RIGHT = 1,
        MIDDLE = 2
    };
    std::ostream& operator<<(std::ostream& os, const MouseButton& button);

    class EventMouseClick final : public Event {
        public:
            EventMouseClick() = default;


            [[nodiscard]] bool hasMod(KeyMods mod) const
            {
                return (mods & static_cast<int>(mod));
            }

            MouseButton button{};
            KeyAction action{};
            int mods{};

            LISTENABLE();
    };
    std::ostream& operator<<(std::ostream& os, const EventMouseClick& button);

    class EventMouseScroll final : public Event {
        public:
            EventMouseScroll(const float xOffset, const float yOffset) : x(xOffset), y(yOffset) {};


            float x;
            float y;

            LISTENABLE();
    };
    std::ostream& operator<<(std::ostream& os, const EventMouseScroll& scroll);

    class EventMouseMove : public Event {
        public:
            EventMouseMove(const float xpos, const float ypos) : x(xpos), y(ypos) {};


            float x;
            float y;

            LISTENABLE();
    };
    std::ostream& operator<<(std::ostream& os, const EventMouseMove& mouse);
}
