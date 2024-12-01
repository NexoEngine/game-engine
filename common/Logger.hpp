//// Logger.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the logger utils functions
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <functional>
#include <string>
#include <iostream>
#include <sstream>
#include <format>
#include <string_view>
#include <source_location>

namespace nexo {

    template<typename T>
    auto toFormatFriendly(const T &value)
    {
        if constexpr (std::is_convertible_v<T, std::string_view>)
            return std::string_view(value);
        else
        {
            std::stringstream ss;
            ss << value;
            return ss.str();
        }
    }

    enum class LogLevel {
        FATAL,
        ERROR,
        WARN,
        INFO,
        DEBUG,
        DEV
    };

    inline std::string toString(const LogLevel level)
    {
        switch (level)
        {
            case LogLevel::FATAL: return "FATAL";
            case LogLevel::ERROR: return "ERROR";
            case LogLevel::WARN: return "WARN";
            case LogLevel::INFO: return "INFO";
            case LogLevel::DEBUG: return "DEBUG";
            case LogLevel::DEV: return "DEV";
        }
        return "UNKNOWN";
    }

    inline std::string getFileName(const char *file)
    {
        std::string path(file);
        if (const auto lastSlash = path.find_last_of("/\\"); lastSlash != std::string::npos)
            return path.substr(lastSlash + 1);
        return path;
    }

    inline void defaultCallback(const LogLevel level, const std::string &message)
    {
        if (level == LogLevel::FATAL || level == LogLevel::ERROR)
            std::cerr << "[" << toString(level) << "] " << message << std::endl;
        else
            std::cout << "[" << toString(level) << "] " << message << std::endl;
    }

    class Logger {
        public:
            static void setCallback(std::function<void(LogLevel, const std::string &)> callback)
            {
                logCallback = std::move(callback);
            }

            template<typename... Args>
            static void logWithFormat(const LogLevel level, const std::source_location loc, const std::string_view fmt, Args &&... args)
            {
                auto transformed = std::tuple{toFormatFriendly(std::forward<Args>(args))...};

                const std::string message = std::apply(
                    [&](auto &&... transformedArgs) {
                        return std::vformat(fmt, std::make_format_args(transformedArgs...));
                    },
                    transformed);

                if (level == LogLevel::INFO)
                    logString(level, message);
                else
                {
                    std::stringstream ss;
                    ss << getFileName(loc.file_name()) << ":" << loc.line() << " - " << message;
                    logString(level, ss.str());
                }
            }
        private:
            static void logString(const LogLevel level, const std::string &message)
            {
                if (logCallback)
                    logCallback(level, message);
                else
                    defaultCallback(level, message);
            }
            static inline std::function<void(LogLevel, const std::string &)> logCallback = nullptr;
    };

}

#define LOG(level, fmt, ...) \
    nexo::Logger::logWithFormat(level, std::source_location::current(), fmt, ##__VA_ARGS__)

#define LOG_EXCEPTION(exception) \
    LOG(NEXO_ERROR, "{}:{} - Exception: {}", exception.getFile(), exception.getLine(), exception.getMessage())

#define NEXO_FATAL nexo::LogLevel::FATAL
#define NEXO_ERROR nexo::LogLevel::ERROR
#define NEXO_WARN nexo::LogLevel::WARN
#define NEXO_INFO nexo::LogLevel::INFO
#define NEXO_DEBUG nexo::LogLevel::DEBUG
#define NEXO_DEV nexo::LogLevel::DEV
