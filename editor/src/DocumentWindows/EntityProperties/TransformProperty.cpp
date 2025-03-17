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

    static void drawRowWithDragFloat(const char *label, float *values)
    {
	    std::string labelStr = label;
	    std::string labelX = std::string("##X") + labelStr;
	    std::string labelY = std::string("##Y") + labelStr;
	    std::string labelZ = std::string("##Z") + labelStr;

	    std::string badgeLabelX = std::string("X##") + labelStr;
	    std::string badgeLabelY = std::string("Y##") + labelStr;
	    std::string badgeLabelZ = std::string("Z##") + labelStr;

	    ImGui::TableNextRow();

		ChannelLabel chanLabel;
		chanLabel.label = std::string(label);
		chanLabel.fixedWidth = -1.0f;

		float badgeSize = ImGui::GetFrameHeight();
		std::vector<Badge> badges;
		badges.reserve(3);
		badges.push_back(Badge(badgeLabelX, ImVec2(badgeSize, badgeSize), IM_COL32(80, 0, 0, 255), IM_COL32(80, 0, 0, 255), IM_COL32(80, 0, 0, 255), IM_COL32(255, 180, 180, 255)));
		badges.push_back(Badge(badgeLabelY, ImVec2(badgeSize, badgeSize), IM_COL32(0, 80, 0, 255), IM_COL32(0, 80, 0, 255), IM_COL32(0, 80, 0, 255), IM_COL32(180, 255, 180, 255)));
		badges.push_back(Badge(badgeLabelZ, ImVec2(badgeSize, badgeSize), IM_COL32(0, 0, 80, 255), IM_COL32(0, 0, 80, 255), IM_COL32(0, 0, 80, 255), IM_COL32(180, 180, 255, 255)));

		std::vector<DragFloat> sliders;
		sliders.reserve(3);
		sliders.push_back(DragFloat(labelX, &values[0], 0.3f, -FLT_MAX, FLT_MAX, IM_COL32(60, 60, 60, 255), IM_COL32(80, 80, 80, 255), IM_COL32(100, 100, 100, 255), "%.2f"));
		sliders.push_back(DragFloat(labelY, &values[1], 0.3f, -FLT_MAX, FLT_MAX, IM_COL32(60, 60, 60, 255), IM_COL32(80, 80, 80, 255), IM_COL32(100, 100, 100, 255), "%.2f"));
		sliders.push_back(DragFloat(labelZ, &values[2], 0.3f, -FLT_MAX, FLT_MAX, IM_COL32(60, 60, 60, 255), IM_COL32(80, 80, 80, 255), IM_COL32(100, 100, 100, 255), "%.2f"));

		Channels channels;
		channels.count = 3;
		channels.badges = badges;
		channels.sliders = sliders;

		EntityPropertiesComponents::drawRowLabel(chanLabel);
		EntityPropertiesComponents::drawRowDragFloat(channels);
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
