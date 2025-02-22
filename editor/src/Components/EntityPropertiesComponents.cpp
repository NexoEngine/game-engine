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

	bool EntityPropertiesComponents::drawHeader(const std::string &label, const std::string &headerText)
	{
	    float increasedPadding = 2.0f;
	    ImGui::PushStyleVar(ImGuiStyleVar_FramePadding,
	                        ImVec2(ImGui::GetStyle().FramePadding.x, increasedPadding));

	    bool open = ImGui::TreeNodeEx(label.c_str(),
	                                  ImGuiTreeNodeFlags_DefaultOpen |
	                                  ImGuiTreeNodeFlags_Framed |
	                                  ImGuiTreeNodeFlags_AllowItemOverlap);
	    ImGui::PopStyleVar();

	    ImGui::SetWindowFontScale(1.2f);

	    // Horizontal centering:
	    float arrowPosX = ImGui::GetCursorPosX();
	    ImGui::SameLine(0.0f, 0.0f);
	    float totalWidth = ImGui::GetContentRegionAvail().x + arrowPosX;
	    ImVec2 textSize = ImGui::CalcTextSize(headerText.c_str());
	    float textPosX = (totalWidth - textSize.x) * 0.5f;
	    ImGui::SetCursorPosX(textPosX);
    	ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2.5f); // This stuff seems strange, should check in the long run if there is a better way

	    ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1));
	    ImGui::TextUnformatted(headerText.c_str());
	    ImGui::PopStyleColor();

	    ImGui::SetWindowFontScale(1.0f);

	    return open;
	}

	void EntityPropertiesComponents::drawRowLabel(const ChannelLabel &rowLabel)
	{
		ImGui::TableNextColumn();
		if (rowLabel.fixedWidth != -1.0f)
		{
		    float fixedCellWidth = rowLabel.fixedWidth;
		    ImVec2 textSize = ImGui::CalcTextSize(rowLabel.label.c_str());
		    float offsetX = (fixedCellWidth - textSize.x) * 0.5f;
		    float rowHeight = ImGui::GetTextLineHeightWithSpacing();
		    float offsetY = (rowHeight - textSize.y) * 0.5f;
		    ImVec2 cellPos = ImGui::GetCursorPos();
		}
		ImGui::SetWindowFontScale(1.11f);

		ImGui::TextUnformatted(rowLabel.label.c_str());
		ImGui::SetWindowFontScale(1.0f);
	}

	void EntityPropertiesComponents::drawRowDragFloat(const Channels &channels)
	{
		for (unsigned int i = 0; i < channels.count; ++i)
		{
			ImGui::TableNextColumn();
			if (!channels.badges[i].label.empty())
			{
				auto &badge = channels.badges[i];
				Components::drawButton(badge.label, badge.size, badge.bg, badge.bgHovered, badge.bgActive, badge.txtColor);
			}
			ImGui::SameLine(0, 2);
			auto &slider = channels.sliders[i];
			Components::drawDragFloat(
				slider.label,
				slider.value,
				slider.speed,
				slider.min,
				slider.max,
				slider.format,
				slider.bg,
				slider.bgHovered,
				slider.bgActive);
		}
	}

	bool EntityPropertiesComponents::drawToggleButtonWithSeparator(const std::string &label, bool* toggled)
	{
		bool clicked = false;
        ImGui::PushID(label.c_str());

        const ImVec2 buttonSize(24, 24);
        std::string arrowLabel = "##arrow" + label;
        if (ImGui::InvisibleButton(arrowLabel.c_str(), buttonSize))
            clicked = true;
        if (clicked)
            *toggled = !(*toggled);

        ImVec2 btnPos = ImGui::GetItemRectMin();
        ImVec2 btnSize = ImGui::GetItemRectSize();
        ImVec2 center(btnPos.x + btnSize.x * 0.5f, btnPos.y + btnSize.y * 0.5f);

        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        float arrowSize = 5.0f;
        ImU32 arrowColor = ImGui::GetColorU32(ImGuiCol_Text);
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
        ImVec2 separatorPos = ImGui::GetCursorScreenPos();
        float separatorHeight = buttonSize.y; // match button height
        draw_list->AddLine(separatorPos, ImVec2(separatorPos.x, separatorPos.y + separatorHeight),
                             ImGui::GetColorU32(ImGuiCol_Separator), 1.0f);
        ImGui::Dummy(ImVec2(4, buttonSize.y));

        ImGui::SameLine();
        Components::drawCustomSeparatorText(label, 10.0f, 0.1f, 0.5f, IM_COL32(255, 255, 255, 255), IM_COL32(255, 255, 255, 255));
        ImGui::PopID();
        return clicked;
	}
}
