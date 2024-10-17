/*
** EPITECH PROJECT, 2024
** Nexo
** File description:
** Math
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"

namespace engine {
    namespace math {
        Matrix matrixFromFloat16(const float16& matrix);

        void ExtractCameraViewComponents(Matrix viewMatrix, Vector3& position, Vector3& target, Vector3& up);

        Matrix createTransformMatrix(Vector3 translation, Quaternion rotation, Vector3 scale, bool inDeg = true);

        Matrix createTransformMatrixEuler(Vector3 translation, Vector3 rotation, Vector3 scale, bool inDeg = true);

        void decomposeTransformMatrix(Matrix mat, Vector3 &translation, Quaternion &rotation, Vector3 &scale); 

        void decomposeTransformMatrixEuler(Matrix mat, Vector3 &outTranslation, Vector3 &outRotation, Vector3 &outScale);

        Vector3 quaternionToEulerSafe(Quaternion q1);

    }
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec);
std::ostream& operator<<(std::ostream& os, const Matrix& mat);