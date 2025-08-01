//// Gizmo.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        01/06/2025
//  Description: Source file for the gizmo rendering
//
///////////////////////////////////////////////////////////////////////////////

#include "EditorScene.hpp"
#include "components/Parent.hpp"
#include "context/Selector.hpp"
#include "context/ActionManager.hpp"

#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/matrix_decompose.hpp>

namespace nexo::editor {
    // Class-level variables for tracking gizmo state between frames
    bool EditorScene::s_wasUsingGizmo = false;
    ImGuizmo::OPERATION EditorScene::s_lastOperation = ImGuizmo::OPERATION::UNIVERSAL;
    std::unordered_map<ecs::Entity, components::TransformComponent::Memento> EditorScene::s_initialTransformStates;

    static ImGuizmo::OPERATION getActiveGuizmoOperation()
    {
        for (int bitPos = 0; bitPos <= 13; bitPos++)
        {
            auto op = static_cast<ImGuizmo::OPERATION>(1u << bitPos);
            if (ImGuizmo::IsOver(op))
                return op;
        }
        return ImGuizmo::OPERATION::UNIVERSAL;
    }

    static std::optional<ecs::Entity> findEntityWithTransform(const std::vector<int>& entities)
    {
        const auto& coord = nexo::Application::m_coordinator;

        for (const auto& entity : entities) {
            if (coord->tryGetComponent<components::TransformComponent>(entity)) {
                return entity;
            }
        }

        return std::nullopt;
    }

    void EditorScene::setupGizmoContext(const components::CameraComponent& camera) const
    {
        ImGuizmo::SetOrthographic(camera.type == components::CameraType::ORTHOGRAPHIC);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetRect(m_viewportBounds[0].x, m_viewportBounds[0].y, m_contentSize.x, m_contentSize.y);
        ImGuizmo::Enable(true);
    }

    static glm::mat4 getEntityParentWorldMatrix(const ecs::Entity entity)
    {
        const auto& coord = nexo::Application::m_coordinator;

        const auto parentComponent = coord->tryGetComponent<components::ParentComponent>(entity);
        if (!parentComponent)
            return {1.0f}; // No parent, return identity

        const ecs::Entity parentEntity = parentComponent->get().parent;

        if (parentEntity == ecs::INVALID_ENTITY)
            return {1.0f}; // No parent, return identity

        const auto parentTransform = coord->tryGetComponent<components::TransformComponent>(parentEntity);
        if (!parentTransform)
            return {1.0f}; // Parent has no transform, return identity

        return parentTransform->get().worldMatrix;
    }

    static glm::mat4 calculateWorldMatrix(const components::TransformComponent& transform)
    {
        return glm::translate(glm::mat4(1.0f), transform.pos) *
               glm::toMat4(transform.quat) *
               glm::scale(glm::mat4(1.0f), transform.size);
    }

    static void updateEntityWorldMatrix(const ecs::Entity entity)
    {
        const auto& coord = nexo::Application::m_coordinator;
        const auto transform = coord->tryGetComponent<components::TransformComponent>(entity);

        if (!transform)
            return;

        // Get parent's world matrix
        const glm::mat4 parentWorldMatrix = getEntityParentWorldMatrix(entity);

        // Calculate local matrix
        const glm::mat4 localMatrix = calculateWorldMatrix(transform->get());

        // Update world matrix
        transform->get().worldMatrix = parentWorldMatrix * localMatrix;
    }

    static void updateEntityWorldMatrixRecursive(const ecs::Entity entity)
    {
        const auto& coord = nexo::Application::m_coordinator;
        const auto parentComponent = coord->tryGetComponent<components::ParentComponent>(entity);
        if (parentComponent) {
            const ecs::Entity parentEntity = parentComponent->get().parent;
            if (parentEntity != ecs::INVALID_ENTITY) {
                updateEntityWorldMatrixRecursive(parentEntity);
            }
        }
        updateEntityWorldMatrix(entity);
    }

    static void updateLocalTransformFromWorld(components::TransformComponent& transform, const glm::mat4& worldMatrix, const ecs::Entity entity)
    {
        const glm::mat4 parentWorldMatrix = getEntityParentWorldMatrix(entity);

        // Calculate local matrix by inverting parent world matrix and multiplying by entity's world matrix
        const glm::mat4 localMatrix = glm::inverse(parentWorldMatrix) * worldMatrix;

        glm::vec3 skew;
        glm::vec4 perspective;
        glm::decompose(
            localMatrix,
            transform.size,
            transform.quat,
            transform.pos,
            skew,
            perspective
        );

        transform.quat = glm::normalize(transform.quat);
        transform.worldMatrix = worldMatrix;
    }

