//// Frustum.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        15/02/2026
//  Description: Header file for frustum culling utilities
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>
#include <array>
#include <algorithm>

namespace nexo::math {

    struct Plane {
        glm::vec3 normal{};
        float distance = 0.0f;

        void normalize()
        {
            const float len = glm::length(normal);
            normal /= len;
            distance /= len;
        }

        [[nodiscard]] float distanceTo(const glm::vec3 &point) const
        {
            return glm::dot(normal, point) + distance;
        }
    };

    class Frustum {
        public:
            explicit Frustum(const glm::mat4 &vp)
            {
                extractPlanes(vp);
            }

            /**
             * @brief Tests whether an axis-aligned bounding box intersects the frustum.
             *
             * Uses the p-vertex optimization: for each frustum plane, finds the AABB vertex
             * most in the direction of the plane normal. If that vertex is behind the plane,
             * the entire AABB is outside the frustum.
             *
             * @param aabbMin The minimum corner of the AABB in world space.
             * @param aabbMax The maximum corner of the AABB in world space.
             * @return true if the AABB is at least partially inside the frustum.
             */
            [[nodiscard]] bool intersectsAABB(const glm::vec3 &aabbMin, const glm::vec3 &aabbMax) const
            {
                for (const auto &plane : m_planes)
                {
                    // P-vertex: the corner most in the direction of the plane normal
                    const glm::vec3 pVertex(
                        plane.normal.x >= 0.0f ? aabbMax.x : aabbMin.x,
                        plane.normal.y >= 0.0f ? aabbMax.y : aabbMin.y,
                        plane.normal.z >= 0.0f ? aabbMax.z : aabbMin.z
                    );

                    if (plane.distanceTo(pVertex) < 0.0f)
                        return false;
                }
                return true;
            }

        private:
            std::array<Plane, 6> m_planes;

            /**
             * @brief Extracts frustum planes from a view-projection matrix.
             *
             * Uses the Gribb/Hartmann method to extract and normalize the six frustum planes
             * directly from the combined view-projection matrix.
             */
            void extractPlanes(const glm::mat4 &vp)
            {
                // Left: Row3 + Row0
                m_planes[0].normal.x = vp[0][3] + vp[0][0];
                m_planes[0].normal.y = vp[1][3] + vp[1][0];
                m_planes[0].normal.z = vp[2][3] + vp[2][0];
                m_planes[0].distance = vp[3][3] + vp[3][0];

                // Right: Row3 - Row0
                m_planes[1].normal.x = vp[0][3] - vp[0][0];
                m_planes[1].normal.y = vp[1][3] - vp[1][0];
                m_planes[1].normal.z = vp[2][3] - vp[2][0];
                m_planes[1].distance = vp[3][3] - vp[3][0];

                // Bottom: Row3 + Row1
                m_planes[2].normal.x = vp[0][3] + vp[0][1];
                m_planes[2].normal.y = vp[1][3] + vp[1][1];
                m_planes[2].normal.z = vp[2][3] + vp[2][1];
                m_planes[2].distance = vp[3][3] + vp[3][1];

                // Top: Row3 - Row1
                m_planes[3].normal.x = vp[0][3] - vp[0][1];
                m_planes[3].normal.y = vp[1][3] - vp[1][1];
                m_planes[3].normal.z = vp[2][3] - vp[2][1];
                m_planes[3].distance = vp[3][3] - vp[3][1];

                // Near: Row3 + Row2
                m_planes[4].normal.x = vp[0][3] + vp[0][2];
                m_planes[4].normal.y = vp[1][3] + vp[1][2];
                m_planes[4].normal.z = vp[2][3] + vp[2][2];
                m_planes[4].distance = vp[3][3] + vp[3][2];

                // Far: Row3 - Row2
                m_planes[5].normal.x = vp[0][3] - vp[0][2];
                m_planes[5].normal.y = vp[1][3] - vp[1][2];
                m_planes[5].normal.z = vp[2][3] - vp[2][2];
                m_planes[5].distance = vp[3][3] - vp[3][2];

                for (auto &plane : m_planes)
                    plane.normalize();
            }
    };

    /**
     * @brief Transforms a local-space AABB by a 4x4 matrix to produce a world-space AABB.
     *
     * Uses Arvo's method for efficient AABB-matrix transformation, producing the tightest
     * axis-aligned bounding box that contains the transformed original box.
     */
    inline void transformAABB(const glm::vec3 &localMin, const glm::vec3 &localMax,
                              const glm::mat4 &transform,
                              glm::vec3 &worldMin, glm::vec3 &worldMax)
    {
        // Start with the translation component
        const glm::vec3 translation(transform[3]);
        worldMin = translation;
        worldMax = translation;

        // Apply rotation/scale contribution using Arvo's method
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                const float a = transform[j][i] * localMin[j];
                const float b = transform[j][i] * localMax[j];
                worldMin[i] += std::min(a, b);
                worldMax[i] += std::max(a, b);
            }
        }
    }

} // namespace nexo::math
