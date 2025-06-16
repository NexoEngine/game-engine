//// Widgets.hpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/02/2025
//  Description: Header file for the widgets components
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>
#include <string>
#include "EntityFactory3D.hpp"

#include "Components.hpp"

namespace ImNexo {

    /**
    * @brief Draws a color editor with a button and an optional inline color picker.
    *
    * Displays a custom color button (with a cog icon for picker settings) and, if enabled,
    * an inline color picker. The function returns true if the color was modified.
    *
    * @param label A unique label identifier for the widget.
    * @param selectedEntityColor Pointer to the glm::vec4 representing the current color.
    * @param colorPickerMode Pointer to the ImGuiColorEditFlags for the picker mode.
    * @param showPicker Pointer to a boolean that determines if the inline color picker is visible.
    * @param colorButtonFlags Optional flags for the color button (default is none).
    * @return true if the color was modified; false otherwise.
    */
	bool ColorEditor(
		const std::string &label,
		glm::vec4 *selectedEntityColor,
		ImGuiColorEditFlags *colorPickerMode,
		bool *showPicker,
		ImGuiColorEditFlags colorButtonFlags = ImGuiColorEditFlags_None
	);

	/**
     * @brief Configuration properties for a button in a dropdown menu.
     *
     * This structure defines the appearance and behavior of buttons in a
     * dropdown menu created with ButtonDropDown. It allows for specifying
     * icons, callbacks for different mouse actions, tooltips, and custom styling.
     */
    struct ButtonProps {
        std::string uniqueId;       ///< Unique identifier for ImGui tracking
        std::string icon;           ///< Icon to display on the button (typically FontAwesome)
        std::function<void()> onClick = nullptr;      ///< Callback executed when button is left-clicked
        std::function<void()> onRightClick = nullptr; ///< Callback executed when button is right-clicked
        std::string tooltip;   ///< Tooltip text displayed when hovering

        /**
        * @brief Gradient colors for button styling
        *
        * Default gradient uses a dark blue theme that matches the editor's style.
        * Override this with custom colors to create visually distinct buttons.
        */
        std::vector<GradientStop> buttonGradient = {
            {0.0f, IM_COL32(50, 50, 70, 230)},
            {1.0f, IM_COL32(30, 30, 45, 230)}
        };
    };

    enum class DropdownOrientation {
        DOWN,   // Dropdown appears below the button
        UP,     // Dropdown appears above the button
        RIGHT,  // Dropdown appears to the right of the button
        LEFT    // Dropdown appears to the left of the button
    };

    /**
     * @brief Creates a dropdown menu of buttons at a specified position.
     *
     * Displays a configurable dropdown menu containing multiple buttons defined by ButtonProps.
     * The dropdown automatically closes when a button is clicked or when clicking outside
     * the dropdown area. Button layout adapts based on the specified orientation.
     *
     * @param buttonPos Position where the dropdown should appear, typically the position of the trigger button
     * @param buttonSize Size dimensions for each button in the dropdown
     * @param buttonProps Vector of button configurations (icons, callbacks, tooltips, etc.)
     * @param closure Reference to a boolean flag controlling dropdown visibility; set to false to close
     * @param orientation Direction the dropdown should expand (DOWN, UP, LEFT, RIGHT)
     */
	void ButtonDropDown(
		const ImVec2& buttonPos,
		ImVec2 buttonSize,
		const std::vector<ButtonProps> &buttonProps,
		bool &closure,
		DropdownOrientation orientation = DropdownOrientation::DOWN
	);

	void PrimitiveSubMenu(int sceneId);
	void PrimitiveCustomizationMenu(int sceneId, nexo::Primitives primitive);

}
