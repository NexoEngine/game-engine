//// DockingRegistry.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/03/2025
//  Description: Source file for the docking registry class
//
///////////////////////////////////////////////////////////////////////////////

#include "DockingRegistry.hpp"
#include <optional>
#include <iostream>

namespace nexo::editor {

	void DockingRegistry::setDockId(const std::string& name, const ImGuiID id)
	{
		dockIds[name] = id;
	}

	std::optional<ImGuiID> DockingRegistry::getDockId(const std::string& name) const
	{
		const auto it = dockIds.find(name);
		if (it != dockIds.end()) {
			return it->second;
		}
		return std::nullopt;
	}

	void DockingRegistry::resetDockId(const std::string &name)
	{
        const auto it = dockIds.find(name);
        if (it == dockIds.end())
            return;
        dockIds.erase(it);
	}
}
