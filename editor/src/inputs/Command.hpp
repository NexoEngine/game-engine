//// Command.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        23/05/2025
//  Description: Header file for the input commands
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>
#include <bitset>
#include <vector>
#include <functional>
#include <span>

namespace nexo::editor {

    class Command {
        public:
            Command(
                const std::string &description,
                const std::string &key,
                const std::function<void()> &pressedCallback,
                const std::function<void()> &releaseCallback,
                const std::function<void()> &repeatCallback,
                bool isModifier = false,
                const std::vector<Command> &childrens = {}
            );

            bool exactMatch(const std::bitset<ImGuiKey_NamedKey_COUNT> &inputSignature) const;
            bool partialMatch(const std::bitset<ImGuiKey_NamedKey_COUNT> &inputSignature) const;
            void executePressedCallback() const;
            void executeReleasedCallback() const;
            void executeRepeatCallback() const;
            const std::span<const Command> getChildren() const;
            const std::bitset<ImGuiKey_NamedKey_COUNT> &getSignature() const;
            const std::string &getKey() const;
            const std::string &getDescription() const;
            bool isModifier() const;

            class Builder {
                public:
                    Builder& description(std::string val) { desc = std::move(val); return *this; }
                    Builder& key(std::string val) { k = std::move(val); return *this; }
                    Builder& onPressed(std::function<void()> cb) { pressed = cb; return *this; }
                    Builder& onReleased(std::function<void()> cb) { released = cb; return *this; }
                    Builder& onRepeat(std::function<void()> cb) { repeat = cb; return *this; }
                    Builder& modifier(bool val) { mod = val; return *this; }
                    Builder& addChild(Command child) { children.push_back(std::move(child)); return *this; }

                    Command build() const {
                        return Command(desc, k, pressed, released, repeat, mod, children);
                    }

                private:
                    std::string desc;
                    std::string k;
                    std::function<void()> pressed = nullptr;
                    std::function<void()> released = nullptr;
                    std::function<void()> repeat = nullptr;
                    bool mod = false;
                    std::vector<Command> children;
            };

            static Builder create() { return Builder(); }

        private:
            std::bitset<ImGuiKey_NamedKey_COUNT> m_signature;
            std::string m_description;
            std::string m_key;
            std::function<void()> m_pressedCallback;
            std::function<void()> m_releaseCallback;
            std::function<void()> m_repeatCallback;
            bool m_isModifier;
            std::vector<Command> m_childrens;
    };
}
