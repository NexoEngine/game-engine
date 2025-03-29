//// EntityPropertiesComponents.cpp ///////////////////////////////////////////
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

#include "EntityPropertiesComponents.hpp"
#include "Components.hpp"

namespace nexo::editor {

	bool EntityPropertiesComponents::drawHeader(const std::string &label, std::string_view headerText)
	{
	    float increasedPadding = 2.0f;
	    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	                        ImVec2(ImGui::GetStyle().FramePadding.x, increasedPadding));

	    bool open = ImGui::TreeNodeEx(label.c_str(),
	                                  ImGuiTreeNodeFlags_DefaultOpen |
	                                  ImGuiTreeNodeFlags_Framed |
	                                  ImGuiTreeNodeFlags_AllowItemOverlap);
	    ImGui::PopStyleVar();

	    //ImGui::SetWindowFontScale(1.2f);

	    // Horizontal centering:
	    const float arrowPosX = ImGui::GetCursorPosX();
	    ImGui::SameLine(0.0f, 0.0f);
	    const float totalWidth = ImGui::GetContentRegionAvail().x + arrowPosX;
	    const ImVec2 textSize = ImGui::CalcTextSize(headerText.data());
	    const float textPosX = (totalWidth - textSize.x) * 0.5f;
	    ImGui::SetCursorPosX(textPosX);
    	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.5f); // This stuff seems strange, should check in the long run if there is a better way

	    //ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
	    ImGui::TextUnformatted(headerText.data());
	    //ImGui::PopStyleColor();

	    //ImGui::SetWindowFontScale(1.0f);

	    return open;
	}

	void EntityPropertiesComponents::drawRowLabel(const ChannelLabel &rowLabel)
	{
		ImGui::TableNextColumn();
		if (rowLabel.fixedWidth != -1.0f)
		{
			//TODO: Implement now fixed width row label
		    //float fixedCellWidth = rowLabel.fixedWidth;
		    //ImVec2 textSize = ImGui::CalcTextSize(rowLabel.label.c_str());
		    //float offsetX = (fixedCellWidth - textSize.x) * 0.5f;
		    //float rowHeight = ImGui::GetTextLineHeightWithSpacing();
		    //float offsetY = (rowHeight - textSize.y) * 0.5f;
		    //ImVec2 cellPos = ImGui::GetCursorPos();
		}
		//ImGui::SetWindowFontScale(1.11f);

		ImGui::TextUnformatted(rowLabel.label.c_str());
		//ImGui::SetWindowFontScale(1.0f);
	}

	bool EntityPropertiesComponents::drawRowDragFloat(const Channels &channels)
	{
		bool clicked = false;
		for (unsigned int i = 0; i < channels.count; ++i)
		{
			ImGui::TableNextColumn();
			if (!channels.badges[i].label.empty())
			{
				const auto &[label, size, bg, bgHovered, bgActive, txtColor] = channels.badges[i];
				Components::drawButton(label, size, bg, bgHovered, bgActive, txtColor);
			}
			ImGui::SameLine(0, 2);
			const auto &[label, value, speed, min, max, bg, bgHovered, bgActive, textColor, format] = channels.sliders[i];
			clicked = Components::drawDragFloat(
				label,
				value,
				speed,
				min,
				max,
				format,
				bg,
				bgHovered,
				bgActive) || clicked;
		}
		return clicked;
	}

	bool EntityPropertiesComponents::drawRowDragFloat1(const char *uniqueLabel, const std::string &badgeLabel, float *value, float minValue, float maxValue, float speed)
	{
		const std::string labelStr = uniqueLabel;
		std::string labelX = std::string("##X") + labelStr;

		std::string badgeLabelX = (badgeLabel.empty()) ? "" : badgeLabel + std::string("##") + labelStr;

		ImGui::TableNextRow();

		ChannelLabel chanLabel;
		chanLabel.label = std::string(uniqueLabel);
		chanLabel.fixedWidth = -1.0f;

		float badgeSize = ImGui::GetFrameHeight();
		std::vector<Badge> badges;
		badges.reserve(1);
		badges.emplace_back(Badge{badgeLabelX, {badgeSize, badgeSize}, IM_COL32(80, 0, 0, 255), IM_COL32(80, 0, 0, 255), IM_COL32(80, 0, 0, 255), IM_COL32(255, 180, 180, 255)});

		std::vector<DragFloat> sliders;
		sliders.reserve(1);
		sliders.emplace_back(badgeLabelX, value, speed, minValue, maxValue, 0, 0, 0, 0, "%.2f");

		Channels channels;
		channels.count = 1;
		channels.badges = badges;
		channels.sliders = sliders;

		EntityPropertiesComponents::drawRowLabel(chanLabel);
		return EntityPropertiesComponents::drawRowDragFloat(channels);
	}

	bool EntityPropertiesComponents::drawRowDragFloat2(
		const char *uniqueLabel,
		const std::string &badLabelX,
		const std::string &badLabelY,
		float *values,
		float minValue,
		float maxValue,
		float speed,
		std::vector<ImU32> badgeColor,
		std::vector<ImU32> textBadgeColor,
		const bool inactive
	)
	{
		const std::string labelStr = uniqueLabel;
		std::string labelX = std::string("##X") + labelStr;
		std::string labelY = std::string("##Y") + labelStr;

		const std::string badgeLabelX = badLabelX + std::string("##") + labelStr;
		const std::string badgeLabelY = badLabelY + std::string("##") + labelStr;

		ImGui::TableNextRow();

		ChannelLabel chanLabel;
		chanLabel.label = std::string(uniqueLabel);
		chanLabel.fixedWidth = -1.0f;

		float badgeSize = ImGui::GetFrameHeight();
		if (badgeColor.empty())
			badgeColor = {IM_COL32(80, 0, 0, 255), IM_COL32(0, 80, 0, 255)};
		if (textBadgeColor.empty())
			textBadgeColor = {IM_COL32(255, 180, 180, 255), IM_COL32(180, 255, 180, 255)};
		std::vector<Badge> badges;
		badges.reserve(2);
		badges.emplace_back(Badge{badgeLabelX, {badgeSize, badgeSize}, badgeColor[0], badgeColor[0], badgeColor[0], textBadgeColor[0]});
		badges.emplace_back(Badge{badgeLabelY, {badgeSize, badgeSize}, badgeColor[1], badgeColor[1], badgeColor[1], textBadgeColor[1]});

		std::vector<DragFloat> sliders;
		sliders.reserve(2);
		std::vector<ImU32> sliderColors = {IM_COL32(60, 60, 60, 255), IM_COL32(80, 80, 80, 255), IM_COL32(100, 100, 100, 255), ImGui::GetColorU32(ImGuiCol_Text)};
		if (inactive)
			sliderColors = {IM_COL32(30, 30, 30, 255), IM_COL32(30, 30, 30, 255), IM_COL32(30, 30, 30, 255), IM_COL32(50, 50, 50, 255)};
		sliders.emplace_back(labelX, &values[0], speed, minValue, maxValue, sliderColors[0], sliderColors[1],
		                     sliderColors[2], sliderColors[3], "%.2f");
		sliders.emplace_back(labelY, &values[1], speed, minValue, maxValue, sliderColors[0], sliderColors[1],
		                     sliderColors[2], sliderColors[3], "%.2f");

		Channels channels;
		channels.count = 2;
		channels.badges = badges;
		channels.sliders = sliders;

		EntityPropertiesComponents::drawRowLabel(chanLabel);
		return EntityPropertiesComponents::drawRowDragFloat(channels);
	}

	bool EntityPropertiesComponents::drawRowDragFloat3(
		const char *uniqueLabel,
		const std::string &badLabelX,
		const std::string &badLabelY,
		const std::string &badLabelZ,
		float *values,
		float minValue,
		float maxValue,
		float speed,
		std::vector<ImU32> badgeColors,
		std::vector<ImU32> textBadgeColor
	)
	{
		std::string labelStr = uniqueLabel;
		std::string labelX = std::string("##X") + labelStr;
		std::string labelY = std::string("##Y") + labelStr;
		std::string labelZ = std::string("##Z") + labelStr;

		std::string badgeLabelX = badLabelX + std::string("##") + labelStr;
		std::string badgeLabelY = badLabelY + std::string("##") + labelStr;
		std::string badgeLabelZ = badLabelZ + std::string("##") + labelStr;

		ImGui::TableNextRow();

		ChannelLabel chanLabel;
		chanLabel.label = std::string(uniqueLabel);
		chanLabel.fixedWidth = -1.0f;

		float badgeSize = ImGui::GetFrameHeight();
		if (badgeColors.empty())
			badgeColors = {IM_COL32(80, 0, 0, 255), IM_COL32(0, 80, 0, 255), IM_COL32(0, 0, 80, 255)};
		if (textBadgeColor.empty())
			textBadgeColor = {IM_COL32(255, 180, 180, 255), IM_COL32(180, 255, 180, 255), IM_COL32(180, 180, 255, 255)};
		std::vector<Badge> badges;
		badges.reserve(3);
		badges.emplace_back(Badge{badgeLabelX, {badgeSize, badgeSize}, badgeColors[0], badgeColors[0], badgeColors[0], textBadgeColor[0]});
		badges.emplace_back(Badge{badgeLabelY, {badgeSize, badgeSize}, badgeColors[1], badgeColors[1], badgeColors[1], textBadgeColor[1]});
		badges.emplace_back(Badge{badgeLabelZ, {badgeSize, badgeSize}, badgeColors[2], badgeColors[2], badgeColors[2], textBadgeColor[2]});

		std::vector<DragFloat> sliders;
		sliders.reserve(3);
		sliders.emplace_back(labelX, &values[0], speed, minValue, maxValue, 0,
		                     0, 0, ImGui::GetColorU32(ImGuiCol_Text),
		                     "%.2f");
		sliders.emplace_back(labelY, &values[1], speed, minValue, maxValue, 0,
		                     0, 0, ImGui::GetColorU32(ImGuiCol_Text),
		                     "%.2f");
		sliders.emplace_back(labelZ, &values[2], speed, minValue, maxValue, 0,
		                     0, 0, ImGui::GetColorU32(ImGuiCol_Text),
		                     "%.2f");

		Channels channels;
		channels.count = 3;
		channels.badges = badges;
		channels.sliders = sliders;

		if (!chanLabel.label.empty())
			EntityPropertiesComponents::drawRowLabel(chanLabel);
		return EntityPropertiesComponents::drawRowDragFloat(channels);
	}

	bool EntityPropertiesComponents::drawToggleButtonWithSeparator(const std::string &label, bool* toggled)
	{
		bool clicked = false;
        ImGui::PushID(label.c_str());

		constexpr ImVec2 buttonSize(24, 24);
		if (const std::string arrowLabel = "##arrow" + label; ImGui::InvisibleButton(arrowLabel.c_str(), buttonSize))
            clicked = true;
        if (clicked)
            *toggled = !(*toggled);

        const ImVec2 btnPos = ImGui::GetItemRectMin();
        const ImVec2 btnSize = ImGui::GetItemRectSize();
        const ImVec2 center(btnPos.x + btnSize.x * 0.5f, btnPos.y + btnSize.y * 0.5f);

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
		constexpr float arrowSize = 5.0f;
		const ImU32 arrowColor = ImGui::GetColorU32(ImGuiCol_TextTab);
        if (*toggled)
        {
            // Draw a downward pointing arrow
            draw_list->AddTriangleFilled(
                ImVec2(center.x - arrowSize, center.y - arrowSize),
                ImVec2(center.x + arrowSize, center.y - arrowSize),
                ImVec2(center.x, center.y + arrowSize),
                arrowColor);
        }
        else
        {
            // Draw a rightward pointing arrow
            draw_list->AddTriangleFilled(
                ImVec2(center.x - arrowSize, center.y - arrowSize),
                ImVec2(center.x - arrowSize, center.y + arrowSize),
                ImVec2(center.x + arrowSize, center.y),
                arrowColor);
        }

        ImGui::SameLine();
        const ImVec2 separatorPos = ImGui::GetCursorScreenPos();
		constexpr float separatorHeight = buttonSize.y; // match button height
        draw_list->AddLine(separatorPos, ImVec2(separatorPos.x, separatorPos.y + separatorHeight),
                             ImGui::GetColorU32(ImGuiCol_Separator), 1.0f);
        ImGui::Dummy(ImVec2(4, buttonSize.y));

        ImGui::SameLine();
        Components::drawCustomSeparatorText(label, 10.0f, 0.1f, 0.5f, IM_COL32(255, 255, 255, 255), IM_COL32(255, 255, 255, 255));
        ImGui::PopID();
        return clicked;
	}
}
