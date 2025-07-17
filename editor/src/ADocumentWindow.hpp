//// ADocumentWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Header file for the document window abstract class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <utility>

#include "IDocumentWindow.hpp"
#include "Nexo.hpp"
#include "WindowRegistry.hpp"
#include "inputs/WindowState.hpp"

namespace nexo::editor {

    #define NEXO_WND_USTRID_INSPECTOR "###Inspector"
    #define NEXO_WND_USTRID_SCENE_TREE "###Scene Tree"
    #define NEXO_WND_USTRID_ASSET_MANAGER "###Asset Manager"
    #define NEXO_WND_USTRID_CONSOLE "###Console"
    #define NEXO_WND_USTRID_MATERIAL_INSPECTOR "###Material Inspector"
    #define NEXO_WND_USTRID_PRIMITIVE_WINDOW "###Primitive Window"
    #define NEXO_WND_USTRID_DEFAULT_SCENE "###Default Scene"
    #define NEXO_WND_USTRID_BOTTOM_BAR "###Commands Bar"
    #define NEXO_WND_USTRID_TEST "###Test Window"

    class ADocumentWindow : public IDocumentWindow {
        public:
            /**
             * @brief Constructs a new ADocumentWindow instance with a unique window ID.
             *
             * Initializes the document window by storing a reference to the provided WindowRegistry and assigning a unique window
             * identifier. This setup is essential for integrating the window with the docking management system.
             */
            explicit ADocumentWindow(std::string windowName, WindowRegistry &windowRegistry) : m_windowName(std::move(windowName)), m_windowRegistry(windowRegistry)
            {
                windowId = nextWindowId++;
            };
            ~ADocumentWindow() override = default;

            [[nodiscard]] bool isFocused() const override { return m_focused; }
            [[nodiscard]] bool isOpened() const override { return m_opened; }
            void setOpened(bool opened) override { m_opened = opened; }
            [[nodiscard]] bool isHovered() const override { return m_hovered; }

            [[nodiscard]] const ImVec2 &getContentSize() const override { return m_contentSize; }

            /**
             * @brief Retrieves the open state of the document window.
             *
             * Returns a reference to the internal boolean that tracks whether the window is open.
             * This reference is primarily intended for ImGui integration, allowing direct manipulation
             * of the window's visibility state.
             *
             * @return A reference to the window's open state.
             */
            [[nodiscard]] bool &getOpened() override { return m_opened; }

            [[nodiscard]] const std::string &getWindowName() const override { return m_windowName; }

            [[nodiscard]] const WindowState &getWindowState() const override { return m_windowState; };



            WindowId windowId;
        protected:
            bool m_opened = true;
            bool m_focused = false;
            bool m_hovered = false; // TODO: make these update without user intervention
            bool m_wasVisibleLastFrame = false;
            bool m_isVisibleInDock = true;

            ImVec2 m_windowPos;
            ImVec2 m_windowSize;
            ImVec2 m_contentSizeMin;
            ImVec2 m_contentSizeMax;
            ImVec2 m_contentSize;

            bool m_firstOpened = true;

            std::string m_windowName;
            WindowState m_windowState;

            WindowRegistry &m_windowRegistry;

            void beginRender(const std::string &windowName);
        private:
            /**
            * @brief Initializes the docking configuration for the document window on its first display.
            *
            * This function retrieves the current ImGui window and checks its docking state to ensure it aligns with the expected
            * configuration from the WindowRegistry. On the first open (when m_firstOpened is true), if the window is not actively
            * docked or its current dock ID does not match the expected ID obtained via the provided window name, the function assigns
            * the expected dock ID to the window. If the window is already docked but the dock IDs still differ, the current dock ID is
            * saved to the WindowRegistry. The m_firstOpened flag is then set to false so that the docking configuration is applied only once.
            *
            * @param windowName The name used to look up the expected dock identifier in the WindowRegistry.
            */
            void dockingUpdate(const std::string &windowName);
            void visibilityUpdate();
            void sizeUpdate();
    };
}
