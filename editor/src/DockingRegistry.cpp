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
