//// DockingRegistry.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/03/2025
//  Description: Source file for the docking registry class
//
///////////////////////////////////////////////////////////////////////////////

#include "DockingRegistry.hpp"

namespace nexo::editor {
	/**
	 * @brief Registers or updates the docking ID for a given name.
	 *
	 * Associates the specified docking name with its corresponding ImGuiID in the registry.
	 * If an entry for the given name already exists, its ID is updated.
	 *
	 * @param name The unique name identifying the docking element.
	 * @param id The ImGui docking identifier associated with the element.
	 */
	void DockingRegistry::setDockId(const std::string& name, ImGuiID id)
	{
		dockIds[name] = id;
	}

	/**
	 * @brief Retrieves the dock ID associated with the specified name.
	 *
	 * Searches the registry for the given name and returns the corresponding dock ID. If the name is not found, returns 0.
	 *
	 * @param name The identifier for the dock.
	 * @return ImGuiID The dock ID if found, or 0 otherwise.
	 */
	ImGuiID DockingRegistry::getDockId(const std::string& name) const
	{
		auto it = dockIds.find(name);
		if (it != dockIds.end()) {
			return it->second;
		}
		return 0;
	}
}
