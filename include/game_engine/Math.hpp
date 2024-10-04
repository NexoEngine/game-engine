/*
** EPITECH PROJECT, 2024
** Nexo
** File description:
** Math
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"

namespace engine {
    Matrix matrixFromFloat16(const float16& matrix);

    namespace math {
        Matrix matrixFromFloat16(const float16& matrix);

        void ExtractCameraViewComponents(Matrix viewMatrix, Vector3& position, Vector3& target, Vector3& up);
    }
}