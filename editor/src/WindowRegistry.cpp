//// WindowRegistry.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        24/03/2025
//  Description: Source file for the window registry
//
///////////////////////////////////////////////////////////////////////////////

#include "WindowRegistry.hpp"
#include "Logger.hpp"
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

	std::shared_ptr<IDocumentWindow> WindowRegistry::getFocusedWindow() const
	{
    	for (const auto &[_, windows]: m_windows)
        {
            for (const auto &window : windows)
            {
                if (window->isFocused())
                    return window;
            }
        }
        return nullptr;
	}

	void WindowRegistry::resetDockId(const std::string &name)
	{
	   m_dockingRegistry.resetDockId(name);
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
            	window->show();
            }
        }
	}

	bool WindowRegistry::splitDockAndAttachWindow(
		const std::string& sourceWindowName,
		const std::string& newWindowName,
		const ImGuiDir splitDirection,
		const float splitRatio)
	{
		const ImGuiWindow* sourceImGuiWindow = ImGui::FindWindowByName(sourceWindowName.c_str());

		if (!sourceImGuiWindow || !sourceImGuiWindow->DockId)
		{
			LOG(NEXO_WARN, "Cannot split dock: source window '{}' not found or not docked", sourceWindowName);
			return false;
		}

		const ImGuiID sourceDockId = sourceImGuiWindow->DockId;
		ImGuiID newNode, remainingNode;

		if (!ImGui::DockBuilderSplitNode(sourceDockId, splitDirection, splitRatio, &newNode, &remainingNode))
		{
			LOG(NEXO_WARN, "Failed to split dock node for window '{}'", sourceWindowName);
			return false;
		}

		ImGui::DockBuilderDockWindow(sourceWindowName.c_str(), remainingNode);
		ImGui::DockBuilderDockWindow(newWindowName.c_str(), newNode);
		ImGui::DockBuilderFinish(sourceDockId);

		m_dockingRegistry.setDockId(sourceWindowName, remainingNode);
		m_dockingRegistry.setDockId(newWindowName, newNode);

		LOG(NEXO_DEBUG, "Successfully split dock for '{}' and attached '{}'", sourceWindowName, newWindowName);
		return true;
	}
}
