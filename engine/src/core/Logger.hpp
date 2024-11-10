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
//  Description: Header file for the logger utilities
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <functional>
#include <string>
#include <iostream>
#include <utility>
#include <sstream>

namespace nexo::core {

    enum class LogLevel {
        FATAL,
        ERROR,
        WARN,
        INFO,
        DEBUG
    };

    class Logger {
        public:
            static void setCallback(std::function<void(LogLevel, const std::string &)> callback)
            {
                logCallback = std::move(callback);
            }

            template<typename T>
            static void logFatal(const T &message)
            {
                log(LogLevel::FATAL, message);
            }

            template<typename T>
            static void logError(const T &message)
            {
                log(LogLevel::ERROR, message);
            }

            template<typename T>
            static void logWarn(const T &message)
            {
                log(LogLevel::WARN, message);
            }

            template<typename T>
            static void logInfo(const T &message)
            {
                log(LogLevel::INFO, message);
            }

            template<typename T>
            static void logDebug(const T &message)
            {
                log(LogLevel::DEBUG, message);
            }

            template<typename T>
            static void log(LogLevel level, const T &message)
            {
                std::stringstream ss;
                ss << message;
                logString(level, ss.str());
            }

        private:
            static void defaultCallback(const LogLevel level, const std::string &message)
            {
                if (level == LogLevel::FATAL || level == LogLevel::ERROR)
                    std::cerr << "[" << toString(level) << "] " << message << std::endl;
                else
                    std::cout << "[" << toString(level) << "] " << message << std::endl;
            }

            static void logString(LogLevel level, const std::string &message)
            {
                if (logCallback)
                {
                    logCallback(level, message);
                } else
                {
                    defaultCallback(level, message);
                }
            }

            static std::string toString(const LogLevel level)
            {
                switch (level)
                {
                    case LogLevel::FATAL: return "FATAL";
                    case LogLevel::ERROR: return "ERROR";
                    case LogLevel::WARN: return "WARN";
                    case LogLevel::INFO: return "INFO";
                    case LogLevel::DEBUG: return "DEBUG";
                }
                return "UNKNOWN";
            }

            // Static member to store the callback function
            static inline std::function<void(LogLevel, const std::string &)> logCallback = nullptr;
    };

} // namespace nexo::core

// Macro for logging
#define LOG(level, message) \
    do { \
        switch (level) { \
            case nexo::core::LogLevel::FATAL: nexo::core::Logger::logFatal(message); break; \
            case nexo::core::LogLevel::ERROR: nexo::core::Logger::logError(message); break; \
            case nexo::core::LogLevel::WARN: nexo::core::Logger::logWarn(message); break; \
            case nexo::core::LogLevel::INFO: nexo::core::Logger::logInfo(message); break; \
            case nexo::core::LogLevel::DEBUG: nexo::core::Logger::logDebug(message); break; \
            default: break; \
        } \
    } while (0)


#define NEXO_FATAL nexo::core::LogLevel::FATAL
#define NEXO_ERROR nexo::core::LogLevel::ERROR
#define NEXO_WARN nexo::core::LogLevel::WARN
#define NEXO_INFO nexo::core::LogLevel::INFO
#define NEXO_DEBUG nexo::core::LogLevel::DEBUG
