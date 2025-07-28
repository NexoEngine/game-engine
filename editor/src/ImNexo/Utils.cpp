//// Utils.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        18/04/2025
//  Description: Source file for the ui utils functions
//
///////////////////////////////////////////////////////////////////////////////

#include "Utils.hpp"
#include <imgui_internal.h>

namespace ImNexo::utils {

    ImU32 imLerpColor(const ImU32 colA, const ImU32 colB, const float t)
    {
        const unsigned char a0 = (colA >> 24) & 0xFF;
        const unsigned char r0 = (colA >> 16) & 0xFF;
        const unsigned char g0 = (colA >> 8) & 0xFF;
        const unsigned char b0 = colA & 0xFF;
        const unsigned char a1 = (colB >> 24) & 0xFF;
        const unsigned char r1 = (colB >> 16) & 0xFF;
        const unsigned char g1 = (colB >> 8) & 0xFF;
        const unsigned char b1 = colB & 0xFF;
        const auto a = static_cast<unsigned char>(static_cast<float>(a0) + t * static_cast<float>(a1 - a0));
        const auto r = static_cast<unsigned char>(static_cast<float>(r0) + t * static_cast<float>(r1 - r0));
        const auto g = static_cast<unsigned char>(static_cast<float>(g0) + t * static_cast<float>(g1 - g0));
        const auto b = static_cast<unsigned char>(static_cast<float>(b0) + t * static_cast<float>(b1 - b0));
        return ((a & 0xFF) << 24) | ((r & 0xFF) << 16) | ((g & 0xFF) << 8) | (b & 0xFF);
    }

    void clipPolygonWithLine(const std::vector<ImVec2>& poly, const ImVec2& normal, const float offset, std::vector<ImVec2>& outPoly)
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

    void fillConvexPolygon(ImDrawList* drawList, const std::vector<ImVec2>& poly, const std::vector<ImU32>& polyColors)
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

    std::pair<ImVec2, ImVec2> getItemRect(const ImVec2& padding)
    {
        ImVec2 p_min = ImGui::GetItemRectMin();
        ImVec2 p_max = ImGui::GetItemRectMax();

        p_min.x += padding.x;
        p_min.y += padding.y;
        p_max.x -= padding.x;
        p_max.y -= padding.y;

        return {p_min, p_max};
    }

    /**
    * @brief Positions text centered within a rectangle
    */
    ImVec2 calculateCenteredTextPosition(const std::string& text, const ImVec2& p_min, const ImVec2& p_max)
    {
        const ImVec2 textSize = ImGui::CalcTextSize(text.c_str());
        return {
            p_min.x + (p_max.x - p_min.x - textSize.x) * 0.5f,
            p_min.y + (p_max.y - p_min.y - textSize.y) * 0.5f
        };
    }
}
