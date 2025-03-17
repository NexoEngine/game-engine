//// EntityPropertiesComponents.hpp ///////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/05/2025
//  Description: Header file for the entity properties components
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>
#include <string>
#include <vector>

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
		ImU32 textColor;
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
			static void drawRowLabel(const ChannelLabel &rowLabel);
			static bool drawRowDragFloat1(
				const char *uniqueLabel,
				const std::string &badgeLabel,
				float *value,
				float minValue = -FLT_MAX,
				float maxValue = FLT_MAX,
				float speed = 0.3f);
			static bool drawRowDragFloat2(
				const char *uniqueLabel,
				const std::string &badLabelX,
				const std::string &badLabelY,
				float *values,
				float minValue = -FLT_MAX,
				float maxValue = FLT_MAX,
				float speed = 0.3f,
				std::vector<ImU32> badgeColor = {},
				std::vector<ImU32> textBadgeColor = {},
				bool inactive = false
			);
			static bool drawRowDragFloat3(
				const char *uniqueLabel,
				const std::string &badLabelX,
				const std::string &badLabelY,
				const std::string &badLabelZ,
				float *values,
				float minValue = -FLT_MAX,
				float maxValue = FLT_MAX,
				float speed = 0.3f,
				std::vector<ImU32> badgeColors = {},
				std::vector<ImU32> textBadgeColors = {}
			);
			static bool drawRowDragFloat(const Channels &channels);
			static bool drawToggleButtonWithSeparator(const std::string &label, bool* toggled);
	};
}
