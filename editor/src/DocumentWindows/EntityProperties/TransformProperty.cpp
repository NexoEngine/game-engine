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

        // Header setup (size + color)
        float increasedPadding = 3.5f;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(ImGui::GetStyle().FramePadding.x, increasedPadding));
        ImVec4 customHeaderColor       = ImVec4(0.2f, 0.3f, 0.7f, 1.0f);
        ImVec4 customHeaderHoveredColor = ImVec4(0.3f, 0.4f, 0.8f, 1.0f);
        ImVec4 customHeaderActiveColor  = ImVec4(0.4f, 0.5f, 0.9f, 1.0f);

        ImGui::PushStyleColor(ImGuiCol_Header, customHeaderColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderHovered, customHeaderHoveredColor);
        ImGui::PushStyleColor(ImGuiCol_HeaderActive, customHeaderActiveColor);
        bool open = ImGui::TreeNodeEx("##TransformNode",
                                        ImGuiTreeNodeFlags_DefaultOpen |
                                        ImGuiTreeNodeFlags_Framed |
                                        ImGuiTreeNodeFlags_AllowItemOverlap);
        ImGui::PopStyleVar();
        ImGui::PopStyleColor(3);

        // Header title setup (centering)
        const char* headerText = "Transform Component";
        ImGui::SetWindowFontScale(1.25f);
        ImVec2 textSize = ImGui::CalcTextSize(headerText);

        float windowWidth = ImGui::GetContentRegionAvail().x;
        float windowHeight = ImGui::GetContentRegionAvail().y;
        float centerWidth = windowWidth / 2.0f;
        float centerHeight = windowHeight / 2.0f;

        ImGui::SameLine();
        ImGui::SetCursorPosX(centerWidth - (textSize.x / 2.5f)); // No idea why this value makes it work
        ImGui::SetCursorPosY(centerHeight - (textSize.y / 3.0f)); // Same here, but it makes it centered apparently
        ImGui::TextUnformatted(headerText);
        ImGui::SetWindowFontScale(1.0f);

        // Vertical spacing
        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        if (open)
        {
            // Increase cell padding so rows have more space:
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));

            if (ImGui::BeginTable("InspectorTransformTable", 4,
                ImGuiTableFlags_SizingStretchProp  |
                ImGuiTableFlags_Borders))
            {
                // Only the first column has a fixed width
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoHeaderLabel, 60.0f);
                ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

                auto showRow = [&](const char* label, float* values)
                {
                    std::string labelStr = label;
                    std::string labelX = std::string("##X") + labelStr;
                    std::string labelY = std::string("##Y") + labelStr;
                    std::string labelZ = std::string("##Z") + labelStr;

                    std::string badgeLabelX = std::string("X##") + labelStr;
                    std::string badgeLabelY = std::string("Y##") + labelStr;
                    std::string badgeLabelZ = std::string("Z##") + labelStr;

                    ImGui::TableNextRow();

                    // LABEL COLUMN
                    ImGui::TableNextColumn();
                    float fixedCellWidth = 60.0f;
                    ImVec2 textSize = ImGui::CalcTextSize(label);
                    float offsetX = (fixedCellWidth - textSize.x) * 0.5f;
                    float rowHeight = ImGui::GetTextLineHeightWithSpacing();
                    float offsetY = (rowHeight - textSize.y) * 0.5f;
                    ImVec2 cellPos = ImGui::GetCursorPos();
                    ImGui::SetCursorPos(ImVec2(cellPos.x + offsetX, cellPos.y + offsetY));
                    ImGui::TextUnformatted(label);

                    // Helper to get a badge size that aligns with the drag float.
                    float badgeSize = ImGui::GetFrameHeight();

                    // X COLUMN
                    ImGui::TableNextColumn();
                    // Small button for the X value
                    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(180, 30, 30, 255));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(180, 30, 30, 255));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(180, 30, 30, 255));
                    ImGui::Button(badgeLabelX.c_str(), ImVec2(badgeSize, badgeSize));
                    ImGui::PopStyleColor(3);
                    ImGui::SameLine();
                    // Now the DragFloat for the X value with its own background style
                    ImGui::PushStyleColor(ImGuiCol_FrameBg,        IM_COL32(60, 60, 60, 255));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(80, 80, 80, 255));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  IM_COL32(100, 100, 100, 255));
                    ImGui::DragFloat(labelX.c_str(), &values[0], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");
                    ImGui::PopStyleColor(3);

                    // Y COLUMN
                    ImGui::TableNextColumn();
                    badgeSize = ImGui::GetFrameHeight();
                    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(30, 180, 30, 255));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(30, 180, 30, 255));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(30, 180, 30, 255));
                    ImGui::Button(badgeLabelY.c_str(), ImVec2(badgeSize, badgeSize));
                    ImGui::PopStyleColor(3);
                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_FrameBg,        IM_COL32(60, 60, 60, 255));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(80, 80, 80, 255));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  IM_COL32(100, 100, 100, 255));
                    ImGui::DragFloat(labelY.c_str(), &values[1], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");
                    ImGui::PopStyleColor(3);

                    // Z COLUMN
                    ImGui::TableNextColumn();
                    badgeSize = ImGui::GetFrameHeight();
                    ImGui::PushStyleColor(ImGuiCol_Button, IM_COL32(30, 30, 120, 255));
                    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(30, 30, 120, 255));
                    ImGui::PushStyleColor(ImGuiCol_ButtonActive, IM_COL32(30, 30, 120, 255));
                    ImGui::Button(badgeLabelZ.c_str(), ImVec2(badgeSize, badgeSize));
                    ImGui::PopStyleColor(3);
                    ImGui::SameLine();
                    ImGui::PushStyleColor(ImGuiCol_FrameBg,        IM_COL32(60, 60, 60, 255));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, IM_COL32(80, 80, 80, 255));
                    ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  IM_COL32(100, 100, 100, 255));
                    ImGui::DragFloat(labelZ.c_str(), &values[2], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");
                    ImGui::PopStyleColor(3);
                };

                showRow("Position", &pos.x);
                showRow("Rotation", &rotation.x);
                showRow("Scale",    &size.x);

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
