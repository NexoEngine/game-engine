//// comparison.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/11/2024
//  Description: Header containing some utilitary function for comparing stuff
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>

namespace nexo::utils {
    inline void expectMat4Near(const glm::mat4& a, const glm::mat4& b, const float epsilon = 1e-5f)
    {
        for (int i = 0; i < 4; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                EXPECT_NEAR(a[i][j], b[i][j], epsilon) << "Mismatch at (" << i << ", " << j << ")";
            }
        }
    }

    inline void expectVec4Near(const glm::vec4 &a, const glm::vec4 &b, const float epsilon = 1e-5f)
    {
        EXPECT_NEAR(a.x, b.x, epsilon);
        EXPECT_NEAR(a.y, b.y, epsilon);
        EXPECT_NEAR(a.z, b.z, epsilon);
        EXPECT_NEAR(a.w, b.w, epsilon);
    }

    inline void expectVec3Near(const glm::vec3 &a, const glm::vec3 &b, const float epsilon = 1e-5f)
    {
        EXPECT_NEAR(a.x, b.x, epsilon);
        EXPECT_NEAR(a.y, b.y, epsilon);
        EXPECT_NEAR(a.z, b.z, epsilon);
    }

    inline void expectVec2Near(const glm::vec2 &a, const glm::vec2 &b, const float epsilon = 1e-5f)
    {
        EXPECT_NEAR(a.x, b.x, epsilon);
        EXPECT_NEAR(a.y, b.y, epsilon);
    }

    inline void expectFloatNear(const float a, const float b, const float epsilon = 1e-5f)
    {
        EXPECT_NEAR(a, b, epsilon);
    }

}

#define EXPECT_MAT4_NEAR(a, b, epsilon) nexo::utils::expectMat4Near(a, b, epsilon)
#define EXPECT_VEC4_NEAR(a, b, epsilon) nexo::utils::expectVec4Near(a, b, epsilon)
#define EXPECT_VEC2_NEAR(a, b, epsilon) nexo::utils::expectVec2Near(a, b, epsilon)
#define EXPECT_VEC3_NEAR(a, b, epsilon) nexo::utils::expectVec3Near(a, b, epsilon)
#define EXPECT_FLOAT_NEAR(a, b, epsilon) nexo::utils::expectFloatNear(a, b, epsilon)
