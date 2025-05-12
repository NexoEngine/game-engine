//// Elements.hpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        18/04/2025
//  Description: Header file for the ui elements
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <string>
#include <vector>

namespace ImNexo {

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
    * @brief Draw an icon centered within a rectangle with optional vertical positioning
    * @param[in] icon Text of the icon to draw
    * @param[in] p_min Minimum bounds of the rectangle
    * @param[in] p_max Maximum bounds of the rectangle
    * @param[in] color Color of the icon
    * @param[in] scale Scale factor for the icon font
    * @param[in] verticalPosition Vertical position factor (0-1), 0.5 for centered
    * @param[in] horizontalPosition Horizontal position factor (0-1), 0.5 for centered
    * @param[in] font Font to use (nullptr for current font)
    */
    void CenteredIcon(
        const std::string& icon,
        const ImVec2& p_min,
        const ImVec2& p_max,
        ImU32 color,
        float scale = 1.0f,
        float verticalPosition = 0.5f,
        float horizontalPosition = 0.5f,
        ImFont* font = nullptr
    );


    /**
    * @brief Draw wrapped text within bounds, attempts to split on spaces for better appearance
    * @param[in] text Text to draw
    * @param[in] p_min Minimum bounds
    * @param[in] p_max Maximum bounds
    * @param[in] color Text color
    * @param[in] verticalPosition Vertical position (0-1), 0.5 for centered
    */
    void WrappedCenteredText(
        const std::string& text,
        const ImVec2& p_min,
        const ImVec2& p_max,
        ImU32 color,
        float verticalPosition = 0.5f
    );

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
	bool Button(
		const std::string &label,
		const ImVec2& size = ImVec2(0, 0),
		ImU32 bg = 0,
		ImU32 bgHovered = 0,
		ImU32 bgActive = 0,
		ImU32 txtColor = 0
	);

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
    void ButtonBorder(
        ImU32 borderColor,
        ImU32 borderColorHovered,
        ImU32 borderColorActive,
        float rounding = 2.0f,
        ImDrawFlags flags = 0,
        float thickness = 3.0f
    );

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
    void ButtonInnerBorder(
		ImU32 borderColor,
		ImU32 borderColorHovered,
		ImU32 borderColorActive,
		float rounding = 2.0f,
		ImDrawFlags flags = 0,
		float thickness = 3.0f
	);

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
    bool DragFloat(
        const std::string &label,
        float *values,
        float speed,
        float min,
        float max,
        const std::string &format,
        ImU32 bg = 0,
        ImU32 bgHovered = 0,
        ImU32 bgActive = 0,
        ImU32 textColor = 0
    );

    /**
    * @struct GradientStop
    * @brief Defines a color position in a gradient
    *
    * Each gradient stop has a position (from 0.0 to 1.0) that represents
    * where along the gradient the color appears, and a color value in ImGui's
    * 32-bit color format.
    */
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
    void RectFilledLinearGradient(
		const ImVec2& pMin,
		const ImVec2& pMax,
		float angle,
		std::vector<GradientStop> stops,
		ImDrawList* drawList = nullptr
	);

    /**
    * @brief Draws a collapsible header with centered text
    *
    * @param[in] label Unique label/ID for the header
    * @param[in] headerText Text to display in the header
    * @return true if the header is open/expanded, false otherwise
    */
	bool Header(const std::string &label, std::string_view headerText);

    /**
    * @brief Draws a row label in the current table column
    *
    * @param[in] rowLabel The label configuration to draw
    */
	void RowLabel(const ChannelLabel &rowLabel);

    /**
    * @brief Draws an arrow shape indicating expanded/collapsed state
    *
    * Creates a filled triangle pointing downward (expanded) or rightward (collapsed)
    * that is commonly used to indicate a toggleable/expandable UI element.
    *
    * @param center Center point of the arrow
    * @param isExpanded Whether the arrow should show expanded state (down arrow) or collapsed state (right arrow)
    * @param color Color of the arrow
    * @param size Size of the arrow from center to tip
    */
	void Arrow(const ImVec2& center, bool isExpanded, ImU32 color, float size);

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
	void CustomSeparatorText(const std::string &text, float textPadding, float leftSpacing, float thickness, ImU32 lineColor, ImU32 textColor);

    /**
    * @brief ImGui::Image wrapper with different default UV coordinates (to flip the Y-axis).
    *
    * This function behaves exactly like ImGui::Image, except that the default UV coordinates are
    * flipped to invert the image vertically. If you provide custom UV coordinates, the flipping
    * behavior is effectively disabled.
    *
    * @param[in] user_texture_id  The texture identifier for ImGui to render.
    * @param[in] image_size       The size of the image on the screen (in pixels).
    * @param[in] uv0              The normalized UV coordinate for the top-left corner of the texture.
    * @param[in] uv1              The normalized UV coordinate for the bottom-right corner of the texture.
    * @param[in] tint_col         The tint color applied to the image.
    * @param[in] border_col       The border color drawn around the image, if any.
    */
    void Image(
        ImTextureID user_texture_id,
        const ImVec2& image_size,
        const ImVec2& uv0 = ImVec2(0, 1), // Flipped Y
        const ImVec2& uv1 = ImVec2(1, 0), // Flipped Y
        const ImVec4& tint_col = ImVec4(1, 1, 1, 1),
        const ImVec4& border_col = ImVec4(0, 0, 0, 0)
    );

    /**
    * @brief ImGui::ImageButton wrapper with different default UV coordinates (to flip the Y-axis).
    *
    * This function behaves exactly like ImGui::ImageButton, except that the default UV coordinates are
    * flipped to invert the image vertically. If you provide custom UV coordinates, the flipping
    * behavior is effectively disabled.
    *
    * @param[in] str_id           The unique label for the image button.
    * @param[in] user_texture_id  The texture identifier for ImGui to render.
    * @param[in] image_size       The size of the image on the screen (in pixels).
    * @param[in] uv0              The normalized UV coordinate for the top-left corner of the texture.
    * @param[in] uv1              The normalized UV coordinate for the bottom-right corner of the texture.
    * @param[in] bg_col           The background color of the button.
    * @param[in] tint_col         The tint color applied to the image.
    */
    bool ImageButton(
        const char* str_id,
        ImTextureID user_texture_id,
        const ImVec2& image_size,
        const ImVec2& uv0 = ImVec2(0, 1), // Flipped Y
        const ImVec2& uv1 = ImVec2(1, 0), // Flipped Y
        const ImVec4& bg_col = ImVec4(0, 0, 0, 0),
        const ImVec4& tint_col = ImVec4(1, 1, 1, 1)
    );

}
