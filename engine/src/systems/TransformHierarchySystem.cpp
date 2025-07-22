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
#include "components/Parent.hpp"
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

        const std::span<const ecs::Entity> entitySpan = m_group->entities();
        const auto &transformComponentArray = get<components::TransformComponent>();

        // Process all root entities in the current scene
        for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
            const ecs::Entity rootEntity = entitySpan[i];
            if (!transformComponentArray->hasComponent(rootEntity))
                continue;

            auto& rootTransform = transformComponentArray->get(rootEntity);
            glm::mat4 rootWorldMatrix = calculateLocalMatrix(rootTransform);
            rootTransform.worldMatrix = rootWorldMatrix;
            updateChildTransforms(transformComponentArray, rootTransform.children, rootWorldMatrix);
        }
    }

    void TransformHierarchySystem::updateChildTransforms(
        const std::shared_ptr<ecs::ComponentArray<components::TransformComponent>> &transformComponentArray,
        const std::vector<ecs::Entity>& children,
        const glm::mat4& parentWorldMatrix)
    {
        for (const auto& childEntity : children) {
            if (!transformComponentArray->hasComponent(childEntity))
                continue;

            auto& transform = transformComponentArray->get(childEntity);

            glm::mat4 localMatrix = calculateLocalMatrix(transform);
            transform.worldMatrix = parentWorldMatrix * localMatrix;

            if (!transform.children.empty())
                updateChildTransforms(transformComponentArray, transform.children, transform.worldMatrix);
        }
    }

    glm::mat4 TransformHierarchySystem::calculateLocalMatrix(const components::TransformComponent& transform) const
    {
        return glm::translate(glm::mat4(1.0f), transform.pos) *
               glm::toMat4(transform.quat) *
               glm::scale(glm::mat4(1.0f), transform.size);
    }
}
