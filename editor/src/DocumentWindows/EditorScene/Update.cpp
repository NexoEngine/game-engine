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
#include "context/Selector.hpp"
#include "components/Uuid.hpp"

namespace nexo::editor {
    int EditorScene::sampleEntityTexture(float mx, float my)
    {
        const auto &coord = getApp().m_coordinator;
        const auto &cameraComponent = coord->getComponent<components::CameraComponent>(static_cast<ecs::Entity>(m_activeCamera));

        cameraComponent.m_renderTarget->bind();
        int entityId = cameraComponent.m_renderTarget->getPixel<int>(1, static_cast<int>(mx), static_cast<int>(my));
        cameraComponent.m_renderTarget->unbind();
        return entityId;
    }

    static SelectionType getSelectionType(int entityId)
    {
        const auto &coord = getApp().m_coordinator;
        SelectionType selType = SelectionType::ENTITY;
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
        auto &selector = Selector::get();
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

    void EditorScene::updateSelection(int entityId, bool isShiftPressed, bool isCtrlPressed)
    {
        const auto &coord = getApp().m_coordinator;
        const auto uuid = coord->tryGetComponent<components::UuidComponent>(entityId);
        if (!uuid)
            return;

        // Determine selection type
        SelectionType selType = getSelectionType(entityId);
        auto &selector = Selector::get();

        // Handle different selection modes
        if (isCtrlPressed)
            selector.toggleSelection(uuid->get().uuid, entityId, selType);
        else if (isShiftPressed)
            selector.addToSelection(uuid->get().uuid, entityId, selType);
        else
            selector.selectEntity(uuid->get().uuid, entityId, selType);

        updateWindowState();
        selector.setSelectedScene(m_sceneId);
    }

    void EditorScene::handleSelection()
    {
        auto [mx, my] = ImGui::GetMousePos();
        mx -= m_viewportBounds[0].x;
        my -= m_viewportBounds[0].y;

        // Flip the y-coordinate to match opengl texture format
        my = m_viewSize.y - my;

        // Check if mouse is inside viewport
        if (!(mx >= 0 && my >= 0 && mx < m_viewSize.x && my < m_viewSize.y))
            return;
        int entityId = sampleEntityTexture(mx, my);

        // Check for multi-selection key modifiers
        bool isShiftPressed = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
        bool isCtrlPressed = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);
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
        auto &selector = Selector::get();
        if (!m_opened || m_activeCamera == -1)
            return;
        SceneType sceneType = m_activeCamera == m_editorCamera ? SceneType::EDITOR : SceneType::GAME;
        runEngine(m_sceneId, RenderingType::FRAMEBUFFER, sceneType);


        // Handle mouse clicks for selection
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing() && m_focused)
            handleSelection();
    }
}
