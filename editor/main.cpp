//// main.cpp /////////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Main file for the nexo editor
//
///////////////////////////////////////////////////////////////////////////////

#include "src/Editor.hpp"
#include "src/DocumentWindows/ConsoleWindow.hpp"
#include "src/DocumentWindows/MainScene.hpp"
#include "src/DocumentWindows/SceneTreeWindow.hpp"
#include "src/DocumentWindows/AssetManagerWindow.hpp"

#include <thread>
#include <core/exceptions/Exceptions.hpp>

/**
 * @brief Entry point for the Nexo Editor application.
 *
 * Initializes logging and retrieves singleton instances for the editor and scene view manager. Sets up the default scene 
 * and registers the main application windows including the Scene Tree, Scene View Manager, Console, and Asset Manager.
 * Enters the main loop where it continuously renders and updates the editor until it is closed, ensuring a consistent 
 * frame rate by adjusting the sleep duration based on frame execution time. If a nexo::Exception is thrown, the error is 
 * logged and the application exits with a non-zero status.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return int Returns 0 on normal shutdown, or 1 if an exception occurs.
 */
int main(int argc, char **argv)
{
    try {
        loguru::init(argc, argv);
        loguru::g_stderr_verbosity = loguru::Verbosity_3;
        nexo::editor::Editor& editor = nexo::editor::Editor::getInstance();
        const auto &sceneViewManager = nexo::editor::SceneViewManager::getInstance();
        sceneViewManager->addNewScene("Default scene", std::make_shared<nexo::editor::MainScene>("Default scene", true));
        editor.registerWindow("Scene Tree", std::make_shared<nexo::editor::SceneTreeWindow>());
        editor.registerWindow("Scene View Manager", sceneViewManager);
        editor.registerWindow("Console", std::make_shared<nexo::editor::ConsoleWindow>(editor));
        editor.registerWindow("Asset Manager", std::make_shared<nexo::editor::AssetManagerWindow>());
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
        return 0;
    } catch (const nexo::Exception &e) {
        LOG_EXCEPTION(e);
        return 1;
    }

}

