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

#include <format>

namespace nexo {
    const char *Exception::what() const noexcept
    {
        return m_formattedMessage.c_str();
    }

    const std::string& Exception::getMessage() const noexcept
    {
        return m_unformattedMessage;
    }

    const std::string& Exception::getFormattedMessage() const noexcept
    {
        return m_formattedMessage;
    }

    const char* Exception::getFile() const noexcept
    {
        return m_location.file_name();
    }

    unsigned int Exception::getLine() const noexcept
    {
        return m_location.line();
    }

    const char* Exception::getFunction() const noexcept
    {
        return m_location.function_name();
    }

    const std::source_location& Exception::getSourceLocation() const noexcept
    {
        return m_location;
    }
}
