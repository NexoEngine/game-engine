//// Widgets.cpp //////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        22/02/2025
//  Description: Source file for the widgets components
//
///////////////////////////////////////////////////////////////////////////////

#include "Widgets.hpp"

#include <imgui.h>

#include "IconsFontAwesome.h"
#include "Nexo.hpp"
#include "Texture.hpp"
#include "components/Camera.hpp"
#include "components/Uuid.hpp"
#include "ImNexo.hpp"

namespace ImNexo {

	bool ColorEditor(
			const std::string &label,
			glm::vec4 *selectedEntityColor,
			ImGuiColorEditFlags *colorPickerMode,
			bool *showPicker,
			const ImGuiColorEditFlags colorButtonFlags
	) {
		const ImGuiStyle &style = ImGui::GetStyle();
        const ImVec2 contentAvailable = ImGui::GetContentRegionAvail();
        bool colorModified = false;

        const std::string colorButton = std::string("##ColorButton") + label;

		const ImVec2 cogIconSize = ImGui::CalcTextSize(ICON_FA_COG);
		const ImVec2 cogIconPadding = style.FramePadding;
		const ImVec2 itemSpacing = style.ItemSpacing;

        // Color button
        ColorButton(
           	colorButton,
           	ImVec2(contentAvailable.x - cogIconSize.x - cogIconPadding.x * 2 - itemSpacing.x, 0), // Make room for the cog button
           	ImVec4(selectedEntityColor->x, selectedEntityColor->y, selectedEntityColor->z, selectedEntityColor->w),
            showPicker,
            colorButtonFlags
        );

        ImGui::SameLine();

		const std::string pickerSettings = std::string("##PickerSettings") + label;
        const std::string colorPickerPopup = std::string("##ColorPickerPopup") + label;

		// Cog button
        if (Button(std::string(ICON_FA_COG) + pickerSettings)) {
           	ImGui::OpenPopup(colorPickerPopup.c_str());
        }

        if (ImGui::BeginPopup(colorPickerPopup.c_str()))
        {
            ImGui::Text("Picker Mode:");
            if (ImGui::RadioButton("Hue Wheel", *colorPickerMode == ImGuiColorEditFlags_PickerHueWheel))
                *colorPickerMode = ImGuiColorEditFlags_PickerHueWheel;
            if (ImGui::RadioButton("Hue bar", *colorPickerMode == ImGuiColorEditFlags_PickerHueBar))
                *colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
            ImGui::EndPopup();
        }

        const std::string colorPickerInline = std::string("##ColorPickerInline") + label;
        if (*showPicker)
        {
            ImGui::Spacing();
            colorModified = ImGui::ColorPicker4(colorPickerInline.c_str(),
                                reinterpret_cast<float*>(selectedEntityColor), *colorPickerMode);
            if (ImGui::IsItemActive())
                setItemActive();
            if (ImGui::IsItemActivated())
                setItemActivated();
            if (ImGui::IsItemDeactivated())
                setItemDeactivated();
        }
        return colorModified;
	}

	void ButtonDropDown(const ImVec2& buttonPos, const ImVec2 buttonSize, const std::vector<ButtonProps> &buttonProps, bool &closure, DropdownOrientation orientation)
	{
	    constexpr float buttonSpacing = 5.0f;
        constexpr float padding = 10.0f;

        // Calculate menu dimensions
        const float menuWidth = buttonSize.x + padding;  // Add padding
        const float menuHeight =static_cast<float>(buttonProps.size()) * buttonSize.y +
                                (static_cast<float>(buttonProps.size()) - 1.0f) * buttonSpacing + 2 * buttonSpacing;

        // Calculate menu position based on orientation
        ImVec2 menuPos;
        switch (orientation) {
            case DropdownOrientation::DOWN:
                menuPos = ImVec2(buttonPos.x - padding / 2.0f, buttonPos.y + buttonSize.y);
                break;
            case DropdownOrientation::UP:
                menuPos = ImVec2(buttonPos.x - padding / 2.0f, buttonPos.y - menuHeight);
                break;
            case DropdownOrientation::RIGHT:
                menuPos = ImVec2(buttonPos.x + buttonSize.x, buttonPos.y - padding / 2.0f);
                break;
            case DropdownOrientation::LEFT:
                menuPos = ImVec2(buttonPos.x - menuWidth, buttonPos.y - padding / 2.0f);
                break;
        }

        // Adjust layout for horizontal orientations
        bool isHorizontal = (orientation == DropdownOrientation::LEFT ||
                            orientation == DropdownOrientation::RIGHT);

        // For horizontal layouts, swap width and height
        ImVec2 menuSize = isHorizontal ?
                         ImVec2(menuHeight, buttonSize.y + 10.0f) :
                         ImVec2(menuWidth, menuHeight);

        ImGui::SetNextWindowPos(menuPos);
        ImGui::SetNextWindowSize(menuSize);
        ImGui::SetNextWindowBgAlpha(0.2f);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5.0f, buttonSpacing));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing,
                           isHorizontal ? ImVec2(buttonSpacing, 0) : ImVec2(0, buttonSpacing));
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        if (ImGui::Begin("##PrimitiveMenuOverlay", nullptr,
                        ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_AlwaysAutoResize))
        {
            for (const auto &button : buttonProps)
            {
                // Strangely here the clicked inside here does not seem to work
                IconGradientButton(button.uniqueId, button.icon, ImVec2(buttonSize.x, buttonSize.y), button.buttonGradient);
                // So we rely on IsItemClicked from imgui
                if (button.onClick && ImGui::IsItemClicked(ImGuiMouseButton_Left))
                {
                    button.onClick();
                    closure = false;
                }
                if (button.onRightClick && ImGui::IsItemClicked(ImGuiMouseButton_Right))
                {
                    button.onRightClick();
                }
                if (!button.tooltip.empty() && ImGui::IsItemHovered())
                    ImGui::SetTooltip("%s", button.tooltip.c_str());
            }
        }
        // Check for clicks outside to close menu
        if (ImGui::IsMouseClicked(0) && !ImGui::IsWindowHovered())
        {
            closure = false;
        }
        ImGui::End();

        ImGui::PopStyleVar(3);
	}
}
