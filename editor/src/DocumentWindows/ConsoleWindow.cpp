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
     * This function maps a given loguru verbosity level to a predefined string representation,
     * such as "[FATAL]", "[ERROR]", "[WARNING]", "[INFO]", "[INVALID]", "[DEBUG]", or "[DEV]".
     * If the provided level does not match any known values, it returns "[UNKNOWN]".
     *
     * @param level The loguru verbosity level to convert.
     * @return std::string The string label corresponding to the provided verbosity level.
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
            case LogLevel::DEBUG: return loguru::Verbosity_1;
            case LogLevel::DEV: return loguru::Verbosity_2;
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
            case loguru::Verbosity_1: color = ImVec4(0.898f, 0.0f, 1.0f, 1.0f); // Debug
                break; // Pink
            case loguru::Verbosity_2: color = ImVec4(0.388f, 0.055f, 0.851f, 1.0f); // Debug
                break; // Purple
            default: color = ImVec4(1, 1, 1, 1); // White
        }
        return color;
    }

    /**
     * @brief Processes a loguru message and adds it to the console log.
     *
     * Converts a loguru message to the internal log format and appends it to the ConsoleWindow's log list.
     * The userData pointer is cast to a ConsoleWindow instance, which is then used to record the message details,
     * including verbosity, message content, and prefix.
     *
     * @param userData Pointer to the ConsoleWindow instance.
     * @param message The loguru message carrying the log details.
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
     * @brief Constructs and initializes a ConsoleWindow.
     *
     * This constructor sets up the console's logging functionality by registering a loguru callback via
     * loguru::add_callback to route log messages to the console (using the static loguruCallback) and by
     * establishing an engine log callback that maps custom LogLevel messages to loguru verbosity levels
     * using nexoLevelToLoguruLevel before logging them with VLOG_F.
     *
     * @param registry The window registry used to register this console window.
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
     * @brief Placeholder for setup operations.
     *
     * This method is intentionally left empty since all necessary setup, including logging initialization,
     * is performed in the constructor. It remains available for potential future setup procedures.
     */
    void ConsoleWindow::setup()
    {
    	//All the setup is made in the constructor because the rest of the editor needs the log setup before setting up the windows
    }

    /**
     * @brief Clears all stored log entries during shutdown.
     *
     * This method resets the console's log by invoking clearLog(), ensuring that all previous
     * log entries are removed as part of the shutdown process.
     */
    void ConsoleWindow::shutdown()
    {
    	clearLog();
    }

    /**
     * @brief Appends a log message to the console's log collection.
     *
     * This method adds the provided log message to the internal container, ensuring it is available
     * for display in the console window.
     *
     * @param message The log message to append.
     */
    void ConsoleWindow::addLog(const LogMessage &message)
    {
        m_logs.push_back(message);
    }

    /**
     * @brief Clears all log entries and display items.
     *
     * Removes all log messages from the internal storage and resets the list of display items.
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
     * @brief Updates the horizontal padding for log entries.
     *
     * Iterates over the stored log messages to compute the maximum width of the verbosity tag text for
     * messages that are currently visible (filtered by selected verbosity levels). The computed maximum
     * width is then increased by the spacing defined in the ImGui style to ensure proper alignment in the UI.
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
     * @brief Renders the console window interface.
     *
     * This method initializes and displays the console window using ImGui. It sets a predefined window size and
     * creates a scrolling region to display log messages filtered by selected verbosity levels. When the console is
     * opened for the first time, it performs an initial docking setup. The function also adjusts log padding for proper
     * alignment and automatically scrolls to the bottom if new messages have been added.
     *
     * An input field is provided for entering commands, which are executed upon pressing Enter, with the input buffer
     * cleared afterward. Additionally, a popup for adjusting verbosity settings is available, accessible via a button.
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
     * @brief Placeholder update method for the console window.
     *
     * This method is intentionally left empty as no update operations are required.
     */
    void ConsoleWindow::update()
    {
    	//No need to update anything
    }
}
