//// Init.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the setup function of the console window
//
///////////////////////////////////////////////////////////////////////////////

#include "ConsoleWindow.hpp"
#include "Path.hpp"

namespace nexo::editor {
    void ConsoleWindow::loguruCallback([[maybe_unused]] void *userData,
                                const loguru::Message &message)
    {
        const auto console = static_cast<ConsoleWindow *>(userData);
        LogMessage newMessage;
        newMessage.verbosity = message.verbosity;
        newMessage.message = message.message;
        newMessage.prefix = message.prefix;
        console->addLog(newMessage);
    }


    ConsoleWindow::ConsoleWindow(const std::string &windowName, WindowRegistry &registry) : ADocumentWindow(windowName, registry)
    {
		loguru::add_callback(LOGURU_CALLBACK_NAME, &ConsoleWindow::loguruCallback,
		                         this, loguru::Verbosity_MAX);

    	auto engineLogCallback = [](const LogLevel level, const std::source_location& loc, const std::string &message) {
    		const auto loguruLevel = nexoLevelToLoguruLevel(level);
    		if (loguruLevel > loguru::current_verbosity_cutoff())
    			return;
    		loguru::log(loguruLevel, loc.file_name(), loc.line(), "%s", message.c_str());
		};
		Logger::setCallback(engineLogCallback);
    	try {
    		const auto logFilePath = generateLogFilePath();
    		if (logFilePath.empty()) {
    			throw std::runtime_error("Generated log file path is empty.");
    		}

    		const auto resolvedPath = Path::resolvePathRelativeToExe(logFilePath);
    		if (resolvedPath.empty()) {
    			throw std::runtime_error("Resolved log file path is empty.");
    		}

    		m_logFilePath = resolvedPath.string();
    	} catch (const std::exception &e) {
    	    LOG(NEXO_ERROR, "Error setting up log file: {}", e.what());
    		m_logFilePath.clear();
    	}
		m_logs.reserve(m_maxLogCapacity);
		m_bufferLogsToExport.reserve(m_maxBufferLogToExportCapacity);
    };

    void ConsoleWindow::setup()
    {
    	//All the setup is made in the constructor because the rest of the editor needs the log setup before setting up the windows
    }
}
