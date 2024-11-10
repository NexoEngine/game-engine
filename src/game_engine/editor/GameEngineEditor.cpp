//// GameEngineEditor.cpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        20/10/2024
//  Description: GameEngineEditor class definition.
//
///////////////////////////////////////////////////////////////////////////////

#define IMGUI_ENABLE_FREETYPE

#include "imgui.h"
#include "rlImGui.h"

#include <imgui_impl_raylib.h>
//#include "rlImGuiColors.h"

#include "game_engine/editor/GameEngineEditor.hpp"

#include <ranges>
#include <utility>

#include "game_engine/ecs/Coordinator.hpp"
#include "game_engine/ecs/components/Shapes.hpp"
#include "game_engine/GameEngine.hpp"
#include "game_engine/EventHandling.hpp"

#include "raylib.h"
#include "loguru/loguru.hpp"
#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <ImGuizmo.h>
#include <string>
#include <ostream>

std::string formatString(const char* format, va_list args) {
	// Copy of va_list for the second pass, as va_list can't be reused
	va_list args_copy;
	va_copy(args_copy, args);

	// Calculate the length of the formatted string
	int length = std::vsnprintf(nullptr, 0, format, args);
	if (length <= 0) {
		va_end(args_copy);
		return ""; // Formatting error
	}

	// Allocate memory for the formatted string
	char* buffer = static_cast<char*>(std::malloc(length + 1)); // +1 for null terminator
	if (!buffer) {
		va_end(args_copy);
		return ""; // Allocation failure
	}

	// Format the string
	std::vsnprintf(buffer, length + 1, format, args_copy);
	va_end(args_copy);

	// Create a std::string from the buffer
	std::string formattedString(buffer);

	// Free the buffer
	std::free(buffer);

	return formattedString;
}

static constexpr loguru::Verbosity ConvertToLoguruLevel(int level) {
	switch (level) {
		case 0:   return loguru::Verbosity_MAX;
		case 1:   return loguru::Verbosity_1;
		case 2:    return loguru::Verbosity_INFO;
		case 3: return loguru::Verbosity_WARNING;
		case 4:   return loguru::Verbosity_ERROR;
		default:             return loguru::Verbosity_INFO;
	}
}

// raylib TraceLogCallback
void traceLogCallBack(int logLevel, const char* text, va_list args)
{
	auto result = formatString(text, args);
	loguru::log(ConvertToLoguruLevel(logLevel), "[raylib]", 0, "%s", result.c_str());
}

void engine::editor::GameEngineEditor::setupLogs()
{
	SetTraceLogCallback(&traceLogCallBack);

	loguru::add_callback(LOGURU_CALLBACK_NAME, &GameEngineEditor::loguruCallback,
		this, loguru::Verbosity_MAX);


	LOG_F(INFO, "GameEngineEditor initialized");
}

void engine::editor::GameEngineEditor::addLog(const engine::editor::LogMessage& message)
{
	_logs.push_back(message);
}

const std::vector<engine::editor::LogMessage>& engine::editor::GameEngineEditor::getLogs() const
{
	return _logs;
}

void engine::editor::GameEngineEditor::loguruCallback([[maybe_unused]] void *user_data, const loguru::Message& message)
{
	const auto editor = static_cast<GameEngineEditor*>(user_data);
	editor->addLog({
		.verbosity = message.verbosity,
		.message = message.message,
		.prefix = message.prefix
	});
}


engine::editor::GameEngineEditor::GameEngineEditor()
{
	setupLogs();
	setupEngine();
	setupStyle();
	setupDockspace();
}

bool engine::editor::GameEngineEditor::isOpen() const
{
	return !_quit && engine::isWindowOpen();
}

void engine::editor::GameEngineEditor::setupEngine()
{
	SetConfigFlags(FLAG_MSAA_4X_HINT | FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE | FLAG_WINDOW_HIGHDPI);
	SetTargetFPS(GetMonitorRefreshRate(GetCurrentMonitor()));
	engine::initEngine(false);

	rlImGuiBeginInitImGui();

	ImGui::StyleColorsDark();
	ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
	ImGuizmo::Enable(true);
	setupFonts();
	rlImGuiEndInitImGui();
}

