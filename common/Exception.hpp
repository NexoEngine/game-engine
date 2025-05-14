//// Exception.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        12/11/2024
//  Description: Common header file for the nexo base exception class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <format>
#include <string>
#include <source_location>
#include <exception>

namespace nexo {
    class Exception : public std::exception {
        public:
            explicit Exception(const std::string& message, const std::source_location& loc)
                :
                m_unformattedMessage(message),
                m_formattedMessage(std::format("Exception occurred in {} : {} - {}", loc.file_name(), loc.line(), message)),
                m_location(loc)
            {
            }

            ~Exception() override = default;

            [[nodiscard]] const char *what() const noexcept override;

            [[nodiscard]] const std::string& getMessage() const noexcept;
            [[nodiscard]] const std::string& getFormattedMessage() const noexcept;

            [[nodiscard]] const char *getFile() const noexcept;
            [[nodiscard]] unsigned int getLine() const noexcept;
            [[nodiscard]] const char *getFunction() const noexcept;
            [[nodiscard]] const std::source_location& getSourceLocation() const noexcept;

        private:
            std::string m_unformattedMessage;
            std::string m_formattedMessage;
            std::source_location m_location;
    };
}

#define THROW_EXCEPTION(ExceptionType, ...) \
throw ExceptionType(__VA_OPT__(__VA_ARGS__,) std::source_location::current())

