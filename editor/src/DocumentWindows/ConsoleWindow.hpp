//// ConsoleWindow.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Header file for the console window class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "ADocumentWindow.hpp"
#include "Editor.hpp"

namespace nexo::editor {

    class ConsoleWindow final : public ADocumentWindow {
        public:
        explicit ConsoleWindow(const Editor& editor)
            : _editor(editor) {}

        ~ConsoleWindow() override;

        void setup() override;
        void shutdown() override;
        void show() override;
        void update() override;

        void addLog(const char* fmt, ...);
        void executeCommand(const char* command_line);

        private:
        char inputBuf[512] = {};
        std::deque<std::string> items;
        bool scrollToBottom = true;
        std::vector<std::string> commands; // History of executed commands.

        std::set<loguru::Verbosity> selectedVerbosityLevels = {
            loguru::Verbosity_FATAL,
            loguru::Verbosity_ERROR,
            loguru::Verbosity_WARNING,
            loguru::Verbosity_INFO,
        };

        // Command and variables handling (optional)
        // std::map<std::string, std::function<void(const std::string&)>> commandsMap;

        const Editor& _editor;

        void clearLog();
    };

}