//// String.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        26/03/2025
//  Description: Source file for the strings utils
//
///////////////////////////////////////////////////////////////////////////////

#include "String.hpp"

#include <algorithm>
#include <cctype>
#include <ranges>
#include <string>

namespace nexo::editor::utils {
	std::string removeIconPrefix(const std::string &str)
	{
		if (const size_t pos = str.find(' '); pos != std::string::npos)
			return str.substr(pos + 1);
		return str;
	}

	void trim(std::string &s)
    {
        auto not_space = [](char c){ return !std::isspace(static_cast<unsigned char>(c)); };

        s.erase(s.begin(), std::ranges::find_if(s, not_space));
        auto rit = std::ranges::find_if(
            s | std::views::reverse,
            not_space
        );
        s.erase(rit.base(),s.end());
    }
}
