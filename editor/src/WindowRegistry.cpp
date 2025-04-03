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
            	window->show();
            }
        }
	}
}
