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
#include "src/DocumentWindows/ConsoleWindow/ConsoleWindow.hpp"
#include "src/DocumentWindows/EditorScene/EditorScene.hpp"
#include "src/DocumentWindows/SceneTreeWindow/SceneTreeWindow.hpp"
#include "src/DocumentWindows/InspectorWindow/InspectorWindow.hpp"
#include "src/DocumentWindows/AssetManager/AssetManagerWindow.hpp"
#include "src/DocumentWindows/MaterialInspector/MaterialInspector.hpp"
#include "src/DocumentWindows/PrimitiveWindow/PrimitiveWindow.hpp"
#include "src/DocumentWindows/GameWindow/GameWindow.hpp"

#include <thread>
#include <loguru.hpp>
#include <core/exceptions/Exceptions.hpp>

#include "Path.hpp"
#include "scripting/native/ManagedTypedef.hpp"
#include "scripting/native/Scripting.hpp"

int main(int argc, char **argv)
try {
    loguru::init(argc, argv);
    loguru::g_stderr_verbosity = loguru::Verbosity_3;
    nexo::editor::Editor &editor = nexo::editor::Editor::getInstance();

    editor.registerWindow<nexo::editor::EditorScene>(
        std::format("Default Scene{}{}", NEXO_WND_USTRID_DEFAULT_SCENE, 0)
    );
    editor.registerWindow<nexo::editor::SceneTreeWindow>(NEXO_WND_USTRID_SCENE_TREE);
    editor.registerWindow<nexo::editor::InspectorWindow>(NEXO_WND_USTRID_INSPECTOR);
    editor.registerWindow<nexo::editor::ConsoleWindow>(NEXO_WND_USTRID_CONSOLE);
    editor.registerWindow<nexo::editor::MaterialInspector>(NEXO_WND_USTRID_MATERIAL_INSPECTOR);
    editor.registerWindow<nexo::editor::PrimitiveWindow>(NEXO_WND_USTRID_PRIMITIVE_WINDOW);
    editor.registerWindow<nexo::editor::AssetManagerWindow>(NEXO_WND_USTRID_ASSET_MANAGER);

    if (const auto defaultScene = editor.getWindow<nexo::editor::EditorScene>(std::format("Default Scene{}{}", NEXO_WND_USTRID_DEFAULT_SCENE, 0)).lock())
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
} catch (const std::exception &e) {
    LOG(NEXO_ERROR, "Unhandled exception: {}", e.what());
    return 1;
} catch (...) {
    LOG(NEXO_ERROR, "Unhandled unknown exception");
    return 1;
}
