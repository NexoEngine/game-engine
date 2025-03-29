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

            	ImVec4* colors = ImGui::GetStyle().Colors;
            	ImVec4 bgColor = ImVec4(0, 0, 0, 0.30);
            	ImVec4 backupBgColor = colors[ImGuiCol_WindowBg];
            	//bgColor.w = 0.10f;

            	if (!isWindowDocked && false) {
            		//ImGui::PushStyleColor(ImGuiCol_WindowBg, bgColor);
            		colors[ImGuiCol_WindowBg] = bgColor;
            	}

	            //ImGui::SetNextWindowBgAlpha(0.0f);


            	window->show();
            	if (!isWindowDocked && false) {
            		//ImGui::PopStyleColor();
            		colors[ImGuiCol_WindowBg] = backupBgColor;
            	}
            	/*ImGui::Begin(window->getWindowName().c_str(), &window->getOpened());
            	ImGui::Button("test", ImVec2(100, 100));
            	ImGui::End();*/

            	//if (isWindowDocked) ImGui::PopStyleColor();
            }
        }
	}
}
