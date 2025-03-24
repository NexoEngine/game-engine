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
            explicit ADocumentWindow(WindowRegistry &windowRegistry) : m_windowRegistry(windowRegistry)
            {
                windowId = nextWindowId++;
            };
            ~ADocumentWindow() override = default;

            [[nodiscard]] bool isFocused() const override { return m_focused; }
            [[nodiscard]] bool isOpened() const override { return m_opened; }

            /**
             * @brief Get the opened value
             * @note Used to give a ref to ImGui
             * @return ref to bool _opened
             */
            [[nodiscard]] bool &getOpened() override { return m_opened; }

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
