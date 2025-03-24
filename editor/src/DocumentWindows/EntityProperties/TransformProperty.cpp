//// TransformProperty.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/03/2025
//  Description: Source file for the transform property inspector
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>

#include "TransformProperty.hpp"
#include "Components/EntityPropertiesComponents.hpp"
#include "math/Vector.hpp"

namespace nexo::editor {
    bool TransformProperty::show(ecs::Entity entity)
    {
        auto& [pos, size, quat] = Application::getEntityComponent<components::TransformComponent>(entity);

        static glm::vec3 lastDisplayedEuler(0.0f);

        if (EntityPropertiesComponents::drawHeader("##TransformNode", "Transform Component"))
        {
            // Increase cell padding so rows have more space:
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));

            if (ImGui::BeginTable("InspectorTransformTable", 4,
                ImGuiTableFlags_SizingStretchProp))
            {
                // Only the first column has a fixed width
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

                EntityPropertiesComponents::drawRowDragFloat3("Position", "X", "Y", "Z", &pos.x);

                const glm::vec3 computedEuler = math::customQuatToEuler(quat);

                lastDisplayedEuler = computedEuler;
                glm::vec3 rotation = lastDisplayedEuler;

                // Draw the Rotation row.
                // When the user edits the rotation, we compute the delta from the last displayed Euler,
                // convert that delta into an incremental quaternion, and update the master quaternion.
                if (EntityPropertiesComponents::drawRowDragFloat3("Rotation", "X", "Y", "Z", &rotation.x)) {
                    const glm::vec3 deltaEuler = rotation - lastDisplayedEuler;
                    const glm::quat deltaQuat = glm::radians(deltaEuler);
                    quat = glm::normalize(deltaQuat * quat);
                    lastDisplayedEuler = math::customQuatToEuler(quat);
                    rotation = lastDisplayedEuler;
                }
                EntityPropertiesComponents::drawRowDragFloat3("Scale", "X", "Y", "Z", &size.x);

                ImGui::EndTable();
            }
            ImGui::PopStyleVar();
            ImGui::TreePop();
        }
        return true;
    }
}
