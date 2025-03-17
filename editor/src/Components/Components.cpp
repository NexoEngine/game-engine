//// Components.cpp ///////////////////////////////////////////////////////////
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

	bool Components::drawButton(
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

	void Components::drawButtonBorder(
			ImU32 borderColor,
			ImU32 borderColorHovered,
			ImU32 borderColorActive,
			float rounding,
			ImDrawFlags flags,
		 	float thickness
	) {
		ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        ImU32 color = borderColor;
        if (ImGui::IsItemHovered())
            color = borderColorHovered;
        if (ImGui::IsItemActive())
            color = borderColorActive;

        ImGui::GetWindowDrawList()->AddRect(p_min, p_max, color, rounding, flags, thickness);
	}

	bool Components::drawDragFloat(
			const std::string &label,
			float *values,  float speed,
			float min, float max,
			const std::string &format,
			ImU32 bg, ImU32 bgHovered, ImU32 bgActive, ImU32 textColor
	) {
		ImGui::PushStyleColor(ImGuiCol_Text, 		   textColor);
		ImGui::PushStyleColor(ImGuiCol_FrameBg,        bg);
		ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, bgHovered);
		ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  bgActive);
		bool clicked = ImGui::DragFloat(label.c_str(), values, speed, min, max, format.c_str());
		ImGui::PopStyleColor(4);
		return clicked;
	}

	void Components::drawColorButton(const std::string &label, ImVec2 size, ImVec4 color, bool *clicked, ImGuiColorEditFlags flags)
	{
		flags |= ImGuiColorEditFlags_NoTooltip;
		if (ImGui::ColorButton(label.c_str(),
								color,
                                flags,
                                size))
        {
        	if (clicked)
            	*clicked = !*clicked;
        }

        Components::drawButtonBorder(IM_COL32(150, 150, 150, 255), IM_COL32(200, 200, 200, 255), IM_COL32(250, 250, 250, 255));
	}

	void Components::drawCustomSeparatorText(const std::string &text, float textPadding, float leftSpacing, float thickness, ImU32 lineColor, ImU32 textColor)
	{
		ImGui::SetWindowFontScale(1.2f);
        ImVec2 pos = ImGui::GetCursorScreenPos();
        float availWidth = ImGui::GetContentRegionAvail().x;
        float textWidth  = ImGui::CalcTextSize(text.c_str()).x;

        // Compute the length of each line. Clamp to zero if the region is too small.
        float lineWidth = (availWidth - textWidth - 2 * textPadding) * leftSpacing;
        if (lineWidth < 0.0f)
            lineWidth = 0.0f;

        // Compute Y coordinate to draw lines so they align with the text center.
        float lineY = pos.y + ImGui::GetTextLineHeight() * 0.5f;
        ImU32 col = ImGui::GetColorU32(ImGuiCol_Separator);

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        ImVec2 lineStart(pos.x, lineY);
        ImVec2 lineEnd(pos.x + lineWidth, lineY);
        draw_list->AddLine(lineStart, lineEnd, lineColor, thickness);

        ImVec2 textPos(pos.x + lineWidth + textPadding, pos.y);
        draw_list->AddText(textPos, textColor, text.c_str());
        ImGui::SetWindowFontScale(1.0f);

        ImVec2 rightLineStart(pos.x + lineWidth + textPadding + textWidth + textPadding, lineY);
        ImVec2 rightLineEnd(pos.x + availWidth, lineY);
        draw_list->AddLine(rightLineStart, rightLineEnd, lineColor, thickness);

        ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
	}
}
