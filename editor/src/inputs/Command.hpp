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

#include <bitset>
#include <functional>
#include <imgui.h>
#include <span>
#include <vector>

namespace nexo::editor {

    class Command {
       public:
        /** @brief Constructs a Command object with the specified parameters.
         * @param description A brief description of the command.
         * @param key A string representing the key combination for the command (e.g., "Ctrl+S").
         * @param pressedCallback A function to be called when the command is activated (key pressed).
         * @param releaseCallback A function to be called when the command is deactivated (key released).
         * @param repeatCallback A function to be called when the command is repeated (key held down).
         * @param isModifier A boolean indicating if the command is a modifier key (e.g., Ctrl, Shift).
         * @param children A vector of child Command objects representing sub-commands or related commands.
         */
        Command(std::string description, const std::string &key, const std::function<void()> &pressedCallback,
                const std::function<void()> &releaseCallback, const std::function<void()> &repeatCallback,
                bool isModifier = false, const std::vector<Command> &children = {});

        /**
         * @brief Checks if the input signature exactly matches the command's signature.
         * An exact match means that both signatures contain the same keys with no additional keys in either signature.
         * @param inputSignature The bitset representing the current input key combination.
         * @return True if there is an exact match, false otherwise.
         */
        [[nodiscard]] bool exactMatch(const std::bitset<ImGuiKey_NamedKey_COUNT> &inputSignature) const;

        /** @brief Checks if the input signature partially matches the command's signature.
         * A partial match means that all keys in the command's signature are present in the input signature,
         * but the input signature may contain additional keys.
         * @param inputSignature The bitset representing the current input key combination.
         * @return True if there is a partial match, false otherwise.
         */
        [[nodiscard]] bool partialMatch(const std::bitset<ImGuiKey_NamedKey_COUNT> &inputSignature) const;

        /**
         * @brief Executes the pressed callback if it is set.
         */
        void executePressedCallback() const;

        /**
         * @brief Executes the callback associated with the release of the command if it is set.
         */
        void executeReleasedCallback() const;

        /**
         * @brief Executes the repeat callback if it is set.
         */
        void executeRepeatCallback() const;

        /**
         * @brief Retrieves a span of child Command objects.
         * @return A std::span containing the child Command objects.
         */
        [[nodiscard]] std::span<const Command> getChildren() const;

        /**
         * @brief Retrieves the signature bitset representing the command's key combination.
         * @return A const reference to the bitset representing the command's signature.
         */
        [[nodiscard]] const std::bitset<ImGuiKey_NamedKey_COUNT> &getSignature() const;

        /**
         * @brief Retrieves the key string associated with the command.
         * @return A const reference to the key string.
         */
        [[nodiscard]] const std::string &getKey() const;

        /**
         * @brief Retrieves the description of the command.
         * @return A const reference to the command's description string.
         */
        [[nodiscard]] const std::string &getDescription() const;

        /**
         * @brief Checks if the command is a modifier key.
         * @return True if the command is a modifier key, false otherwise.
         */
        [[nodiscard]] bool isModifier() const;

        /**
         * @class Builder
         * @brief A builder class for constructing Command objects with a fluent interface.
         *
         * This class allows for step-by-step construction of Command objects by setting various properties
         * such as description, key, callbacks, modifier status, and child commands.
         */
        class Builder {
           public:
            /**
             * @brief Sets the description of the command.
             * @param val The description string.
             * @return A reference to the Builder instance for method chaining.
             */
            Builder &description(std::string val)
            {
                desc = std::move(val);
                return *this;
            }

            /**
             * @brief Sets the key combination for the command.
             * @param val The key combination string (e.g., "Ctrl+S").
             * @return A reference to the Builder instance for method chaining.
             */
            Builder &key(std::string val)
            {
                k = std::move(val);
                return *this;
            }

            /**
             * @brief Sets the callback function to be executed when the command is pressed.
             * @param cb The function to be called on key press.
             * @return A reference to the Builder instance for method chaining.
             */
            Builder &onPressed(const std::function<void()> &cb)
            {
                pressed = cb;
                return *this;
            }

            /**
             * @brief Sets the callback function to be executed when the command is released.
             * @param cb The function to be called on key release.
             * @return A reference to the Builder instance for method chaining.
             */
            Builder &onReleased(const std::function<void()> &cb)
            {
                released = cb;
                return *this;
            }

            /**
             * @brief Sets the callback function to be executed when the command is repeated (key held down).
             * @param cb The function to be called on key repeat.
             * @return A reference to the Builder instance for method chaining.
             */
            Builder &onRepeat(const std::function<void()> &cb)
            {
                repeat = cb;
                return *this;
            }

            /**
             * @brief Sets whether the command is a modifier key.
             * @param val True if the command is a modifier key, false otherwise.
             * @return A reference to the Builder instance for method chaining.
             */
            Builder &modifier(const bool val)
            {
                mod = val;
                return *this;
            }

            /**
             * @brief Adds a child Command to the current command.
             * @param child The child Command object to be added.
             * @return A reference to the Builder instance for method chaining.
             */
            Builder &addChild(Command child)
            {
                children.push_back(std::move(child));
                return *this;
            }

            /**
             * @brief Builds and returns the constructed Command object.
             * @return The constructed Command object.
             */
            [[nodiscard]] Command build() const
            {
                return {desc, k, pressed, released, repeat, mod, children};
            }

           private:
            std::string desc;
            std::string k;
            std::function<void()> pressed  = nullptr;
            std::function<void()> released = nullptr;
            std::function<void()> repeat   = nullptr;
            bool mod                       = false;
            std::vector<Command> children;
        };

        /**
         * @brief Creates and returns a new Builder instance for constructing Command objects.
         * @return A new Builder instance.
         */
        static Builder create()
        {
            return {};
        }

       private:
        std::bitset<ImGuiKey_NamedKey_COUNT> m_signature;
        std::string m_description;
        std::string m_key;
        std::function<void()> m_pressedCallback;
        std::function<void()> m_releaseCallback;
        std::function<void()> m_repeatCallback;
        bool m_isModifier;
        std::vector<Command> m_children;
    };
} // namespace nexo::editor
