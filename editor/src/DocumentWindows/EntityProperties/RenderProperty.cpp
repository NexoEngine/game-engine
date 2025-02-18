/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** RenderProperty.cpp
*/

#include <imgui.h>

#include "RenderProperty.hpp"
#include "IconsFontAwesome.h"
#include "AEntityProperty.hpp"
#include "Components/Components.hpp"
#include "SceneManagerBridge.hpp"

#include <loguru/loguru.hpp>

namespace nexo::editor {
    RenderProperty::RenderProperty(const std::string& name)
        : AEntityProperty(name)
    {
    }

    RenderProperty::~RenderProperty()
    {
    }

    void RenderProperty::update()
    {
    }

    int RenderProperty::show(ecs::Entity entity)
    {
    	auto const& App = getApp();
        auto& renderComponent = App.getEntityComponent<components::RenderComponent>(entity);

        glm::vec4 *selectedEntityColor = nullptr;
        if (renderComponent.type == components::RenderType::RENDER_3D)
        {
            auto renderable3D = std::dynamic_pointer_cast<components::Renderable3D>(renderComponent.renderable);
            if (renderable3D)
            {
            	selectedEntityColor = &renderable3D->material.albedoColor;
            }
        }
        else if (renderComponent.type == components::RenderType::RENDER_2D)
        {
            auto renderable2D = std::dynamic_pointer_cast<components::Renderable2D>(renderComponent.renderable);
            if (renderable2D)
            {
                auto& [color, texture, sprite] = renderable2D->sprite;
                selectedEntityColor = &color;
            }
        }

        bool open = EntityPropertiesComponents::drawHeader("##RenderNode", "Render Component");
        static bool showColorPicker = false;
        static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;

        if (open)
        {
            ImGui::SetWindowFontScale(1.2f);
        	ImGui::Text("Hide");
         	ImGui::SameLine(0, 12);
         	bool hidden = !renderComponent.isRendered;
            ImGui::Checkbox("##HideCheckBox", &hidden);
            renderComponent.isRendered = !hidden;

            ImGui::Text("Color");
            ImGui::SetWindowFontScale(1.0f);
            ImGui::SameLine(0, 10);

            float availableWidth = ImGui::GetContentRegionAvail().x;

            ImGui::PushStyleColor(ImGuiCol_Button,        IM_COL32(60, 60, 60, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(80, 80, 80, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,  IM_COL32(100, 100, 100, 255));
            ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 3.0f);
            ImGui::PushStyleColor(ImGuiCol_Border, IM_COL32(100, 100, 100, 255));

            if (ImGui::ColorButton("##ColorButton",
                                   ImVec4(selectedEntityColor->x,
                                          selectedEntityColor->y,
                                          selectedEntityColor->z,
                                          selectedEntityColor->w),
                                   ImGuiColorEditFlags_NoTooltip,
                                   ImVec2(availableWidth - 35, 25))) // Make room for the cog button
            {
                showColorPicker = !showColorPicker;
            }

            ImVec2 p_min = ImGui::GetItemRectMin();
            ImVec2 p_max = ImGui::GetItemRectMax();
            ImU32 borderColor = IM_COL32(150, 150, 150, 255);
            if (ImGui::IsItemHovered())
                borderColor = IM_COL32(200, 200, 200, 255);
            if (ImGui::IsItemActive())
                borderColor = IM_COL32(250, 250, 250, 255);

            // Draw the border manually using the window's draw list
            ImGui::GetWindowDrawList()->AddRect(p_min, p_max, borderColor, 2.0f, 0, 3.0f);
            ImGui::PopStyleVar();
            ImGui::PopStyleColor(4);

            ImGui::SameLine();

            float cogButtonWidth = 25;
            float cogButtonHeight = 25;

            ImGui::PushStyleColor(ImGuiCol_Button,        IM_COL32(60, 60, 60, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, IM_COL32(80, 80, 80, 255));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive,  IM_COL32(100, 100, 100, 255));

            float fontSize = ImGui::GetFontSize();
            float verticalPadding = (cogButtonHeight - fontSize) * 0.5f;
            ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, verticalPadding + 2)); // Slightly add more padding to center the cog icon

            if (ImGui::Button(ICON_FA_COG, ImVec2(cogButtonWidth, cogButtonHeight)))
            {
                ImGui::OpenPopup("ColorPickerSettings");
            }

            ImGui::PopStyleVar();
            ImGui::PopStyleColor(3);
            if (ImGui::BeginPopup("ColorPickerSettings"))
            {
                ImGui::Text("Picker Mode:");
                if (ImGui::RadioButton("Hue Wheel", colorPickerMode == ImGuiColorEditFlags_PickerHueWheel))
                    colorPickerMode = ImGuiColorEditFlags_PickerHueWheel;
                if (ImGui::RadioButton("Hue bar", colorPickerMode == ImGuiColorEditFlags_PickerHueBar))
                    colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
                ImGui::EndPopup();
            }

            if (showColorPicker)
            {
                ImGui::Spacing();
                ImGui::ColorPicker4("##ColorPickerInline",
                                    reinterpret_cast<float*>(selectedEntityColor), colorPickerMode);
            }

            ImGui::TreePop();
        }

        return true;
    }

    void RenderProperty::showEnd()
    {
        AEntityProperty::showEnd();
    }
}
