//// ConsoleWindow.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Source file for the console window class
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>
#include <format>
#include "ConsoleWindow.hpp"

namespace nexo::editor {
    /**
     * @brief Converts a loguru verbosity level to its corresponding string label.
     *
     * This function maps a given loguru::Verbosity level to a predefined string representation.
     * Recognized levels include fatal, error, warning, info, invalid, debug, and development.
     * If an unrecognized level is provided, "[UNKNOWN]" is returned.
     *
     * @param level The loguru verbosity level to convert.
     * @return std::string The string representation of the provided verbosity level.
     */
    static constexpr std::string verbosityToString(const loguru::Verbosity level)
    {
        switch (level)
        {
            case loguru::Verbosity_FATAL: return "[FATAL]";
            case loguru::Verbosity_ERROR: return "[ERROR]";
            case loguru::Verbosity_WARNING: return "[WARNING]";
            case loguru::Verbosity_INFO: return "[INFO]";
            case loguru::Verbosity_INVALID: return "[INVALID]";
            case loguru::Verbosity_1: return "[DEBUG]";
            case loguru::Verbosity_2: return "[DEV]";
            default: return "[UNKNOWN]";
        }
    }

    /**
     * @brief Converts a custom log level to its corresponding loguru verbosity level.
     *
     * This function maps a LogLevel value to the appropriate loguru::Verbosity constant.
     * If the provided log level does not match any defined cases, it returns loguru::Verbosity_INVALID.
     *
     * @param level Custom log level to be converted.
     * @return loguru::Verbosity The corresponding loguru verbosity level, or loguru::Verbosity_INVALID if the log level is unrecognized.
     */
    loguru::Verbosity nexoLevelToLoguruLevel(const LogLevel level)
    {
        switch (level)
        {
            case LogLevel::FATAL: return loguru::Verbosity_FATAL;
            case LogLevel::ERROR: return loguru::Verbosity_ERROR;
            case LogLevel::WARN: return loguru::Verbosity_WARNING;
            case LogLevel::INFO: return loguru::Verbosity_INFO;
            case LogLevel::DEBUG: return loguru::Verbosity_1;
            case LogLevel::DEV: return loguru::Verbosity_2;
            default: return loguru::Verbosity_INVALID;
        }
        return loguru::Verbosity_INVALID;
    }

    /**
     * @brief Returns the color corresponding to a given logging verbosity level.
     *
     * Maps a loguru verbosity level to a specific ImVec4 color used to visually distinguish log messages. 
     * The color scheme includes:
     * - Red for FATAL and ERROR levels.
     * - Yellow for WARNING.
     * - Blue for INFO.
     * - Pink for Verbosity_1.
     * - Purple for Verbosity_2.
     * - White for any other levels.
     *
     * @param level The loguru verbosity level.
     * @return ImVec4 The color associated with the verbosity level.
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
            case loguru::Verbosity_1: color = ImVec4(0.898f, 0.0f, 1.0f, 1.0f); // Debug
                break; // Pink
            case loguru::Verbosity_2: color = ImVec4(0.388f, 0.055f, 0.851f, 1.0f); // Debug
                break; // Purple
            default: color = ImVec4(1, 1, 1, 1); // White
        }
        return color;
    }

    /**
     * @brief Callback function for Loguru to add a log entry to the console.
     *
     * This function is called by Loguru when a log message is generated. It casts the provided user data to a
     * ConsoleWindow pointer and appends a new log entry using the information from the given loguru::Message.
     *
     * @param userData Pointer to the ConsoleWindow instance receiving the log message.
     * @param message The loguru message containing the log details such as verbosity, message content, and prefix.
     */
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

