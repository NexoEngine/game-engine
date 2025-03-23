//// Config.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/03/2025
//  Description: Header file for the config utils
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>
#include <imgui.h>

namespace nexo::editor {
	/**
	* @brief Finds and returns the dock ID of a given window from the ImGui configuration file.
	*
	* This function reads the configuration file (typically the imgui.ini or a custom layout file)
	* and extracts the DockId for the window identified by @p windowName. The dock ID is returned as an
	* ImGuiID (unsigned integer). If the window or the dock ID is not found, the function returns 0.
	*
	* @param windowName The name of the window whose dock ID is to be found.
	* @return ImGuiID The dock ID corresponding to the window. Returns 0 if not found.
	*/
	ImGuiID findWindowDockIDFromConfig(const std::string& windowName);
}
