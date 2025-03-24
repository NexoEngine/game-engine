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

namespace nexo::editor {

	/**
	 * @brief Initializes all registered windows.
	 *
	 * Iterates over the collection of registered windows and invokes each window's
	 * setup() method to perform its initialization.
	 */
	void WindowRegistry::setup() const
	{
		for (const auto &[_, window]: m_windows)
        {
            window->setup();
        }
	}

	/**
	 * @brief Shuts down all registered windows.
	 *
	 * Iterates over the window collection and calls the shutdown method on each window,
	 * ensuring proper cleanup of all managed windows.
	 */
	void WindowRegistry::shutdown() const
	{
		for (const auto &[_, window]: m_windows)
        {
            window->shutdown();
        }
	}

	/**
	 * @brief Assigns a docking identifier to the specified window.
	 *
	 * This method sets a docking ID for the window identified by the given name by delegating the operation to the docking registry.
	 *
	 * @param name The unique name of the window.
	 * @param id The docking identifier to be associated with the window.
	 */
	void WindowRegistry::setDockId(const std::string& name, ImGuiID id)
	{
		m_dockingRegistry.setDockId(name, id);
	}

	/**
	 * @brief Retrieves the docking identifier associated with a window name.
	 *
	 * This method delegates the lookup to the docking registry, returning the docking ID for the specified window.
	 *
	 * @param name The name of the window whose docking ID is requested.
	 * @return ImGuiID The docking identifier corresponding to the given window name.
	 */
	ImGuiID WindowRegistry::getDockId(const std::string& name) const
	{
		return m_dockingRegistry.getDockId(name);
	}

	/**
	 * @brief Updates each registered window.
	 *
	 * Iterates over the collection of registered windows and calls the update() method on each one,
	 * allowing individual windows to perform any necessary per-frame updates.
	 */
	void WindowRegistry::update() const
	{
		for (const auto &[_, window]: m_windows)
        {
            window->update();
        }
	}

	/**
	 * @brief Renders all open windows.
	 *
	 * Iterates over the registered windows and invokes their show() method if they are currently opened.
	 */
	void WindowRegistry::render() const
	{
		for (const auto &[_, window]: m_windows)
        {
            if (window->isOpened())
                window->show();
        }
	}
}