    /**
     * @brief Constructs a ConsoleWindow and registers logging callbacks.
     *
     * This constructor initializes the ConsoleWindow by registering a loguru callback that routes
     * log messages to the console via the static `loguruCallback` method. It also establishes a
     * lambda callback for engine logs, which maps custom log levels to loguru verbosity levels
     * before logging the messages.
     *
     * @param registry A reference to the window registry used to manage document windows.
     */
    ConsoleWindow::ConsoleWindow(WindowRegistry &registry) : ADocumentWindow(registry)
    {
		loguru::add_callback(LOGURU_CALLBACK_NAME, &ConsoleWindow::loguruCallback,
		                         this, loguru::Verbosity_MAX);

		auto engineLogCallback = [](const LogLevel level, const std::string &message) {
		    const auto loguruLevel = nexoLevelToLoguruLevel(level);
		    VLOG_F(loguruLevel, "%s", message.c_str());
		};
		Logger::setCallback(engineLogCallback);
    };

    /**
     * @brief Placeholder for console window setup.
     *
     * This method is intentionally left empty because all necessary initialization,
     * particularly for logging, is conducted in the constructor to ensure that the logging
     * system is ready before any window setup occurs.
     */
    void ConsoleWindow::setup()
    {
    	//All the setup is made in the constructor because the rest of the editor needs the log setup before setting up the windows
    }

    /**
     * @brief Shuts down the console window by clearing its log entries.
     *
     * This method clears all stored log messages, ensuring that the log 
     * is empty when the console window is closed.
     */
    void ConsoleWindow::shutdown()
    {
    	clearLog();
    }

    /**
     * @brief Appends a log message to the console's internal log storage.
     *
     * This function adds the provided log message to the log list maintained by the console window.
     *
     * @param message The log message to be stored.
     */
    void ConsoleWindow::addLog(const LogMessage &message)
    {
        m_logs.push_back(message);
    }

    /**
     * @brief Clears all stored log messages.
     *
     * Removes all entries from the internal log container and associated display items,
     * resetting the console's log state.
     */
    void ConsoleWindow::clearLog()
    {
    	m_logs.clear();
        items.clear();
    }

    template<typename... Args>
    void ConsoleWindow::addLog(const char *fmt, Args &&... args)
    {
        try
        {
            std::string formattedMessage = std::vformat(fmt, std::make_format_args(std::forward<Args>(args)...));
            items.emplace_back(formattedMessage);
        } catch (const std::format_error &e)
        {
            items.emplace_back(std::format("[Error formatting log message]: {}", e.what()));
        }

        scrollToBottom = true;
    }

    void ConsoleWindow::executeCommand(const char *command_line)
    {
        commands.emplace_back(command_line);
        addLog("# {}\n", command_line);
    }

    /**
     * @brief Calculates and updates the left padding for log entries.
     *
     * Iterates through the internal log storage, determining the maximum width of the verbosity tag
     * for logs that match the selected verbosity levels. The final padding is computed by adding the
     * default horizontal spacing from ImGui's style to this maximum width, ensuring proper alignment
     * of log messages in the console window.
     */
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
            {loguru::Verbosity_1, "DEBUG"},
            {loguru::Verbosity_2, "DEV"},
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

        ImGui::EndPopup();
    }

    /**
     * @brief Renders the console window UI with log output, command input, and verbosity settings.
     *
     * This method displays the console window using ImGui, handling:
     * - Setting the initial window size and docking configuration on first open.
     * - Displaying log entries in a scrollable region, filtering them based on selected verbosity levels.
     * - Automatically scrolling to the bottom when new logs are added.
     * - Providing an input field for command entry and executing commands upon submission.
     * - Opening a popup for adjusting verbosity settings.
     */
    void ConsoleWindow::show()
    {
        ImGui::SetNextWindowSize(ImVec2(520, 600), ImGuiCond_FirstUseEver);
        ImGui::Begin("Console", &m_opened, ImGuiWindowFlags_NoCollapse);
       	if (m_firstOpened)
        	firstDockSetup("Console");

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


    /**
     * @brief Placeholder update method.
     *
     * This method is intentionally left empty as no periodic updates are required for the ConsoleWindow.
     */
    void ConsoleWindow::update()
    {
    	//No need to update anything
    }
}
