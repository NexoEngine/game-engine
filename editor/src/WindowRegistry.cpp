//// WindowRegistry.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        24/03/2025
//  Description: Source file for the window registry
//
///////////////////////////////////////////////////////////////////////////////

#include "WindowRegistry.hpp"

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
}
