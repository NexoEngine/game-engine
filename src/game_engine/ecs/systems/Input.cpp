/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Input.cpp
*/

#include "game_engine/ecs/components/Input.hpp"
#include "game_engine/ecs/systems/Input.hpp"
#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/GameEngine.hpp"

namespace ecs {
    namespace system {

        using components::input::Keys;
        using components::input::MouseButtons;

        const std::map<ecs::components::input::Keys, KeyboardKey> keysToRlKeys = {
            { ecs::components::input::Keys::KeyNull, KEY_NULL },
            { ecs::components::input::Keys::KeyBack, KEY_BACK },
            { ecs::components::input::Keys::KeyVolumeUp, KEY_VOLUME_UP },
            { ecs::components::input::Keys::KeyVolumeDown, KEY_VOLUME_DOWN },
            { ecs::components::input::Keys::KeyApostrophe, KEY_APOSTROPHE },
            { ecs::components::input::Keys::KeyComma, KEY_COMMA },
            { ecs::components::input::Keys::KeyMinus, KEY_MINUS },
            { ecs::components::input::Keys::KeyPeriod, KEY_PERIOD },
            { ecs::components::input::Keys::KeySlash, KEY_SLASH },
            { ecs::components::input::Keys::KeyZero, KEY_ZERO },
            { ecs::components::input::Keys::KeyOne, KEY_ONE },
            { ecs::components::input::Keys::KeyTwo, KEY_TWO },
            { ecs::components::input::Keys::KeyThree, KEY_THREE },
            { ecs::components::input::Keys::KeyFour, KEY_FOUR },
            { ecs::components::input::Keys::KeyFive, KEY_FIVE },
            { ecs::components::input::Keys::KeySix, KEY_SIX },
            { ecs::components::input::Keys::KeySeven, KEY_SEVEN },
            { ecs::components::input::Keys::KeyEight, KEY_EIGHT },
            { ecs::components::input::Keys::KeyNine, KEY_NINE },
            { ecs::components::input::Keys::KeySemicolon, KEY_SEMICOLON },
            { ecs::components::input::Keys::KeyEqual, KEY_EQUAL },
            { ecs::components::input::Keys::KeyA, KEY_A },
            { ecs::components::input::Keys::KeyB, KEY_B },
            { ecs::components::input::Keys::KeyC, KEY_C },
            { ecs::components::input::Keys::KeyD, KEY_D },
            { ecs::components::input::Keys::KeyE, KEY_E },
            { ecs::components::input::Keys::KeyF, KEY_F },
            { ecs::components::input::Keys::KeyG, KEY_G },
            { ecs::components::input::Keys::KeyH, KEY_H },
            { ecs::components::input::Keys::KeyI, KEY_I },
            { ecs::components::input::Keys::KeyJ, KEY_J },
            { ecs::components::input::Keys::KeyK, KEY_K },
            { ecs::components::input::Keys::KeyL, KEY_L },
            { ecs::components::input::Keys::KeyM, KEY_M },
            { ecs::components::input::Keys::KeyN, KEY_N },
            { ecs::components::input::Keys::KeyO, KEY_O },
            { ecs::components::input::Keys::KeyP, KEY_P },
            { ecs::components::input::Keys::KeyQ, KEY_Q },
            { ecs::components::input::Keys::KeyR, KEY_R },
            { ecs::components::input::Keys::KeyS, KEY_S },
            { ecs::components::input::Keys::KeyT, KEY_T },
            { ecs::components::input::Keys::KeyU, KEY_U },
            { ecs::components::input::Keys::KeyV, KEY_V },
            { ecs::components::input::Keys::KeyW, KEY_W },
            { ecs::components::input::Keys::KeyX, KEY_X },
            { ecs::components::input::Keys::KeyY, KEY_Y },
            { ecs::components::input::Keys::KeyZ, KEY_Z },
            { ecs::components::input::Keys::KeyLeftBracket, KEY_LEFT_BRACKET },
            { ecs::components::input::Keys::KeyBackslash, KEY_BACKSLASH },
            { ecs::components::input::Keys::KeyRightBracket, KEY_RIGHT_BRACKET },
            { ecs::components::input::Keys::KeyGrave, KEY_GRAVE },
            { ecs::components::input::Keys::KeySpace, KEY_SPACE },
            { ecs::components::input::Keys::KeyEscape, KEY_ESCAPE },
            { ecs::components::input::Keys::KeyEnter, KEY_ENTER },
            { ecs::components::input::Keys::KeyTab, KEY_TAB },
            { ecs::components::input::Keys::KeyBackspace, KEY_BACKSPACE },
            { ecs::components::input::Keys::KeyInsert, KEY_INSERT },
            { ecs::components::input::Keys::KeyDelete, KEY_DELETE },
            { ecs::components::input::Keys::KeyRight, KEY_RIGHT },
            { ecs::components::input::Keys::KeyLeft, KEY_LEFT },
            { ecs::components::input::Keys::KeyDown, KEY_DOWN },
            { ecs::components::input::Keys::KeyUp, KEY_UP },
            { ecs::components::input::Keys::KeyPageUp, KEY_PAGE_UP },
            { ecs::components::input::Keys::KeyPageDown, KEY_PAGE_DOWN },
            { ecs::components::input::Keys::KeyHome, KEY_HOME },
            { ecs::components::input::Keys::KeyEnd, KEY_END },
            { ecs::components::input::Keys::KeyCapsLock, KEY_CAPS_LOCK },
            { ecs::components::input::Keys::KeyScrollLock, KEY_SCROLL_LOCK },
            { ecs::components::input::Keys::KeyNumLock, KEY_NUM_LOCK },
            { ecs::components::input::Keys::KeyPrintScreen, KEY_PRINT_SCREEN },
            { ecs::components::input::Keys::KeyPause, KEY_PAUSE },
            { ecs::components::input::Keys::KeyF1, KEY_F1 },
            { ecs::components::input::Keys::KeyF2, KEY_F2 },
            { ecs::components::input::Keys::KeyF3, KEY_F3 },
            { ecs::components::input::Keys::KeyF4, KEY_F4 },
            { ecs::components::input::Keys::KeyF5, KEY_F5 },
            { ecs::components::input::Keys::KeyF6, KEY_F6 },
            { ecs::components::input::Keys::KeyF7, KEY_F7 },
            { ecs::components::input::Keys::KeyF8, KEY_F8 },
            { ecs::components::input::Keys::KeyF9, KEY_F9 },
            { ecs::components::input::Keys::KeyF10, KEY_F10 },
            { ecs::components::input::Keys::KeyF11, KEY_F11 },
            { ecs::components::input::Keys::KeyF12, KEY_F12 },
            { ecs::components::input::Keys::KeyLeftShift, KEY_LEFT_SHIFT },
            { ecs::components::input::Keys::KeyLeftControl, KEY_LEFT_CONTROL },
            { ecs::components::input::Keys::KeyLeftAlt, KEY_LEFT_ALT },
            { ecs::components::input::Keys::KeyLeftSuper, KEY_LEFT_SUPER },
            { ecs::components::input::Keys::KeyRightShift, KEY_RIGHT_SHIFT },
            { ecs::components::input::Keys::KeyRightControl, KEY_RIGHT_CONTROL },
            { ecs::components::input::Keys::KeyRightAlt, KEY_RIGHT_ALT },
            { ecs::components::input::Keys::KeyRightSuper, KEY_RIGHT_SUPER },
            { ecs::components::input::Keys::KeyKbMenu, KEY_KB_MENU },
            { ecs::components::input::Keys::KeyKp0, KEY_KP_0 },
            { ecs::components::input::Keys::KeyKp1, KEY_KP_1 },
            { ecs::components::input::Keys::KeyKp2, KEY_KP_2 },
            { ecs::components::input::Keys::KeyKp3, KEY_KP_3 },
            { ecs::components::input::Keys::KeyKp4, KEY_KP_4 },
            { ecs::components::input::Keys::KeyKp5, KEY_KP_5 },
            { ecs::components::input::Keys::KeyKp6, KEY_KP_6 },
            { ecs::components::input::Keys::KeyKp7, KEY_KP_7 },
            { ecs::components::input::Keys::KeyKp8, KEY_KP_8 },
            { ecs::components::input::Keys::KeyKp9, KEY_KP_9 },
            { ecs::components::input::Keys::KeyKpDecimal, KEY_KP_DECIMAL },
            { ecs::components::input::Keys::KeyKpDivide, KEY_KP_DIVIDE },
            { ecs::components::input::Keys::KeyKpMultiply, KEY_KP_MULTIPLY },
            { ecs::components::input::Keys::KeyKpSubtract, KEY_KP_SUBTRACT },
            { ecs::components::input::Keys::KeyKpAdd, KEY_KP_ADD },
            { ecs::components::input::Keys::KeyKpEnter, KEY_KP_ENTER },
            { ecs::components::input::Keys::KeyKpEqual, KEY_KP_EQUAL },
            { ecs::components::input::Keys::KeyMenu, KEY_MENU }
        };

