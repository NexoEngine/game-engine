//// ConsoleWindow.hpp ////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Header file for the console window class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ADocumentWindow.hpp"
#include "Editor.hpp"
#include <loguru.hpp>

namespace nexo::editor {

    std::string verbosityToString(loguru::Verbosity level);
    loguru::Verbosity nexoLevelToLoguruLevel(LogLevel level);
    ImVec4 getVerbosityColor(loguru::Verbosity level);
    std::string generateLogFilePath();

	constexpr auto LOGURU_CALLBACK_NAME = "GEE";

	/**
     * @brief Structure representing a formatted log message.
     *
     * Contains all necessary information for displaying a log message in the console,
     * including its verbosity level, the message text, and an optional prefix.
     */
    struct LogMessage {
        loguru::Verbosity verbosity; ///< The verbosity level of the log message
        std::string message;         ///< The content of the log message
        std::string prefix;          ///< Optional prefix for the log message
    };


    /**
     * @brief Console window for displaying and managing application logs.
     *
     * This class provides a visual interface for viewing log messages with different
     * verbosity levels, executing commands, and managing log settings. It integrates
     * with the loguru logging system to display real-time log messages.
     */
    class ConsoleWindow final : public ADocumentWindow {
        public:
            /**
            * @brief Constructs and initializes a ConsoleWindow.
            *
            * This constructor sets up the console's logging functionality by registering a loguru callback via
            * loguru::add_callback to route log messages to the console (using the static loguruCallback) and by
            * establishing an engine log callback that maps custom LogLevel messages to loguru verbosity levels
            * using nexoLevelToLoguruLevel before logging them with VLOG_F.
            *
            * @param windowName The name of the window
            * @param registry The window registry used to register this console window.
            */
            explicit ConsoleWindow(const std::string &windowName, WindowRegistry &registry);

            /**
             * @brief Destructor that cleans up the ConsoleWindow.
             *
             * Removes the registered loguru callback identified by LOGURU_CALLBACK_NAME to prevent further logging after the window is destroyed.
             */
            ~ConsoleWindow() override
            {
                loguru::remove_callback(LOGURU_CALLBACK_NAME);
            };

            // No-op method in this class
            void setup() override;

            /**
             * @brief Clears all stored log entries during shutdown.
             *
             * This method resets the console's log by invoking clearLog(), ensuring that all previous
             * log entries are removed as part of the shutdown process.
             */
            void shutdown() override;

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
            void show() override;

            // No-op method in this class
            void update() override;

            /**
             * @brief Executes a command entered in the console.
             *
             * Processes the given command line, adds it to the command history,
             * and displays it in the log.
             *
             * @param commandLine The command text to execute.
             * @note not implemented yet
             */
            void executeCommand(const char* commandLine);

        private:
            char m_inputBuf[512] = {};
            std::vector<std::string> m_commands; // History of executed commands.

            std::string m_logFilePath;
            bool m_exportLog = true;

            bool m_scrollToBottom = true;


            std::set<loguru::Verbosity> m_selectedVerbosityLevels = {
                loguru::Verbosity_FATAL,
                loguru::Verbosity_ERROR,
                loguru::Verbosity_WARNING,
                loguru::Verbosity_INFO,
                loguru::Verbosity_1,
            };

            float m_logPadding = 0.0f;
            std::vector<LogMessage> m_logs;
            size_t m_maxLogCapacity = 200;
            std::vector<LogMessage> m_bufferLogsToExport;
            size_t m_maxBufferLogToExportCapacity = 20;


            /**
             * @brief Clears all log entries and display items.
             *
             * Removes all log messages from the internal storage and resets the list of display items.
             */
            void clearLog();

            /**
             * @brief Appends a log message to the console's log collection.
             *
             * This method adds the provided log message to the internal container, ensuring it is available
             * for display in the console window.
             *
             * @param message The log message to append.
             */
            void addLog(const LogMessage& message);

            /**
             * @brief Adds a formatted log message to the console.
             *
             * Creates a log message using printf-style formatting and adds it to the log collection.
             *
             * @tparam Args Variadic template for format arguments
             * @param fmt Format string similar to printf
             * @param args Arguments for the format string
             */
             template<typename... Args>
             void addLog(std::format_string<Args...> fmt, Args&&... args)
             {
                 try {
                     std::string formattedString = std::format(fmt, std::forward<Args>(args)...);

                     LogMessage newMessage;
                     newMessage.verbosity = loguru::Verbosity_1;
                     newMessage.message = formattedString;
                     newMessage.prefix = "";
                     m_logs.push_back(newMessage);
                 } catch (const std::exception &e) {
                     LogMessage newMessage;
                     newMessage.verbosity = loguru::Verbosity_ERROR;

                     char errorBuffer[1024];

                     // format up to sizeof(errorBuffer)-1 characters
                     auto result = std::format_to_n(
                         std::begin(errorBuffer),
                         std::size(errorBuffer) - 1,
                         "Error formatting log message: {}",
                         e.what()
                     );

                     // null-terminate
                     *result.out = '\0';

                     newMessage.message = std::string(errorBuffer);
                     newMessage.prefix = "";
                     m_logs.push_back(newMessage);
                 }

                 m_scrollToBottom = true;
             }

            /**
             * @brief Displays a single log entry in the console UI.
             *
             * Renders a log message with appropriate styling based on its verbosity level.
             *
             * @param verbosity The verbosity level that determines the message's color
             * @param msg The message text to display
             */
            void displayLog(loguru::Verbosity verbosity, const std::string &msg) const;

            /**
             * @brief Exports buffered logs to the log file.
             *
             * Writes any logs in the export buffer to the configured log file,
             * helping to prevent memory buildup from excessive logging.
             */
            void exportLogsBuffered() const;

            /**
             * @brief Displays the popup for configuring verbosity settings.
             *
             * Shows a popup menu that allows the user to select which verbosity levels
             * to display in the console and configure other log-related settings.
             */
            void showVerbositySettingsPopup();

            /**
             * @brief Updates the horizontal padding for log entries.
             *
             * Iterates over the stored log messages to compute the maximum width of the verbosity tag text for
             * messages that are currently visible (filtered by selected verbosity levels). The computed maximum
             * width is then increased by the spacing defined in the ImGui style to ensure proper alignment in the UI.
             */
            void calcLogPadding();

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
            static void loguruCallback(void *userData, const loguru::Message& message);
    };

}
