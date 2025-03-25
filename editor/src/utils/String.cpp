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

namespace nexo::editor::utils {
	std::string removeIconPrefix(const std::string &str)
	{
        size_t pos = str.find(" ");
        if (pos != std::string::npos)
        {
            return str.substr(pos + 1);
        }
        return str;
	}
}
