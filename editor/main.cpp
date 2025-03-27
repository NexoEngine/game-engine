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
#include "src/DocumentWindows/EditorScene.hpp"
#include "src/DocumentWindows/SceneTreeWindow.hpp"
#include "src/DocumentWindows/InspectorWindow.hpp"
#include "src/DocumentWindows/AssetManagerWindow.hpp"
#include "src/DocumentWindows/MaterialInspector.hpp"

#include <thread>
#include <core/exceptions/Exceptions.hpp>

/**
 * @brief Entry point for the Nexo Editor application.
 *
 * Initializes logging, registers editor windows (including the default scene, scene tree, inspector, console, material inspector, and asset manager), sets up the default scene if available, and starts the main loop that handles rendering and updating the editor at roughly 60 FPS. Upon exit or in case of an exception, the editor is properly shut down.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return int Returns 0 if the application exits normally; returns 1 if an exception is caught.
 */
int main(int argc, char **argv)
{
    try {
        loguru::init(argc, argv);
        loguru::g_stderr_verbosity = loguru::Verbosity_3;
        nexo::editor::Editor &editor = nexo::editor::Editor::getInstance();

        editor.registerWindow<nexo::editor::EditorScene>("Default scene");
        editor.registerWindow<nexo::editor::SceneTreeWindow>("Scene tree");
        editor.registerWindow<nexo::editor::InspectorWindow>("Inspector");
        editor.registerWindow<nexo::editor::ConsoleWindow>("Console");
        editor.registerWindow<nexo::editor::MaterialInspector>("Material inspector");
        editor.registerWindow<nexo::editor::AssetManagerWindow>("Asset Manager");

        if (auto defaultScene = editor.getWindow<nexo::editor::EditorScene>("Default scene").lock())
            defaultScene->setDefault();

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
