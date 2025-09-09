//// Utils.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        18/04/2025
//  Description: Header file for the ui utils functions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>
#include <vector>
#include <string>

namespace ImNexo::utils {

    /**
    * @brief Linearly interpolates between two colors (ImU32, ImGui 32-bits ARGB format).
    * @param[in] colA The first color (ARGB format).
    * @param[in] colB The second color (ARGB format).
    * @param[in] t The interpolation factor (0.0 to 1.0).
    * @return The interpolated color (ARGB format).
    */
    ImU32 imLerpColor(ImU32 colA, ImU32 colB, float t);

    /**
    * @brief Clip a convex polygon against a half-plane defined by: (dot(normal, v) >= offset)
    *
    * This function uses the Sutherland-Hodgman algorithm to clip a polygon against a line defined by a normal vector and an offset.
    * @param[in] poly Vector of vertices representing the polygon to be clipped.
    * @param[in] normal The normal vector of the line used for clipping.
    * @param[in] offset The offset from the origin of the line.
    * @param[out] outPoly Output vector to store the clipped polygon vertices.
    */
    void clipPolygonWithLine(const std::vector<ImVec2>& poly, const ImVec2& normal, float offset, std::vector<ImVec2>& outPoly);

    /**
    * @brief Fill a convex polygon with triangles using a triangle fan.
    * @param[in] drawList The ImDrawList to which the triangles will be added.
    * @param[in] poly Vector of vertices representing the polygon to be filled.
    * @param[in] polyColors Vector of colors for each vertex in the polygon.
    */
    void fillConvexPolygon(ImDrawList* drawList, const std::vector<ImVec2>& poly, const std::vector<ImU32>& polyColors);

    /**
    * @brief Helper to get the current item's rect and optionally apply padding
    */
    std::pair<ImVec2, ImVec2> getItemRect(const ImVec2& padding = ImVec2(0, 0));

    /**
    * @brief Positions text centered within a rectangle
    */
    ImVec2 calculateCenteredTextPosition(const std::string& text, const ImVec2& p_min, const ImVec2& p_max);
}
