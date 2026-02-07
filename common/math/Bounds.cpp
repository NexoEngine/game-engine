//// Bounds.cpp ///////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        05/10/2025
//  Description: Source file for the bounds utils
//
///////////////////////////////////////////////////////////////////////////////

#include <algorithm>
#include <cmath>

#include "Bounds.hpp"

namespace nexo::math {
    bool AABB::empty() const
    {
        return min.x > max.x || min.y > max.y || min.z > max.z;
    }

    AABB aabbUnion(const AABB& a, const AABB& b)
    {
        if (a.empty())
            return b;
        if (b.empty())
            return a;

        AABB o;
        o.min = glm::min(a.min, b.min);
        o.max = glm::max(a.max, b.max);
        return o;
    }

    BSphere sphereFromAABB(const AABB& b)
    {
        BSphere s{};
        if (b.empty())
            return s;
        s.c = 0.5f * (b.min + b.max);
        const glm::vec3 e = 0.5f * (b.max - b.min);
        s.r = glm::length(e);
        return s;
    }

    AABB aabbTransform(const AABB& local, const glm::mat4& M)
    {
        if (local.empty())
            return {};
        const glm::vec3 lc = 0.5f * (local.min + local.max);
        const glm::vec3 le = 0.5f * (local.max - local.min);

        const glm::vec3 t = { M[3][0], M[3][1], M[3][2] };
        // upper-left 3x3
        const auto L = glm::mat3(M);

        // world center
        const glm::vec3 wc = L * lc + t;
        const auto A  = glm::mat3( glm::abs(L[0][0]), glm::abs(L[0][1]), glm::abs(L[0][2]),
                                        glm::abs(L[1][0]), glm::abs(L[1][1]), glm::abs(L[1][2]),
                                        glm::abs(L[2][0]), glm::abs(L[2][1]), glm::abs(L[2][2]) );
        const glm::vec3 we  = A * le;

        AABB out;
        out.min = wc - we;
        out.max = wc + we;
        return out;
    }

    static float maxAxisScale(const glm::mat4& M)
    {
        const float sx = glm::length(glm::vec3(M[0])); // column 0
        const float sy = glm::length(glm::vec3(M[1])); // column 1
        const float sz = glm::length(glm::vec3(M[2])); // column 2
        return std::max(sx, std::max(sy, sz));
    }

    BSphere sphereTransform(const BSphere& s, const glm::mat4& M)
    {
        BSphere out;
        out.c = glm::vec3(M * glm::vec4(s.c, 1.0f));
        out.r = s.r * maxAxisScale(M); // conservative under non-uniform scale
        return out;
    }
}
