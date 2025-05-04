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

#include <thread>
#include <loguru.hpp>
#include <core/exceptions/Exceptions.hpp>

#include "Path.hpp"
#include "scripting/native/Scripting.hpp"

int runScriptExample(const nexo::scripting::HostHandler::Parameters& params)
{
    // Get the instance of the HostHandler singleton
    nexo::scripting::HostHandler& host = nexo::scripting::HostHandler::getInstance();

    // Initialize the host
    nexo::scripting::HostHandler::Status status = host.initialize(params);
    if (status != nexo::scripting::HostHandler::SUCCESS) {
        return EXIT_FAILURE;
    }

    // Get function pointers to managed methods
    // Regular method
    component_entry_point_fn hello = host.getManagedFptr<component_entry_point_fn>(
        STR("Nexo.Lib, Nexo"),
        STR("Hello"),
        nullptr
    );

    if (hello == nullptr) {
        return EXIT_FAILURE;
    }

    // Run managed code
    struct lib_args {
        const char_t* message;
        int number;
    };

    // Call the Hello method multiple times
    for (int i = 0; i < 3; ++i) {
        lib_args args {
            STR("from host!"),
            i
        };

        hello(&args, sizeof(args));
    }

    // Get function pointer for UnmanagedCallersOnly method
    typedef void (CORECLR_DELEGATE_CALLTYPE *custom_entry_point_fn)(lib_args args);
    custom_entry_point_fn custom_unmanaged = host.getManagedFptr<custom_entry_point_fn>(
        STR("Nexo.Lib, Nexo"),
        STR("CustomEntryPointUnmanagedCallersOnly"),
        UNMANAGEDCALLERSONLY_METHOD
    );

    if (custom_unmanaged == nullptr) {
        return EXIT_FAILURE;
    }

    // Call UnmanagedCallersOnly method
    lib_args args_unmanaged {
        STR("from host!"),
        -1
    };
    custom_unmanaged(args_unmanaged);

    // Get function pointer for custom delegate type method
    custom_entry_point_fn custom = host.getManagedFptr<custom_entry_point_fn>(
        STR("Nexo.Lib, Nexo"),
        STR("CustomEntryPoint"),
        STR("Nexo.Lib+CustomEntryPointDelegate, Nexo")
    );

    if (custom == nullptr) {
        return EXIT_FAILURE;
    }

    // Call custom delegate type method
    custom(args_unmanaged);

    return EXIT_SUCCESS;
}

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
    editor.registerWindow<nexo::editor::AssetManagerWindow>(NEXO_WND_USTRID_ASSET_MANAGER);

    if (const auto defaultScene = editor.getWindow<nexo::editor::EditorScene>(std::format("Default Scene{}{}", NEXO_WND_USTRID_DEFAULT_SCENE, 0)).lock())
        defaultScene->setDefault();

    editor.init();

    /*if (int rc = nexo::scripting::load_hostfxr() != 0) {
        LOG(NEXO_ERROR, "Failed to load hostfxr error code {}", rc);
    }*/
    const nexo::scripting::HostHandler::Parameters params = {
        .errorCallback = [](const nexo::scripting::HostString& message) {
            LOG(NEXO_ERROR, "Scripting host error: {}", message.to_utf8());
        },
    };
    runScriptExample(params);

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
