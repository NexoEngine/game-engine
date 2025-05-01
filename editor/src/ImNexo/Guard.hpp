//// Guard.hpp ////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        18/04/2025
//  Description: Header for the utils guard class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <imgui.h>
#include <imgui_internal.h>
#include <vector>
#include <string>

namespace ImNexo {
    /**
     * @brief Guard class for managing ImGui style colors.
     *
     * Automatically pushes ImGui style colors on construction and pops them on destruction,
     * ensuring the style state is properly restored even when exceptions occur. Supports
     * chaining multiple color changes with the push() method.
     */
    class StyleGuard {
        public:
            /**
             * @brief Constructs a StyleGuard and pushes the initial style color.
             *
             * @param col The ImGui color enumeration to modify
             * @param color The new color value (0 to skip pushing)
             */
            explicit StyleGuard(const ImGuiCol_ col, const ImU32 color)
            {
                if (color != 0) {
                    m_colorIndices.push_back(col);
                    ImGui::PushStyleColor(col, color);
                }
            }

            /**
             * @brief Pushes an additional style color to the guard.
             *
             * @param col The ImGui color enumeration to modify
             * @param color The new color value (0 to skip pushing)
             * @return Reference to this guard for method chaining
             */
            StyleGuard& push(const ImGuiCol_ col, const ImU32 color)
            {
                if (color != 0) {
                    m_colorIndices.push_back(col);
                    ImGui::PushStyleColor(col, color);
                }
                return *this;
            }

            /**
             * @brief Destructor that automatically pops all pushed style colors.
             */
            ~StyleGuard()
            {
                if (!m_colorIndices.empty())
                    ImGui::PopStyleColor(static_cast<int>(m_colorIndices.size()));
            }

        private:
            std::vector<ImGuiCol_> m_colorIndices; ///< Tracks which color indices were pushed
    };

    /**
     * @brief Guard class for managing ImGui style variables.
     *
     * Automatically pushes ImGui style variables on construction and pops them on destruction,
     * ensuring the style state is properly restored even when exceptions occur. Supports
     * chaining multiple variable changes with the push() method.
     */
    class StyleVarGuard {
        public:
            /**
             * @brief Constructs a StyleVarGuard and pushes an initial vector style variable.
             *
             * @param var The ImGui style variable enumeration to modify
             * @param value The new vector value
             */
            explicit StyleVarGuard(const ImGuiStyleVar_ var, const ImVec2 value)
            {
                m_varCount++;
                ImGui::PushStyleVar(var, value);
            }

            /**
             * @brief Constructs a StyleVarGuard and pushes an initial scalar style variable.
             *
             * @param var The ImGui style variable enumeration to modify
             * @param value The new scalar value
             */
            explicit StyleVarGuard(const ImGuiStyleVar_ var, const float value)
            {
                m_varCount++;
                ImGui::PushStyleVar(var, value);
            }

            /**
             * @brief Pushes an additional vector style variable to the guard.
             *
             * @param var The ImGui style variable enumeration to modify
             * @param value The new vector value
             * @return Reference to this guard for method chaining
             */
            StyleVarGuard& push(const ImGuiStyleVar_ var, const ImVec2 value)
            {
                m_varCount++;
                ImGui::PushStyleVar(var, value);
                return *this;
            }

            /**
             * @brief Pushes an additional scalar style variable to the guard.
             *
             * @param var The ImGui style variable enumeration to modify
             * @param value The new scalar value
             * @return Reference to this guard for method chaining
             */
            StyleVarGuard& push(const ImGuiStyleVar_ var, const float value)
            {
                m_varCount++;
                ImGui::PushStyleVar(var, value);
                return *this;
            }

            /**
             * @brief Destructor that automatically pops all pushed style variables.
             */
            ~StyleVarGuard()
            {
                if (m_varCount > 0)
                    ImGui::PopStyleVar(m_varCount);
            }

        private:
            int m_varCount = 0; ///< Counts how many style variables were pushed
    };

    /**
     * @brief Guard class for managing ImGui ID stack.
     *
     * Automatically pushes an ID to the ImGui ID stack on construction and pops
     * it on destruction, ensuring proper nesting and scoping of unique identifiers
     * even when exceptions occur.
     */
    class IdGuard {
        public:
            /**
             * @brief Constructs an IdGuard and pushes the specified ID.
             *
             * @param id The string identifier to push onto the ImGui ID stack
             */
            explicit IdGuard(const std::string& id)
            {
                ImGui::PushID(id.c_str());
            }

            /**
             * @brief Destructor that automatically pops the pushed ID.
             */
            ~IdGuard()
            {
                ImGui::PopID();
            }
    };

    /**
     * @brief Guard class for managing ImGui font scaling.
     *
     * Temporarily changes the window font scale factor and restores it
     * to the default scale (1.0) when the guard goes out of scope.
     */
    class FontScaleGuard {
        public:
            /**
             * @brief Constructs a FontScaleGuard and sets the font scale.
             *
             * @param scale The scaling factor to apply to the current window's font
             */
            explicit FontScaleGuard(const float scale) : m_scale(scale)
            {
                ImGui::SetWindowFontScale(scale);
            }

            /**
             * @brief Destructor that automatically resets the font scale to default.
             */
            ~FontScaleGuard()
            {
                ImGui::SetWindowFontScale(1.0f);
            }
        private:
            float m_scale; ///< Stores the applied scale factor
    };
}
