//// Components.hpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/02/2025
//  Description: Header file for the utilitary ImGui functions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>
#include <glm/glm.hpp>
#include <string>

namespace nexo::editor {

	class Components {
		public:
			static bool drawButton(const std::string &label, const ImVec2& size, ImU32 bg, ImU32 bgHovered, ImU32 bgActive, ImU32 txtColor);
			static void drawButtonBorder(ImU32 borderColor, ImU32 borderColorHovered, ImU32 borderColorActive, float rounding = 2.0f, ImDrawFlags flags = 0, float thickness = 3.0f);
			static bool drawDragFloat(const std::string &label, float *values, float speed, float min, float max, const std::string &format, ImU32 bg, ImU32 bgHovered, ImU32 bgActive);
			static bool drawIconButton(const std::string &label, ImVec2 size, ImU32 bg, ImU32 bgHovered, ImU32 bgActive, ImU32 txtColor);
			static void drawColorButton(const std::string &label, ImVec2 size, ImVec4 color, bool *clicked = nullptr, ImGuiColorEditFlags flags = ImGuiColorEditFlags_None);
			static void drawCustomSeparatorText(const std::string &text, float textPadding, float leftSpacing, float thickness, ImU32 lineColor, ImU32 textColor);
	};
}
