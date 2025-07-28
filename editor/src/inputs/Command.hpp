//// Command.hpp ///////////////////////////////////////////////////////////////
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
                std::string description,
                const std::string &key,
                const std::function<void()> &pressedCallback,
                const std::function<void()> &releaseCallback,
                const std::function<void()> &repeatCallback,
                bool isModifier = false,
                const std::vector<Command> &childrens = {}
            );

            [[nodiscard]] bool exactMatch(const std::bitset<ImGuiKey_NamedKey_COUNT> &inputSignature) const;
            [[nodiscard]] bool partialMatch(const std::bitset<ImGuiKey_NamedKey_COUNT> &inputSignature) const;
            void executePressedCallback() const;
            void executeReleasedCallback() const;
            void executeRepeatCallback() const;
            [[nodiscard]] std::span<const Command> getChildren() const;
            [[nodiscard]] const std::bitset<ImGuiKey_NamedKey_COUNT> &getSignature() const;
            [[nodiscard]] const std::string &getKey() const;
            [[nodiscard]] const std::string &getDescription() const;
            [[nodiscard]] bool isModifier() const;

            class Builder {
                public:
                    Builder& description(std::string val) { desc = std::move(val); return *this; }
                    Builder& key(std::string val) { k = std::move(val); return *this; }
                    Builder& onPressed(const std::function<void()> &cb) { pressed = cb; return *this; }
                    Builder& onReleased(const std::function<void()> &cb) { released = cb; return *this; }
                    Builder& onRepeat(const std::function<void()> &cb) { repeat = cb; return *this; }
                    Builder& modifier(const bool val) { mod = val; return *this; }
                    Builder& addChild(Command child) { children.push_back(std::move(child)); return *this; }

                    [[nodiscard]] Command build() const {
                        return {desc, k, pressed, released, repeat, mod, children};
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

            static Builder create() { return {}; }

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
