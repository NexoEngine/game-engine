/*
** EPITECH PROJECT, 2024
** game-engine
** File description:
** Math
*/

#include "game_engine/GameEngine.hpp"
#include "game_engine/Math.hpp"
#ifndef M_PI
    #define M_PI 3.14159265358979323846
#endif

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

    Matrix math::createTransformMatrixEuler(Vector3 translation, Vector3 rotation, Vector3 scale, bool inDeg)
    {
        Matrix transform = MatrixIdentity();

        Matrix scaleMatrix = MatrixScale(scale.x, scale.y, scale.z);

        Matrix rotationMatrix = MatrixRotateZYX(rotation);

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

    void math::decomposeTransformMatrixEuler(Matrix mat, Vector3 &outTranslation, Vector3 &outRotation, Vector3 &outScale)
    {
        // Extract translation
        outTranslation = { mat.m12, mat.m13, mat.m14 };

        // Extract scale (length of each row)
        outScale.x = Vector3Length({ mat.m0, mat.m1, mat.m2 });
        outScale.y = Vector3Length({ mat.m4, mat.m5, mat.m6 });
        outScale.z = Vector3Length({ mat.m8, mat.m9, mat.m10 });

        // Remove scale factor from each row to prepare for rotation extraction
        Vector3 rowX = {mat.m0, mat.m1, mat.m2};
        rowX = Vector3Scale(rowX, 1.0f / outScale.x);

        Vector3 rowY = {mat.m4, mat.m5, mat.m6};
        rowY = Vector3Scale(rowX, 1.0f / outScale.y);

        Vector3 rowZ = {mat.m8, mat.m9, mat.m10};
        rowZ = Vector3Scale(rowX, 1.0f / outScale.z);

        // Reset projection
        Vector3 tmp_z_axis = Vector3CrossProduct({mat.m0, mat.m1, mat.m2}, {mat.m4, mat.m5, mat.m6});
        if (Vector3DotProduct(tmp_z_axis, {mat.m8, mat.m9, mat.m10}) < 0) {
            outScale.x *= -1;
            mat.m0 = -mat.m0;
            mat.m1 = -mat.m1;
            mat.m2 = -mat.m2;
        }

        // Extract rotation
        outRotation.x = atan2(mat.m6, mat.m10);
        float c2 = sqrt(mat.m0 * mat.m0 + mat.m1 * mat.m1);
        outRotation.y = atan2(-mat.m2, c2);
        float s1 = sin(outRotation.x);
        float c1 = cos(outRotation.x);
        outRotation.z = atan2(s1 * mat.m8 - c1 * mat.m4, c1 * mat.m5 - s1 * mat.m9);
    }
}

std::ostream& operator<<(std::ostream& os, const Vector3& vec) {
    os << "Vector3(" << vec.x << ", " << vec.y << ", " << vec.z << ")";
    return os;
}

std::ostream& operator<<(std::ostream& os, const Matrix& mat) {
    os << "Matrix(\n";
    os << "  [" << mat.m0  << ", " << mat.m1  << ", " << mat.m2  << ", " << mat.m3  << "]\n";
    os << "  [" << mat.m4  << ", " << mat.m5  << ", " << mat.m6  << ", " << mat.m7  << "]\n";
    os << "  [" << mat.m8  << ", " << mat.m9  << ", " << mat.m10 << ", " << mat.m11 << "]\n";
    os << "  [" << mat.m12 << ", " << mat.m13 << ", " << mat.m14 << ", " << mat.m15 << "]\n";
    os << ")";
    return os;
}