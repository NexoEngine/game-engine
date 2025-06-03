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

        // Get spans for RootComponent and TransformComponent
        const auto rootSpan = get<components::RootComponent>();
        const std::span<const ecs::Entity> entitySpan = m_group->entities();
        const auto &transformComponentArray = get<components::TransformComponent>();

        // Process all root entities in the current scene
        for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
            const ecs::Entity rootEntity = entitySpan[i];

            // Get the root transform
            if (!transformComponentArray->hasComponent(rootEntity)) {
                continue;
            }

            const auto& rootTransform = transformComponentArray->get(rootEntity);

            // Calculate root entity's world matrix
            // For root entities, the local matrix is the world matrix
            glm::mat4 rootWorldMatrix = calculateLocalMatrix(rootTransform);

            // Update the root's world matrix
            auto& mutableRootTransform = coord->getComponent<components::TransformComponent>(rootEntity);
            mutableRootTransform.worldMatrix = rootWorldMatrix;

            // Update children recursively
            updateChildTransforms(rootTransform.children, rootWorldMatrix);
        }
    }

    void TransformHierarchySystem::updateChildTransforms(
        const std::vector<ecs::Entity>& children,
        const glm::mat4& parentWorldMatrix)
    {
        for (const auto& childEntity : children) {
            if (!coord->entityHasComponent<components::TransformComponent>(childEntity)) {
                continue;
            }

            auto& transform = coord->getComponent<components::TransformComponent>(childEntity);

            // Calculate the local transform matrix
            glm::mat4 localMatrix = calculateLocalMatrix(transform);

            // Set the world matrix by combining parent world and local matrix
            transform.worldMatrix = parentWorldMatrix * localMatrix;

            // Recursively update this entity's children
            if (!transform.children.empty()) {
                updateChildTransforms(transform.children, transform.worldMatrix);
            }
        }
    }

    glm::mat4 TransformHierarchySystem::calculateLocalMatrix(const components::TransformComponent& transform) const
    {
        // Standard TRS (Translation-Rotation-Scale) matrix composition
        return glm::translate(glm::mat4(1.0f), transform.pos) *
               glm::toMat4(transform.quat) *
               glm::scale(glm::mat4(1.0f), transform.size);
    }
}