    float* EditorScene::getSnapSettingsForOperation(const ImGuizmo::OPERATION operation)
    {
        if (m_snapTranslateOn && operation & ImGuizmo::OPERATION::TRANSLATE)
            return &m_snapTranslate.x;
        if (m_snapRotateOn && operation & ImGuizmo::OPERATION::ROTATE) {
            return &m_angleSnap;
        }
        return nullptr;
    }

    void EditorScene::captureInitialTransformStates(const std::vector<int>& entities)
    {
        const auto& coord = nexo::Application::m_coordinator;
        s_initialTransformStates.clear();

        for (const auto& entity : entities) {
            auto transform = coord->tryGetComponent<components::TransformComponent>(entity);
            if (transform) {
                s_initialTransformStates[entity] = transform->get().save();
            }
        }
    }

    void EditorScene::applyTransformToEntities(
        const ecs::Entity sourceEntity,
        const glm::mat4& oldWorldMatrix,
        const glm::mat4& newWorldMatrix,
        const std::vector<int>& targetEntities)
    {
        const auto& coord = Application::m_coordinator;

        const glm::mat4 deltaMatrix = newWorldMatrix * glm::inverse(oldWorldMatrix);

        // Apply to all selected entities except the source
        for (const auto& entity : targetEntities) {
            if (entity == static_cast<int>(sourceEntity)) continue;

            const auto entityTransform = coord->tryGetComponent<components::TransformComponent>(entity);
            if (!entityTransform) continue;

            // Apply world space delta and convert back to local space
            glm::mat4 newEntityWorldMatrix = deltaMatrix * entityTransform->get().worldMatrix;
            updateLocalTransformFromWorld(entityTransform->get(), newEntityWorldMatrix, entity);
        }
    }

    static bool hasTransformChanged(const components::TransformComponent::Memento& before,
                                   const components::TransformComponent::Memento& after)
    {
        return before.position != after.position ||
               before.rotation != after.rotation ||
               before.scale != after.scale;
    }

    void EditorScene::createTransformUndoActions(const std::vector<int>& entities)
    {
        const auto& coord = Application::m_coordinator;
        auto& actionManager = ActionManager::get();

        if (entities.size() > 1) {
            auto groupAction = ActionManager::createActionGroup();
            bool anyChanges = false;

            for (const auto& entity : entities) {
                auto transform = coord->tryGetComponent<components::TransformComponent>(entity);
                if (!transform) continue;

                auto it = s_initialTransformStates.find(entity);
                if (it == s_initialTransformStates.end()) continue;

                auto beforeState = it->second;
                auto afterState = transform->get().save();

                // Check if anything actually changed
                if (hasTransformChanged(beforeState, afterState)) {
                    auto action = std::make_unique<ComponentChangeAction<components::TransformComponent>>(
                        entity, beforeState, afterState);
                    groupAction->addAction(std::move(action));
                    anyChanges = true;
                }
            }

            if (anyChanges) {
                actionManager.recordAction(std::move(groupAction));
            }
        }
        else if (entities.size() == 1) {
            auto entity = entities[0];
            auto transform = coord->tryGetComponent<components::TransformComponent>(entity);

            if (s_initialTransformStates.contains(entity)) {
                auto beforeState = s_initialTransformStates[entity];
                auto afterState = transform->get().save();

                if (hasTransformChanged(beforeState, afterState)) {
                    actionManager.recordComponentChange<components::TransformComponent>(
                        entity, beforeState, afterState);
                }
            }
        }

        // Reset stored states
        s_initialTransformStates.clear();
    }

