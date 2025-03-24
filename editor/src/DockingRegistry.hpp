//// DockingRegistry.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/03/2025
//  Description: Header file for the docking registry class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <unordered_map>
#include <imgui.h>
#include <typeindex>
#include <string>

namespace nexo::editor {

	class DockingRegistry {
		public:
			void setDockId(const std::string& name, ImGuiID id);
			ImGuiID getDockId(const std::string& name) const;

		private:
			std::unordered_map<std::string, ImGuiID> dockIds;
	};
}
