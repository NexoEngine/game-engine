//// Bounds.hpp ///////////////////////////////////////////////////////////////
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
//  Date:        04/10/2025
//  Description: Header file for the bounds utils
//
///////////////////////////////////////////////////////////////////////////////

#pragma once
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace nexo::math {
    /**
     * @struct AABB
     * @brief Axis-aligned bounding box defined by min/max corners.
     *
     * Default-constructed boxes are "empty" (min > max) so they can be
     * safely adopted by the first union/merge operation.
     *
     * Members:
     *  - glm::vec3 min — minimum corner (x/y/z).
     *  - glm::vec3 max — maximum corner (x/y/z).
     */
    struct AABB {
        glm::vec3 min{ +FLT_MAX, +FLT_MAX, +FLT_MAX };
        glm::vec3 max{ -FLT_MAX, -FLT_MAX, -FLT_MAX };

        /**
         * @brief Whether this AABB currently represents no valid volume.
         *
         * An AABB is considered empty if any component of min is greater
         * than the corresponding component of max.
         *
         * @return true if empty (e.g., before any points/boxes were merged), false otherwise.
         */
        bool empty() const;
    };

    /**
     * @struct BSphere
     * @brief Bounding sphere (center + radius).
     *
     * Members:
     *  - glm::vec3 c — sphere center.
     *  - float r — non-negative radius.
     */
    struct BSphere {
        glm::vec3 c{0.0f};
        float     r{0.0f};
    };

    /**
     * @brief Component-wise union of two AABBs.
     *
     * If one input is empty, the other is returned. If both are empty,
     * an empty AABB is returned.
     *
     * @param a First AABB.
     * @param b Second AABB.
     * @return AABB that minimally encloses both @p a and @p b.
     */
    AABB aabbUnion(const AABB& a, const AABB& b);

    /**
     * @brief Construct a conservative bounding sphere from an AABB.
     *
     * The sphere center is the AABB center. The radius equals the length
     * of the half-extents (distance to a farthest corner). Suitable for
     * quick frustum/broad-phase rejects.
     *
     * @param b Source AABB (may be empty).
     * @return BSphere enclosing @p b. For an empty AABB, returns {c=(0,0,0), r=0}.
     */
    BSphere sphereFromAABB(const AABB& b);

    /**
     * @brief Transform a local-space AABB by a 4×4 matrix and return a world-space AABB.
     *
     * Interprets the local AABB as an oriented box (center + half-extents) and applies:
     *   world_center = L * local_center + t
     *   world_half_extents = |L| * local_half_extents
     * where L is the upper-left 3×3 of @p M and t is the translation. Using the
     * absolute value of L yields a conservative axis-aligned enclosure under rotation
     * and non-uniform scale.
     *
     * @param local Local-space AABB.
     * @param M     Transform from local to world space.
     * @return World-space AABB that encloses the transformed local box.
     */
    AABB aabbTransform(const AABB& local, const glm::mat4& M);
}
