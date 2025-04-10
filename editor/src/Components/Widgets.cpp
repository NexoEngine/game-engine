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

#include <imgui_internal.h>
#include <Logger.hpp>

#include "Components.hpp"
#include "IconsFontAwesome.h"
#include "tinyfiledialogs.h"

namespace nexo::editor {
	bool Widgets::drawColorEditor(
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
        Components::drawColorButton(
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
        if (Components::drawButton(std::string(ICON_FA_COG) + pickerSettings)) {
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
        }
        return colorModified;
	}

	bool Widgets::drawTextureButton(const std::string &label, std::shared_ptr<renderer::Texture2D> &texture)
	{
		bool textureModified = false;
		constexpr ImVec2 previewSize(32, 32);
        ImGui::PushID(label.c_str());

        const ImTextureID textureId = texture ? static_cast<ImTextureID>(static_cast<intptr_t>(texture->getId())) : 0;
        const std::string textureButton = std::string("##TextureButton") + label;

        if (ImGui::ImageButton(textureButton.c_str(), textureId, previewSize))
        {
            const char* filePath = tinyfd_openFileDialog(
                "Open Texture",
                "",
                0,
                nullptr,
                nullptr,
                0
            );

            if (filePath)
            {
	            const std::string path(filePath);
                std::shared_ptr<renderer::Texture2D> newTexture = renderer::Texture2D::create(path);
                if (newTexture)
                {
                    texture = newTexture;
                    textureModified = true;
                }
            }
        }
        Components::drawButtonBorder(IM_COL32(255,255,255,0), IM_COL32(255,255,255,255), IM_COL32(255,255,255,0), 0.0f, 0, 2.0f);
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::Text("%s", label.c_str());
		return textureModified;
	}

	bool Widgets::drawMaterialInspector(components::Material *material)
	{
		bool modified = false;
		// --- Shader Selection ---
		ImGui::BeginGroup();
		{
			ImGui::Text("Shader:");
			ImGui::SameLine();

			static int currentShaderIndex = 0;
			const char* shaderOptions[] = { "Standard", "Unlit", "CustomPBR" };
			const float availableWidth = ImGui::GetContentRegionAvail().x;
			ImGui::SetNextItemWidth(availableWidth);

			if (ImGui::Combo("##ShaderCombo", &currentShaderIndex, shaderOptions, IM_ARRAYSIZE(shaderOptions)))
			{
				//TODO: implement shader selection
			}
		}
		ImGui::EndGroup();
		ImGui::Spacing();

		// --- Rendering mode selection ---
		ImGui::Text("Rendering mode:");
		ImGui::SameLine();
		static int currentRenderingModeIndex = 0;
		const char* renderingModeOptions[] = { "Opaque", "Transparent", "Refraction" };
		float availableWidth = ImGui::GetContentRegionAvail().x;

		ImGui::SetNextItemWidth(availableWidth);
		if (ImGui::Combo("##RenderingModeCombo", &currentRenderingModeIndex, renderingModeOptions, IM_ARRAYSIZE(renderingModeOptions)))
		{
			//TODO: implement rendering mode
		}

	    // --- Albedo texture ---
	    static ImGuiColorEditFlags colorPickerModeAlbedo = ImGuiColorEditFlags_PickerHueBar;
	    static bool showColorPickerAlbedo = false;
		modified = Widgets::drawTextureButton("Albedo texture", material->albedoTexture) || modified;
		ImGui::SameLine();
		modified = Widgets::drawColorEditor("##ColorEditor Albedo texture", &material->albedoColor, &colorPickerModeAlbedo, &showColorPickerAlbedo) || modified;

		// --- Specular texture ---
		static ImGuiColorEditFlags colorPickerModeSpecular = ImGuiColorEditFlags_PickerHueBar;
		static bool showColorPickerSpecular = false;
		modified = Widgets::drawTextureButton("Specular texture", material->metallicMap) || modified;
		ImGui::SameLine();
		modified = Widgets::drawColorEditor("##ColorEditor Specular texture", &material->specularColor, &colorPickerModeSpecular, &showColorPickerSpecular) || modified;
		return modified;
	}
}
