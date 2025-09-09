//// Components.hpp ///////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        17/02/2025
//  Description: Header file for the utilitary ImGui functions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <filesystem>
#include <imgui.h>
#include <string>
#include <vector>
#include <functional>

#include "ecs/Coordinator.hpp"
#include "renderer/Texture.hpp"
#include "Elements.hpp"

namespace ImNexo {

    /**
     * @brief Creates a button with both an icon and text label.
     *
     * Renders a custom button with an icon positioned at the top portion of the button
     * and a text label below it. The text automatically wraps if it's too wide for the button.
     *
     * @param uniqueId A unique identifier string for ImGui to track this widget
     * @param icon The icon string to display (typically a FontAwesome character)
     * @param label The text label to display below the icon
     * @param itemSize The size dimensions of the button
     * @return true if the button was clicked, false otherwise
     */
	bool ButtonWithIconAndText(const std::string &uniqueId, const std::string &icon, const std::string &label, const ImVec2 &itemSize);

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
	void ColorButton(const std::string &label, ImVec2 size, ImVec4 color, bool *clicked = nullptr, ImGuiColorEditFlags flags = ImGuiColorEditFlags_None);

    /**
    * @brief Draws a texture button that displays a texture preview.
    *
    * When pressed, opens a file dialog to select a new texture. If a path was
    * selected, it is set in `outPath` and the function returns true.
    *
    * @param[in] label A unique label identifier for the button.
    * @param[in] texture A shared pointer to the renderer::NxTexture2D that holds the texture.
    * @param[out] outPath The path to the selected texture.
    * @return true if a texture path was set; false otherwise.
    */
	bool TextureButton(const std::string &label, const std::shared_ptr<nexo::renderer::NxTexture2D>& texture, std::filesystem::path& outPath);

    /**
     * @brief Creates a customizable gradient button with a centered icon.
     *
     * Renders a button with a linear gradient background, customizable border colors
     * for different states (normal, hovered, active), and a centered icon.
     *
     * @param uniqueId A unique identifier string for ImGui to track this widget
     * @param icon The icon string to display (typically a FontAwesome character)
     * @param size Button dimensions (width, height)
     * @param gradientStops Array of gradient color stops defining the background gradient
     * @param gradientAngle Angle of the linear gradient in degrees
     * @param borderColor Color of the button border in normal state
     * @param borderColorHovered Color of the button border when hovered
     * @param borderColorActive Color of the button border when active/pressed
     * @param iconColor Color of the icon
     * @return true if the button was clicked, false otherwise
     */
    bool IconGradientButton(const std::string& uniqueId, const std::string& icon,
		const ImVec2& size = ImVec2(40, 40),
        const std::vector<GradientStop>& gradientStops = {
            {0.0f, IM_COL32(60, 60, 80, 255)},
            {1.0f, IM_COL32(30, 30, 40, 255)}
        },
        float gradientAngle = 45.0f,
        ImU32 borderColor = IM_COL32(100, 100, 120, 255),
        ImU32 borderColorHovered = IM_COL32(150, 150, 200, 255),
        ImU32 borderColorActive = IM_COL32(200, 200, 255, 255),
        ImU32 iconColor = IM_COL32(255, 255, 255, 255)
	);

    /**
     * @brief Displays a dropdown to select an entity from a list.
     *
     * Creates a row in a table with a label and dropdown menu showing
     * available entities. Updates the target entity when a selection is made.
     *
     * @param label Text label displayed next to the dropdown
     * @param targetEntity Reference to the entity variable that will be updated with the selection
     * @param entities Vector of available entities to choose from
     * @param getNameFunc Function that converts an entity ID to a displayable name string
     * @return true if an entity was selected (value changed), false otherwise
     */
    bool RowEntityDropdown(const std::string &label, nexo::ecs::Entity &targetEntity,
                                const std::vector<nexo::ecs::Entity>& entities,
                                const std::function<std::string(nexo::ecs::Entity)>& getNameFunc);

	/**
    * @brief Draws a row with multiple channels (badge + slider pairs)
    *
    * This is a lower-level function used by the other drawRowDragFloatX functions.
    *
    * @param[in] channels The channel configuration to draw
    * @return true if any value was changed, false otherwise
    */
	bool RowDragFloat(const Channels &channels);

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
	bool RowDragFloat1(
		const char *uniqueLabel,
		const std::string &badgeLabel,
		float *value,
		float minValue = -FLT_MAX,
		float maxValue = FLT_MAX,
		float speed = 0.3f
	);


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
    * @param[in] disabled If true, renders in an inactive/disabled state (default: false)
    * @return true if any value was changed, false otherwise
    */
	bool RowDragFloat2(
		const char *uniqueLabel,
		const std::string &badLabelX,
		const std::string &badLabelY,
		float *values,
		float minValue = -FLT_MAX,
		float maxValue = FLT_MAX,
		float speed = 0.3f,
		std::vector<ImU32> badgeColor = {},
		std::vector<ImU32> textBadgeColor = {},
		bool disabled = false
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
	bool RowDragFloat3(
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
    * @brief Draws a toggle button with a separator and label
    *
    * Creates a collapsible section control with an arrow that toggles
    * between expanded and collapsed states.
    *
    * @param[in] label The label to display
    * @param[in,out] toggled Pointer to bool that tracks the toggle state
    * @return true if the toggle state changed, false otherwise
    */
	bool ToggleButtonWithSeparator(const std::string &label, bool* toggled);
}
