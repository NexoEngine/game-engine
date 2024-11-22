//// Editor.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/11/2024
//  Description: Header file for the main editor class
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#define L_DEBUG 1
#include <vector>
#include <loguru/loguru.hpp>
#include <memory>
#include <unordered_map>

#include "IDocumentWindow.hpp"
#include "DocumentWindows/SceneViewManager.hpp"
#include "SceneManagerBridge.hpp"
#include "Nexo.hpp"

namespace nexo::editor {
    constexpr auto LOGURU_CALLBACK_NAME = "GEE";

    struct LogMessage {
        loguru::Verbosity verbosity;
        std::string message;
        std::string prefix;
    };

    class Editor {
        public:
            Editor();
            ~Editor();

            /**
             * @brief Initializes the engine, setting up necessary components and systems.
             */
            void init();

            [[nodiscard]] bool isOpen() const;


            void update();
            void render();
            void destroy();

            void registerWindow(const std::string& name, std::shared_ptr<IDocumentWindow> window);
            void addLog(const LogMessage& message);
            [[nodiscard]] const std::vector<LogMessage>& getLogs() const;
        private:
            void setupLogs();
            void setupEngine();
            void setupStyle();
            void setupFonts(float scaleFactorX, float scaleFactorY);
            void buildDockspace();
            void drawMenuBar();

            static void loguruCallback(void *userData, const loguru::Message& message);

            bool m_quit = false;
            bool m_showDemoWindow = false;
            std::unordered_map<std::string, std::shared_ptr<IDocumentWindow>> m_windows;

            std::vector<LogMessage> m_logs;

            std::shared_ptr<Application>  m_app;
            std::shared_ptr<SceneManagerBridge> m_sceneManagerBridge;
    };
}