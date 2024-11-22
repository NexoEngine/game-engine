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

#include "src/Editor.hpp"
#include "src/DocumentWindows/ConsoleWindow.hpp"
#include "src/DocumentWindows/MainScene.hpp"
#include "src/DocumentWindows/SceneTreeWindow.hpp"

#include <thread>
#include <core/exceptions/Exceptions.hpp>

int main(int argc, char **argv)
{
    try {
        loguru::init(argc, argv);
        loguru::g_stderr_verbosity = loguru::Verbosity_1;
        nexo::editor::Editor editor;
        auto &sceneViewManager = nexo::editor::SceneViewManager::getInstance();
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

        return 0;
    } catch (const nexo::Exception &e) {
        LOG_EXCEPTION(e);
        return 1;
    }
}

