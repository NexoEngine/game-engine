//// Components.hpp ///////////////////////////////////////////////////////////////
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

#include <iostream>
#include <vector>
#include <imgui.h>

namespace nexo::editor {

	struct ChannelLabel {
		std::string label;
		float fixedWidth = -1.0f;
	};

	struct Badge {
		std::string label;
		ImVec2 size;
		ImU32 bg;
		ImU32 bgHovered;
		ImU32 bgActive;
		ImU32 txtColor;
	};

	struct DragFloat {
		std::string label;
		float *value;
		float speed;
		float min;
		float max;
		ImU32 bg;
		ImU32 bgHovered;
		ImU32 bgActive;
		std::string format;
	};

	struct Channels {
		unsigned int count;
		std::vector<Badge> badges;
		std::vector<DragFloat> sliders;
	};

	class EntityPropertiesComponents {
		public:
			static bool drawHeader(const std::string &label, const std::string &headerText);
			static bool drawButton(const std::string &label, const ImVec2& size, ImU32 bg, ImU32 bgHovered, ImU32 bgActive, ImU32 txtColor);
			static void drawDragFloat(const std::string &label, float *values, float speed, float min, float max, const std::string &format, ImU32 bg, ImU32 bgHovered, ImU32 bgActive);
			static void drawRowLabel(const ChannelLabel &rowLabel);
			static void drawRowDragFloat(const Channels &channels);
			static bool drawColorButton(const std::string &label, ImVec2 size, ImVec4 color);
			static bool drawIconButton(const std::string &label, ImVec2 size, ImU32 bg, ImU32 bgHovered, ImU32 bgActive, ImU32 txtColor);
	};
}
