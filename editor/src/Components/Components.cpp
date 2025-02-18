//// Components.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/02/2025
//  Description: Source file for the utilitary ImGui functions
//
///////////////////////////////////////////////////////////////////////////////

#include "Components.hpp"
#include <imgui.h>

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
		ImGui::SetWindowFontScale(1.12f);

		float arrowPosX = ImGui::GetCursorPosX();

		ImGui::SameLine(0.0f, 0.0f);

		float totalWidth = ImGui::GetContentRegionAvail().x + arrowPosX;

		ImVec2 textSize = ImGui::CalcTextSize(headerText.c_str());
		float textPosX = (totalWidth - textSize.x) * 0.5f;
		ImGui::SetCursorPosX(textPosX);
		ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0, 0, 0, 1.0f));
		ImGui::TextUnformatted(headerText.c_str());
		ImGui::PopStyleColor();
		ImGui::SetWindowFontScale(1.0f);
		return open;
	}

	bool EntityPropertiesComponents::drawButton(
			const std::string &label,
			const ImVec2 &size,
			ImU32 bg,
			ImU32 bgHovered,
			ImU32 bgActive, ImU32 txtColor
	) {
		ImGui::PushStyleColor(ImGuiCol_Button, bg);
        ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bgHovered);
        ImGui::PushStyleColor(ImGuiCol_ButtonActive, bgActive);
        ImGui::PushStyleColor(ImGuiCol_Text, txtColor);

        bool clicked = ImGui::Button(label.c_str(), size);

        ImGui::PopStyleColor(4);
		return clicked;
	}

	void EntityPropertiesComponents::drawDragFloat(
			const std::string &label,
			float *values,  float speed,
			float min, float max,
			const std::string &format,
			ImU32 bg, ImU32 bgHovered, ImU32 bgActive)
	{
		ImGui::PushStyleColor(ImGuiCol_FrameBg,        bg);
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, bgHovered);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  bgActive);
		ImGui::DragFloat(label.c_str(), values, speed, min, max, format.c_str());
		ImGui::PopStyleColor(3);
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
				EntityPropertiesComponents::drawButton(badge.label, badge.size, badge.bg, badge.bgHovered, badge.bgActive, badge.txtColor);
			}
			ImGui::SameLine(0, 2);
			auto &slider = channels.sliders[i];
			EntityPropertiesComponents::drawDragFloat(
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

	bool EntityPropertiesComponents::drawColorButton(const std::string &label, ImVec2 size, ImVec4 color)
	{
		static bool clicked = false;
		if (ImGui::ColorButton(label.c_str(),
								color,
                                ImGuiColorEditFlags_NoTooltip,
                                size))
        {
            clicked = !clicked;
        }

        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        ImU32 borderColor = IM_COL32(150, 150, 150, 255);
        if (ImGui::IsItemHovered())
            borderColor = IM_COL32(200, 200, 200, 255);
        if (ImGui::IsItemActive())
            borderColor = IM_COL32(250, 250, 250, 255);

        // Draw the border manually using the window's draw list
        ImGui::GetWindowDrawList()->AddRect(p_min, p_max, borderColor, 2.0f, 0, 3.0f);
        return clicked;
	}
}
