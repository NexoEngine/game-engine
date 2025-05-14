//// Gizmo.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the gizmo rendering
//
///////////////////////////////////////////////////////////////////////////////

#include "EditorScene.hpp"
#include "context/Selector.hpp"
#include "context/ActionManager.hpp"
#include "math/Matrix.hpp"

#include <glm/gtc/type_ptr.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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

    static glm::mat4 buildEntityTransformMatrix(const components::TransformComponent& transform)
    {
        const glm::mat4 rotationMat = glm::toMat4(transform.quat);
        return glm::translate(glm::mat4(1.0f), transform.pos) *
               rotationMat *
               glm::scale(glm::mat4(1.0f), transform.size);
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
        const components::TransformComponent& sourceTransform,
        const components::TransformComponent& newTransform,
        const std::vector<int>& targetEntities) const
    {
        const auto& coord = nexo::Application::m_coordinator;

        // Calculate transformation deltas
        const glm::vec3 positionDelta = newTransform.pos - sourceTransform.pos;
        const glm::vec3 scaleFactor = newTransform.size / sourceTransform.size;
        const glm::quat rotationDelta = newTransform.quat * glm::inverse(sourceTransform.quat);

        // Apply transforms to all selected entities except the source
        for (const auto& entity : targetEntities) {
            if (entity == static_cast<int>(sourceEntity)) continue;

            auto entityTransform = coord->tryGetComponent<components::TransformComponent>(entity);
            if (!entityTransform) continue;

            // Apply relevant transformations based on current operation
            if (m_currentGizmoOperation & ImGuizmo::OPERATION::TRANSLATE) {
                entityTransform->get().pos += positionDelta;
            }

            if (m_currentGizmoOperation & ImGuizmo::OPERATION::ROTATE) {
                entityTransform->get().quat = rotationDelta * entityTransform->get().quat;
            }

            if (m_currentGizmoOperation & ImGuizmo::OPERATION::SCALE) {
                entityTransform->get().size.x *= scaleFactor.x;
                entityTransform->get().size.y *= scaleFactor.y;
                entityTransform->get().size.z *= scaleFactor.z;
            }
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
        const auto& coord = nexo::Application::m_coordinator;
        auto& actionManager = ActionManager::get();

        // If multiple entities selected, create a group action
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
        // Single entity selected - simpler action
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
        const auto& coord = nexo::Application::m_coordinator;
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

        // Camera setup
        const auto& cameraTransform = coord->getComponent<components::TransformComponent>(m_activeCamera);
        auto& camera = coord->getComponent<components::CameraComponent>(m_activeCamera);

        // Configure ImGuizmo
        setupGizmoContext(camera);
        ImGuizmo::SetID(static_cast<int>(primaryEntity));

        // Prepare matrices
        glm::mat4 viewMatrix = camera.getViewMatrix(cameraTransform);
        glm::mat4 projectionMatrix = camera.getProjectionMatrix();
        glm::mat4 transformMatrix = buildEntityTransformMatrix(primaryTransform->get());

        // Track which operation is active
        if (!ImGuizmo::IsUsing()) {
            s_lastOperation = getActiveGuizmoOperation();
        }

        // Get snap settings if applicable
        const float* snap = getSnapSettingsForOperation(s_lastOperation);

        // Capture initial state when starting to use gizmo
        if (!s_wasUsingGizmo && ImGui::IsMouseDown(ImGuiMouseButton_Left) && ImGuizmo::IsOver()) {
            captureInitialTransformStates(selectedEntities);
        }

        // Perform the actual manipulation
        ImGuizmo::Manipulate(
            glm::value_ptr(viewMatrix),
            glm::value_ptr(projectionMatrix),
            m_currentGizmoOperation,
            m_currentGizmoMode,
            glm::value_ptr(transformMatrix),
            nullptr,
            snap
        );

        // Update isUsingGizmo after manipulation
        bool isUsingGizmo = ImGuizmo::IsUsing();

        if (isUsingGizmo) {
            // Disable camera movement during manipulation
            camera.active = false;

            // Extract the original transform values
            const components::TransformComponent originalTransform = primaryTransform->get();

            // Extract the new transform values from the matrix
            glm::vec3 newPos;
            glm::vec3 newScale;
            glm::quat newRot;
            math::decomposeTransformQuat(transformMatrix, newPos, newRot, newScale);

            // Update the primary entity's transform
            primaryTransform->get().pos = newPos;
            primaryTransform->get().quat = newRot;
            primaryTransform->get().size = newScale;

            // Apply changes to other selected entities
            applyTransformToEntities(
                primaryEntity,
                originalTransform,
                primaryTransform->get(),
                selectedEntities
            );
        }
        else if (s_wasUsingGizmo) {
            // Re-enable camera when done
            camera.active = true;

            // Create undo/redo actions
            createTransformUndoActions(selectedEntities);
        }

        // Update state for next frame
        s_wasUsingGizmo = isUsingGizmo;
    }
}
