//// InputOpenGl.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the input polling opengl class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "core/event/Input.hpp"
#include "Logger.hpp"

namespace nexo::event {
    class InputOpenGl final : public Input {
        public:
        explicit InputOpenGl(const std::shared_ptr<renderer::NxWindow>& window) : Input(window)
        {
            LOG(NEXO_DEV, "Opengl input handler initialized");
        };

        [[nodiscard]] bool isKeyPressed(int keycode) const override;
        [[nodiscard]] bool isKeyReleased(int keycode) const override;
        [[nodiscard]] bool isKeyRepeat(int keycode) const override;

        [[nodiscard]] bool isMouseDown(int button) const override;
        [[nodiscard]] bool isMouseReleased(int button) const override;

        [[nodiscard]] glm::vec2 getMousePosition() const override;
    };
}