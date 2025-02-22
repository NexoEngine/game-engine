//// Widgets.cpp ///////////////////////////////////////////////////////////////
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
#include "Components.hpp"
#include "IconsFontAwesome.h"

namespace nexo::editor {
	bool Widgets::drawColorEditor(
			const std::string &label,
			glm::vec4 *selectedEntityColor,
			ImGuiColorEditFlags *colorPickerMode,
			bool *showPicker
	) {
        float availableWidth = ImGui::GetContentRegionAvail().x;
        bool colorModified = false;

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
        std::string colorButton = std::string("##ColorButton") + label;

        // Color button
        Components::drawColorButton(
           	colorButton.c_str(),
           	ImVec2(availableWidth - 35, 25), // Make room for the cog button
           	ImVec4(selectedEntityColor->x, selectedEntityColor->y, selectedEntityColor->z, selectedEntityColor->w),
            showPicker
        );

        ImGui::PopStyleVar();

        ImGui::SameLine();

        float cogButtonWidth = 25;
        float cogButtonHeight = 25;

        float fontSize = ImGui::GetFontSize();
        float verticalPadding = (cogButtonHeight - fontSize) * 0.5f;
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, verticalPadding + 2)); // Slightly add more textPaddingding to center the cog icon
        std::string pickerSettings = std::string("##PickerSettings") + label;
        std::string colorPickerPopup = std::string("##ColorPickerPopup") + label;

        // Cog button
        if (Components::drawButton(
           	std::string(ICON_FA_COG) + pickerSettings,
           	ImVec2(cogButtonWidth, cogButtonHeight),
           	IM_COL32(60, 60, 60, 255), IM_COL32(80, 80, 80, 255),
           	IM_COL32(100, 100, 100, 255), IM_COL32(255, 255, 255, 255)))
        {
           	ImGui::OpenPopup(colorPickerPopup.c_str());
        }
        ImGui::PopStyleVar();

        if (ImGui::BeginPopup(colorPickerPopup.c_str()))
        {
            ImGui::Text("Picker Mode:");
            if (ImGui::RadioButton("Hue Wheel", *colorPickerMode == ImGuiColorEditFlags_PickerHueWheel))
                *colorPickerMode = ImGuiColorEditFlags_PickerHueWheel;
            if (ImGui::RadioButton("Hue bar", *colorPickerMode == ImGuiColorEditFlags_PickerHueBar))
                *colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
            ImGui::EndPopup();
        }

        std::string colorPickerInline = std::string("##ColorPickerInline") + label;
        if (*showPicker)
        {
            ImGui::Spacing();
            colorModified = ImGui::ColorPicker4(colorPickerInline.c_str(),
                                reinterpret_cast<float*>(selectedEntityColor), *colorPickerMode);
        }
        return colorModified;
	}
}
