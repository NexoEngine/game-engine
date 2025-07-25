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
#include <unordered_set>
#include <mutex>

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

    enum class LogLevel : uint32_t {
        FATAL,
        ERR,
        WARN,
        INFO,
        DEBUG,
        DEV,
        USER
    };

    inline std::string toString(const LogLevel level)
    {
        switch (level)
        {
            case LogLevel::FATAL: return "FATAL";
            case LogLevel::ERR: return "ERROR";
            case LogLevel::WARN: return "WARN";
            case LogLevel::INFO: return "INFO";
            case LogLevel::USER: return "USER";
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

    inline void defaultCallback(const LogLevel level, const std::source_location& loc, const std::string &message)
    {
        std::ostream& outputStream = level == LogLevel::FATAL || level == LogLevel::ERR
            ? std::cerr
            : std::cout;

        outputStream << "[" << toString(level) << "] " << getFileName(loc.file_name()) << ":" << loc.line() << " - " << message << std::endl;
    }

    /**
     * @brief Registry to track which log messages have been emitted
     *
     * This class is used to ensure certain messages are only logged once
     * until they are explicitly reset.
     */
    class OnceRegistry {
    public:
        /**
         * @brief Get the singleton instance of the registry
         *
         * @return OnceRegistry& Reference to the registry
         */
        static OnceRegistry& instance() {
            static OnceRegistry registry;
            return registry;
        }

        /**
         * @brief Check if a message has been logged and mark it as logged
         *
         * @param key The unique identifier for the message
         * @return true If this is the first time seeing this message
         * @return false If this message has been logged before
         */
        bool shouldLog(const std::string& key) {
            std::scoped_lock lock(m_mutex);
            return m_loggedKeys.insert(key).second;
        }

        /**
         * @brief Reset a specific message so it can be logged again
         *
         * @param key The unique identifier for the message to reset
         */
        void reset(const std::string& key) {
            std::scoped_lock lock(m_mutex);
            m_loggedKeys.erase(key);
        }

        /**
         * @brief Reset all messages so they can be logged again
         */
        void resetAll() {
            std::scoped_lock lock(m_mutex);
            m_loggedKeys.clear();
        }

    private:
        OnceRegistry() = default;

        std::unordered_set<std::string> m_loggedKeys;
        std::mutex m_mutex;
    };

    class Logger {
        public:
            static void setCallback(std::function<void(LogLevel, const std::source_location& loc, const std::string&)> callback)
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

                logCallback(level, loc, message);
            }

            /**
             * @brief Generate a key incorporating format string and parameter values
             *
             * @tparam Args Variadic template for format arguments
             * @param fmt Format string
             * @param args Format arguments
             * @return std::string Key incorporating the parameters
             */
            template<typename... Args>
            static std::string generateKey(const std::string_view fmt, const std::string& location, Args&&... args)
            {
                std::stringstream ss;
                ss << fmt << "@" << location << "|";

                // Add parameter values to the key
                ((ss << toFormatFriendly(args) << "|"), ...);

                return ss.str();
            }

            /**
             * @brief Log a message only once until reset
             *
             * This method logs a message only the first time it is called with a given key.
             * The key incorporates both the format string and the parameter values.
             *
             * @tparam Args Variadic template for format arguments
             * @param level Log level
             * @param loc Source location of the log call
             * @param fmt Format string
             * @param key Key including format string and parameter values
             * @param args Format arguments
             */
            template<typename... Args>
            static void logOnce(const LogLevel level, const std::source_location loc,
                                const std::string_view fmt, const std::string& key, Args &&... args)
            {
                if (OnceRegistry::instance().shouldLog(key)) {
                    logWithFormat(level, loc, fmt, std::forward<Args>(args)...);
                }
            }

            /**
             * @brief Reset a specific log message so it can be logged again with logOnce
             *
             * @param key The unique identifier for the log message to reset
             */
            static void resetOnce(const std::string& key) {
                OnceRegistry::instance().reset(key);
            }

            /**
             * @brief Reset all log messages so they can be logged again with logOnce
             */
            static void resetAllOnce() {
                OnceRegistry::instance().resetAll();
            }

        private:
            static inline std::function<void(LogLevel, const std::source_location& loc, const std::string &)> logCallback = defaultCallback;
    };

}

#define LOG(level, fmt, ...) \
    nexo::Logger::logWithFormat(level, std::source_location::current(), fmt, ##__VA_ARGS__)

#define LOG_EXCEPTION(exception) \
    LOG(NEXO_ERROR, "{}:{} - Exception: {}", exception.getFile(), exception.getLine(), exception.getMessage())

/**
 * @brief Generate a unique key for a log message incorporating format and parameters
 *
 * This creates a key that includes both the format string and the parameter values,
 * allowing specific message instances to be reset later.
 */
#define NEXO_LOG_ONCE_KEY(fmt, ...) \
    nexo::Logger::generateKey(fmt, std::string(__FILE__) + ":" + std::to_string(__LINE__), ##__VA_ARGS__)

/**
 * @brief Log a message only once until it's reset
 *
 * This ensures the message is only logged the first time this line is executed
 * with these specific parameters. Subsequent calls with the same parameters
 * will be ignored until the message is reset.
 */
#define LOG_ONCE(level, fmt, ...) \
    nexo::Logger::logOnce(level, std::source_location::current(), fmt, \
        NEXO_LOG_ONCE_KEY(fmt, ##__VA_ARGS__), ##__VA_ARGS__)

#define NEXO_FATAL nexo::LogLevel::FATAL
#define NEXO_ERROR nexo::LogLevel::ERR
#define NEXO_WARN nexo::LogLevel::WARN
#define NEXO_INFO nexo::LogLevel::INFO
#define NEXO_DEBUG nexo::LogLevel::DEBUG
#define NEXO_DEV nexo::LogLevel::DEV
