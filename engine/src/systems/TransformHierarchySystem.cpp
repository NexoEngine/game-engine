//// TransformHierarchySystem.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        14/05/2025
//  Description: Source file for the transform hierarchy system
//
///////////////////////////////////////////////////////////////////////////////

#include "TransformHierarchySystem.hpp"
#include "components/Model.hpp"
#include "components/Transform.hpp"
#include "components/SceneComponents.hpp"
#include "components/RenderContext.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace nexo::system {
    void TransformHierarchySystem::update()
    {
        auto &renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1)
            return;

        const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

        const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
            [](const components::SceneTag& tag) { return tag.id; }
        );
        const auto *partition = scenePartition.getPartition(sceneRendered);
        if (!partition) {
            return;
        }

        const auto modelSpan = get<components::ModelComponent>();
        const std::span<const ecs::Entity> entitySpan = m_group->entities();
        const auto &transformComponentArray = get<components::TransformComponent>();

        for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
            const ecs::Entity rootEntity = entitySpan[i];
            const auto& modelComponent = modelSpan[i];
            const auto& rootTransform = transformComponentArray->get(rootEntity);

            updateChildTransforms(modelComponent.children, rootTransform);
        }
    }

    void TransformHierarchySystem::updateChildTransforms(
        const std::vector<components::SubMeshIndex>& children,
        const components::TransformComponent& parentWorldTransform)
    {
        for (const auto& childIndex : children) {
            const ecs::Entity childEntity = childIndex.child;
            if (!coord->entityHasComponent<components::TransformComponent>(childEntity)) {
                continue;
            }

            auto& transform = coord->getComponent<components::TransformComponent>(childEntity);

            // Simply multiply parent world matrix with child local matrix
            transform.worldMatrix = parentWorldTransform.worldMatrix * transform.localMatrix;

            if (!childIndex.children.empty())
                updateChildTransforms(childIndex.children, transform);
        }
    }
}
