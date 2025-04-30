//// EntityProperties.cpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/02/2025
//  Description: Source file for the entity properties components
//
///////////////////////////////////////////////////////////////////////////////

#include "Widgets.hpp"
#include "Guard.hpp"
#include "Utils.hpp"
#include "EntityProperties.hpp"
#include "IconsFontAwesome.h"
#include "Nexo.hpp"
#include "components/Camera.hpp"
#include "context/Selector.hpp"
#include "components/Uuid.hpp"
#include "components/Light.hpp"
#include "components/Transform.hpp"
#include "math/Vector.hpp"

namespace ImNexo {

    InteractionState Ambient(
        nexo::components::AmbientLightComponent &ambientComponent,
        nexo::components::AmbientLightComponent::Memento &beforeState
    ) {
        ImGui::Spacing();
        InteractionState state = InteractionState::NONE;
        static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
        static bool showColorPicker = false;

        ImGui::Text("Color");
        ImGui::SameLine();
        glm::vec4 color = {ambientComponent.color, 1.0f};
        ImNexo::ColorEditor("##ColorEditor Ambient light", &color, &colorPickerMode, &showColorPicker);

        state = trackEditingLifecylce(ambientComponent, beforeState);
        ambientComponent.color = color;
        return state;
    }

    void Transform(nexo::components::TransformComponent &transformComponent, glm::vec3 &lastDisplayedEuler)
	{
	    // Increase cell padding so rows have more space:
           ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
           auto& [pos, size, quat] = transformComponent;

           if (ImGui::BeginTable("InspectorTransformTable", 4,
               ImGuiTableFlags_SizingStretchProp))
           {
               // Only the first column has a fixed width
               ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
               ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
               ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
               ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

               RowDragFloat3("Position", "X", "Y", "Z", &pos.x);

               const glm::vec3 computedEuler = nexo::math::customQuatToEuler(quat);

               lastDisplayedEuler = computedEuler;
               glm::vec3 rotation = lastDisplayedEuler;

               // Draw the Rotation row.
               // When the user edits the rotation, we compute the delta from the last displayed Euler,
               // convert that delta into an incremental quaternion, and update the master quaternion.
               if (RowDragFloat3("Rotation", "X", "Y", "Z", &rotation.x)) {
                   const glm::vec3 deltaEuler = rotation - lastDisplayedEuler;
                   const glm::quat deltaQuat = glm::radians(deltaEuler);
                   quat = glm::normalize(deltaQuat * quat);
                   lastDisplayedEuler = nexo::math::customQuatToEuler(quat);
                   rotation = lastDisplayedEuler;
               }
               RowDragFloat3("Scale", "X", "Y", "Z", &size.x);

               ImGui::EndTable();
           }
           ImGui::PopStyleVar();
	}

	void Camera(nexo::components::CameraComponent &cameraComponent)
	{
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
        if (ImGui::BeginTable("CameraInspectorViewPortParams", 4,
                                ImGuiTableFlags_SizingStretchProp))
        {
            ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
            ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
            ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
            ImGui::TableSetupColumn("##Lock", ImGuiTableColumnFlags_WidthStretch);
            glm::vec2 viewPort = {cameraComponent.width, cameraComponent.height};
            std::vector<ImU32> badgeColors;
            std::vector<ImU32> textBadgeColors;

            const bool disabled = cameraComponent.viewportLocked;
            if (disabled)
                ImGui::BeginDisabled();
            if (RowDragFloat2("Viewport size", "W", "H", &viewPort.x, -FLT_MAX, FLT_MAX, 1.0f, badgeColors, textBadgeColors, disabled))
            {
                if (!cameraComponent.viewportLocked)
                cameraComponent.resize(static_cast<unsigned int>(viewPort.x), static_cast<unsigned int>(viewPort.y));
            }
            if (disabled)
                ImGui::EndDisabled();

            ImGui::TableSetColumnIndex(3);

            // Lock button
            const std::string lockBtnLabel = cameraComponent.viewportLocked ? ICON_FA_LOCK "##ViewPortSettings" : ICON_FA_UNLOCK "##ViewPortSettings";
            if (Button(lockBtnLabel)) {
                cameraComponent.viewportLocked = !cameraComponent.viewportLocked;
            }
            ImGui::EndTable();
        }

        if (ImGui::BeginTable("InspectorCameraVariables", 2, ImGuiTableFlags_SizingStretchProp))
        {
            ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
            ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

            RowDragFloat1("FOV", "", &cameraComponent.fov, 30.0f, 120.0f, 0.3f);
            RowDragFloat1("Near plane", "", &cameraComponent.nearPlane, 0.01f, 1.0f, 0.001f);
            RowDragFloat1("Far plane", "", &cameraComponent.farPlane, 100.0f, 10000.0f, 1.0f);

            ImGui::EndTable();
        }
        ImGui::PopStyleVar();

        ImGui::Spacing();
        static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
        static bool showColorPicker = false;
        ImGui::AlignTextToFramePadding();
        ImGui::Text("Clear Color");
        ImGui::SameLine();
        ColorEditor("##ColorEditor Spot light", &cameraComponent.clearColor, &colorPickerMode, &showColorPicker);
	}

	void CameraTarget(nexo::components::PerspectiveCameraTarget &cameraTargetComponent)
	{
        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
        if (ImGui::BeginTable("InspectorControllerTable", 2,
                            ImGuiTableFlags_SizingStretchProp))
        {
            auto &app = nexo::getApp();
            auto &selector = nexo::editor::Selector::get();
            ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
            ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

            const std::vector<nexo::ecs::Entity> &entities = app.m_coordinator->getAllEntitiesWith<
                                                            nexo::components::TransformComponent,
                                                            nexo::ecs::Exclude<nexo::components::CameraComponent>,
                                                            nexo::ecs::Exclude<nexo::components::DirectionalLightComponent>,
                                                            nexo::ecs::Exclude<nexo::components::PointLightComponent>,
                                                            nexo::ecs::Exclude<nexo::components::AmbientLightComponent>,
                                                            nexo::ecs::Exclude<nexo::components::SpotLightComponent>>();

            RowDragFloat1("Mouse sensitivity", "", &cameraTargetComponent.mouseSensitivity, 0.1f);
            RowDragFloat1("Distance", "", &cameraTargetComponent.distance, 0.1f);
            RowEntityDropdown(
                "Target Entity",
                cameraTargetComponent.targetEntity, entities,
                [&app, &selector](nexo::ecs::Entity e) {
                    return selector.getUiHandle(
                        app.m_coordinator->getComponent<nexo::components::UuidComponent>(e).uuid,
                        std::to_string(e)
                    );
                }
            );
            ImGui::EndTable();
        }
        ImGui::PopStyleVar();
	}

	void CameraController(nexo::components::PerspectiveCameraController &cameraControllerComponent)
	{
        ImGui::Spacing();

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
        if (ImGui::BeginTable("InspectorControllerTable", 2,
                            ImGuiTableFlags_SizingStretchProp))
        {
            ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
            ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

            RowDragFloat1("Mouse sensitivity", "", &cameraControllerComponent.mouseSensitivity);

            ImGui::EndTable();
        }
        ImGui::PopStyleVar();
	}

}
