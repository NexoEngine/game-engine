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

    struct TransparentStringHash {
        using is_transparent = void; // Enable heterogeneous lookup
        std::size_t operator()(const std::string_view key) const noexcept {
            return std::hash<std::string_view>{}(key);
        }
    };

    struct TransparentStringEqual {
        using is_transparent = void; // Enable heterogeneous lookup
        bool operator()(const std::string_view lhs, const std::string_view rhs) const noexcept {
            return lhs == rhs;
        }
    };

    class Editor {
        public:
            Editor();
            ~Editor() = default;

            /**
             * @brief Initializes the engine, setting up necessary components and systems.
             */
            void init() const;

            [[nodiscard]] bool isOpen() const;


            void update() const;
            void render();
            void shutdown() const;

            void registerWindow(const std::string& name, std::shared_ptr<IDocumentWindow> window);
            void addLog(const LogMessage& message);
            [[nodiscard]] const std::vector<LogMessage>& getLogs() const;
        private:
            void setupLogs();
            void setupEngine() const;
            void setupStyle() const;
            void setupFonts(float scaleFactorX, float scaleFactorY) const;
            void buildDockspace() const;
            void drawMenuBar();

            static void loguruCallback(void *userData, const loguru::Message& message);

            bool m_quit = false;
            bool m_showDemoWindow = false;
            std::unordered_map<std::string, std::shared_ptr<IDocumentWindow>, TransparentStringHash, TransparentStringEqual> m_windows;

            std::vector<LogMessage> m_logs;

            std::shared_ptr<SceneManagerBridge> m_sceneManagerBridge;
    };
}
