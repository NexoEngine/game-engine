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

    int TransformProperty::show(ecs::Entity entity)
    {
        auto const& App = getApp();
        auto& [pos, size, rotation] = App.getEntityComponent<components::TransformComponent>(entity);

        bool open = EntityPropertiesComponents::drawHeader("##TransformNode", "Transform Component");

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
                EntityPropertiesComponents::drawRowDragFloat3("Rotation", "X", "Y", "Z", &rotation.x);
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
