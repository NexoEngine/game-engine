//// Components.cpp ///////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/02/2025
//  Description: Source file for the utilitary ImGui functions
//
///////////////////////////////////////////////////////////////////////////////

#include "Components.hpp"
#include "Elements.hpp"
#include "Guard.hpp"
#include "Utils.hpp"
#include "tinyfiledialogs.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <numbers>
#include <limits>

namespace ImNexo {

    bool ButtonWithIconAndText(
        const std::string &uniqueId,
        const std::string &icon,
        const std::string &label,
        const ImVec2 &itemSize
    )
    {
        IdGuard idGuard(uniqueId);
        std::string invisButtonLabel = "##" + uniqueId;

        if (ImGui::InvisibleButton(invisButtonLabel.c_str(), itemSize))
            return true;

        // Draw the background
        auto [p0, p1] = utils::getItemRect();
        ImGui::GetWindowDrawList()->AddRectFilled(
            p0, p1,
            ImGui::GetColorU32(ImGui::IsItemHovered() ? ImGuiCol_ButtonHovered : ImGuiCol_Button),
            ImGui::GetStyle().FrameRounding
        );

        // Draw the icon at 25% from top
        CenteredIcon(
            icon,
            p0, p1,
            ImGui::GetColorU32(ImGuiCol_Text),
            1.5f,
            0.25f,
            0.5f
        );

        // Draw the label with wrapping if needed
        WrappedCenteredText(
            label,
            p0, p1,
            ImGui::GetColorU32(ImGuiCol_Text),
            0.6f  // Position at 60% from top
        );

        // Use drawButtonBorder instead of direct drawing
        ButtonBorder(
            0,  // Use default color
            ImGui::GetColorU32(ImGuiCol_ButtonHovered),
            ImGui::GetColorU32(ImGuiCol_ButtonActive),
            ImGui::GetStyle().FrameRounding
        );

        return false;
    }

    void ColorButton(const std::string &label, const ImVec2 size, const ImVec4 color, bool *clicked, ImGuiColorEditFlags flags)
    {
        flags |= ImGuiColorEditFlags_NoTooltip;
        constexpr float borderThickness = 3.0f;
        const float defaultSize = ImGui::GetFrameHeight() + borderThickness;
        const auto calculatedSize = ImVec2(
            size.x == 0 ? defaultSize : size.x - borderThickness * 2,
            size.y == 0 ? defaultSize : size.y - borderThickness * 2
        );

        if (ImGui::ColorButton(label.c_str(), color, flags, calculatedSize) && clicked)
        {
            *clicked = !*clicked;
        }

        ButtonBorder(
            ImGui::GetColorU32(ImGuiCol_Button),
            ImGui::GetColorU32(ImGuiCol_ButtonHovered),
            ImGui::GetColorU32(ImGuiCol_ButtonActive),
            borderThickness
        );
    }


