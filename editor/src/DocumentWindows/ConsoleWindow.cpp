//// ConsoleWindow.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Source file for the console window class
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>
#include <format>
#include <iostream>
#include <fstream>
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <string>
#include "ConsoleWindow.hpp"
#include "Editor.hpp"
#include "Logger.hpp"
#include "Path.hpp"
#include "tinyfiledialogs.h"
#include "utils/FileSystem.hpp"

#include <IconsFontAwesome.h>

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
    static inline std::string verbosityToString(const loguru::Verbosity level)
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
            case LogLevel::ERROR: return loguru::Verbosity_ERROR;
            case LogLevel::WARN: return loguru::Verbosity_WARNING;
            case LogLevel::INFO: return loguru::Verbosity_INFO;
            case LogLevel::USER: return loguru::Verbosity_1;
            case LogLevel::DEBUG: return loguru::Verbosity_2;
            case LogLevel::DEV: return loguru::Verbosity_3;
            default: return loguru::Verbosity_INVALID;
        }
        return loguru::Verbosity_INVALID;
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
    static constexpr ImVec4 getVerbosityColor(loguru::Verbosity level)
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

    static const std::string generateLogFilePath()
    {
        auto now = std::chrono::system_clock::now();
        std::time_t now_time = std::chrono::system_clock::to_time_t(now);

        std::tm localTime;
#if defined(_MSC_VER) || defined(__MINGW32__)
    localtime_s(&localTime, &now_time);
#else
    localtime_r(&now_time, &localTime);
#endif

        std::ostringstream oss;
        // Format: YYYY-MM-DD-HHMMSS, e.g., 2025-04-15-123045
        oss << "../logs/NEXO-" << std::put_time(&localTime, "%Y-%m-%d-%H%M%S") << ".log";
        return oss.str();
    }


    void ConsoleWindow::loguruCallback([[maybe_unused]] void *userData,
                                const loguru::Message &message)
    {
        const auto console = static_cast<ConsoleWindow *>(userData);
        console->addLog({
            .verbosity = message.verbosity,
            .message = message.message,
            .prefix = message.prefix
        });
    }


    ConsoleWindow::ConsoleWindow(const std::string &windowName, WindowRegistry &registry) : ADocumentWindow(windowName, registry)
    {
		loguru::add_callback(LOGURU_CALLBACK_NAME, &ConsoleWindow::loguruCallback,
		                         this, loguru::Verbosity_MAX);

		auto engineLogCallback = [](const LogLevel level, const std::string &message) {
		    const auto loguruLevel = nexoLevelToLoguruLevel(level);
		    VLOG_F(loguruLevel, "%s", message.c_str());
		};
		Logger::setCallback(engineLogCallback);
		m_logFilePath = Path::resolvePathRelativeToExe(generateLogFilePath()).string();
		m_logs.reserve(m_maxLogCapacity);
		m_bufferLogsToExport.reserve(m_maxBufferLogToExportCapacity);
    };

    void ConsoleWindow::setup()
    {
    	//All the setup is made in the constructor because the rest of the editor needs the log setup before setting up the windows
    }

    void ConsoleWindow::shutdown()
    {
    	clearLog();
    }

    void ConsoleWindow::addLog(const LogMessage &message)
    {
        if (m_logs.size() >= m_maxLogCapacity) {
            m_bufferLogsToExport.push_back(message);
            m_logs.erase(m_logs.begin());
        }

        if (m_bufferLogsToExport.size() > m_maxBufferLogToExportCapacity) {
            exportLogsBuffered();
            m_bufferLogsToExport.clear();
        }

        m_logs.push_back(message);
    }

    void ConsoleWindow::clearLog()
    {
        exportLogsBuffered();
        if (m_exportLog) {
            std::ofstream logFile(m_logFilePath, std::ios::app);
            for (const auto& log : m_logs) {
                logFile << verbosityToString(log.verbosity) << " "
                        << log.message << std::endl;
            }
        }

    	m_logs.clear();
    }

    template<typename... Args>
    void ConsoleWindow::addLog(const char *fmt, Args &&... args)
    {
        try {
            std::string formattedMessage = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
            m_logs.emplace_back(LogMessage{nexoLevelToLoguruLevel(LogLevel::USER), formattedMessage});
        } catch (const std::format_error &e) {
            m_logs.emplace_back(LogMessage{nexoLevelToLoguruLevel(LogLevel::ERROR), std::format("[Error formatting log message]: {}", e.what())});
        }

        scrollToBottom = true;
    }

    void ConsoleWindow::executeCommand(const char *command_line)
    {
        commands.emplace_back(command_line);
        addLog("{}\n", command_line);
    }

    void ConsoleWindow::calcLogPadding()
    {
        m_logPadding = 0.0f;
        for (const auto &[verbosity, message, prefix]: m_logs)
        {
            if (!selectedVerbosityLevels.contains(verbosity))
                continue;

            const std::string tag = verbosityToString(verbosity);
            const ImVec2 textSize = ImGui::CalcTextSize(tag.c_str());
            if (textSize.x > m_logPadding)
            {
                m_logPadding = textSize.x;
            }
        }
        m_logPadding += ImGui::GetStyle().ItemSpacing.x;
    }


    void ConsoleWindow::displayLog(loguru::Verbosity verbosity, const std::string &msg) const
    {
        ImVec4 color = getVerbosityColor(verbosity);
        ImGui::PushStyleColor(ImGuiCol_Text, color);

        const std::string tag = verbosityToString(verbosity);
        ImGui::TextUnformatted(tag.c_str());
        ImGui::PopStyleColor();

        ImGui::SameLine();
        ImGui::SetCursorPosX(m_logPadding);

        ImGui::PushTextWrapPos(ImGui::GetContentRegionAvail().x);
        ImGui::TextWrapped("%s", msg.c_str());
        ImGui::PopTextWrapPos();
    }

    void ConsoleWindow::exportLogsBuffered()
    {
        if (!m_exportLog)
            return;
        std::ofstream logFile(m_logFilePath, std::ios::app);
        for (const auto& log : m_bufferLogsToExport) {
            logFile << verbosityToString(log.verbosity) << " "
                    << log.message << std::endl;
        }
    }

    void ConsoleWindow::showVerbositySettingsPopup()
    {
        ImGui::Text("Select Verbosity Levels");
        ImGui::Separator();

        const struct {
            loguru::Verbosity level;
            const char *name;
        } levels[] = {
            {loguru::Verbosity_FATAL, "FATAL"},
            {loguru::Verbosity_ERROR, "ERROR"},
            {loguru::Verbosity_WARNING, "WARNING"},
            {loguru::Verbosity_INFO, "INFO"},
            {loguru::Verbosity_1, "USER"},
            {loguru::Verbosity_2, "DEBUG"},
            {loguru::Verbosity_3, "DEV"}
        };

        for (const auto &[level, name]: levels)
        {
            bool selected = (selectedVerbosityLevels.contains(level));
            if (ImGui::Checkbox(name, &selected))
            {
                if (selected)
                {
                    selectedVerbosityLevels.insert(level);
                    calcLogPadding();
                } else
                {
                    selectedVerbosityLevels.erase(level);
                    calcLogPadding();
                }
            }
        }

        ImGui::Separator();
        ImGui::Checkbox("File logging", &m_exportLog);
        if (ImGui::Button("Open log folder"))
            utils::openFolder(Path::resolvePathRelativeToExe("../logs"));

        ImGui::EndPopup();
    }

    void ConsoleWindow::show()
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin(ICON_FA_FILE_TEXT " Console" "###" NEXO_WND_USTRID_CONSOLE, &m_opened, ImGuiWindowFlags_NoCollapse);
        firstDockSetup(NEXO_WND_USTRID_CONSOLE);

        const float footerHeight = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
        ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footerHeight), false, ImGuiWindowFlags_HorizontalScrollbar);

        if (m_logPadding == 0.0f)
            calcLogPadding();

        auto id = 0;
        for (const auto &[verbosity, message, prefix]: m_logs)
        {
            if (!selectedVerbosityLevels.contains(verbosity))
                continue;

            ImGui::PushID(id++);
            displayLog(verbosity, message);
            ImGui::PopID();
        }

        if (scrollToBottom)
            ImGui::SetScrollHereY(1.0f);
        scrollToBottom = false;

        ImGui::EndChild();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 60.0f);

        if (ImGui::InputText("Input", inputBuf, IM_ARRAYSIZE(inputBuf), ImGuiInputTextFlags_EnterReturnsTrue))
        {
            executeCommand(inputBuf);
            std::memset(inputBuf, '\0', sizeof(inputBuf));
        }

        ImGui::SameLine();
        if (ImGui::Button("..."))
            ImGui::OpenPopup("VerbositySettings");

        if (ImGui::BeginPopup("VerbositySettings"))
            showVerbositySettingsPopup();

        ImGui::End();
    }

    void ConsoleWindow::update()
    {
    	//No need to update anything
    }
}
