//// TransformHierarchySystem.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        14/05/2025
//  Description: Header file for the transform hierarchy system
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "components/Model.hpp"
#include "ecs/GroupSystem.hpp"
#include "components/Transform.hpp"
#include "components/SceneComponents.hpp"
#include "components/RenderContext.hpp"

namespace nexo::system {
    /**
     * @class TransformHierarchySystem
     * @brief System that handles hierarchical transformations for entities with parents.
     *
     * This system updates the transforms of entities with parent relationships,
     * ensuring child entities inherit the transformations of their parents.
     */
     class TransformHierarchySystem final : public ecs::GroupSystem<
		ecs::Owned<
	        ecs::Read<components::ModelComponent>>,
        ecs::NonOwned<
            ecs::Read<components::TransformComponent>,
           	ecs::Read<components::SceneTag>>,
        ecs::WriteSingleton<components::RenderContext>> {
			public:
                void update();
            private:
                /**
                    * @brief Recursively updates transforms of child entities based on parent transform
                    *
                    * @param children Vector of child indices to process
                    * @param parentTransform The parent's transform component
                    */
                void updateChildTransforms(
                    const std::vector<components::SubMeshIndex>& children,
                    const components::TransformComponent& parentTransform);

                /**
                    * @brief Creates a transformation matrix from a transform component
                    *
                    * @param transform The transform component
                    * @return glm::mat4 The resulting transformation matrix
                    */
                static glm::mat4 createTransformMatrix(const components::TransformComponent& transform);

                /**
                    * @brief Decomposes a transformation matrix into position, rotation, and scale
                    *
                    * @param matrix The transformation matrix to decompose
                    * @param position Output parameter for position
                    * @param rotation Output parameter for rotation quaternion
                    * @param scale Output parameter for scale
                    */
                static void decomposeTransformMatrix(
                    const glm::mat4& matrix,
                    glm::vec3& position,
                    glm::quat& rotation,
                    glm::vec3& scale);

                static glm::mat4 createLocalTransformMatrix(const components::LocalTransformComponent& localTransform);
	};
}
