//// TransformSystem.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      iMeaNz
//  Date:        09/11/2025
//  Description: Unified transform system implementation
//
///////////////////////////////////////////////////////////////////////////////

#include "TransformSystem.hpp"
#include "SystemProfiler.hpp"
#include "components/Parent.hpp"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace nexo::system {

    void TransformSystem::update()
    {
        const std::span<const ecs::Entity> entitySpan = m_group->entities();
        PROFILE_SYSTEM("TransformSystem", entitySpan.size());

        const auto& renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1) return;

        const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

        // Get scene partition for cache-friendly access
        const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
            [](const components::SceneTag& tag) { return tag.id; });
        const auto* partition = scenePartition.getPartition(sceneRendered);
        if (!partition) return;

        const auto& transformArray = get<components::TransformComponent>();
        auto rootSpan = get<components::RootComponent>();

        for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i) {
            ecs::Entity rootEntity = entitySpan[i];
            auto& rootTransform = transformArray->get(rootEntity);
            auto &rootComp = rootSpan[i];
            if (rootTransform.dirty) {
                const glm::mat4 localMatrix = calculateLocalMatrix(rootTransform);
                rootTransform.localMatrix = localMatrix;
                rootTransform.worldMatrix = localMatrix;
                for (const ecs::Entity childEntity : rootTransform.children) {
                    updateHierarchyRecursive(transformArray, childEntity, rootTransform.worldMatrix, true);
                }
            }
            if (!rootTransform.dirty && rootComp.hierarchyDirty) {
                for (const ecs::Entity childEntity : rootTransform.children) {
                    if (lookForDirtyChildren(transformArray, childEntity, rootTransform.worldMatrix)) {
                        break;
                    }
                }
                rootComp.hierarchyDirty = false;
            }
            rootTransform.dirty = false;
        }
    }

    bool TransformSystem::lookForDirtyChildren(
        const std::shared_ptr<ecs::ComponentArray<components::TransformComponent>>& transformArray,
        ecs::Entity entity,
        const glm::mat4& parentWorldMatrix)
    {
        auto& transform = transformArray->get(entity);

        // Update only if dirty or forced (parent was dirty)
        if (!transform.dirty) {
            for (const ecs::Entity childEntity : transform.children) {
                if (lookForDirtyChildren(transformArray, childEntity, transform.worldMatrix))
                    return true;
            }
            return false;
        }

        // Calculate local transformation matrix
        const glm::mat4 localMatrix = calculateLocalMatrix(transform);
        transform.localMatrix = localMatrix;

        // Combine with parent's world matrix
        transform.worldMatrix = parentWorldMatrix * localMatrix;
        transform.dirty = false;

        // Propagate to children if any exist
        for (const ecs::Entity childEntity : transform.children) {
            // Force dirty on children since this transform changed
            updateHierarchyRecursive(transformArray, childEntity, transform.worldMatrix, true);
        }
        return true;

    }

    void TransformSystem::updateHierarchyRecursive(
        const std::shared_ptr<ecs::ComponentArray<components::TransformComponent>>& transformArray,
        ecs::Entity entity,
        const glm::mat4& parentWorldMatrix,
        bool forceDirty)
    {
        auto& transform = transformArray->get(entity);

        // Update only if dirty or forced (parent was dirty)
        if (!transform.dirty && !forceDirty) return;

        // Calculate local transformation matrix
        const glm::mat4 localMatrix = calculateLocalMatrix(transform);
        transform.localMatrix = localMatrix;

        // Combine with parent's world matrix
        transform.worldMatrix = parentWorldMatrix * localMatrix;
        transform.dirty = false;

        // Propagate to children if any exist
        if (!transform.children.empty()) {
            for (const ecs::Entity childEntity : transform.children) {
                // Force dirty on children since this transform changed
                updateHierarchyRecursive(transformArray, childEntity, transform.worldMatrix, true);
            }
        }
    }

    glm::mat4 TransformSystem::calculateLocalMatrix(const components::TransformComponent& transform)
    {
        // Optimized matrix calculation: T * R * S
        return glm::translate(glm::mat4(1.0f), transform.pos) *
               glm::toMat4(transform.quat) *
               glm::scale(glm::mat4(1.0f), transform.size);
    }

} // namespace nexo::system
