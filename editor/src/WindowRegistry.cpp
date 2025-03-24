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
	 * @brief Initializes all managed windows.
	 *
	 * Iterates through the collection of windows and calls the `setup()` method on each one.
	 * This function does not handle errors; it assumes each window's setup process runs without exceptions.
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
	 * Iterates through each window in the registry and invokes its shutdown routine.
	 */
	void WindowRegistry::shutdown() const
	{
		for (const auto &[_, window]: m_windows)
        {
            window->shutdown();
        }
	}

	/**
	 * @brief Assigns a docking identifier to a window.
	 *
	 * Registers the specified docking identifier for the window identified by its name by delegating to the docking registry.
	 *
	 * @param name The unique name of the window.
	 * @param id The docking identifier to associate with the window.
	 */
	void WindowRegistry::setDockId(const std::string& name, ImGuiID id)
	{
		m_dockingRegistry.setDockId(name, id);
	}

	/**
	 * @brief Retrieves the docking identifier associated with a specified window.
	 *
	 * This function queries the docking registry for the docking identifier corresponding to the given window name.
	 * If the window does not have an assigned docking ID, an empty optional is returned.
	 *
	 * @param name The name of the window whose docking identifier is being requested.
	 * @return std::optional<ImGuiID> The docking identifier if it exists; otherwise, an empty optional.
	 */
	std::optional<ImGuiID> WindowRegistry::getDockId(const std::string& name) const
	{
		return m_dockingRegistry.getDockId(name);
	}

	/**
	 * @brief Updates all registered windows.
	 *
	 * Iterates over the collection of registered windows and calls the update() method on each,
	 * ensuring that their state is refreshed.
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
	 * Iterates through the registered windows and invokes the show() method on each window that is currently opened.
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
