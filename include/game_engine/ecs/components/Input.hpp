/*
** EPITECH PROJECT, 2023
** B-CPP-500-STG-5-2-rtype-remi.mergen
** File description:
** Input.hpp
*/

#pragma once

#include "my_raylib.h"
#include "../SingletonComponent.hpp"

#include <memory>
#include <array>

namespace ecs {
    namespace components {
        namespace input {
            enum class Keys {
                KeyNull = 0,
                KeyBack = 1,
                KeyVolumeUp = 2,
                KeyVolumeDown = 3,
                KeyApostrophe = 4,
                KeyComma = 5,
                KeyMinus = 6,
                KeyPeriod = 7,
                KeySlash = 8,
                KeyZero = 9,
                KeyOne = 10,
                KeyTwo = 11,
                KeyThree = 12,
                KeyFour = 13,
                KeyFive = 14,
                KeySix = 15,
                KeySeven = 16,
                KeyEight = 17,
                KeyNine = 18,
                KeySemicolon = 19,
                KeyEqual = 20,
                KeyA = 21,
                KeyB = 22,
                KeyC = 23,
                KeyD = 24,
                KeyE = 25,
                KeyF = 26,
                KeyG = 27,
                KeyH = 28,
                KeyI = 29,
                KeyJ = 30,
                KeyK = 31,
                KeyL = 32,
                KeyM = 33,
                KeyN = 34,
                KeyO = 35,
                KeyP = 36,
                KeyQ = 37,
                KeyR = 38,
                KeyS = 39,
                KeyT = 40,
                KeyU = 41,
                KeyV = 42,
                KeyW = 43,
                KeyX = 44,
                KeyY = 45,
                KeyZ = 46,
                KeyLeftBracket = 47,
                KeyBackslash = 48,
                KeyRightBracket = 49,
                KeyGrave = 50,
                KeySpace = 51,
                KeyEscape = 52,
                KeyEnter = 53,
                KeyTab = 54,
                KeyBackspace = 55,
                KeyInsert = 56,
                KeyDelete = 57,
                KeyRight = 58,
                KeyLeft = 59,
                KeyDown = 60,
                KeyUp = 61,
                KeyPageUp = 62,
                KeyPageDown = 63,
                KeyHome = 64,
                KeyEnd = 65,
                KeyCapsLock = 66,
                KeyScrollLock = 67,
                KeyNumLock = 68,
                KeyPrintScreen = 69,
                KeyPause = 70,
                KeyF1 = 71,
                KeyF2 = 72,
                KeyF3 = 73,
                KeyF4 = 74,
                KeyF5 = 75,
                KeyF6 = 76,
                KeyF7 = 77,
                KeyF8 = 78,
                KeyF9 = 79,
                KeyF10 = 80,
                KeyF11 = 81,
                KeyF12 = 82,
                KeyLeftShift = 83,
                KeyLeftControl = 84,
                KeyLeftAlt = 85,
                KeyLeftSuper = 86,
                KeyRightShift = 87,
                KeyRightControl = 88,
                KeyRightAlt = 89,
                KeyRightSuper = 90,
                KeyKbMenu = 91,
                KeyKp0 = 92,
                KeyKp1 = 93,
                KeyKp2 = 94,
                KeyKp3 = 95,
                KeyKp4 = 96,
                KeyKp5 = 97,
                KeyKp6 = 98,
                KeyKp7 = 99,
                KeyKp8 = 100,
                KeyKp9 = 101,
                KeyKpDecimal = 102,
                KeyKpDivide = 103,
                KeyKpMultiply = 104,
                KeyKpSubtract = 105,
                KeyKpAdd = 106,
                KeyKpEnter = 107,
                KeyKpEqual = 108,
                KeyMenu = 109
            };

            struct keyState {
                bool keyPressed = false;
                bool keyReleased = false;
                bool keyDown = false;
                bool keyUp = false;
            };

            enum class MouseButtons {
                MouseLeft = 0,
                MouseRight = 1,
                MouseMiddle = 2,
                MouseButton4 = 3,
                MouseButton5 = 4,
            };

            struct mouseState {
                bool buttonPressed = false;
                bool buttonReleased = false;
                bool buttonDown = false;
                bool buttonUp = false;
            };

            struct Input : public ecs::ISingletonComponent {
                std::array<keyState, 110> keys;
                std::array<mouseState, 5> mouseButtons;
                Vector2 mousePosition = {0.0f, 0.0f};
                Vector2 mouseDelta = {0.0f, 0.0f};
                Vector2 mouseScroll = {0.0f, 0.0f};
            };
        }
    }
}