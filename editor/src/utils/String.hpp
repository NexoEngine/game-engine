//// String.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/03/2025
//  Description: Header file for string utils
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <string>

namespace nexo::editor::utils {
	/**
	* @brief Removes the icon prefix from a string.
	*
	* This function assumes that an icon is prefixed to the string followed by a space.
	* It searches for the first space in the string and returns the substring following that space.
	* If no space is found, the original string is returned unchanged.
	*
	* @param str The input string that may contain an icon prefix.
	* @return std::string The string with the icon prefix removed.
	*/
	std::string removeIconPrefix(const std::string &str);

	void trim(std::string &s);
}
