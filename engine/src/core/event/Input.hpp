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

	/**
	* @brief Abstract class for input handling.
	*
	* Provides an interface for querying keyboard and mouse input states. Implementations
	* (e.g., for OpenGL using GLFW) must provide concrete behavior for checking key and mouse
	* states as well as retrieving the mouse position.
	*
	* Usage:
	*  - Call Input::init(window) to initialize the input instance.
	*  - Use the static inline functions (e.g., isKeyPressed) for convenience.
	*/
    class Input {
        public:
            virtual ~Input() = default;

            /**
             * @brief Constructs an Input instance bound to a specific window.
             *
             * @param window Shared pointer to the window for input polling.
             */
            explicit Input(const std::shared_ptr<renderer::Window> &window) : m_window(window) {};

            /**
             * @brief Checks if the specified key is currently pressed.
             *
             * @param keycode The keycode to check.
             * @return true if the key is pressed, false otherwise.
             */
            [[nodiscard]] virtual bool isKeyPressed(int keycode) const = 0;

            /**
             * @brief Checks if the specified key is currently released.
             *
             * @param keycode The keycode to check.
             * @return true if the key is released, false otherwise.
             */
            [[nodiscard]] virtual bool isKeyReleased(int keycode) const = 0;

            /**
             * @brief Checks if the specified key is in a repeat state.
             *
             * @param keycode The keycode to check.
             * @return true if the key is repeating, false otherwise.
             */
            [[nodiscard]] virtual bool isKeyRepeat(int keycode) const = 0;

            /**
             * @brief Checks if the specified mouse button is currently pressed.
             *
             * @param button The mouse button to check.
             * @return true if the mouse button is pressed, false otherwise.
             */
            [[nodiscard]] virtual bool isMouseDown(int button) const = 0;

            /**
             * @brief Checks if the specified mouse button is currently released.
             *
             * @param button The mouse button to check.
             * @return true if the mouse button is released, false otherwise.
             */
            [[nodiscard]] virtual bool isMouseReleased(int button) const = 0;

            /**
             * @brief Retrieves the current mouse position.
             *
             * @return glm::vec2 The (x, y) coordinates of the mouse cursor.
             */
            [[nodiscard]] virtual glm::vec2 getMousePosition() const = 0;

            static std::shared_ptr<Input> getInstance()
            {
                return _instance;
            }

            /**
             * @brief Initializes the input system with the given window.
             *
             * Must be called before any input queries.
             *
             * @param window Shared pointer to the window used for input.
             */
            static void init(const std::shared_ptr<renderer::Window>& window);

        protected:
            std::shared_ptr<renderer::Window> m_window;
        private:
            static std::shared_ptr<Input> _instance;
    };

    // Inline convenience functions that delegate to the Input singleton.
    inline bool isKeyPressed(const int keycode) { return Input::getInstance()->isKeyPressed(keycode); }
    inline bool isKeyReleased(const int keycode) { return Input::getInstance()->isKeyReleased(keycode); }
    inline bool isKeyRepeat(const int keycode) { return Input::getInstance()->isKeyRepeat(keycode); }

    inline bool isMouseDown(const int button) { return Input::getInstance()->isMouseDown(button); }
    inline bool isMouseReleased(const int button) { return Input::getInstance()->isMouseReleased(button); }

    inline glm::vec2 getMousePosition() { return Input::getInstance()->getMousePosition(); }
}
