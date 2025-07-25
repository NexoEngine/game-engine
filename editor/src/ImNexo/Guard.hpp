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
          * @brief Delete copy constructor since we can't precisely duplicate the pushed styles.
          *
          * We only track which color indices were pushed, but not their actual values.
          * Without storing the color values, we can't properly recreate the pushed styles.
          */
         StyleGuard(const StyleGuard&) = delete;

         /**
          * @brief Delete copy assignment operator for the same reason as the copy constructor.
          */
         StyleGuard& operator=(const StyleGuard&) = delete;

         /**
          * @brief Move constructor - transfers ownership of pushed styles.
          */
         StyleGuard(StyleGuard&& other) noexcept : m_colorIndices(std::move(other.m_colorIndices))
         {
             // The moved-from object should no longer pop any style colors
             other.m_colorIndices.clear();
         }

         /**
          * @brief Move assignment operator - transfers ownership of pushed styles.
          */
         StyleGuard& operator=(StyleGuard&& other) noexcept
         {
             if (this != &other) {
                 // Pop our current styles
                 if (!m_colorIndices.empty())
                     ImGui::PopStyleColor(static_cast<int>(m_colorIndices.size()));

                 // Take ownership of other's styles
                 m_colorIndices = std::move(other.m_colorIndices);

                 // Ensure other doesn't pop our newly acquired styles
                 other.m_colorIndices.clear();
             }
             return *this;
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
          * @brief Delete copy constructor since we can't recreate the exact pushed styles.
          *
          * The class only tracks how many styles were pushed, not which ones or their values.
          */
         StyleVarGuard(const StyleVarGuard&) = delete;

         /**
          * @brief Delete copy assignment operator for the same reason as the copy constructor.
          */
         StyleVarGuard& operator=(const StyleVarGuard&) = delete;

         /**
          * @brief Move constructor - transfers responsibility for popping style variables.
          */
         StyleVarGuard(StyleVarGuard&& other) noexcept : m_varCount(other.m_varCount)
         {
             // Prevent the moved-from object from popping our styles
             other.m_varCount = 0;
         }

         /**
          * @brief Move assignment operator - transfers responsibility for popping style variables.
          */
         StyleVarGuard& operator=(StyleVarGuard&& other) noexcept
         {
             if (this != &other) {
                 // Pop our current styles
                 if (m_varCount > 0)
                     ImGui::PopStyleVar(m_varCount);

                 // Take over responsibility for other's styles
                 m_varCount = other.m_varCount;

                 // Prevent the moved-from object from popping our styles
                 other.m_varCount = 0;
             }
             return *this;
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
             m_pushed = true;
         }

         /**
          * @brief Delete copy constructor since we can't duplicate the pushed ID.
          *
          * We don't store the ID string after construction, so we can't recreate
          * the exact same state in a copy.
          */
         IdGuard(const IdGuard&) = delete;

         /**
          * @brief Delete copy assignment operator for the same reason as copy constructor.
          */
         IdGuard& operator=(const IdGuard&) = delete;

         /**
          * @brief Move constructor - transfers responsibility for popping the ID.
          */
         IdGuard(IdGuard&& other) noexcept : m_pushed(other.m_pushed)
         {
             // Prevent the moved-from object from popping the ID
             other.m_pushed = false;
         }

         /**
          * @brief Move assignment operator - transfers responsibility for popping the ID.
          */
         IdGuard& operator=(IdGuard&& other) noexcept
         {
             if (this != &other) {
                 // Pop our current ID if we pushed one
                 if (m_pushed)
                     ImGui::PopID();

                 // Take over responsibility for other's ID
                 m_pushed = other.m_pushed;

                 // Prevent the moved-from object from popping the ID
                 other.m_pushed = false;
             }
             return *this;
         }

         /**
          * @brief Destructor that automatically pops the pushed ID.
          */
         ~IdGuard()
         {
             if (m_pushed)
                 ImGui::PopID();
         }

     private:
         bool m_pushed = false; ///< Tracks whether an ID was pushed and needs to be popped
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
         explicit FontScaleGuard(const float scale)
         {
             ImGui::SetWindowFontScale(scale);
         }

         /**
          * @brief Delete copy constructor since we can't properly duplicate this action.
          *
          * Copying this guard would require knowledge of the current window context
          * and possibly interfere with other active guards.
          */
         FontScaleGuard(const FontScaleGuard&) = delete;

         /**
          * @brief Delete copy assignment operator for the same reasons as copy constructor.
          */
         FontScaleGuard& operator=(const FontScaleGuard&) = delete;

         /**
          * @brief Move constructor - transfers responsibility for resetting the font scale.
          */
         FontScaleGuard(FontScaleGuard&& other) noexcept
             : m_active(other.m_active)
         {
             // Prevent the moved-from object from resetting the font scale
             other.m_active = false;
         }

         /**
          * @brief Move assignment operator - transfers responsibility for resetting the font scale.
          */
         FontScaleGuard& operator=(FontScaleGuard&& other) noexcept
         {
             if (this != &other) {
                 // Reset our scale if we're currently active
                 if (m_active)
                     ImGui::SetWindowFontScale(1.0f);

                 // Take over responsibility
                 m_active = other.m_active;

                 // Prevent the moved-from object from resetting the scale
                 other.m_active = false;
             }
             return *this;
         }

         /**
          * @brief Destructor that automatically resets the font scale to default.
          */
         ~FontScaleGuard()
         {
             if (m_active)
                 ImGui::SetWindowFontScale(1.0f);
         }

     private:
         bool m_active = true; ///< Tracks whether this guard is responsible for resetting the scale
     };
}
