//// WindowEvent.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Source file for the windows event
//
///////////////////////////////////////////////////////////////////////////////
#include "WindowEvent.hpp"

namespace nexo::event {

    std::ostream& operator<<(std::ostream& os, const EventWindowResize& event)
    {
        os << "[RESIZE WINDOW EVENT]: " << event.width << "x" << event.height;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const KeyAction& action)
    {
        switch (action)
        {
            case PRESSED: os << "PRESSED"; break;
            case RELEASED: os << "RELEASED"; break;
            case REPEAT: os << "REPEAT"; break;
            default: os << "UNKNOWN_ACTION"; break;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const KeyMods& mod)
    {
        switch (mod)
        {
            case KeyMods::NONE: os << "NONE"; break;
            case KeyMods::SHIFT: os << "SHIFT"; break;
            case KeyMods::CONTROL: os << "CONTROL"; break;
            case KeyMods::ALT: os << "ALT"; break;
            default: os << "UNKNOWN_MOD"; break;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const EventKey& event)
    {
        std::string mod;
        if (event.hasMod(KeyMods::ALT))
            mod.append("ALT");
        if (event.hasMod(KeyMods::CONTROL)) {
            if (!mod.empty())
                mod.append(" + ");
            mod.append("CTRL");
        }
        if (event.hasMod(KeyMods::SHIFT)) {
            if (!mod.empty())
                mod.append(" + ");
            mod.append("SHIFT");
        }
        os << "[KEYBOARD EVENT] : " << event.keycode << " with action : " << event.action << " " << mod;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const MouseButton& button)
    {
        switch (button)
        {
            case LEFT: os << "LEFT"; break;
            case RIGHT: os << "RIGHT"; break;
            case MIDDLE: os << "MIDDLE"; break;
            default: os << "UNKNOWN_MOD"; break;
        }
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const EventMouseClick& event)
    {
        std::string mod;
        if (event.hasMod(KeyMods::ALT))
            mod.append("ALT");
        if (event.hasMod(KeyMods::CONTROL)) {
            if (!mod.empty())
                mod.append(" + ");
            mod.append("CTRL");
        }
        if (event.hasMod(KeyMods::SHIFT)) {
            if (!mod.empty())
                mod.append(" + ");
            mod.append("SHIFT");
        }
        os << "[MOUSE BUTTON EVENT] : " << event.button << " with action : " << event.action << " " << mod;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const EventMouseScroll& scroll)
    {
        os << "[MOUSE SCROLL EVENT] xOffset : " << scroll.x << " yOffset : " << scroll.y;
        return os;
    }

    std::ostream& operator<<(std::ostream& os, const EventMouseMove& mouse)
    {
        os << "[MOUSE MOVE EVENT] x : " << mouse.x << " y : " << mouse.y;
        return os;
    }
}
