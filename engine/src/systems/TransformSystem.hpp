//// TransformSystem.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      iMeaNz
//  Date:        09/11/2025
//  Description: Unified transform system handling both matrix updates and hierarchy
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "components/Parent.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/Transform.hpp"
#include "ecs/GroupSystem.hpp"
#include <vector>
#include <glm/mat4x4.hpp>

namespace nexo::system {
    /**
     * @class TransformSystem
     * @brief Unified system that efficiently handles both transform matrix updates and hierarchical relationships.
     *
     * This system combines the functionality of TransformMatrixSystem and TransformHierarchySystem
     * into a single optimized pass that:
     * - Updates transform matrices only when dirty
     * - Propagates transformations through hierarchy efficiently
     * - Minimizes redundant calculations
     * - Uses cache-friendly data access patterns
     */
    class TransformSystem final
        : public ecs::GroupSystem<
              ecs::Owned<ecs::Write<components::RootComponent>>,
              ecs::NonOwned<ecs::Write<components::TransformComponent>, ecs::Read<components::SceneTag>>,
              ecs::WriteSingleton<components::RenderContext>> {
       public:
        void update();

       private:
        /**
         * @brief Creates a transformation matrix from position, rotation, and scale
         * @param transform The TransformComponent containing transformation data
         * @return glm::mat4 The resulting transformation matrix
         */
        [[nodiscard]] static glm::mat4 calculateLocalMatrix(const components::TransformComponent& transform);

        /**
         * @brief Recursively updates transforms of child entities with dirty flag propagation
         * @param transformArray Component array for direct access
         * @param entity Current entity to process
         * @param parentWorldMatrix World matrix of the parent
         * @param forceDirty Whether to force update regardless of dirty flag
         */
        static void updateHierarchyRecursive(
            const std::shared_ptr<ecs::ComponentArray<components::TransformComponent>>& transformArray,
            ecs::Entity entity,
            const glm::mat4& parentWorldMatrix,
            bool forceDirty);

        static bool lookForDirtyChildren(
            const std::shared_ptr<ecs::ComponentArray<components::TransformComponent>>& transformArray,
            ecs::Entity entity,
            const glm::mat4& parentWorldMatrix);
    };
} // namespace nexo::system
