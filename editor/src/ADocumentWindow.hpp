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
             * @brief Constructs a new ADocumentWindow.
             *
             * Initializes a document window with the provided WindowRegistry for managing docking and registers a unique window identifier.
             *
             * @param windowRegistry The registry object used for window docking and management.
             */
            explicit ADocumentWindow(WindowRegistry &windowRegistry) : m_windowRegistry(windowRegistry)
            {
                windowId = nextWindowId++;
            };
            ~ADocumentWindow() override = default;

            [[nodiscard]] bool isFocused() const override { return m_focused; }
            [[nodiscard]] bool isOpened() const override { return m_opened; }

            /**
 * @brief Returns a reference to the window's open state.
 *
 * Provides access to the internal boolean flag that indicates whether the document window is open.
 * This reference is intended for use by GUI components (such as ImGui) to directly update and track the window's visibility.
 *
 * @return bool& A reference to the open state flag.
 */
            [[nodiscard]] bool &getOpened() override { return m_opened; }

            /**
             * @brief Configures the docking for the window on its first opening.
             *
             * On its first invocation, if the current ImGui window is valid, this function checks whether
             * the window is docked and if its current dock ID matches the one obtained from the window registry
             * for the provided window name. If the window is not docked or the dock IDs do not match, it assigns
             * the correct dock ID. This setup is performed only once per window instance.
             *
             * @param windowName The name used to retrieve the corresponding dock ID from the window registry.
             */
            void firstDockSetup(const std::string &windowName)
            {
	            if (ImGuiWindow* currentWindow = ImGui::GetCurrentWindow(); m_firstOpened && currentWindow)
		        {
					const bool isDocked = currentWindow->DockIsActive;
					const ImGuiID currentDockID = currentWindow->DockId;

					if (!isDocked || currentDockID != m_windowRegistry.getDockId(windowName))
						currentWindow->DockId = m_windowRegistry.getDockId(windowName);
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
