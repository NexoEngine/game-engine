//// Elements.cpp /////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        18/04/2025
//  Description: Source file for the ui elements
//
///////////////////////////////////////////////////////////////////////////////

#include "Elements.hpp"
#include "Guard.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <numbers>

namespace ImNexo {

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
        const ImU32 color,
        const float scale,
        const float verticalPosition,
        const float horizontalPosition,
        ImFont* font
    ) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();

        // Use specified font or current font
        if (font) {
            ImGui::PushFont(font);
        }

        // Calculate icon size with scale
        ImGui::SetWindowFontScale(scale);
        const ImVec2 iconSize = ImGui::CalcTextSize(icon.c_str());
        ImGui::SetWindowFontScale(1.0f);

        // Calculate position
        const auto iconPos = ImVec2(
            p_min.x + (p_max.x - p_min.x - iconSize.x) * horizontalPosition,
            p_min.y + (p_max.y - p_min.y) * verticalPosition - iconSize.y * 0.5f
        );

        // Draw the icon
        drawList->AddText(
            font ? font : ImGui::GetFont(),
            ImGui::GetFontSize() * scale,
            iconPos, color,
            icon.c_str()
        );

        if (font) {
            ImGui::PopFont();
        }
    }

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
        const ImU32 color,
        const float verticalPosition
    ) {
        ImDrawList* drawList = ImGui::GetWindowDrawList();
        const float textHeight = ImGui::GetFontSize();
        const float wrapWidth = p_max.x - p_min.x - 10.0f; // 5px padding on each side
        const float textY = p_min.y + (p_max.y - p_min.y) * verticalPosition;

        // Calculate text size to determine if wrapping is needed
        const ImVec2 textSize = ImGui::CalcTextSize(text.c_str());

        if (textSize.x > wrapWidth) {
            // Try to find a space to split the text
            size_t splitPos = text.find(' ');

            if (splitPos != std::string::npos) {
                // Split text into two lines
                const std::string line1 = text.substr(0, splitPos);
                const std::string line2 = text.substr(splitPos + 1);

                // Calculate positions for both lines
                const auto line1Pos = ImVec2(
                    p_min.x + (p_max.x - p_min.x - ImGui::CalcTextSize(line1.c_str()).x) * 0.5f,
                    textY - textHeight * 0.5f
                );

                const auto line2Pos = ImVec2(
                    p_min.x + (p_max.x - p_min.x - ImGui::CalcTextSize(line2.c_str()).x) * 0.5f,
                    textY + textHeight * 0.5f
                );

                // Draw both lines
                drawList->AddText(line1Pos, color, line1.c_str());
                drawList->AddText(line2Pos, color, line2.c_str());
            } else {
                // No space to split, draw single line (might be clipped)
                const auto textPos = ImVec2(
                    p_min.x + (p_max.x - p_min.x - textSize.x) * 0.5f,
                    textY - textHeight * 0.5f
                );
                drawList->AddText(textPos, color, text.c_str());
            }
        } else {
            // No wrapping needed, draw centered
            const auto textPos = ImVec2(
                p_min.x + (p_max.x - p_min.x - textSize.x) * 0.5f,
                textY - textHeight * 0.5f
            );
            drawList->AddText(textPos, color, text.c_str());
        }
    }

    bool Button(
        const std::string &label,
        const ImVec2 &size,
        const ImU32 bg,
        const ImU32 bgHovered,
        const ImU32 bgActive,
        const ImU32 txtColor
    )
    {
        StyleGuard colorGuard(ImGuiCol_Button, bg);
        colorGuard
            .push(ImGuiCol_ButtonHovered, bgHovered)
            .push(ImGuiCol_ButtonActive, bgActive)
            .push(ImGuiCol_Text, txtColor);

        return ImGui::Button(label.c_str(), size);
    }

    void ButtonBorder(
        const ImU32 borderColor,
        const ImU32 borderColorHovered,
        const ImU32 borderColorActive,
        const float rounding,
        const ImDrawFlags flags,
        const float thickness
    )
    {
        auto [p_min, p_max] = utils::getItemRect();
        ImU32 color = borderColor ? borderColor : ImGui::GetColorU32(ImGuiCol_Button);

        if (ImGui::IsItemHovered())
            color = borderColorHovered ? borderColorHovered : ImGui::GetColorU32(ImGuiCol_ButtonHovered);
        if (ImGui::IsItemActive())
            color = borderColorActive ? borderColorActive : ImGui::GetColorU32(ImGuiCol_ButtonActive);

        ImGui::GetWindowDrawList()->AddRect(p_min, p_max, color, rounding, flags, thickness);
    }

    bool DragFloat(
        const std::string &label,
        float *values, const float speed,
        const float min, const float max,
        const std::string &format,
        const ImU32 bg, const ImU32 bgHovered, const ImU32 bgActive, const ImU32 textColor
    )
    {
        StyleGuard colorGuard(ImGuiCol_FrameBg, bg);
        colorGuard
            .push(ImGuiCol_FrameBgHovered, bgHovered)
            .push(ImGuiCol_FrameBgActive, bgActive)
            .push(ImGuiCol_Text, textColor);

        return ImGui::DragFloat(label.c_str(), values, speed, min, max, format.c_str());
    }

    /**
    * @brief Sanitizes gradient stops to ensure proper ordering and range
    */
    static void sanitizeGradientStops(std::vector<GradientStop>& stops)
    {
        if (stops.size() < 2)
            return;

        // Sort stops by position if needed
        std::ranges::sort(stops,
                          [](const GradientStop& a, const GradientStop& b) {
                              return a.pos < b.pos;
                          });

        // Clamp positions to valid range
        float stop_max = 0.0f;
        for (auto& [pos, color] : stops) {
            // Clamp stop position to [0.0f, 1.0f]
            pos = std::clamp(pos, 0.0f, 1.0f);

            // Ensure stops are monotonically increasing
            pos = std::max(pos, stop_max);
            stop_max = pos;
        }

        // if first stop does not start at 0.0f, we need to add a stop at 0.0f
        if (stops[0].pos > 0.0f) {
            stops.insert(stops.begin(), { 0.0f, stops[0].color });
        }
        // if last stop does not end at 1.0f, we need to add a stop at 1.0f
        if (stops.back().pos < 1.0f) {
            stops.push_back({ 1.0f, stops.back().color });
        }
    }

    void RectFilledLinearGradient(
        const ImVec2& pMin,
        const ImVec2& pMax,
        float angle,
        std::vector<GradientStop> stops,
        ImDrawList* drawList
    )
    {
        if (!drawList)
            drawList = ImGui::GetWindowDrawList();

        // Check if we have at least two stops.
        if (stops.size() < 2)
            return;

        angle -= 90.0f; // rotate 90 degrees to match the CSS gradients rotations

        // Convert angle from degrees to radians and normalize
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
            d_min = std::min(d_min, d);
            d_max = std::max(d_max, d);
        }

        // Sanitize gradient stops
        sanitizeGradientStops(stops);

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
            utils::clipPolygonWithLine(segPoly, gradDir, segStart, tempPoly);
            segPoly = tempPoly; // copy result
            // Clip against upper boundary: d <= seg_end
            // To clip with an upper-bound, invert the normal.
            utils::clipPolygonWithLine(segPoly, ImVec2(-gradDir.x, -gradDir.y), -segEnd, tempPoly);
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
                polyColors.push_back(utils::imLerpColor(stops[posStart].color, stops[posEnd].color, t));
            }

            // Draw the filled and colored polygon.
            utils::fillConvexPolygon(drawList, segPoly, polyColors);
        }
    }

    bool Header(const std::string &label, const std::string_view headerText)
    {
        StyleVarGuard styleGuard(ImGuiStyleVar_FramePadding,
            ImVec2(ImGui::GetStyle().FramePadding.x, 3.0f));

        bool open = ImGui::TreeNodeEx(
            label.c_str(),
            ImGuiTreeNodeFlags_DefaultOpen |
            ImGuiTreeNodeFlags_Framed |
            ImGuiTreeNodeFlags_AllowItemOverlap |
            ImGuiTreeNodeFlags_SpanAvailWidth
        );

        // Get the bounding box and draw centered text
        auto [p_min, p_max] = utils::getItemRect();
        const ImVec2 textPos = utils::calculateCenteredTextPosition(std::string(headerText), p_min, p_max);

        ImGui::GetWindowDrawList()->AddText(
            ImGui::GetFont(),
            ImGui::GetFontSize(),
            textPos,
            ImGui::GetColorU32(ImGuiCol_Text),
            headerText.data()
        );

        return open;
    }

    void RowLabel(const ChannelLabel &rowLabel)
    {
        ImGui::TableNextColumn();
        ImGui::AlignTextToFramePadding();
        ImGui::TextUnformatted(rowLabel.label.c_str());
    }

    // Helper method to draw arrow indicators
    void Arrow(const ImVec2& center, const bool isExpanded, const ImU32 color, const float size)
    {
        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        if (isExpanded) {
            // Downward arrow (expanded)
            draw_list->AddTriangleFilled(
                ImVec2(center.x - size, center.y - size),
                ImVec2(center.x + size, center.y - size),
                ImVec2(center.x, center.y + size),
                color);
        } else {
            // Rightward arrow (collapsed)
            draw_list->AddTriangleFilled(
                ImVec2(center.x - size, center.y - size),
                ImVec2(center.x - size, center.y + size),
                ImVec2(center.x + size, center.y),
                color);
        }
    }

    void CustomSeparatorText(
        const std::string &text,
        const float textPadding,
        const float leftSpacing,
        const float thickness,
        const ImU32 lineColor,
        const ImU32 textColor
    )
    {
        const ImVec2 pos = ImGui::GetCursorScreenPos();
        const float availWidth = ImGui::GetContentRegionAvail().x;
        const float textWidth = ImGui::CalcTextSize(text.c_str()).x;

        // Compute the length of each line. Clamp to zero if the region is too small.
        float lineWidth = (availWidth - textWidth - 2 * textPadding) * leftSpacing;
        lineWidth = std::max(lineWidth, 0.0f);

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

    void Image(const ImTextureID user_texture_id, const ImVec2& image_size, const ImVec2& uv0, const ImVec2& uv1,
        const ImVec4& tint_col, const ImVec4& border_col)
    {
        ImGui::Image(user_texture_id, image_size, uv0, uv1, tint_col, border_col);
    }

    bool ImageButton(const char *str_id, ImTextureID user_texture_id, const ImVec2& image_size, const ImVec2& uv0,
        const ImVec2& uv1, const ImVec4& bg_col, const ImVec4& tint_col)
    {
        return ImGui::ImageButton(str_id, user_texture_id, image_size, uv0, uv1, bg_col, tint_col);
    }
}
