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

#include "DocumentWindows/SceneViewManager.hpp"
#include "src/Editor.hpp"
#include "src/DocumentWindows/ConsoleWindow.hpp"
#include "src/DocumentWindows/MainScene.hpp"
#include "src/DocumentWindows/SceneTreeWindow.hpp"
#include "src/DocumentWindows/InspectorWindow.hpp"
#include "src/DocumentWindows/MaterialInspector.hpp"

#include <thread>
#include <core/exceptions/Exceptions.hpp>
#include <DocumentWindows/InspectorWindow.hpp>

/**
 * @brief Entry point for the nexo editor application.
 *
 * Initializes logging via loguru, registers essential editor windows using template-based registration,
 * and adds a default scene to the SceneViewManager. The function then enters a main loop that renders and
 * updates the editor while maintaining a steady frame rate by adjusting sleep time based on frame processing duration.
 * If a nexo::Exception is thrown, it is logged and the application terminates with a non-zero exit status.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return int Returns 0 on successful execution or 1 if an exception occurs.
 */
int main(int argc, char **argv)
{
    try {
        loguru::init(argc, argv);
        loguru::g_stderr_verbosity = loguru::Verbosity_3;
        nexo::editor::Editor editor;

        editor.registerWindow<nexo::editor::SceneViewManager>();
        editor.registerWindow<nexo::editor::SceneTreeWindow>();
        editor.registerWindow<nexo::editor::InspectorWindow>();
        editor.registerWindow<nexo::editor::ConsoleWindow>();
        editor.registerWindow<nexo::editor::MaterialInspector>();

        editor.init();
        editor.getWindow<nexo::editor::SceneViewManager>()->addNewScene("Default scene", true);

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
