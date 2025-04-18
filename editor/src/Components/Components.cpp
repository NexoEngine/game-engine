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

#include <imgui.h>
#include <imgui_internal.h>
#include <numbers>
#include <limits>

namespace nexo::editor {

    bool Components::drawButton(
            const std::string &label,
            const ImVec2 &size,
            const ImU32 bg,
            const ImU32 bgHovered,
            const ImU32 bgActive, const ImU32 txtColor
    ) {
        if (bg != 0) ImGui::PushStyleColor(ImGuiCol_Button, bg);
        if (bgHovered != 0) ImGui::PushStyleColor(ImGuiCol_ButtonHovered, bgHovered);
        if (bgActive != 0) ImGui::PushStyleColor(ImGuiCol_ButtonActive, bgActive);
        if (txtColor != 0) ImGui::PushStyleColor(ImGuiCol_Text, txtColor);

        const bool clicked = ImGui::Button(label.c_str(), size);

        const int popCount = (bg != 0) + (bgHovered != 0) + (bgActive != 0) + (txtColor != 0);
        ImGui::PopStyleColor(popCount);
        return clicked;
    }

    bool Components::drawComponentButton(
            const std::string &uniqueId,
            const std::string &icon,
            const std::string &label,
            const ImVec2 &itemSize
    ) {
        ImGui::PushID(uniqueId.c_str());
        std::string invisButtonLabel = "##" + uniqueId;
        // Create invisible button
        if (ImGui::InvisibleButton(invisButtonLabel.c_str(), itemSize))
        {
            ImGui::PopID();
            return true;
            // app.m_coordinator->addComponent(camera, components::PerspectiveCameraTarget{});
            //showComponentSelector = false;
        }

        // Draw the background (like a button would have)
        ImVec2 p0 = ImGui::GetItemRectMin();
        ImVec2 p1 = ImGui::GetItemRectMax();
        ImGui::GetWindowDrawList()->AddRectFilled(
            p0, p1,
            ImGui::GetColorU32(ImGui::IsItemHovered() ? ImGuiCol_ButtonHovered : ImGuiCol_Button),
            ImGui::GetStyle().FrameRounding
        );

        // Increase icon size even further (2x larger)
        float iconScale = 1.5f;
        ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImGui::SetWindowFontScale(iconScale);
        ImVec2 iconSize = ImGui::CalcTextSize(icon.c_str());
        ImGui::SetWindowFontScale(1.0f);
        ImGui::PopFont();

        // Draw the icon centered
        ImVec2 iconPos = ImVec2(
            p0.x + (p1.x - p0.x - iconSize.x) * 0.45f,
            p0.y + (p1.y - p0.y) * 0.25f - iconSize.y * 0.5f  // Position at 35% from top
        );
        ImGui::GetWindowDrawList()->AddText(
            ImGui::GetFont(), ImGui::GetFontSize() * iconScale,
            iconPos, ImGui::GetColorU32(ImGuiCol_Text),
            icon.c_str()
        );

        // Implement text wrapping for the label
        float wrapWidth = p1.x - p0.x - 10.0f; // Available width with padding
        float textHeight = ImGui::GetFontSize();
        float textY = p0.y + (p1.y - p0.y) * 0.60f; // Position at 75% from top

        // Calculate if we need to wrap (if text is wider than button)
        ImVec2 textSize = ImGui::CalcTextSize(label.c_str());
        if (textSize.x > wrapWidth) {
            // Find where to split the text
            size_t splitPos = label.find(" ");

            if (splitPos != std::string::npos) {
                // Split at space
                std::string line1 = label.substr(0, splitPos);
                std::string line2 = label.substr(splitPos + 1);

                // Draw first line
                ImVec2 line1Pos = ImVec2(
                    p0.x + (p1.x - p0.x - ImGui::CalcTextSize(line1.c_str()).x) * 0.5f,
                    textY - textHeight * 0.5f
                );
                ImGui::GetWindowDrawList()->AddText(line1Pos, ImGui::GetColorU32(ImGuiCol_Text), line1.c_str());

                // Draw second line
                ImVec2 line2Pos = ImVec2(
                    p0.x + (p1.x - p0.x - ImGui::CalcTextSize(line2.c_str()).x) * 0.5f,
                    textY + textHeight * 0.5f
                );
                ImGui::GetWindowDrawList()->AddText(line2Pos, ImGui::GetColorU32(ImGuiCol_Text), line2.c_str());
            } else {
                // No space to split, just draw the text with possible cutoff
                ImVec2 textPos = ImVec2(
                    p0.x + (p1.x - p0.x - textSize.x) * 0.5f,
                    textY
                );
                ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), label.c_str());
            }
        } else {
            // No wrapping needed, just center the text
            ImVec2 textPos = ImVec2(
                p0.x + (p1.x - p0.x - textSize.x) * 0.5f,
                textY
            );
            ImGui::GetWindowDrawList()->AddText(textPos, ImGui::GetColorU32(ImGuiCol_Text), label.c_str());
        }

        // Add border when hovered or active
        if (ImGui::IsItemHovered() || ImGui::IsItemActive())
            ImGui::GetWindowDrawList()->AddRect(p0, p1, ImGui::GetColorU32(ImGuiCol_ButtonActive), ImGui::GetStyle().FrameRounding);

        ImGui::PopID();
        return false;
    }

    void Components::drawButtonBorder(
        const ImU32 borderColor,
        const ImU32 borderColorHovered,
        const ImU32 borderColorActive,
        const float rounding,
        const ImDrawFlags flags,
        const float thickness
    ) {
        const ImVec2 p_min = ImGui::GetItemRectMin();
        const ImVec2 p_max = ImGui::GetItemRectMax();
        ImU32 color = borderColor ? borderColor : ImGui::GetColorU32(ImGuiCol_Button);
        if (ImGui::IsItemHovered())
            color = borderColorHovered ? borderColorHovered : ImGui::GetColorU32(ImGuiCol_ButtonHovered);
        if (ImGui::IsItemActive())
            color = borderColorActive ? borderColorActive : ImGui::GetColorU32(ImGuiCol_ButtonActive);

        ImGui::GetWindowDrawList()->AddRect(p_min, p_max, color, rounding, flags, thickness);
    }

    void Components::drawButtonInnerBorder(
        const ImU32 borderColor,
        const ImU32 borderColorHovered,
        const ImU32 borderColorActive,
        const float rounding,
        const ImDrawFlags flags,
        const float thickness
    ) {
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();
        ImU32 color = borderColor ? borderColor : ImGui::GetColorU32(ImGuiCol_Button);
        if (ImGui::IsItemHovered())
            color = borderColorHovered ? borderColorHovered : ImGui::GetColorU32(ImGuiCol_ButtonHovered);
        if (ImGui::IsItemActive())
            color = borderColorActive ? borderColorActive : ImGui::GetColorU32(ImGuiCol_ButtonActive);

        ImGui::GetWindowDrawList()->AddRect(
            ImVec2(p_min.x + thickness, p_min.y + thickness),
            ImVec2(p_max.x - thickness, p_max.y - thickness),
            color, rounding, flags, thickness);
    }

    bool Components::drawDragFloat(
            const std::string &label,
            float *values, const float speed,
            const float min, const float max,
            const std::string &format,
            const ImU32 bg, const ImU32 bgHovered, const ImU32 bgActive, const ImU32 textColor
    ) {
        if (bg) ImGui::PushStyleColor(ImGuiCol_FrameBg, bg);
        if (bgHovered) ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, bgHovered);
        if (bgActive) ImGui::PushStyleColor(ImGuiCol_FrameBgActive, bgActive);
        if (textColor) ImGui::PushStyleColor(ImGuiCol_Text, textColor);
        const bool clicked = ImGui::DragFloat(label.c_str(), values, speed, min, max, format.c_str());

        const int popCount = (bg != 0) + (bgHovered != 0) + (bgActive != 0) + (textColor != 0);
        ImGui::PopStyleColor(popCount);
        return clicked;
    }

    void Components::drawColorButton(const std::string &label, const ImVec2 size, const ImVec4 color, bool *clicked, ImGuiColorEditFlags flags)
    {
        flags |= ImGuiColorEditFlags_NoTooltip;
        constexpr float borderThickness = 3.0f;
        const float defaultSize = ImGui::GetFrameHeight() + borderThickness;
        const auto calculatedSize = ImVec2(size.x == 0 ? defaultSize : size.x - borderThickness * 2, size.y == 0 ? defaultSize : size.y - borderThickness * 2);
        if (ImGui::ColorButton(label.c_str(),
                               color,
                               flags,
                               calculatedSize) && clicked)
        {
            *clicked = !*clicked;
        }
        Components::drawButtonBorder(ImGui::GetColorU32(ImGuiCol_Button), ImGui::GetColorU32(ImGuiCol_ButtonHovered), ImGui::GetColorU32(ImGuiCol_ButtonActive), borderThickness);
    }

    void Components::drawCustomSeparatorText(const std::string &text, const float textPadding, const float leftSpacing, const float thickness, ImU32 lineColor, ImU32 textColor)
    {
        const ImVec2 pos = ImGui::GetCursorScreenPos();
        const float availWidth = ImGui::GetContentRegionAvail().x;
        const float textWidth  = ImGui::CalcTextSize(text.c_str()).x;

        // Compute the length of each line. Clamp to zero if the region is too small.
        float lineWidth = (availWidth - textWidth - 2 * textPadding) * leftSpacing;
        if (lineWidth < 0.0f)
            lineWidth = 0.0f;

        // Compute Y coordinate to draw lines so they align with the text center.
        const float lineY = pos.y + ImGui::GetTextLineHeight() * 0.5f;

        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        const ImVec2 lineStart(pos.x, lineY);
        const ImVec2 lineEnd(pos.x + lineWidth, lineY);
        draw_list->AddLine(lineStart, lineEnd, lineColor, thickness);

        const ImVec2 textPos(pos.x + lineWidth + textPadding, pos.y);
        draw_list->AddText(textPos, textColor, text.c_str());

        const ImVec2 rightLineStart(pos.x + lineWidth + textPadding + textWidth + textPadding, lineY);
        const ImVec2 rightLineEnd(pos.x + availWidth, lineY);
        draw_list->AddLine(rightLineStart, rightLineEnd, lineColor, thickness);

        ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
    }

    /**
     * @brief Linearly interpolates between two colors (ImU32, ImGui 32-bits ARGB format).
     * @param[in] colA The first color (ARGB format).
     * @param[in] colB The second color (ARGB format).
     * @param[in] t The interpolation factor (0.0 to 1.0).
     * @return The interpolated color (ARGB format).
     */
    static ImU32 imLerpColor(const ImU32 colA, const ImU32 colB, const float t)
    {
        const unsigned char a0 = (colA >> 24) & 0xFF, r0 = (colA >> 16) & 0xFF, g0 = (colA >> 8) & 0xFF, b0 = colA & 0xFF;
        const unsigned char a1 = (colB >> 24) & 0xFF, r1 = (colB >> 16) & 0xFF, g1 = (colB >> 8) & 0xFF, b1 = colB & 0xFF;
        const auto a = static_cast<unsigned char>(static_cast<float>(a0) + t * static_cast<float>(a1 - a0));
        const auto r = static_cast<unsigned char>(static_cast<float>(r0) + t * static_cast<float>(r1 - r0));
        const auto g = static_cast<unsigned char>(static_cast<float>(g0) + t * static_cast<float>(g1 - g0));
        const auto b = static_cast<unsigned char>(static_cast<float>(b0) + t * static_cast<float>(b1 - b0));
        return ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
    }

    /**
     * @brief Clip a convex polygon against a half-plane defined by: (dot(normal, v) >= offset)
     *
     * This function uses the Sutherland-Hodgman algorithm to clip a polygon against a line defined by a normal vector and an offset.
     * @param[in] poly Vector of vertices representing the polygon to be clipped.
     * @param[in] normal The normal vector of the line used for clipping.
     * @param[in] offset The offset from the origin of the line.
     * @param[out] outPoly Output vector to store the clipped polygon vertices.
     */
    static void clipPolygonWithLine(const std::vector<ImVec2>& poly, const ImVec2& normal, float offset, std::vector<ImVec2>& outPoly)
    {
        outPoly.clear();
        const auto count = poly.size();
        outPoly.reserve(count * 2); // Preallocate space for the output polygon (prepare worst case)
        for (size_t i = 0; i < count; i++) {
            const ImVec2& a = poly[i];
            const ImVec2& b = poly[(i + 1) % count];
            const float da = ImDot(a, normal) - offset;
            const float db = ImDot(b, normal) - offset;
            if (da >= 0)
                outPoly.push_back(a);
            // if the edge spans the boundary, compute intersection
            if ((da >= 0 && db < 0) || (da < 0 && db >= 0)) {
                const float t = da / (da - db);
                ImVec2 inter;
                inter.x = a.x + t * (b.x - a.x);
                inter.y = a.y + t * (b.y - a.y);
                outPoly.push_back(inter);
            }
        }
    }

    /**
     * @brief Fill a convex polygon with triangles using a triangle fan.
     * @param[in] drawList The ImDrawList to which the triangles will be added.
     * @param[in] poly Vector of vertices representing the polygon to be filled.
     * @param[in] polyColors Vector of colors for each vertex in the polygon.
     */
    static void fillConvexPolygon(ImDrawList* drawList, const std::vector<ImVec2>& poly, const std::vector<ImU32>& polyColors)
    {
        if (poly.size() < 3)
            return;
        const auto count = static_cast<int>(poly.size());
        drawList->PrimReserve((count - 2) * 3, count);
        // Use the first vertex as pivot.
        for (int i = 1; i < count - 1; i++) {
            const auto currentIdx = drawList->_VtxCurrentIdx;
            drawList->PrimWriteIdx(static_cast<ImDrawIdx>(currentIdx));
            drawList->PrimWriteIdx(static_cast<ImDrawIdx>(currentIdx + i));
            drawList->PrimWriteIdx(static_cast<ImDrawIdx>(currentIdx + i + 1));
        }
        // Write vertices with their computed colors.
        for (int i = 0; i < count; i++) {
            // For a vertex, we determine its position t between the segment boundaries later.
            // Here we assume the provided poly_colors already correspond vertex-by-vertex.
            drawList->PrimWriteVtx(poly[i], drawList->_Data->TexUvWhitePixel, polyColors[i]);
        }
    }


    void Components::drawRectFilledLinearGradient(const ImVec2& pMin, const ImVec2& pMax, float angle,
        std::vector<GradientStop> stops, ImDrawList* drawList)
    {
        if (!drawList)
            drawList = ImGui::GetWindowDrawList();

        // Check if we have at least two stops.
        // If not, we can't create a gradient.
        if (stops.size() < 2)
            return;

        angle -= 90.0f; // rotate 90 degrees to match the CSS gradients rotations

        // Convert angle from degrees to radians. Also keep it in range of radians
        // [0, 2*PI) for consistency.
        angle = fmodf(angle, 360.0f);
        if (angle < 0.0f)
            angle += 360.0f;
        angle = angle * std::numbers::pi_v<float> / 180.0f;

        const auto gradDir = ImVec2(cosf(angle), sinf(angle));

        // Define rectangle polygon (clockwise order).
        const std::vector<ImVec2> rectPoly = { pMin, ImVec2(pMax.x, pMin.y), pMax, ImVec2(pMin.x, pMax.y) };

        // Compute projection range (d_min, d_max) for the rectangle.
        float d_min = std::numeric_limits<float>::max();
        float d_max = -std::numeric_limits<float>::max();
        for (auto const& v : rectPoly) {
            const float d = ImDot(v, gradDir);
            if (d < d_min) d_min = d;
            if (d > d_max) d_max = d;
        }

        // sanitize stops
        float stop_max = 0.0f;
        for (auto& [pos, color] : stops) {
            (void)color; // ignore color for now
            // Clamp stop position to [0.0f, 1.0f]
            if (pos < 0.0f) pos = 0.0f;
            if (pos > 1.0f) pos = 1.0f;

            // Clamp stop position to [stop_max, 1.0f]
            if (pos < stop_max) {
                pos = stop_max;
            } else {
                stop_max = pos;
            }
        }

        // if first stop does not start at 0.0f, we need to add a stop at 0.0f
        if (stops[0].pos > 0.0f) {
            stops.insert(stops.begin(), { 0.0f, stops[0].color });
        }
        // if last stop does not end at 1.0f, we need to add a stop at 1.0f
        if (stops[stops.size() - 1].pos < 1.0f) {
            stops.push_back({ 1.0f, stops[stops.size() - 1].color });
        }

        // For each segment defined by consecutive stops:
        for (long i = static_cast<long>(stops.size()) - 1; i > 0; i--) {
            const long posStart = i - 1;
            const long posEnd = i;
            // Compute threshold projections for the current segment.
            const float segStart = d_min + stops[posStart].pos * (d_max - d_min);
            const float segEnd   = d_min + stops[posEnd].pos * (d_max - d_min);

            // Start with the whole rectangle.
            std::vector<ImVec2> segPoly = rectPoly;
            std::vector<ImVec2> tempPoly;
            // Clip against lower boundary: d >= seg_start
            clipPolygonWithLine(segPoly, gradDir, segStart, tempPoly);
            segPoly = tempPoly; // copy result
            // Clip against upper boundary: d <= seg_end
            // To clip with an upper-bound, invert the normal.
            clipPolygonWithLine(segPoly, ImVec2(-gradDir.x, -gradDir.y), -segEnd, tempPoly);
            segPoly = tempPoly;

            if (segPoly.empty())
                continue;

            // Now, compute per-vertex colors for the segment polygon.
            std::vector<ImU32> polyColors;
            polyColors.reserve(segPoly.size());
            for (const ImVec2& v : segPoly) {
                // Compute projection for the vertex.
                const float d = ImDot(v, gradDir);
                // Map projection to [0,1] relative to current segment boundaries.
                const float t = (d - segStart) / (segEnd - segStart);
                // Interpolate the color between the two stops.
                polyColors.push_back(imLerpColor(stops[posStart].color, stops[posEnd].color, t));
            }

            // Draw the filled and colored polygon.
            fillConvexPolygon(drawList, segPoly, polyColors);
        }
    }


    bool Components::drawToolbarButton(const std::string& uniqueId, const std::string& icon,
                                    const ImVec2& size,
                                    const std::vector<GradientStop>& gradientStops,
                                    float gradientAngle,
                                    const ImU32 borderColor,
                                    const ImU32 borderColorHovered,
                                    const ImU32 borderColorActive,
                                    const ImU32 iconColor)
	{
    	ImGui::PushID(uniqueId.c_str());

        // Create invisible button for interaction
        bool clicked = ImGui::InvisibleButton(("##" + uniqueId).c_str(), size);

        // Get button rectangle coordinates
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();

        // Draw the gradient background
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        drawRectFilledLinearGradient(p_min, p_max, gradientAngle, gradientStops, drawList);

        // Draw the icon
        //ImGui::PushFont(ImGui::GetIO().Fonts->Fonts[0]);
        ImVec2 iconSize = ImGui::CalcTextSize(icon.c_str());
        ImVec2 iconPos = ImVec2(
            p_min.x + (p_max.x - p_min.x - iconSize.x) * 0.5f,
            p_min.y + (p_max.y - p_min.y - iconSize.y) * 0.5f
        );

        // Draw the icon with specified color
        drawList->AddText(iconPos, iconColor, icon.c_str());
        //ImGui::PopFont();

        // Draw border with highlighting
        ImU32 currentBorderColor = borderColor;
        if (ImGui::IsItemHovered())
            currentBorderColor = borderColorHovered;
        if (ImGui::IsItemActive())
            currentBorderColor = borderColorActive;

        const float borderThickness = 1.5f;
        drawList->AddRect(p_min, p_max, currentBorderColor, 3.0f, 0, borderThickness);

        ImGui::PopID();
        return clicked;
	}

    bool Components::drawRowEntityDropdown(const std::string &label, ecs::Entity &targetEntity,
                                        const std::vector<ecs::Entity>& entities,
                                        const std::function<std::string(ecs::Entity)>& getNameFunc)
    {
        ImGui::TableNextRow();
        ImGui::TableNextColumn();
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(label.c_str());

        ImGui::TableNextColumn();

        // Create a unique ID for this widget
        ImGui::PushID(label.c_str());

        //ImGui::SetNextItemWidth(-FLT_MIN); // Use all available width

        bool changed = false;

        // Build entity-name mapping
        static std::vector<std::pair<ecs::Entity, std::string>> entityNamePairs;
        static ecs::Entity lastTargetEntity = 0;
        static std::vector<ecs::Entity> lastEntities;

        // Only rebuild the mapping if entities list changed or target entity changed
        bool needRebuild = lastTargetEntity != targetEntity ||
                          lastEntities.size() != entities.size();

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

            for (ecs::Entity entity : entities) {
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
        if (ImGui::BeginCombo("##entity_dropdown", currentItemName.c_str()))
        {
            // Optional: Add a "None" option for clearing the target
            if (ImGui::Selectable("None", targetEntity == ecs::MAX_ENTITIES)) {
                targetEntity = ecs::MAX_ENTITIES;
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

        ImGui::PopID();
        return changed;
    }
}
