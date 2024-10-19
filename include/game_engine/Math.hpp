/*
** EPITECH PROJECT, 2024
** Nexo
** File description:
** Math
*/

#pragma once

#include "game_engine/ecs/Coordinator.hpp"
#include "CameraHandling.hpp"

namespace engine {
    namespace math {
        Matrix matrixFromFloat16(const float16& matrix);

        void ExtractCameraViewComponents(Matrix viewMatrix, Vector3& position, Vector3& target, Vector3& up);

        Matrix createTransformMatrix(Vector3 translation, Quaternion rotation, Vector3 scale, bool inDeg = true);

        Matrix createTransformMatrixEuler(Vector3 translation, Vector3 rotation, Vector3 scale, bool inDeg = true);

        void decomposeTransformMatrix(Matrix mat, Vector3 &translation, Quaternion &rotation, Vector3 &scale); 

        void decomposeTransformMatrixEuler(Matrix mat, Vector3 &outTranslation, Vector3 &outRotation, Vector3 &outScale);

        Vector3 quaternionToEulerSafe(Quaternion q1);

        Ray castRayFromMouse(Vector2 mousePos, std::shared_ptr<engine::core::EngineCamera> engineCamera, float nearPlane = 0.01, float farPlane = 1000.0f);
        Ray castRayFromMouse(Vector2 mousePos, engine::core::EngineCamera engineCamera, float nearPlane = 0.01f, float farPlane = 1000.0f);

        bool rayOBBCollisionFromAABBTransformed(Vector3 rayOrigin, Vector3 rayDirection, Vector3 aabbMin, Vector3 aabbMax, Matrix modelMatrix, float &intersectionDistance);

    }
}

std::ostream& operator<<(std::ostream& os, const Vector2& vec);
std::ostream& operator<<(std::ostream& os, const Vector3& vec);
std::ostream& operator<<(std::ostream& os, const Matrix& mat);