    bool TextureButton(const std::string &label, std::shared_ptr<nexo::renderer::NxTexture2D> &texture)
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
                std::shared_ptr<nexo::renderer::NxTexture2D> newTexture = nexo::renderer::NxTexture2D::create(path);
                if (newTexture)
                {
                    texture = newTexture;
                    textureModified = true;
                }
            }
        }
        ButtonBorder(IM_COL32(255,255,255,0), IM_COL32(255,255,255,255), IM_COL32(255,255,255,0), 0.0f, 0, 2.0f);
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::Text("%s", label.c_str());
		return textureModified;
	}

    bool IconGradientButton(
        const std::string& uniqueId,
        const std::string& icon,
        const ImVec2& size,
        const std::vector<GradientStop>& gradientStops,
        float gradientAngle,
        const ImU32 borderColor,
        const ImU32 borderColorHovered,
        const ImU32 borderColorActive,
        const ImU32 iconColor
    )
    {
        IdGuard idGuard(uniqueId);

        // Create invisible button for interaction
        bool clicked = ImGui::InvisibleButton(("##" + uniqueId).c_str(), size);

        // Get button rectangle coordinates
        auto [p_min, p_max] = utils::getItemRect();

        // Draw the gradient background
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        RectFilledLinearGradient(p_min, p_max, gradientAngle, gradientStops, drawList);

        // Draw the icon centered using our helper function
        CenteredIcon(icon, p_min, p_max, iconColor);

        // Use our drawButtonBorder helper instead of direct drawing
        ButtonBorder(
            borderColor,
            borderColorHovered,
            borderColorActive,
            3.0f, // rounding
            0,    // no flags
            1.5f  // thickness
        );

        return clicked;
    }

    bool RowEntityDropdown(
        const std::string &label,
        nexo::ecs::Entity &targetEntity,
        const std::vector<nexo::ecs::Entity>& entities,
        const std::function<std::string(nexo::ecs::Entity)>& getNameFunc
    )
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label.c_str());

        ImGui::TableNextColumn();
        IdGuard idGuard(label);

        bool changed = false;

        // Build entity-name mapping
        static std::vector<std::pair<nexo::ecs::Entity, std::string>> entityNamePairs;
        static nexo::ecs::Entity lastTargetEntity = 0;
        static std::vector<nexo::ecs::Entity> lastEntities;

        // Only rebuild the mapping if entities list changed or target entity changed
        bool needRebuild = lastTargetEntity != targetEntity || lastEntities.size() != entities.size();

        if (!needRebuild) {
            for (size_t i = 0; i < entities.size() && !needRebuild; i++) {
                needRebuild = lastEntities[i] != entities[i];
            }
        }

        if (needRebuild) {
            entityNamePairs.clear();
            entityNamePairs.reserve(entities.size());
            lastEntities = entities;
            lastTargetEntity = targetEntity;

            for (nexo::ecs::Entity entity : entities) {
                std::string name = getNameFunc(entity);
                entityNamePairs.emplace_back(entity, name);
            }
        }

        // Find current index
        int currentIndex = -1;
        for (size_t i = 0; i < entityNamePairs.size(); i++) {
            if (entityNamePairs[i].first == targetEntity) {
                currentIndex = static_cast<int>(i);
                break;
            }
        }

        // Add a "None" option if we want to allow null selection
        std::string currentItemName = currentIndex >= 0 ? entityNamePairs[currentIndex].second : "None";

        // Draw the combo box
        ImGui::SetNextItemWidth(-FLT_MIN); // Use all available width
        if (ImGui::BeginCombo("##entity_dropdown", currentItemName.c_str()))
        {
            // Optional: Add a "None" option for clearing the target
            if (ImGui::Selectable("None", targetEntity == nexo::ecs::MAX_ENTITIES)) {
                targetEntity = nexo::ecs::MAX_ENTITIES;
                changed = true;
            }

            for (size_t i = 0; i < entityNamePairs.size(); i++)
            {
                const bool isSelected = (currentIndex == static_cast<int>(i));
                if (ImGui::Selectable(entityNamePairs[i].second.c_str(), isSelected))
                {
                    targetEntity = entityNamePairs[i].first;
                    changed = true;
                }

                if (isSelected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndCombo();
        }

        return changed;
    }

    bool RowDragFloat(const Channels &channels)
    {
        bool modified = false;

        for (unsigned int i = 0; i < channels.count; ++i)
        {
            ImGui::TableNextColumn();

            // Draw the badge (if provided)
            if (!channels.badges[i].label.empty())
            {
                const auto &badge = channels.badges[i];
                Button(badge.label, badge.size, badge.bg, badge.bgHovered,
                    badge.bgActive, badge.txtColor);
                ImGui::SameLine(0, 2);
            }

            // Draw the drag float control
            const auto &slider = channels.sliders[i];
            bool changed = DragFloat(
                slider.label,
                slider.value,
                slider.speed,
                slider.min,
                slider.max,
                slider.format,
                slider.bg,
                slider.bgHovered,
                slider.bgActive,
                slider.textColor);

            modified |= changed;
        }

        return modified;
    }

    bool RowDragFloat1(
        const char *uniqueLabel,
        const std::string &badgeLabel,
        float *value,
        float minValue,
        float maxValue,
        float speed
    )
    {
        ImGui::TableNextRow();

        ChannelLabel chanLabel;
        chanLabel.label = uniqueLabel;
        RowLabel(chanLabel);

        // Create channels structure for a single value
        std::string labelId = "##X" + std::string(uniqueLabel);
        std::string badgeId = badgeLabel.empty() ? "" : badgeLabel + "##" + uniqueLabel;

        // Setup single badge and control
        Channels channels;
        channels.count = 1;
        channels.badges.push_back({
            badgeId,
            {0, 0},
            IM_COL32(80, 0, 0, 255),
            IM_COL32(80, 0, 0, 255),
            IM_COL32(80, 0, 0, 255),
            IM_COL32(255, 180, 180, 255)
        });

        channels.sliders.push_back({
            labelId,
            value,
            speed,
            minValue,
            maxValue,
            0, 0, 0, 0,
            "%.2f"
        });

        return RowDragFloat(channels);
    }

    bool RowDragFloat2(
        const char *uniqueLabel,
        const std::string &badLabelX,
        const std::string &badLabelY,
        float *values,
        float minValue,
        float maxValue,
        float speed,
        std::vector<ImU32> badgeColor,
        std::vector<ImU32> textBadgeColor,
        const bool disabled
    )
    {
        ImGui::TableNextRow();

        ChannelLabel chanLabel;
        chanLabel.label = uniqueLabel;
        RowLabel(chanLabel);

        // Setup badge colors with defaults if not provided
        if (badgeColor.size() < 2) {
            badgeColor = {IM_COL32(102, 28, 28, 255), IM_COL32(0, 80, 0, 255)};
        }

        if (textBadgeColor.size() < 2) {
            textBadgeColor = {IM_COL32(255, 180, 180, 255), IM_COL32(180, 255, 180, 255)};
        }

        // Base ID for controls
        std::string baseId = uniqueLabel;

        // Set up channels structure
        Channels channels;
        channels.count = 2;

        // Badge labels
        channels.badges = {
            {badLabelX + "##" + baseId, {0, 0}, badgeColor[0], badgeColor[0], badgeColor[0], textBadgeColor[0]},
            {badLabelY + "##" + baseId, {0, 0}, badgeColor[1], badgeColor[1], badgeColor[1], textBadgeColor[1]}
        };

        // Slider colors
        ImU32 textColor = disabled ? ImGui::GetColorU32(ImGuiCol_TextDisabled) : ImGui::GetColorU32(ImGuiCol_Text);
        ImU32 bgColor = ImGui::GetColorU32(ImGuiCol_FrameBg);
        ImU32 bgHoveredColor = ImGui::GetColorU32(ImGuiCol_FrameBgHovered);
        ImU32 bgActiveColor = ImGui::GetColorU32(ImGuiCol_FrameBgActive);

        // Slider controls
        channels.sliders = {
            {"##X" + baseId, &values[0], speed, minValue, maxValue, bgColor, bgHoveredColor, bgActiveColor, textColor, "%.2f"},
            {"##Y" + baseId, &values[1], speed, minValue, maxValue, bgColor, bgHoveredColor, bgActiveColor, textColor, "%.2f"}
        };

        return RowDragFloat(channels);
    }

    // Creates standard badge colors for X/Y/Z axes if not provided
    static void setupAxisBadgeColors(std::vector<ImU32>& badgeColors, std::vector<ImU32>& textBadgeColors)
    {
        if (badgeColors.empty()) {
            badgeColors = {
                IM_COL32(102, 28, 28, 255),   // X - Red
                IM_COL32(0, 80, 0, 255),      // Y - Green
                IM_COL32(38, 49, 121, 255)    // Z - Blue
            };
        }

        if (textBadgeColors.empty()) {
            textBadgeColors = {
                IM_COL32(255, 180, 180, 255), // X - Light Red
                IM_COL32(180, 255, 180, 255), // Y - Light Green
                IM_COL32(180, 180, 255, 255)  // Z - Light Blue
            };
        }
    }

    bool RowDragFloat3(
        const char *uniqueLabel,
        const std::string &badLabelX,
        const std::string &badLabelY,
        const std::string &badLabelZ,
        float *values,
        float minValue,
        float maxValue,
        float speed,
        std::vector<ImU32> badgeColors,
        std::vector<ImU32> textBadgeColors
    )
    {
        ImGui::TableNextRow();

        // Setup the label for the row
        ChannelLabel chanLabel;
        chanLabel.label = uniqueLabel;

        // Setup standard axis colors if not provided
        setupAxisBadgeColors(badgeColors, textBadgeColors);

        // Base ID for controls
        std::string baseId = uniqueLabel;
        float badgeSize = ImGui::GetFrameHeight();

        // Set up channels structure
        Channels channels;
        channels.count = 3;

        // Badge labels
        channels.badges = {
            {badLabelX + "##" + baseId, {badgeSize, badgeSize}, badgeColors[0], badgeColors[0], badgeColors[0], textBadgeColors[0]},
            {badLabelY + "##" + baseId, {badgeSize, badgeSize}, badgeColors[1], badgeColors[1], badgeColors[1], textBadgeColors[1]},
            {badLabelZ + "##" + baseId, {badgeSize, badgeSize}, badgeColors[2], badgeColors[2], badgeColors[2], textBadgeColors[2]}
        };

        ImU32 textColor = ImGui::GetColorU32(ImGuiCol_Text);

        // Slider controls
        channels.sliders = {
            {"##X" + baseId, &values[0], speed, minValue, maxValue, 0, 0, 0, textColor, "%.2f"},
            {"##Y" + baseId, &values[1], speed, minValue, maxValue, 0, 0, 0, textColor, "%.2f"},
            {"##Z" + baseId, &values[2], speed, minValue, maxValue, 0, 0, 0, textColor, "%.2f"}
        };

        if (!chanLabel.label.empty())
            RowLabel(chanLabel);

        return RowDragFloat(channels);
    }

    bool ToggleButtonWithSeparator(const std::string &label, bool* toggled)
    {
        IdGuard idGuard(label);
        bool clicked = false;

        // Create the toggle button
        constexpr ImVec2 buttonSize(24, 24);
        if (ImGui::InvisibleButton("##arrow", buttonSize))
        {
            clicked = true;
            *toggled = !(*toggled);
        }

        // Get button bounds and draw the arrow
        auto [p_min, p_max] = utils::getItemRect();
        ImVec2 center((p_min.x + p_max.x) * 0.5f, (p_min.y + p_max.y) * 0.5f);

        constexpr float arrowSize = 5.0f;
        const ImU32 arrowColor = ImGui::GetColorU32(ImGuiCol_TextTab);
        Arrow(center, *toggled, arrowColor, arrowSize);

        ImGui::SameLine();

        // Draw separator line
        const ImVec2 separatorPos = ImGui::GetCursorScreenPos();
        constexpr float separatorHeight = 24.0f; // match button height
        ImGui::GetWindowDrawList()->AddLine(
            separatorPos,
            ImVec2(separatorPos.x, separatorPos.y + separatorHeight),
            ImGui::GetColorU32(ImGuiCol_Separator),
            1.0f
        );

        ImGui::Dummy(ImVec2(4, buttonSize.y));
        ImGui::SameLine();

        // Use the existing custom separator text component
        CustomSeparatorText(
            label,
            10.0f,
            0.1f,
            0.5f,
            IM_COL32(255, 255, 255, 255),
            IM_COL32(255, 255, 255, 255)
        );

        return clicked;
    }
}
