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

#include "IDocumentWindow.hpp"
#include "Nexo.hpp"
#include "WindowRegistry.hpp"

#include <imgui_internal.h>

namespace nexo::editor {
    class ADocumentWindow : public IDocumentWindow {
        public:
            /**
             * @brief Constructs a new ADocumentWindow instance with a unique window ID.
             *
             * Initializes the document window by storing a reference to the provided WindowRegistry and assigning a unique window
             * identifier. This setup is essential for integrating the window with the docking management system.
             */
            explicit ADocumentWindow(const std::string &windowName, WindowRegistry &windowRegistry) : m_windowName(windowName), m_windowRegistry(windowRegistry)
            {
                windowId = nextWindowId++;
            };
            ~ADocumentWindow() override = default;

            [[nodiscard]] bool isFocused() const override { return m_focused; }
            [[nodiscard]] bool isOpened() const override { return m_opened; }

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
            void firstDockSetup(const std::string &windowName)
            {
	            if (ImGuiWindow* currentWindow = ImGui::GetCurrentWindow(); currentWindow)
		        {
					const bool isDocked = currentWindow->DockIsActive;
					const ImGuiID currentDockID = currentWindow->DockId;
					auto dockId = m_windowRegistry.getDockId(windowName);

					if (m_firstOpened && (!isDocked || (dockId && currentDockID != *dockId)))
						currentWindow->DockId = *dockId;
					else if (dockId && currentDockID != *dockId)
						m_windowRegistry.setDockId(windowName, currentDockID);
					m_firstOpened = false;
		        }
            }

            WindowId windowId;
        protected:
            bool m_opened = true;
            bool m_focused = false;

            bool m_firstOpened = true;

            std::string m_windowName;

            WindowRegistry &m_windowRegistry;
    };
}
