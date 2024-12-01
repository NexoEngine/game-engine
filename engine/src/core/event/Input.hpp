//// Input.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for the input class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/vec2.hpp>

#include "renderer/Window.hpp"

namespace nexo::event {

    class Input {
        public:
            virtual ~Input() = default;

            explicit Input(const std::shared_ptr<renderer::Window> &window) : m_window(window) {};

            [[nodiscard]] virtual bool isKeyPressed(int keycode) const = 0;
            [[nodiscard]] virtual bool isKeyReleased(int keycode) const = 0;
            [[nodiscard]] virtual bool isKeyRepeat(int keycode) const = 0;

            [[nodiscard]] virtual bool isMouseDown(int button) const = 0;
            [[nodiscard]] virtual bool isMouseReleased(int button) const = 0;

            [[nodiscard]] virtual glm::vec2 getMousePosition() const = 0;

            static std::shared_ptr<Input> getInstance()
            {
                return _instance;
            }

            static void init(const std::shared_ptr<renderer::Window>& window);

        protected:
            std::shared_ptr<renderer::Window> m_window;
        private:
            static std::shared_ptr<Input> _instance;
    };

    inline bool isKeyPressed(const int keycode) { return Input::getInstance()->isKeyPressed(keycode); }
    inline bool isKeyReleased(const int keycode) { return Input::getInstance()->isKeyReleased(keycode); }
    inline bool isKeyRepeat(const int keycode) { return Input::getInstance()->isKeyRepeat(keycode); }

    inline bool isMouseDown(const int button) { return Input::getInstance()->isMouseDown(button); }
    inline bool isMouseReleased(const int button) { return Input::getInstance()->isMouseReleased(button); }

    inline glm::vec2 getMousePosition() { return Input::getInstance()->getMousePosition(); }
}
