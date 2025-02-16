/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** TransformProperty.cpp
*/

#include <imgui.h>

#include "TransformProperty.hpp"
#include "AEntityProperty.hpp"

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

            if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
            {
                if (ImGui::BeginTable("InspectorTransformTable", 4, ImGuiTableFlags_SizingStretchProp))
                {
                    auto showRow = [](const char* label, float* values)
                    {
                        ImGui::PushID("%s", label);
                        ImGui::TableNextRow(ImGuiTableRowFlags_None);
                        ImGui::TableNextColumn();
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 11.0f);
                        ImGui::Text("%s", label);
                        ImGui::SameLine();

                        ImGui::TableNextColumn();
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 11.0f);
                        ImGui::Text("X");
                        ImGui::SameLine();
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 11.0f);
                        ImGui::DragFloat("##X", &values[0], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");

                        ImGui::TableNextColumn();
                        ImGui::Text("Y");
                        ImGui::SameLine();
                        ImGui::DragFloat("##Y", &values[1], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");

                        ImGui::TableNextColumn();
                        ImGui::Text("Z");
                        ImGui::SameLine();
                        ImGui::DragFloat("##Z", &values[2], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");
                        ImGui::PopID();
                    };

                    showRow("Position", &pos.x);
                    showRow("Rotation", &rotation.x);
                    showRow("Scale", &size.x);
                    ImGui::EndTable();
                }
                ImGui::TreePop();
            }

        return true;
    }

    void TransformProperty::showEnd()
    {
        AEntityProperty::showEnd();
    }
}
