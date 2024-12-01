//// Exception.cpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        12/11/2024
//  Description: Source for the nexo exception base class
//
///////////////////////////////////////////////////////////////////////////////

#include "Exception.hpp"

#include <sstream>

namespace nexo {
    const char * Exception::what() const noexcept
    {
        formattedMessage = formatMessage();
        return formattedMessage.c_str();
    }

    std::string Exception::formatMessage() const
    {
        std::ostringstream oss;
        oss << "Exception occurred in " << file << ":" << line << " - " << message;
        return oss.str();
    }
}