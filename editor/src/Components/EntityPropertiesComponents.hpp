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

	/**
     * @struct ChannelLabel
     * @brief Represents a label for a channel in the entity properties editor
     *
     * Labels can have optional fixed width for precise layout control.
     */
	struct ChannelLabel {
		std::string label;
		float fixedWidth = -1.0f;
	};

	/**
     * @struct Badge
     * @brief A styled badge component with customizable appearance
     *
     * Used as visual indicators or labels in the UI, typically alongside sliders.
     */
	struct Badge {
		std::string label;  ///< The displayed text
		ImVec2 size;        ///< Size of the badge in pixels
		ImU32 bg;           ///< Background color
		ImU32 bgHovered;    ///< Background color when hovered
		ImU32 bgActive;     ///< Background color when active
		ImU32 txtColor;     ///< Text color
	};

	/**
     * @struct DragFloat
     * @brief A drag float slider component with customizable appearance
     *
     * Used for editing float values with adjustable range and visual styling.
     */
	struct DragFloat {
		std::string label;   ///< Unique label/ID for the component
		float *value;        ///< Pointer to the value being edited
		float speed;         ///< Speed of value change during dragging
		float min;           ///< Minimum value
		float max;           ///< Maximum value
		ImU32 bg;            ///< Background color
		ImU32 bgHovered;     ///< Background color when hovered
		ImU32 bgActive;      ///< Background color when active
		ImU32 textColor;     ///< Text color
		std::string format;  ///< Format string for displaying the value
	};

	/**
     * @struct Channels
     * @brief A collection of badges and sliders forming a multi-channel editing row
     *
     * Used to create rows with multiple editable values (like X, Y, Z components).
     */
	struct Channels {
		unsigned int count;              ///< Number of channels
		std::vector<Badge> badges;       ///< Badge component for each channel
		std::vector<DragFloat> sliders;  ///< Slider component for each channel
	};

	/**
     * @class EntityPropertiesComponents
     * @brief Static class providing UI components for entity property editing
     *
     * This class offers methods to draw various ImGui-based UI components
     * specifically designed for editing entity properties in a consistent
     * and visually appealing way.
     */
	class EntityPropertiesComponents {
		public:
			/**
             * @brief Draws a collapsible header with centered text
             *
             * @param[in] label Unique label/ID for the header
             * @param[in] headerText Text to display in the header
             * @return true if the header is open/expanded, false otherwise
             */
			static bool drawHeader(const std::string &label, const std::string &headerText);

			/**
             * @brief Draws a row label in the current table column
             *
             * @param[in] rowLabel The label configuration to draw
             */
			static void drawRowLabel(const ChannelLabel &rowLabel);

			/**
             * @brief Draws a row with a single float value slider
             *
             * @param[in] uniqueLabel Unique label/ID for the component
             * @param[in] badgeLabel Text for the badge (empty for no badge)
             * @param[in,out] value Pointer to the float value to edit
             * @param[in] minValue Minimum allowed value (default: -FLT_MAX)
             * @param[in] maxValue Maximum allowed value (default: FLT_MAX)
             * @param[in] speed Speed of value change during dragging (default: 0.3f)
             * @return true if the value was changed, false otherwise
             */
			static bool drawRowDragFloat1(
				const char *uniqueLabel,
				const std::string &badgeLabel,
				float *value,
				float minValue = -FLT_MAX,
				float maxValue = FLT_MAX,
				float speed = 0.3f);

			/**
             * @brief Draws a row with two float value sliders (X and Y components)
             *
             * @param[in] uniqueLabel Unique label/ID for the component
             * @param[in] badLabelX Text for the X component badge
             * @param[in] badLabelY Text for the Y component badge
             * @param[in,out] values Pointer to array of two float values to edit
             * @param[in] minValue Minimum allowed value (default: -FLT_MAX)
             * @param[in] maxValue Maximum allowed value (default: FLT_MAX)
             * @param[in] speed Speed of value change during dragging (default: 0.3f)
             * @param[in] badgeColor Optional custom colors for badges
             * @param[in] textBadgeColor Optional custom text colors for badges
             * @param[in] inactive If true, renders in an inactive/disabled state (default: false)
             * @return true if any value was changed, false otherwise
             */
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

			/**
             * @brief Draws a row with three float value sliders (X, Y, and Z components)
             *
             * @param[in] uniqueLabel Unique label/ID for the component
             * @param[in] badLabelX Text for the X component badge
             * @param[in] badLabelY Text for the Y component badge
             * @param[in] badLabelZ Text for the Z component badge
             * @param[in,out] values Pointer to array of three float values to edit
             * @param[in] minValue Minimum allowed value (default: -FLT_MAX)
             * @param[in] maxValue Maximum allowed value (default: FLT_MAX)
             * @param[in] speed Speed of value change during dragging (default: 0.3f)
             * @param[in] badgeColors Optional custom colors for badges
             * @param[in] textBadgeColors Optional custom text colors for badges
             * @return true if any value was changed, false otherwise
             */
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

			/**
             * @brief Draws a row with multiple channels (badge + slider pairs)
             *
             * This is a lower-level function used by the other drawRowDragFloatX functions.
             *
             * @param[in] channels The channel configuration to draw
             * @return true if any value was changed, false otherwise
             */
			static bool drawRowDragFloat(const Channels &channels);

			/**
             * @brief Draws a toggle button with a separator and label
             *
             * Creates a collapsible section control with an arrow that toggles
             * between expanded and collapsed states.
             *
             * @param[in] label The label to display
             * @param[in,out] toggled Pointer to bool that tracks the toggle state
             * @return true if the toggle state changed, false otherwise
             */
			static bool drawToggleButtonWithSeparator(const std::string &label, bool* toggled);
	};
}
