///////////////////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        23/05/2025
//  Description: Source file for the input command
//
///////////////////////////////////////////////////////////////////////////////

#include "Command.hpp"
#include <imgui.h>
#include <sstream>
#include <algorithm>
#include <unordered_map>
#include <cctype>
#include <iostream>

#include "String.hpp"

namespace nexo::editor {
    Command::Command(
        const std::string &description,
        const std::string &key,
        const std::function<void()> &pressedCallback,
        const std::function<void()> &releaseCallback,
        const std::function<void()> &repeatCallback,
        bool isModifier,
        const std::vector<Command> &children)
    : m_description(description), m_key(key), m_pressedCallback(pressedCallback), m_releaseCallback(releaseCallback), m_repeatCallback(repeatCallback), m_isModifier(isModifier), m_childrens(children)
    {
        // Create a mapping of key names to ImGuiKey values
        static const std::unordered_map<std::string, ImGuiKey> keyMap = {
            // Common modifiers
            {"ctrl", ImGuiKey_LeftCtrl},
            {"control", ImGuiKey_LeftCtrl},
            {"shift", ImGuiKey_LeftShift},
            {"alt", ImGuiKey_LeftAlt},
            {"super", ImGuiKey_LeftSuper},
            {"cmd", ImGuiKey_LeftSuper},
            {"win", ImGuiKey_LeftSuper},

            // Alphanumeric keys
            {"a", ImGuiKey_A}, {"b", ImGuiKey_B}, {"c", ImGuiKey_C}, {"d", ImGuiKey_D},
            {"e", ImGuiKey_E}, {"f", ImGuiKey_F}, {"g", ImGuiKey_G}, {"h", ImGuiKey_H},
            {"i", ImGuiKey_I}, {"j", ImGuiKey_J}, {"k", ImGuiKey_K}, {"l", ImGuiKey_L},
            {"m", ImGuiKey_M}, {"n", ImGuiKey_N}, {"o", ImGuiKey_O}, {"p", ImGuiKey_P},
            {"q", ImGuiKey_Q}, {"r", ImGuiKey_R}, {"s", ImGuiKey_S}, {"t", ImGuiKey_T},
            {"u", ImGuiKey_U}, {"v", ImGuiKey_V}, {"w", ImGuiKey_W}, {"x", ImGuiKey_X},
            {"y", ImGuiKey_Y}, {"z", ImGuiKey_Z},

            // Numbers
            {"0", ImGuiKey_0}, {"1", ImGuiKey_1}, {"2", ImGuiKey_2}, {"3", ImGuiKey_3},
            {"4", ImGuiKey_4}, {"5", ImGuiKey_5}, {"6", ImGuiKey_6}, {"7", ImGuiKey_7},
            {"8", ImGuiKey_8}, {"9", ImGuiKey_9},

            // Function keys
            {"f1", ImGuiKey_F1}, {"f2", ImGuiKey_F2}, {"f3", ImGuiKey_F3}, {"f4", ImGuiKey_F4},
            {"f5", ImGuiKey_F5}, {"f6", ImGuiKey_F6}, {"f7", ImGuiKey_F7}, {"f8", ImGuiKey_F8},
            {"f9", ImGuiKey_F9}, {"f10", ImGuiKey_F10}, {"f11", ImGuiKey_F11}, {"f12", ImGuiKey_F12},

            // Special keys
            {"space", ImGuiKey_Space},
            {"enter", ImGuiKey_Enter},
            {"return", ImGuiKey_Enter},
            {"escape", ImGuiKey_Escape},
            {"esc", ImGuiKey_Escape},
            {"tab", ImGuiKey_Tab},
            {"backspace", ImGuiKey_Backspace},
            {"delete", ImGuiKey_Delete},
            {"insert", ImGuiKey_Insert},
            {"home", ImGuiKey_Home},
            {"end", ImGuiKey_End},
            {"pageup", ImGuiKey_PageUp},
            {"pagedown", ImGuiKey_PageDown},
            {"up", ImGuiKey_UpArrow},
            {"down", ImGuiKey_DownArrow},
            {"left", ImGuiKey_LeftArrow},
            {"right", ImGuiKey_RightArrow},
            {"capslock", ImGuiKey_CapsLock},
            {"numlock", ImGuiKey_NumLock},
            {"printscreen", ImGuiKey_PrintScreen},
            {"pause", ImGuiKey_Pause},

            // Keypad
            {"keypad0", ImGuiKey_Keypad0},
            {"keypad1", ImGuiKey_Keypad1},
            {"keypad2", ImGuiKey_Keypad2},
            {"keypad3", ImGuiKey_Keypad3},
            {"keypad4", ImGuiKey_Keypad4},
            {"keypad5", ImGuiKey_Keypad5},
            {"keypad6", ImGuiKey_Keypad6},
            {"keypad7", ImGuiKey_Keypad7},
            {"keypad8", ImGuiKey_Keypad8},
            {"keypad9", ImGuiKey_Keypad9},
            {"keypad.", ImGuiKey_KeypadDecimal},
            {"keypad+", ImGuiKey_KeypadAdd},
            {"keypad-", ImGuiKey_KeypadSubtract},
            {"keypad*", ImGuiKey_KeypadMultiply},
            {"keypad/", ImGuiKey_KeypadDivide}
        };

        // Split the key string by '+' (e.g., "Ctrl+Shift+S")
        std::istringstream keyStream(key);
        std::string segment;

        while (std::getline(keyStream, segment, '+')) {
            // Trim whitespace
            segment.erase(0, segment.find_first_not_of(" \t"));
            segment.erase(segment.find_last_not_of(" \t") + 1);

            // Convert to lowercase for case-insensitive comparison
            std::transform(segment.begin(), segment.end(), segment.begin(),
                          [](unsigned char c){ return std::tolower(c); });

            // Look up in the map and set the bit in the signature
            auto it = keyMap.find(segment);
            if (it != keyMap.end()) {
                m_signature.set(static_cast<size_t>(it->second - ImGuiKey_NamedKey_BEGIN));
            }
        }
    }

    bool Command::exactMatch(const std::bitset<ImGuiKey_NamedKey_COUNT> &inputSignature) const
    {
        return m_signature == inputSignature;
    }

    bool Command::partialMatch(const std::bitset<ImGuiKey_NamedKey_COUNT> &inputSignature) const
    {
        return (m_signature & inputSignature) == m_signature;
    }

    void Command::executePressedCallback() const
    {
        if (m_pressedCallback)
            m_pressedCallback();
    }

    void Command::executeReleasedCallback() const
    {
        if (m_releaseCallback)
            m_releaseCallback();
    }

    void Command::executeRepeatCallback() const
    {
        if (m_repeatCallback)
            m_repeatCallback();
    }

    const std::span<const Command> Command::getChildren() const
    {
        return std::span<const Command>(m_childrens);
    }

    const std::bitset<ImGuiKey_NamedKey_COUNT> &Command::getSignature() const
    {
        return m_signature;
    }

    const std::string &Command::getKey() const
    {
        return m_key;
    }

    bool Command::isModifier() const
    {
        return m_isModifier;
    }

    const std::string &Command::getDescription() const
    {
        return m_description;
    }

}