        void ecs::system::InputSystem::handleInputs() {
            auto &input = _coord->getSingletonComponent<components::input::Input>();
            for (int i = 0; i < 110; i++) {
                input.keys[i].keyPressed = false;
                input.keys[i].keyReleased = false;
                input.keys[i].keyDown = false;
                input.keys[i].keyUp = false;
            }

            for (int i = 0; i < 5; i++) {
                input.mouseButtons[i].buttonPressed = false;
                input.mouseButtons[i].buttonReleased = false;
                input.mouseButtons[i].buttonDown = false;
                input.mouseButtons[i].buttonUp = false;
            }

            for (int i = 0; i < 110; i++) {
                if (IsKeyPressed(keysToRlKeys.at(static_cast<Keys>(i))))
                    input.keys[i].keyPressed = true;
                if (IsKeyReleased(keysToRlKeys.at(static_cast<Keys>(i))))
                    input.keys[i].keyReleased = true;
                if (IsKeyDown(keysToRlKeys.at(static_cast<Keys>(i))))
                    input.keys[i].keyDown = true;
                if (IsKeyUp(keysToRlKeys.at(static_cast<Keys>(i))))
                    input.keys[i].keyUp = true;
            }

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                input.mouseButtons[static_cast<int>(MouseButtons::MouseLeft)].buttonPressed = true;
            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON))
                input.mouseButtons[static_cast<int>(MouseButtons::MouseLeft)].buttonReleased = true;
            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON))
                input.mouseButtons[static_cast<int>(MouseButtons::MouseLeft)].buttonDown = true;
            if (IsMouseButtonUp(MOUSE_LEFT_BUTTON))
                input.mouseButtons[static_cast<int>(MouseButtons::MouseLeft)].buttonUp = true;

            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
                input.mouseButtons[static_cast<int>(MouseButtons::MouseRight)].buttonPressed = true;
            if (IsMouseButtonReleased(MOUSE_RIGHT_BUTTON))
                input.mouseButtons[static_cast<int>(MouseButtons::MouseRight)].buttonReleased = true;
            if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON))
                input.mouseButtons[static_cast<int>(MouseButtons::MouseRight)].buttonDown = true;
            if (IsMouseButtonUp(MOUSE_RIGHT_BUTTON))
                input.mouseButtons[static_cast<int>(MouseButtons::MouseRight)].buttonUp = true;

            Vector2 currentMousePosition = GetMousePosition();
            input.mouseDelta = Vector2Subtract(currentMousePosition, input.mousePosition);
            input.mousePosition = currentMousePosition;

            input.mouseScroll = GetMouseWheelMoveV();
        }
    }
}