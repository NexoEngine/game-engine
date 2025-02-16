//// main.cpp /////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        10/11/2024
//  Description: Main file for the nexo editor
//
///////////////////////////////////////////////////////////////////////////////

#include <sentry.h>
#include "src/Editor.hpp"
#include "src/DocumentWindows/ConsoleWindow.hpp"
#include "src/DocumentWindows/MainScene.hpp"
#include "src/DocumentWindows/SceneTreeWindow.hpp"

#include <thread>
#include <core/exceptions/Exceptions.hpp>

int main(int argc, char **argv)
{
    // Initialize Sentry
    sentry_options_t *options = sentry_options_new();
    sentry_options_set_dsn(options, "https://d8b6a2e6dba9385c2322bec13c2eed2f@o4508817940873216.ingest.de.sentry.io/4508829070327888");
    sentry_options_set_database_path(options, ".sentry-native");
    sentry_options_set_release(options, "nexo-editor@1.0.0");
    sentry_options_set_debug(options, 1);
    sentry_init(options);

    sentry_capture_event(sentry_value_new_message_event(
  /*   level */ SENTRY_LEVEL_INFO,
  /*  logger */ "custom",
  /* message */ "It works!"
));

    try {
        loguru::init(argc, argv);
        loguru::g_stderr_verbosity = loguru::Verbosity_3;
        nexo::editor::Editor editor;
        const auto &sceneViewManager = nexo::editor::SceneViewManager::getInstance();
        sceneViewManager->addNewScene("Default scene", std::make_shared<nexo::editor::MainScene>("Default scene", true));
        editor.registerWindow("Scene Tree", std::make_shared<nexo::editor::SceneTreeWindow>());
        editor.registerWindow("Scene view manager", sceneViewManager);
        editor.registerWindow("Console", std::make_shared<nexo::editor::ConsoleWindow>(editor));
        editor.init();

        while (editor.isOpen())
        {
            auto start = std::chrono::high_resolution_clock::now();
            editor.render();
            editor.update();

            auto end = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> elapsed = end - start;
            std::this_thread::sleep_for(std::chrono::milliseconds(16) - elapsed);
        }

        editor.shutdown();
        sentry_close();  // Make sure to close Sentry before exiting
        return 0;
    } catch (const nexo::Exception &e) {
        sentry_capture_exception(e);  // Capture the exception in Sentry
        LOG_EXCEPTION(e);
        sentry_close();
        return 1;
    }
}