void engine::editor::GameEngineEditor::setupStyle()
{
	ImGui::Spectrum::StyleColorsSpectrum();



	// Retrieve DPI scale
	Vector2 scale = GetWindowScaleDPI();
	if (scale.x > 1.0f || scale.y > 1.0f)
		LOG_F(WARNING, "High DPI detected! If you have rendering issues please try another scaling factor.");

	ImGuiIO& io = ImGui::GetIO();
	io.DisplayFramebufferScale = ImVec2(scale.x, scale.y); // Apply the DPI scale to ImGui rendering
	io.ConfigWindowsMoveFromTitleBarOnly = true;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


	ImGuiStyle *style = &ImGui::GetStyle();
	style->CircleTessellationMaxError = 0.10f;
	style->WindowRounding = 10.0f;
	style->ChildRounding = 6.0f;
	style->PopupRounding = 4.0f;
	//style->ScaleAllSizes(scale.x);

	ImVec4* colors = ImGui::GetStyle().Colors;
	colors[ImGuiCol_Tab]                    = ImVec4(0.26f, 0.52f, 0.83f, 0.93f);
	colors[ImGuiCol_TabHovered]             = ImVec4(0.12f, 0.52f, 0.99f, 0.80f);
	colors[ImGuiCol_TabActive]              = ImVec4(0.06f, 0.32f, 0.63f, 1.00f);
	colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.15f, 0.44f, 0.79f, 1.00f);
}

void engine::editor::GameEngineEditor::setupFonts()
{
	ImFontConfig font_config;
	font_config.OversampleH = 3; // Horizontal oversampling
	font_config.OversampleV = 3; // Vertical oversampling

	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->AddFontDefault();

	float fontSize = 18.0f;

	ImFont* font = io.Fonts->AddFontFromFileTTF("ressources/game_engine/SourceSans3-Regular.ttf", fontSize, &font_config);
	IM_ASSERT(font != nullptr);
	io.FontDefault = font;

	/*
	ImFontConfig fontawesome_config;
	fontawesome_config.MergeMode = true;
	fontawesome_config.OversampleH = 3; // Horizontal oversampling
	fontawesome_config.OversampleV = 3; // Vertical oversampling
	static const ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
	io.Fonts->AddFontFromFileTTF("ressources/game_engine/fontawesome4.ttf", fontSize, &fontawesome_config, icon_ranges);
	*/

	Imgui_ImplRaylib_BuildFontAtlas();
}

void engine::editor::GameEngineEditor::setupDockspace()
{
	// To prevent autosave by ImGui
	ImGuiIO& io = ImGui::GetIO();
	io.IniFilename = nullptr;
	//ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);

}

void engine::editor::GameEngineEditor::drawMenuBar()
{
	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Import"))
				_windows["ModelViewer"]->getOpened() = true;

			if (ImGui::MenuItem("Exit"))
				_quit = true;

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Window"))
		{
			for (const auto& [name, window]: _windows) {
				ImGui::MenuItem(name.c_str(), nullptr, &window->getOpened(), &window->getOpened());
			}
			ImGui::EndMenu();
		}
		ImGui::EndMainMenuBar();
	}
}

void engine::editor::GameEngineEditor::saveEditor()
{
	try {
		_editorSaveFolder.save();
		LOG_F(INFO, "Editor save folder saved successfully");
	} catch (const std::runtime_error& e) {
		LOG_F(FATAL, "Failed to save editor save folder: %s", e.what());
	}
}

void engine::editor::GameEngineEditor::loadEditor()
{
	try {
		_editorSaveFolder.load();
		LOG_F(INFO, "Editor save folder loaded successfully");
	} catch (const std::runtime_error& e) {
		LOG_F(ERROR, "Failed to load editor save folder: %s", e.what());
	}
}

engine::editor::GameEngineEditor::~GameEngineEditor()
{
	rlImGuiShutdown();
	// De-Initialization
	//--------------------------------------------------------------------------------------
	CloseWindow();       // Close window and OpenGL context
	//--------------------------------------------------------------------------------------
	loguru::remove_callback(LOGURU_CALLBACK_NAME);
}

void engine::editor::GameEngineEditor::init()
{
	for (const auto& [_, window]: _windows) {
		window->setup();
	}
}

void engine::editor::GameEngineEditor::update()
{
	//engine::runEngineTextureMode();
	for (const auto& [_, window]: _windows) {
		window->update();
	}
}

void engine::editor::GameEngineEditor::render()
{
	BeginDrawing();
	ClearBackground(DARKGRAY);

	rlImGuiBegin();
	ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
	ImGuizmo::BeginFrame();

	ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());

	drawMenuBar();
	ImGui::ShowDemoWindow();

	for (const auto& [_, window]: _windows) {
		if (window->isOpened())
			window->show();
	}

	rlImGuiEnd();

	EndDrawing();
}

void engine::editor::GameEngineEditor::destroy()
{
	for (const auto& [_, window]: _windows) {
		window->shutdown();
	}
}

void engine::editor::GameEngineEditor::registerWindow(const std::string& name, std::shared_ptr<IDocumentWindow> window)
{
	_windows[name] = std::move(window);
}