    void EditorScene::renderGizmo()
    {
        const auto& coord = Application::m_coordinator;
        auto const& selector = Selector::get();

        // Skip if no valid selection
        if (selector.getPrimarySelectionType() == SelectionType::SCENE ||
            selector.getSelectedScene() != m_sceneId ||
            !selector.hasSelection()) {
            return;
        }

        // Find entity with transform component
        const auto& selectedEntities = selector.getSelectedEntities();
        ecs::Entity primaryEntity = selector.getPrimaryEntity();

        auto primaryTransform = coord->tryGetComponent<components::TransformComponent>(primaryEntity);
        if (!primaryTransform) {
            const auto entityWithTransform = findEntityWithTransform(selectedEntities);
            if (!entityWithTransform) return; // No entity with transform found

            primaryEntity = *entityWithTransform;
            primaryTransform = coord->tryGetComponent<components::TransformComponent>(primaryEntity);
        }

        // Make sure world matrix is up to date before manipulating
        updateEntityWorldMatrixRecursive(primaryEntity);

        // Camera setup
        const auto& cameraTransform = coord->getComponent<components::TransformComponent>(m_activeCamera);
        auto& camera = coord->getComponent<components::CameraComponent>(m_activeCamera);

        setupGizmoContext(camera);
        ImGuizmo::SetID(static_cast<int>(primaryEntity));

        glm::mat4 viewMatrix = camera.getViewMatrix(cameraTransform);
        glm::mat4 projectionMatrix = camera.getProjectionMatrix();

        // 1) M₀ = parentWorld * T(pos) * R(quat) * S(size)
        const glm::mat4 parentWorld = getEntityParentWorldMatrix(primaryEntity);
        const glm::mat4 Tpos        = glm::translate(glm::mat4(1.0f), primaryTransform->get().pos);
        const glm::mat4 Rrot        = glm::toMat4(primaryTransform->get().quat);
        const glm::mat4 Sscale      = glm::scale(glm::mat4(1.0f), primaryTransform->get().size);
        const glm::mat4 M0          = parentWorld * Tpos * Rrot * Sscale;

        // 2) “centroid offset” = T(centroidLocal)
        const glm::mat4 C_offset    = glm::translate(glm::mat4(1.0f), primaryTransform->get().localCenter);

        // 3) M1 = M0 * C_offset
        glm::mat4 worldTransformMatrix = M0 * C_offset;

        // (We’ll need “M₀” again after manipulation for decomposing back.)
        const glm::mat4 originalWorldMatrix_ModelOrigin = M0;

        if (!ImGuizmo::IsUsing())
            s_lastOperation = getActiveGuizmoOperation();

        const float* snap = getSnapSettingsForOperation(s_lastOperation);

        if (!s_wasUsingGizmo && ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGuizmo::IsOver())
            captureInitialTransformStates(selectedEntities);

        ImGuizmo::Manipulate(
            glm::value_ptr(viewMatrix),
            glm::value_ptr(projectionMatrix),
            m_currentGizmoOperation,
            m_currentGizmoMode,
            glm::value_ptr(worldTransformMatrix),
            nullptr,
            snap
        );

        const bool isUsingGizmo = ImGuizmo::IsUsing();

        if (isUsingGizmo) {
            // Disable camera movement during manipulation
            camera.active = false;

            const glm::mat4 newWorldMatrix_Centroid = worldTransformMatrix;
            const glm::mat4 invCentroidOffset       = glm::inverse(C_offset);
            const glm::mat4 newWorldMatrix_ModelOrigin = newWorldMatrix_Centroid * invCentroidOffset;

            // Update the primary entity's local transform based on the new world transform
            updateLocalTransformFromWorld(primaryTransform->get(), newWorldMatrix_ModelOrigin, primaryEntity);

            const glm::mat4 deltaMatrix = newWorldMatrix_ModelOrigin * glm::inverse(originalWorldMatrix_ModelOrigin);

            for (const auto entity : selectedEntities) {
                if (static_cast<unsigned int>(entity) == primaryEntity) continue;
                const auto tComp = coord->tryGetComponent<components::TransformComponent>(entity);
                if (!tComp) continue;

                // “OtherEntity_world₀” = tComp->worldMatrix
                glm::mat4 otherWorldMatrix_0 = tComp->get().worldMatrix;
                // “OtherEntity_world₁” = deltaMatrix * otherWorldMatrix_0
                glm::mat4 otherWorldMatrix_1 = deltaMatrix * otherWorldMatrix_0;

                // Now convert that new world matrix back to local space of “entity”:
                updateLocalTransformFromWorld(tComp->get(), otherWorldMatrix_1, entity);
            }
        } else if (s_wasUsingGizmo) {
            // Re-enable camera when done
            camera.active = true;
            createTransformUndoActions(selectedEntities);
        }

        s_wasUsingGizmo = isUsingGizmo;
    }
}
