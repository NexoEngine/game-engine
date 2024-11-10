//// Editor.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        19/10/2024
//  Description: Main entry point for the game engine editor.
//
///////////////////////////////////////////////////////////////////////////////

#define IMGUI_ENABLE_FREETYPE

#include "imgui.h"

#include <editor/DocumentWindows/ConsoleWindow.hpp>
#include <editor/DocumentWindows/EntityPropertiesWindow.hpp>
#include <editor/DocumentWindows/Main3DScene.hpp>
#include <editor/DocumentWindows/SceneTreeWindow.hpp>
#include <editor/DocumentWindows/ModelViewerWindow.hpp>
#include <loguru/loguru.hpp>

#include "game_engine/GameEngine.hpp"

#include "game_engine/editor/GameEngineEditor.hpp"
#include "game_engine/SaveHandling.hpp"

#include <iostream>
#include <thread>

#include <fstream>

int main(int argc, char* argv[])
{
	loguru::init(argc, argv);
	engine::editor::GameEngineEditor editor;

	editor.registerWindow("3D View", std::make_shared<engine::editor::Main3DScene>());
	editor.registerWindow("Scene Tree", std::make_shared<engine::editor::SceneTreeWindow>());
	editor.registerWindow("Properties", std::make_shared<engine::editor::EntityPropertiesWindow>());
	editor.registerWindow("ModelViewer", std::make_shared<engine::editor::ModelViewerWindow>());
	editor.registerWindow("Console", std::make_shared<engine::editor::ConsoleWindow>(editor));
	editor.init();

	editor.loadEditor();

	while (editor.isOpen())    // Detect window close button or ESC key
	{
        auto start = std::chrono::high_resolution_clock::now();
        editor.update();
		editor.render();

        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> elapsed = end - start;
        std::this_thread::sleep_for(std::chrono::milliseconds(16) - elapsed);
	}

	editor.saveEditor();
    editor.destroy();
	return 0;
}