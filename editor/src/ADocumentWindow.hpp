//// ADocumentWindow.hpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
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
            explicit ADocumentWindow(WindowRegistry &windowRegistry) : m_windowRegistry(windowRegistry)
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

            /**
             * @brief Configures the initial docking for the document window.
             *
             * On its first call, this function retrieves the current ImGui window and verifies its docking state. It fetches the expected dock identifier for the given window name from the WindowRegistry. If the window is not currently docked or its dock ID does not match the expected ID, the function updates the ImGui window's dock ID accordingly. The operation is performed only once, as the first-open flag is set to false after execution.
             *
             * @param windowName The name used to retrieve the expected dock identifier from the WindowRegistry.
             */
            void firstDockSetup(const std::string &windowName)
            {
	            if (ImGuiWindow* currentWindow = ImGui::GetCurrentWindow(); m_firstOpened && currentWindow)
		        {
					const bool isDocked = currentWindow->DockIsActive;
					const ImGuiID currentDockID = currentWindow->DockId;
					auto dockId = m_windowRegistry.getDockId(windowName);

					if (!isDocked || (dockId && currentDockID != *dockId))
						currentWindow->DockId = *dockId;
					m_firstOpened = false;
		        }
            }

            WindowId windowId;
        protected:
            bool m_opened = true;
            bool m_focused = false;

            bool m_firstOpened = true;

            WindowRegistry &m_windowRegistry;
    };
}
