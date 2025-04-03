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
        if (textColor) ImGui::PushStyleColor(ImGuiCol_Text, 		   textColor);
        if (bg) ImGui::PushStyleColor(ImGuiCol_FrameBg,        bg);
        if (bgHovered) ImGui::PushStyleColor(ImGuiCol_FrameBgHovered, bgHovered);
        if (bgActive) ImGui::PushStyleColor(ImGuiCol_FrameBgActive,  bgActive);
        const bool clicked = ImGui::DragFloat(label.c_str(), values, speed, min, max, format.c_str());
        ImGui::PopStyleColor(!!textColor + !!bg + !!bgHovered + !!bgActive);
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
        //ImGui::SetWindowFontScale(1.2f);
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
        //ImGui::SetWindowFontScale(1.0f);

        const ImVec2 rightLineStart(pos.x + lineWidth + textPadding + textWidth + textPadding, lineY);
        const ImVec2 rightLineEnd(pos.x + availWidth, lineY);
        draw_list->AddLine(rightLineStart, rightLineEnd, lineColor, thickness);

        ImGui::Dummy(ImVec2(0, ImGui::GetTextLineHeight()));
    }

    // Helper function: Linear interpolation for colors (assumes ARGB packed in ImU32)
    static ImU32 ImLerpColor(const ImU32 colA, const ImU32 colB, const float t)
    {
        const unsigned char a0 = (colA >> 24) & 0xFF, r0 = (colA >> 16) & 0xFF, g0 = (colA >> 8) & 0xFF, b0 = colA & 0xFF;
        const unsigned char a1 = (colB >> 24) & 0xFF, r1 = (colB >> 16) & 0xFF, g1 = (colB >> 8) & 0xFF, b1 = colB & 0xFF;
        const auto a = static_cast<unsigned char>(static_cast<float>(a0) + t * static_cast<float>(a1 - a0));
        const auto r = static_cast<unsigned char>(static_cast<float>(r0) + t * static_cast<float>(r1 - r0));
        const auto g = static_cast<unsigned char>(static_cast<float>(g0) + t * static_cast<float>(g1 - g0));
        const auto b = static_cast<unsigned char>(static_cast<float>(b0) + t * static_cast<float>(b1 - b0));
        return ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
    }

    // Clip a convex polygon against a half-plane defined by: (dot(normal, v) >= offset)
    // Sutherland–Hodgman polygon clipping.
    static void ClipPolygonWithLine(const std::vector<ImVec2>& poly, const ImVec2& normal, float offset, std::vector<ImVec2>& outPoly)
    {
        outPoly.clear();
        const auto count = poly.size();
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

    // Triangulate a convex polygon using a triangle fan and add triangles to the draw list.
    static void FillConvexPolygon(ImDrawList* drawList, const std::vector<ImVec2>& poly, const std::vector<ImU32>& polyColors)
    {
        if (poly.size() < 3)
            return;
        const auto count = static_cast<int>(poly.size());
        drawList->PrimReserve((count - 2) * 3, count);
        // Use the first vertex as pivot.
        for (int i = 1; i < count - 1; i++) {
            drawList->PrimWriteIdx(static_cast<ImDrawIdx>(drawList->_VtxCurrentIdx));
            drawList->PrimWriteIdx(static_cast<ImDrawIdx>(drawList->_VtxCurrentIdx + i));
            drawList->PrimWriteIdx(static_cast<ImDrawIdx>(drawList->_VtxCurrentIdx + i + 1));
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
        std::vector<ImVec2> rectPoly = { pMin, ImVec2(pMax.x, pMin.y), pMax, ImVec2(pMin.x, pMax.y) };

        // Compute projection range (d_min, d_max) for the rectangle.
        float d_min = std::numeric_limits<float>::max(), d_max = -std::numeric_limits<float>::max();
        for (auto& v : rectPoly) {
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
        for (long i = static_cast<long>(stops.size()) - 1; i >= 0; i--)
        {
            // Compute threshold projections for the current segment.
            const float seg_start = d_min + stops[i].pos * (d_max - d_min);
            const float seg_end   = d_min + stops[i+1].pos * (d_max - d_min);

            // Start with the whole rectangle.
            std::vector<ImVec2> segPoly = rectPoly;
            std::vector<ImVec2> tempPoly;
            // Clip against lower boundary: d >= seg_start
            ClipPolygonWithLine(segPoly, gradDir, seg_start, tempPoly);
            segPoly = tempPoly; // copy result
            // Clip against upper boundary: d <= seg_end
            // To clip with an upper-bound, invert the normal.
            ClipPolygonWithLine(segPoly, ImVec2(-gradDir.x, -gradDir.y), -(seg_end), tempPoly);
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
                const float t = (d - seg_start) / (seg_end - seg_start);
                // Interpolate the color between the two stops.
                polyColors.push_back(ImLerpColor(stops[i].color, stops[i+1].color, t));
            }

            // Draw the filled and colored polygon.
            FillConvexPolygon(drawList, segPoly, polyColors);
        }
    }
}
