/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** TransformProperty.cpp
*/

#include <imgui.h>

#include "TransformProperty.hpp"
#include "AEntityProperty.hpp"
#include "Components/EntityPropertiesComponents.hpp"

#include <loguru/loguru.hpp>

namespace nexo::editor {
    TransformProperty::TransformProperty(const std::string& name)
        : AEntityProperty(name)
    {

    }

    TransformProperty::~TransformProperty()
    {
    }

    void TransformProperty::update()
    {

    }

    glm::vec3 customQuatToEuler(const glm::quat &q) {
        glm::vec3 euler;

        float sinp = 2.0f * (q.w * q.y - q.z * q.x);
        if (std::abs(sinp) >= 1.0f)
            euler.x = std::copysign(glm::half_pi<float>(), sinp);
        else
            euler.x = std::asin(sinp);

        euler.y = std::atan2(2.0f * (q.w * q.z + q.x * q.y),
                             1.0f - 2.0f * (q.y * q.y + q.z * q.z));

        euler.z = std::atan2(2.0f * (q.w * q.x + q.y * q.z),
                             1.0f - 2.0f * (q.x * q.x + q.y * q.y));

        return glm::degrees(euler);
    }

    int TransformProperty::show(ecs::Entity entity)
    {
        auto const& App = getApp();
        auto& [pos, size, quat] = App.getEntityComponent<components::TransformComponent>(entity);

        bool open = EntityPropertiesComponents::drawHeader("##TransformNode", "Transform Component");

        static glm::vec3 lastDisplayedEuler(0.0f);

        if (open)
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

                glm::vec3 computedEuler = customQuatToEuler(quat);

                lastDisplayedEuler = computedEuler;
                glm::vec3 rotation = lastDisplayedEuler;

                // Draw the Rotation row.
                // When the user edits the rotation, we compute the delta from the last displayed Euler,
                // convert that delta into an incremental quaternion, and update the master quaternion.
                bool changed = EntityPropertiesComponents::drawRowDragFloat3("Rotation", "X", "Y", "Z", &rotation.x);
                if (changed) {
                    glm::vec3 deltaEuler = rotation - lastDisplayedEuler;
                    glm::quat deltaQuat = glm::quat(glm::radians(deltaEuler));
                    quat = glm::normalize(deltaQuat * quat);
                    lastDisplayedEuler = customQuatToEuler(quat);
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

    void TransformProperty::showEnd()
    {
        AEntityProperty::showEnd();
    }
}
