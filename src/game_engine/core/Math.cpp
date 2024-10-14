/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** Math
*/

#include "game_engine/GameEngine.hpp"
#include "game_engine/Math.hpp"

namespace engine {
    Matrix math::matrixFromFloat16(const float16& matrix)
    {
        Matrix mat;
        mat.m0 = matrix.v[0];
        mat.m1 = matrix.v[1];
        mat.m2 = matrix.v[2];
        mat.m3 = matrix.v[3];
        mat.m4 = matrix.v[4];
        mat.m5 = matrix.v[5];
        mat.m6 = matrix.v[6];
        mat.m7 = matrix.v[7];
        mat.m8 = matrix.v[8];
        mat.m9 = matrix.v[9];
        mat.m10 = matrix.v[10];
        mat.m11 = matrix.v[11];
        mat.m12 = matrix.v[12];
        mat.m13 = matrix.v[13];
        mat.m14 = matrix.v[14];
        mat.m15 = matrix.v[15];
        return mat;
    }

    void math::ExtractCameraViewComponents(Matrix viewMatrix, Vector3& position, Vector3& target, Vector3& up)
    {
        position.x = viewMatrix.m3;
        // TODO: re-add the below line, temporary fix for Main3DScene
        // position.y = viewMatrix.m7;
        position.z = viewMatrix.m11;

        //Vector3 right = {viewMatrix.m0, viewMatrix.m4, viewMatrix.m8};
        up = {viewMatrix.m1, viewMatrix.m5, viewMatrix.m9};
        Vector3 forward = {viewMatrix.m2, viewMatrix.m6, viewMatrix.m10};

        target = Vector3Add(position, Vector3Negate(forward));
    }

    Matrix math::createTransformMatrix(Vector3 translation, Quaternion rotation, Vector3 scale, bool inDeg)
    {
        Matrix transform = MatrixIdentity();

        Matrix scaleMatrix = MatrixScale(scale.x, scale.y, scale.z);

        Matrix rotationMatrix = QuaternionToMatrix(rotation);

        Matrix translationMatrix = MatrixTranslate(translation.x, translation.y, translation.z);

        transform = MatrixMultiply(scaleMatrix, rotationMatrix);
        transform = MatrixMultiply(transform, translationMatrix);

        return transform;
    }

    void math::decomposeTransformMatrix(Matrix mat, Vector3 &translation, Quaternion &rotation, Vector3 &scale) 
    {
        translation = { mat.m12, mat.m13, mat.m14 };

        scale.x = Vector3Length({ mat.m0, mat.m1, mat.m2 });
        scale.y = Vector3Length({ mat.m4, mat.m5, mat.m6 });
        scale.z = Vector3Length({ mat.m8, mat.m9, mat.m10 });

        Matrix rotationMatrix = mat;

        rotationMatrix.m0 /= scale.x;
        rotationMatrix.m1 /= scale.x;
        rotationMatrix.m2 /= scale.x;

        rotationMatrix.m4 /= scale.y;
        rotationMatrix.m5 /= scale.y;
        rotationMatrix.m6 /= scale.y;

        rotationMatrix.m8 /= scale.z;
        rotationMatrix.m9 /= scale.z;
        rotationMatrix.m10 /= scale.z;

        rotation = QuaternionFromMatrix(rotationMatrix);
    }

    Vector3 math::quaternionToEulerSafe(Quaternion q1)
    {
        double sqw = q1.w * q1.w;
        double sqx = q1.x * q1.x;
        double sqy = q1.y * q1.y;
        double sqz = q1.z * q1.z;
        double unit = sqx + sqy + sqz + sqw;
        double test = q1.x * q1.y + q1.z * q1.w;

        Vector3 eulerAngles;

        if (test > 0.499 * unit) 
        {
            eulerAngles.y = 2 * std::atan2(q1.x, q1.w);
            eulerAngles.z = M_PI / 2;
            eulerAngles.x = 0;
            return eulerAngles;
        }

        if (test < -0.499 * unit) 
        {
            eulerAngles.y = -2 * std::atan2(q1.x, q1.w);
            eulerAngles.z = -M_PI / 2;
            eulerAngles.x = 0;
            return eulerAngles;
        }

        eulerAngles.y = std::atan2(2 * q1.y * q1.w - 2 * q1.x * q1.z, sqx - sqy - sqz + sqw);
        eulerAngles.z = std::asin(2 * test / unit);
        eulerAngles.x = std::atan2(2 * q1.x * q1.w - 2 * q1.y * q1.z, -sqx + sqy - sqz + sqw);
        return eulerAngles;
    }
}