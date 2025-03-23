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
			static bool drawButton(const std::string &label, const ImVec2& size, ImU32 bg, ImU32 bgHovered, ImU32 bgActive, ImU32 txtColor);

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
			static bool drawDragFloat(const std::string &label, float *values, float speed, float min, float max, const std::string &format, ImU32 bg, ImU32 bgHovered, ImU32 bgActive, ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text));

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
			static bool drawIconButton(const std::string &label, ImVec2 size, ImU32 bg, ImU32 bgHovered, ImU32 bgActive, ImU32 txtColor);

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
	};
}
