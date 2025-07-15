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
#include "components/Parent.hpp"
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
	        ecs::Read<components::RootComponent>>,
        ecs::NonOwned<
            ecs::Write<components::TransformComponent>,
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
                    const std::shared_ptr<ecs::ComponentArray<components::TransformComponent>> &transformComponentArray,
                    const std::vector<ecs::Entity>& children,
                    const glm::mat4& parentWorldMatrix);
                glm::mat4 calculateLocalMatrix(const components::TransformComponent& transform) const;
	};
}
