//// Utils.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the utils methods
//
///////////////////////////////////////////////////////////////////////////////

#include "ConsoleWindow.hpp"
#include <time.h>

namespace nexo::editor {
    /**
     * @brief Converts a loguru verbosity level to its corresponding string label.
     *
     * This function maps a given loguru verbosity level to a predefined string representation,
     * such as "[FATAL]", "[ERROR]", "[WARNING]", "[INFO]", "[INVALID]", "[DEBUG]", or "[DEV]".
     * If the provided level does not match any known values, it returns "[UNKNOWN]".
     *
     * @param level The loguru verbosity level to convert.
     * @return std::string The string label corresponding to the provided verbosity level.
     */
    std::string verbosityToString(const loguru::Verbosity level)
    {
        switch (level)
        {
            case loguru::Verbosity_FATAL: return "[FATAL]";
            case loguru::Verbosity_ERROR: return "[ERROR]";
            case loguru::Verbosity_WARNING: return "[WARNING]";
            case loguru::Verbosity_INFO: return "[INFO]";
            case loguru::Verbosity_INVALID: return "[INVALID]";
            case loguru::Verbosity_1: return "[USER]";
            case loguru::Verbosity_2: return "[DEBUG]";
            case loguru::Verbosity_3: return "[DEV]";
            default: return "[UNKNOWN]";
        }
    }

    /**
     * @brief Converts a custom LogLevel to its corresponding loguru::Verbosity level.
     *
     * Maps each supported LogLevel to a specific loguru verbosity constant. If the provided
     * level does not match any known value, the function returns loguru::Verbosity_INVALID.
     *
     * @param level The custom logging level to convert.
     * @return The equivalent loguru verbosity level.
     */
    loguru::Verbosity nexoLevelToLoguruLevel(const LogLevel level)
    {
        switch (level)
        {
            case LogLevel::FATAL: return loguru::Verbosity_FATAL;
            case LogLevel::ERR: return loguru::Verbosity_ERROR;
            case LogLevel::WARN: return loguru::Verbosity_WARNING;
            case LogLevel::INFO: return loguru::Verbosity_INFO;
            case LogLevel::USER: return loguru::Verbosity_1;
            case LogLevel::DEBUG: return loguru::Verbosity_2;
            case LogLevel::DEV: return loguru::Verbosity_3;
            default: return loguru::Verbosity_INVALID;
        }
    }

    /**
     * @brief Returns the color corresponding to a log verbosity level.
     *
     * Maps the given loguru::Verbosity level to a specific ImVec4 color used for rendering log messages in the console.
     * - Fatal and error messages are shown in red.
     * - Warnings use yellow.
     * - Informational messages appear in blue.
     * - Debug levels display distinct pink and purple hues.
     * The default color is white for any unrecognized verbosity levels.
     *
     * @param level The verbosity level for which the corresponding color is computed.
     * @return ImVec4 The color associated with the specified verbosity level.
     */
    const ImVec4 getVerbosityColor(const loguru::Verbosity level)
    {
        ImVec4 color;

        switch (level)
        {
            case loguru::Verbosity_FATAL: // Red
            case loguru::Verbosity_ERROR: color = ImVec4(1.0f, 0.0f, 0.0f, 1.0f);
                break; // Red
            case loguru::Verbosity_WARNING: color = ImVec4(1.0f, 1.0f, 0.0f, 1.0f);
                break; // Yellow
            case loguru::Verbosity_INFO: color = ImVec4(0.0f, 0.5f, 1.0f, 1.0f);
                break; // Blue
            case loguru::Verbosity_1: color = ImVec4(0.09f, 0.67f, 0.14f, 1.0f); // User
                break; // Green
            case loguru::Verbosity_2: color = ImVec4(0.898f, 0.0f, 1.0f, 1.0f); // Debug
                break; // Pink
            case loguru::Verbosity_3: color = ImVec4(0.388f, 0.055f, 0.851f, 1.0f); // Dev
                break; // Purple
            default: color = ImVec4(1, 1, 1, 1); // White
        }
        return color;
    }

    std::string generateLogFilePath()
    {
        using namespace std::chrono;

        // Truncate to seconds precision
        auto now = floor<seconds>(system_clock::now());
        zoned_time local_zoned{ current_zone(), now };

        std::string ts = std::format("{:%Y%m%d_%H%M%S}", local_zoned);
        return std::format("../logs/NEXO-{}.log", ts);
    }
}
