//// ADocumentWindow.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        29/04/2025
//  Description: Source file for the abstract document window class
//
///////////////////////////////////////////////////////////////////////////////

#include "ADocumentWindow.hpp"

namespace nexo::editor {

    void ADocumentWindow::beginRender(const std::string &windowName)
    {
        dockingUpdate(windowName);
        visibilityUpdate();
        sizeUpdate();
    }

    void ADocumentWindow::dockingUpdate(const std::string &windowName)
    {
        if (ImGuiWindow* currentWindow = ImGui::GetCurrentWindow(); currentWindow)
        {
            const bool isDocked = currentWindow->DockIsActive;
            const ImGuiID currentDockID = currentWindow->DockId;
            auto dockId = m_windowRegistry.getDockId(windowName);

            // If it's the first time opening the window and we have a dock id saved in the registry, then we force set it
            if (m_firstOpened && ((dockId && currentDockID != *dockId)))
                ImGui::DockBuilderDockWindow(windowName.c_str(), *dockId);
            // If the docks ids differ, it means the window got rearranged in the global layout
            // If we are docked but we dont have a dock id saved in the registry, it means the user moved the window
            // In both cases, we update our docking registry with the new dock id
            else if ((dockId && currentDockID != *dockId) || (isDocked && !dockId))
                m_windowRegistry.setDockId(windowName, currentDockID);


            // If it is not docked anymore, we have a floating window without docking node,
            // So we erase it from the docking registry
            if (!m_firstOpened && !isDocked)
                m_windowRegistry.resetDockId(windowName);
            m_firstOpened = false;
        }
    }

    void ADocumentWindow::visibilityUpdate()
    {
        m_focused = ImGui::IsWindowFocused();
        bool isDocked = ImGui::IsWindowDocked();
        ImGuiWindow* window = ImGui::GetCurrentWindow();

        if (isDocked) {
            ImGuiID dock_id = ImGui::GetWindowDockID();
            ImGuiDockNode* node = ImGui::DockBuilderGetNode(dock_id);

            // If the window is currently being rendered with normal content,
            // and not hidden or set to skip items, then it is visible
            m_isVisibleInDock = !window->Hidden && !window->SkipItems && window->Active;
        }
        else {
            // Not docked windows are visible if we've reached this point
            m_isVisibleInDock = true;
        }

        m_hovered = ImGui::IsWindowHovered();
    }

    void ADocumentWindow::sizeUpdate()
    {
        ImGuiWindow* window = ImGui::GetCurrentWindow();
        m_windowPos = window->Pos;
        m_windowSize = window->Size;
        m_contentSizeMin = ImGui::GetWindowContentRegionMin();
        m_contentSizeMax = ImGui::GetWindowContentRegionMax();
        m_contentSize = ImGui::GetContentRegionAvail();
    }
}
