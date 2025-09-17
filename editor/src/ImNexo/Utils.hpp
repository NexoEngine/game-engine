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
#include <string>
#include <vector>

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
     * This function uses the Sutherland-Hodgman algorithm to clip a polygon against a line defined by a normal vector
     * and an offset.
     * @param[in] poly Vector of vertices representing the polygon to be clipped.
     * @param[in] normal The normal vector of the line used for clipping.
     * @param[in] offset The offset from the origin of the line.
     * @param[out] outPoly Output vector to store the clipped polygon vertices.
     */
    void clipPolygonWithLine(const std::vector<ImVec2>& poly, const ImVec2& normal, float offset,
                             std::vector<ImVec2>& outPoly);

    /**
     * @brief Fill a convex polygon with triangles using a triangle fan.
     *
     * This function fills a convex polygon by creating a triangle fan from the first vertex to all other vertices.
     * @param[in] drawList The ImDrawList to which the triangles will be added.
     * @param[in] poly Vector of vertices representing the polygon to be filled.
     * @param[in] polyColors Vector of colors for each vertex in the polygon.
     */
    void fillConvexPolygon(ImDrawList* drawList, const std::vector<ImVec2>& poly, const std::vector<ImU32>& polyColors);

    /**
     * @brief Get the rectangle of the last item with optional padding.
     *
     * This function retrieves the minimum and maximum coordinates of the last item rendered in ImGui,
     * applying the specified padding to both corners.
     *
     * @param padding The padding to apply to the rectangle (default is (0,0)).
     * @return A pair of ImVec2 representing the minimum and maximum corners of the padded rectangle.
     */
    std::pair<ImVec2, ImVec2> getItemRect(const ImVec2& padding = ImVec2(0, 0));

    /**
     * @brief Calculate the position to render centered text within a given rectangle.
     *
     * This function computes the top-left position to start rendering the specified text
     * so that it appears centered within the rectangle defined by p_min and p_max.
     *
     * @param text The text string to be centered.
     * @param p_min The minimum (top-left) corner of the rectangle.
     * @param p_max The maximum (bottom-right) corner of the rectangle.
     * @return The ImVec2 position where the text should be rendered to be centered.
     */
    ImVec2 calculateCenteredTextPosition(const std::string& text, const ImVec2& p_min, const ImVec2& p_max);
} // namespace ImNexo::utils
