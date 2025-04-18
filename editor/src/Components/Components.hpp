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
#include <string>
#include <vector>
#include <functional>

#include "ecs/Coordinator.hpp"

namespace nexo::editor {

	/**
	* @brief A collection of utility functions for custom ImGui components.
	*
	* This class provides helper functions to draw custom buttons, drag floats,
	* color buttons, and separators with text.
	*/
	class Components {
		public:
  			/**
             * @brief Draws a button with custom style colors.
             *
             * Pushes custom style colors for the button and its states, draws the button,
             * and then pops the style colors.
             *
             * @param label The button label.
             * @param size The size of the button.
             * @param bg The background color.
             * @param bgHovered The background color when hovered.
             * @param bgActive The background color when active.
             * @param txtColor The text color.
             * @return true if the button was clicked; false otherwise.
             */
			static bool drawButton(const std::string &label, const ImVec2& size = ImVec2(0, 0), ImU32 bg = 0, ImU32 bgHovered = 0, ImU32 bgActive = 0, ImU32 txtColor = 0);

			static bool drawComponentButton(const std::string &uniqueId, const std::string &icon, const std::string &label, const ImVec2 &itemSize);

   			/**
             * @brief Draws a border around the last item.
             *
             * Uses the current item's rectangle and draws a border with specified colors
             * for normal, hovered, and active states.
             *
             * @param borderColor The border color for normal state.
             * @param borderColorHovered The border color when hovered.
             * @param borderColorActive The border color when active.
             * @param rounding The rounding of the border corners.
             * @param flags Additional draw flags.
             * @param thickness The thickness of the border.
             */
			static void drawButtonBorder(ImU32 borderColor, ImU32 borderColorHovered, ImU32 borderColorActive, float rounding = 2.0f, ImDrawFlags flags = 0, float thickness = 3.0f);

            /**
             * @brief Draws a border inside the last item.
             *
             * Similar to drawButtonBorder, but draws a border inside the item rectangle instead of outside.
             *
             * @param borderColor The border color for normal state.
             * @param borderColorHovered The border color when hovered.
             * @param borderColorActive The border color when active.
             * @param rounding The rounding of the border corners.
             * @param flags Additional draw flags.
             * @param thickness The thickness of the border.
             */
		    static void drawButtonInnerBorder(ImU32 borderColor, ImU32 borderColorHovered, ImU32 borderColorActive, float rounding = 2.0f, ImDrawFlags flags = 0, float thickness = 3.0f);


   			/**
             * @brief Draws a draggable float widget with custom styling.
             *
             * Pushes custom style colors for the drag float widget, draws it, and then pops the styles.
             *
             * @param label The label for the drag float.
             * @param values Pointer to the float value.
             * @param speed The speed of value change.
             * @param min The minimum allowable value.
             * @param max The maximum allowable value.
             * @param format The display format.
             * @param bg The background color.
             * @param bgHovered The background color when hovered.
             * @param bgActive The background color when active.
             * @param textColor The text color.
             * @return true if the value was changed; false otherwise.
             */
			static bool drawDragFloat(const std::string &label, float *values, float speed, float min, float max, const std::string &format, ImU32 bg = 0, ImU32 bgHovered = 0, ImU32 bgActive = 0, ImU32 textColor = 0);

			/**
             * @brief Draws an icon button with custom style colors.
             *
             * Similar to drawButton, but intended for icon-only buttons.
             *
             * @param label The label for the button.
             * @param size The size of the button.
             * @param bg The background color.
             * @param bgHovered The background color when hovered.
             * @param bgActive The background color when active.
             * @param txtColor The text (icon) color.
             * @return true if the button was clicked; false otherwise.
             */
			static bool drawIconButton(const std::string &label, ImVec2 size = ImVec2(0, 0), ImU32 bg = 0, ImU32 bgHovered = 0, ImU32 bgActive = 0, ImU32 txtColor = 0);

			/**
             * @brief Draws a color button with a border.
             *
             * Displays a color button with the provided label and size. Optionally toggles a clicked state.
             *
             * @param label The label for the color button.
             * @param size The size of the button.
             * @param color The color to display.
             * @param clicked Optional pointer to a boolean that is toggled when the button is clicked.
             * @param flags Additional color edit flags.
             */
			static void drawColorButton(const std::string &label, ImVec2 size, ImVec4 color, bool *clicked = nullptr, ImGuiColorEditFlags flags = ImGuiColorEditFlags_None);

			/**
             * @brief Draws a custom separator with centered text.
             *
             * Renders a separator line with text in the middle, with customizable padding, spacing,
             * thickness, and colors.
             *
             * @param text The text to display at the separator.
             * @param textPadding Padding around the text.
             * @param leftSpacing The spacing multiplier for the left separator line.
             * @param thickness The thickness of the separator lines.
             * @param lineColor The color of the separator lines.
             * @param textColor The color of the text.
             */
			static void drawCustomSeparatorText(const std::string &text, float textPadding, float leftSpacing, float thickness, ImU32 lineColor, ImU32 textColor);

			struct GradientStop
			{
				float pos;   // percentage position along the gradient [0.0f, 1.0f]
				ImU32 color; // color at this stop
			};

		    /**
		     * @brief Draw filled rectangle with a linear gradient defined by an arbitrary angle and gradient stops.
		     * @param pMin Upper left corner position of the rectangle
		     * @param pMax Lower right corner position of the rectangle
		     * @param angle Angle of the gradient in degrees (0.0f = down, 90.0f = right, 180.0f = up, 270.0f = left)
		     * @param stops Vector of gradient stops, each defined by a position (0.0f to 1.0f) and a color
		     */
		    static void drawRectFilledLinearGradient(const ImVec2& pMin, const ImVec2& pMax, float angle, std::vector<GradientStop> stops, ImDrawList* drawList = nullptr);

		    static bool drawToolbarButton(const std::string& uniqueId, const std::string& icon,
				const ImVec2& size = ImVec2(40, 40),
                const std::vector<GradientStop>& gradientStops = {
                    {0.0f, IM_COL32(60, 60, 80, 255)},
                    {1.0f, IM_COL32(30, 30, 40, 255)}
                },
                float gradientAngle = 45.0f,
                const ImU32 borderColor = IM_COL32(100, 100, 120, 255),
                const ImU32 borderColorHovered = IM_COL32(150, 150, 200, 255),
                const ImU32 borderColorActive = IM_COL32(200, 200, 255, 255),
                const ImU32 iconColor = IM_COL32(255, 255, 255, 255)
			);



		    static bool drawRowEntityDropdown(const std::string &label, ecs::Entity &targetEntity,
                                     const std::vector<ecs::Entity>& entities,
                                     const std::function<std::string(ecs::Entity)>& getNameFunc);
	};
}
