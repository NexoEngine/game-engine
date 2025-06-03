//// Update.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the update of the editor scene
//
///////////////////////////////////////////////////////////////////////////////

#include "EditorScene.hpp"
#include "Types.hpp"
#include "components/Transform.hpp"
#include "context/Selector.hpp"
#include "components/Uuid.hpp"
#include "components/Parent.hpp"

namespace nexo::editor {
    int EditorScene::sampleEntityTexture(const float mx, const float my) const
    {
        const auto &coord = Application::m_coordinator;
        const auto &cameraComponent = coord->getComponent<components::CameraComponent>(static_cast<ecs::Entity>(m_activeCamera));

        cameraComponent.m_renderTarget->bind();
        const int entityId = cameraComponent.m_renderTarget->getPixel<int>(1, static_cast<int>(mx), static_cast<int>(my));
        cameraComponent.m_renderTarget->unbind();
        return entityId;
    }

    static SelectionType getSelectionType(const int entityId)
    {
        const auto &coord = Application::m_coordinator;
        auto selType = SelectionType::ENTITY;
        if (coord->entityHasComponent<components::CameraComponent>(entityId)) {
            selType = SelectionType::CAMERA;
        } else if (coord->entityHasComponent<components::DirectionalLightComponent>(entityId)) {
            selType = SelectionType::DIR_LIGHT;
        } else if (coord->entityHasComponent<components::PointLightComponent>(entityId)) {
            selType = SelectionType::POINT_LIGHT;
        } else if (coord->entityHasComponent<components::SpotLightComponent>(entityId)) {
            selType = SelectionType::SPOT_LIGHT;
        } else if (coord->entityHasComponent<components::AmbientLightComponent>(entityId)) {
            selType = SelectionType::AMBIENT_LIGHT;
        }
        return selType;
    }

    void EditorScene::updateWindowState()
    {
        const auto &selector = Selector::get();
        if (selector.hasSelection()) {
            if (m_currentGizmoOperation == ImGuizmo::OPERATION::TRANSLATE)
                m_windowState = m_gizmoTranslateState;
            else if (m_currentGizmoOperation == ImGuizmo::OPERATION::ROTATE)
                m_windowState = m_gizmoRotateState;
            else if (m_currentGizmoOperation == ImGuizmo::OPERATION::SCALE)
                m_windowState = m_gizmoScaleState;
            else
                m_windowState = m_gizmoState;
        }
    }

    ecs::Entity EditorScene::findRootParent(ecs::Entity entityId) const
    {
        const auto &coord = Application::m_coordinator;
        ecs::Entity currentEntity = entityId;

        // Traverse up the hierarchy until we find an entity without a parent
        while (coord->entityHasComponent<components::ParentComponent>(currentEntity)) {
            const auto& parentComp = coord->getComponent<components::ParentComponent>(currentEntity);
            currentEntity = parentComp.parent;
        }

        return currentEntity;
    }

    void EditorScene::updateSelection(const int entityId, const bool isShiftPressed, const bool isCtrlPressed)
    {
        const auto &coord = Application::m_coordinator;

        const auto uuid = coord->tryGetComponent<components::UuidComponent>(entityId);
        if (!uuid)
            return;

        const bool selectDirectEntity = ImGui::IsKeyDown(ImGuiKey_V); // Hold V to select direct entity
        auto &selector = Selector::get();

        if (selectDirectEntity) {
            const SelectionType selType = getSelectionType(entityId);

            if (isCtrlPressed)
                selector.toggleSelection(uuid->get().uuid, entityId, selType);
            else if (isShiftPressed)
                selector.addToSelection(uuid->get().uuid, entityId, selType);
            else
                selector.selectEntity(uuid->get().uuid, entityId, selType);
        } else {
            // Find root parent
            ecs::Entity rootEntity = findRootParent(entityId);

            if (!isShiftPressed && !isCtrlPressed)
                selector.clearSelection();

            // Recursively select the entire hierarchy
            selectEntityHierarchy(rootEntity, isCtrlPressed);
        }

        updateWindowState();
        selector.setSelectedScene(m_sceneId);
    }

    void EditorScene::selectEntityHierarchy(ecs::Entity entityId, const bool isCtrlPressed)
    {
        const auto &coord = Application::m_coordinator;

        const auto uuid = coord->tryGetComponent<components::UuidComponent>(entityId);
        if (uuid) {
            const SelectionType selType = getSelectionType(entityId);

            auto &selector = Selector::get();
            if (isCtrlPressed)
                selector.toggleSelection(uuid->get().uuid, entityId, selType);
            else
                selector.addToSelection(uuid->get().uuid, entityId, selType);
        }

        // Check if entity has a ModelComponent and process its children
        if (coord->entityHasComponent<components::TransformComponent>(entityId)) {
            const auto& transform = coord->getComponent<components::TransformComponent>(entityId);
            selectModelChildren(transform.children, isCtrlPressed);
        }
    }

    void EditorScene::selectModelChildren(const std::vector<ecs::Entity>& children, const bool isCtrlPressed)
    {
        const auto &coord = Application::m_coordinator;

        for (const auto& entity : children) {
            selectEntityHierarchy(entity, isCtrlPressed);
            const auto &childTransform = coord->getComponent<components::TransformComponent>(entity);

            if (!childTransform.children.empty())
                selectModelChildren(childTransform.children, isCtrlPressed);
        }
    }

    void EditorScene::handleSelection()
    {
        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_viewportBounds[0].x;
        my -= m_viewportBounds[0].y;

        // Flip the y-coordinate to match opengl texture format
        my = m_contentSize.y - my;

        // Check if mouse is inside viewport
        if (!(mx >= 0 && my >= 0 && mx < m_contentSize.x && my < m_contentSize.y))
            return;
        const int entityId = sampleEntityTexture(mx, my);

        // Check for multi-selection key modifiers
        const bool isShiftPressed = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
        const bool isCtrlPressed = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
        auto &selector = Selector::get();

        if (entityId == -1) {
            // Clicked on empty space - clear selection unless shift/ctrl is held
            if (!isShiftPressed && !isCtrlPressed) {
                selector.clearSelection();
                m_windowState = m_globalState;
            }
            return;
        }

        updateSelection(entityId, isShiftPressed, isCtrlPressed);
    }

    void EditorScene::update()
    {
        const bool isCurrentlyVisible = m_isVisibleInDock || m_wasVisibleLastFrame;

        if (!m_opened || m_activeCamera == -1 || !isCurrentlyVisible)
            return;
        const SceneType sceneType = m_activeCamera == m_editorCamera ? SceneType::EDITOR : SceneType::GAME;
        Application::SceneInfo sceneInfo{static_cast<scene::SceneId>(m_sceneId), RenderingType::FRAMEBUFFER, sceneType};
        sceneInfo.isChildWindow = true;
        sceneInfo.viewportBounds[0] = glm::vec2{m_viewportBounds[0].x, m_viewportBounds[0].y};
        sceneInfo.viewportBounds[1] = glm::vec2{m_viewportBounds[1].x, m_viewportBounds[1].y};
        runEngine(sceneInfo);


        // Handle mouse clicks for selection
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing() && m_focused)
            handleSelection();
    }
}
