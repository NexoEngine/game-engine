//// WindowRegistry.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/03/2025
//  Description: Source file for the window registry
//
///////////////////////////////////////////////////////////////////////////////

#include "WindowRegistry.hpp"

#include <imgui_internal.h>

namespace nexo::editor {

	void WindowRegistry::setup() const
	{
		for (const auto &[_, windows]: m_windows)
        {
            for (const auto &window : windows)
            {
                window->setup();
            }
        }
	}

	void WindowRegistry::shutdown() const
	{
		for (const auto &[_, windows]: m_windows)
        {
            for (const auto &window : windows)
            {
                window->shutdown();
            }
        }
	}

	void WindowRegistry::setDockId(const std::string& name, ImGuiID id)
	{
		m_dockingRegistry.setDockId(name, id);
	}

	std::optional<ImGuiID> WindowRegistry::getDockId(const std::string& name) const
	{
		return m_dockingRegistry.getDockId(name);
	}

	void WindowRegistry::update() const
	{
		for (const auto &[_, windows]: m_windows)
        {
            for (const auto &window : windows)
            {
                window->update();
            }
        }
	}

	void WindowRegistry::render() const
	{
		for (const auto &[_, windows]: m_windows)
        {
            for (const auto &window : windows)
            {
            	if (!window->isOpened())
            		continue;
            	ImGuiWindow* imguiWindow = ImGui::FindWindowByName(window->getWindowName().c_str());
            	ImGuiDockNode* node = imguiWindow ? imguiWindow->DockNode : nullptr;
            	//LOG(NEXO_DEBUG, "Node ID: {}", node ? std::to_string(node->ID) : "no id");
            	const bool isWindowDocked = imguiWindow != nullptr && imguiWindow->DockIsActive;
            	auto bgColor = IM_COL32(0, 0, 0, 70);
            	//bgColor.w = 0.10f;
            	//if (isWindowDocked) ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
            	ImGui::PushStyleColor(ImGuiCol_WindowBg, IM_COL32(0, 0, 0, 70));

	            ImGui::SetNextWindowBgAlpha(0.0f);
            	window->show();
            	ImGui::PopStyleColor();
            	//if (isWindowDocked) ImGui::PopStyleColor();
            }
        }
	}
}
