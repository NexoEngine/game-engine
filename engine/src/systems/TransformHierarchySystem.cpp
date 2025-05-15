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
            if (!coord->entityHasComponent<components::TransformComponent>(childEntity) ||
                !coord->entityHasComponent<components::LocalTransformComponent>(childEntity)) {
                continue;
            }

            const auto& localTransform = coord->getComponent<components::LocalTransformComponent>(childEntity);
            auto& worldTransform = coord->getComponent<components::TransformComponent>(childEntity);

            // POSITION: parent_pos + (parent_rotation * (parent_scale * local_pos))
            glm::vec3 scaledLocalPos = localTransform.position * parentWorldTransform.size;
            glm::vec3 rotatedScaledLocalPos = parentWorldTransform.quat * scaledLocalPos;
            worldTransform.pos = parentWorldTransform.pos + rotatedScaledLocalPos;

            // ROTATION: parent_rotation * local_rotation
            worldTransform.quat = glm::normalize(parentWorldTransform.quat * localTransform.rotation);

            // SCALE: parent_scale * local_scale (component-wise multiplication)
            worldTransform.size = parentWorldTransform.size * localTransform.scale;

            if (!childIndex.children.empty())
                updateChildTransforms(childIndex.children, worldTransform);
        }
    }

    glm::mat4 TransformHierarchySystem::createTransformMatrix(const components::TransformComponent &transform)
    {
        return glm::translate(glm::mat4(1.0f), transform.pos) *
               glm::toMat4(transform.quat) *
               glm::scale(glm::mat4(1.0f), transform.size);
    }

    glm::mat4 TransformHierarchySystem::createLocalTransformMatrix(const components::LocalTransformComponent &localTransform)
    {
        return glm::translate(glm::mat4(1.0f), localTransform.position) *
               glm::toMat4(localTransform.rotation) *
               glm::scale(glm::mat4(1.0f), localTransform.scale);
    }
}
