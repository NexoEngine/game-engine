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

    class EventWindowClose final : public Event<EventWindowClose> {};

    class EventWindowResize final : public Event<EventWindowResize> {
        public:
            EventWindowResize(const int width, const int height) : width(width), height(height) {};

            int width;
            int height;

            friend std::ostream &operator<<(std::ostream &os, const EventWindowResize &event)
            {
                os << "[RESIZE WINDOW EVENT]: " << event.width << "x" << event.height;
                return os;
            }
    };

    enum KeyAction {
        PRESSED,
        RELEASED,
        REPEAT
    };

    std::ostream &operator<<(std::ostream &os, const KeyAction &action);

    enum class KeyMods {
        NONE    = 0,
        SHIFT   = GLFW_MOD_SHIFT,
        CONTROL = GLFW_MOD_CONTROL,
        ALT     = GLFW_MOD_ALT,
    };

    std::ostream &operator<<(std::ostream &os, const KeyMods &mod);

    class EventKey final : public Event<EventKey> {
        public:
            EventKey() = default;

            EventKey(const int keycode, const KeyAction action, const int mods) : keycode(keycode), action(action),
                                                                                  mods(mods) {};

            [[nodiscard]] bool hasMod(KeyMods mod) const
            {
                return (mods & static_cast<int>(mod));
            }

            int keycode{};
            KeyAction action{};
            int mods{};

            friend std::ostream &operator<<(std::ostream &os, const EventKey &event)
            {
                std::string mod;
                if (event.hasMod(KeyMods::ALT))
                    mod.append("ALT");
                if (event.hasMod(KeyMods::CONTROL))
                {
                    if (!mod.empty())
                        mod.append(" + ");
                    mod.append("CTRL");
                }
                if (event.hasMod(KeyMods::SHIFT))
                {
                    if (!mod.empty())
                        mod.append(" + ");
                    mod.append("SHIFT");
                }
                os << "[KEYBOARD EVENT] : " << event.keycode << " with action : " << event.action << " " << mod;
                return os;
            }
    };

    enum MouseButton {
        LEFT   = 0,
        RIGHT  = 1,
        MIDDLE = 2
    };

    std::ostream &operator<<(std::ostream &os, const MouseButton &button);

    class EventMouseClick final : public Event<EventMouseClick> {
        public:
            EventMouseClick() = default;

            [[nodiscard]] bool hasMod(KeyMods mod) const
            {
                return (mods & static_cast<int>(mod));
            }

            MouseButton button{};
            KeyAction action{};
            int mods{};

            friend std::ostream &operator<<(std::ostream &os, const EventMouseClick &event)
            {
                std::string mod;
                if (event.hasMod(KeyMods::ALT))
                    mod.append("ALT");
                if (event.hasMod(KeyMods::CONTROL))
                {
                    if (!mod.empty())
                        mod.append(" + ");
                    mod.append("CTRL");
                }
                if (event.hasMod(KeyMods::SHIFT))
                {
                    if (!mod.empty())
                        mod.append(" + ");
                    mod.append("SHIFT");
                }
                os << "[MOUSE BUTTON EVENT] : " << event.button << " with action : " << event.action << " " << mod;
                return os;
            }
    };

    class EventMouseScroll final : public Event<EventMouseScroll> {
        public:
            EventMouseScroll(const float xOffset, const float yOffset) : x(xOffset), y(yOffset) {};

            float x;
            float y;

            friend std::ostream &operator<<(std::ostream &os, const EventMouseScroll &scroll)
            {
                os << "[MOUSE SCROLL EVENT] xOffset : " << scroll.x << " yOffset : " << scroll.y;
                return os;
            }
    };

    class EventMouseMove : public Event<EventMouseMove> {
        public:
            EventMouseMove(const float xpos, const float ypos) : x(xpos), y(ypos) {};

            float x;
            float y;

            friend std::ostream &operator<<(std::ostream &os, const EventMouseMove &mouse)
            {
                os << "[MOUSE MOVE EVENT] x : " << mouse.x << " y : " << mouse.y;
                return os;
            }
    };
}
