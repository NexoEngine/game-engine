//// Editor.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/11/2024
//  Description: Source file for the main editor class
//
//////////////////////////////////////////////////////////////////////////////

#include "Nexo.hpp"
#include "Editor.hpp"
#include "Logger.hpp"
#include "Path.hpp"
#include "backends/ImGuiBackend.hpp"
#include "IconsFontAwesome.h"

#include "imgui.h"
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include <algorithm>

namespace nexo::utils {
    loguru::Verbosity nexoLevelToLoguruLevel(const LogLevel level)
    {
        switch (level)
        {
            case LogLevel::FATAL: return loguru::Verbosity_FATAL;
            case LogLevel::ERROR: return loguru::Verbosity_ERROR;
            case LogLevel::WARN: return loguru::Verbosity_WARNING;
            case LogLevel::INFO: return loguru::Verbosity_INFO;
            case LogLevel::DEBUG: return loguru::Verbosity_1;
            case LogLevel::DEV: return loguru::Verbosity_2;
        }
        return loguru::Verbosity_INVALID;
    }
}

namespace nexo::editor {

    void Editor::loguruCallback([[maybe_unused]] void *userData,
                                const loguru::Message &message)
    {
        const auto editor = static_cast<Editor *>(userData);
        editor->addLog({
            .verbosity = message.verbosity,
            .message = message.message,
            .prefix = message.prefix
        });
    }

    void Editor::shutdown() const
    {
        LOG(NEXO_INFO, "Closing editor");
        for (const auto &[_, window]: m_windows)
        {
            window->shutdown();
        }
        LOG(NEXO_INFO, "All windows destroyed");
        ImGuiBackend::shutdown();
        LOG(NEXO_INFO, "Editor closed");
        loguru::remove_callback(LOGURU_CALLBACK_NAME);
    }

    Editor::Editor()
    {
        setupLogs();
        LOG(NEXO_INFO, "Logs initialized");
        setupEngine();
        setupStyle();
        LOG(NEXO_INFO, "Style initialized");
        LOG(NEXO_INFO, "Editor initialized");
        LOG(NEXO_ERROR, "Error log test");
        LOG(NEXO_WARN, "Warning log test");
        m_sceneManagerBridge = std::make_shared<SceneManagerBridge>();
    }

    void Editor::setupLogs()
    {
        loguru::add_callback(LOGURU_CALLBACK_NAME, &Editor::loguruCallback,
                             this, loguru::Verbosity_MAX);

        auto engineLogCallback = [](const LogLevel level, const std::string &message) {
            const auto loguruLevel = utils::nexoLevelToLoguruLevel(level);
            VLOG_F(loguruLevel, "%s", message.c_str());
        };
        Logger::setCallback(engineLogCallback);
    }

    void Editor::setupEngine() const
    {
        auto const &app = Application::getInstance();
        auto &window = app.getWindow();

#ifdef __linux__
    #ifndef WAYLAND_APP_ID
        #warning "WAYLAND_APP_ID not defined, cannot set Wayland app id for window"
        LOG(NEXO_WARN, "WAYLAND_APP_ID not defined, cannot set Wayland app id for window");
    #else
        window->setWaylandAppId(WAYLAND_APP_ID);
        window->setWmClass(WAYLAND_APP_ID, "nexo-editor");
    #endif
#endif

        nexo::init();

        ImGuiBackend::setErrorCallback(nexo::getApp().getWindow());

        ImGui::CreateContext();
        ImGuiBackend::init(nexo::getApp().getWindow());

        ImGuiIO &io = ImGui::GetIO();
        static const std::string iniFilePath = Path::resolvePathRelativeToExe(
            "../config/default-layout.ini").string();
        io.IniFilename = iniFilePath.c_str();
        LOG(NEXO_INFO, "ImGui .ini file path: {}", iniFilePath);

        ImGui::StyleColorsDark();
        ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
        ImGuizmo::Enable(true);
    }

    void Editor::setupStyle() const
    {
        ImGui::Spectrum::StyleColorsSpectrum();

        // Retrieve DPI scale

        float scaleFactorX = 0.0f;
        float scaleFactorY = 0.0f;
        nexo::getApp().getWindow()->getDpiScale(&scaleFactorX, &scaleFactorY);
        nexo::getApp().getWindow()->setWindowIcon(Path::resolvePathRelativeToExe(
            "../assets/nexo.png"));
        if (scaleFactorX > 1.0f || scaleFactorY > 1.0f)
        {
            LOG(NEXO_WARN,
                "Scale factor is greater than 1.0, if you have any issue try adjusting the system's scale factor");
            LOG(NEXO_INFO, "DPI scale: x: {}, y: {}", scaleFactorX, scaleFactorY);
        }

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(nexo::getApp().getWindow()->getWidth()),
                                static_cast<float>(nexo::getApp().getWindow()->getHeight()));
        io.DisplayFramebufferScale = ImVec2(scaleFactorX, scaleFactorY); // Apply the DPI scale to ImGui rendering
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


        ImGuiStyle *style = &ImGui::GetStyle();
        style->CircleTessellationMaxError = 0.10f;
        style->WindowRounding = 10.0f;
        style->ChildRounding = 6.0f;
        style->PopupRounding = 4.0f;
        style->ScaleAllSizes(std::max(scaleFactorX, scaleFactorY));

        ImVec4 darker     = ImVec4(20.f/255.f, 20.f/255.f, 20.f/255.f, 1.0f);

            // Apply the darker color to the title bar variants:
        style->Colors[ImGuiCol_TitleBg]         = darker;
        style->Colors[ImGuiCol_TitleBgActive]   = darker;
        style->Colors[ImGuiCol_TitleBgCollapsed] = darker;

        ImVec4 creamColor   = ImVec4(1.0f, 0.992f, 0.815f, 1.0f);  // Light cream
        ImVec4 creamHovered = ImVec4(1.0f, 1.0f, 0.9f, 1.0f);        // Slightly lighter when hovered
        ImVec4 creamActive  = ImVec4(1.0f, 0.95f, 0.8f, 1.0f);       // Slightly darker when active
        ImVec4 brighterActive = ImVec4(1.0f, 1.0f, 0.95f, 1.0f);

        // Apply the light cream colors to the tabs:
        style->Colors[ImGuiCol_Tab]                = creamColor;
        style->Colors[ImGuiCol_TabHovered]         = creamHovered;
        style->Colors[ImGuiCol_TabActive]          = brighterActive;
        style->Colors[ImGuiCol_TabUnfocused]       = creamColor;
        style->Colors[ImGuiCol_TabUnfocusedActive] = creamActive;
        style->Colors[ImGuiCol_TabSelectedOverline] = ImVec4(1, 1, 1, 1);
        style->Colors[ImGuiCol_TabDimmedSelectedOverline] = ImVec4(1, 1, 1, 0);

        style->Colors[ImGuiCol_Header]                = creamColor;
        style->Colors[ImGuiCol_HeaderHovered]         = creamHovered;
        style->Colors[ImGuiCol_HeaderActive]          = creamActive;

        // Optionally, you might want to adjust the text color if needed:
        setupFonts(scaleFactorX, scaleFactorY);
    }

    void Editor::setupFonts(const float scaleFactorX, const float scaleFactorY) const
    {
        ImFontConfig fontConfig;
        fontConfig.OversampleH = 3; // Horizontal oversampling
        fontConfig.OversampleV = 3; // Vertical oversampling

        ImGuiIO &io = ImGui::GetIO();

        io.Fonts->AddFontDefault();

        float fontSize = 18.0f;
        if (scaleFactorX > 1.0f || scaleFactorY > 1.0f)
        {
            fontSize = std::ceil(fontSize * std::max(scaleFactorX, scaleFactorY));
            LOG(NEXO_WARN, "Font size adjusted to {}", fontSize);
        }

        static const std::string sourceSansPath = Path::resolvePathRelativeToExe(
            "../assets/fonts/SourceSans3-Regular.ttf").string();
        ImFont *font = io.Fonts->AddFontFromFileTTF(sourceSansPath.c_str(), fontSize,
                                                    &fontConfig);
        LOG(NEXO_DEBUG, "Font path: {}", sourceSansPath);
        IM_ASSERT(font != nullptr);
        io.FontDefault = font;

        ImGuiBackend::initFontAtlas();

        ImFontConfig fontawesome_config;
        fontawesome_config.MergeMode = true;
        fontawesome_config.OversampleH = 3; // Horizontal oversampling
        fontawesome_config.OversampleV = 3; // Vertical oversampling
        static constexpr ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        static const std::string fontawesomePath = Path::resolvePathRelativeToExe(
            "../assets/fonts/fontawesome4.ttf").string();
        io.Fonts->AddFontFromFileTTF(fontawesomePath.c_str(), fontSize, &fontawesome_config, icon_ranges);

        LOG(NEXO_DEBUG, "Fonts initialized");
    }

    void Editor::registerWindow(const std::string &name,
                            std::shared_ptr<IDocumentWindow> window)
    {
        window->setSceneManager(m_sceneManagerBridge);
        m_windows[name] = std::move(window);
        LOG(NEXO_INFO, "Registered window: {}", name.c_str());
    }

    void Editor::init() const
    {
        for (const auto &[_, window]: m_windows)
        {
            window->setup();
        }
    }

    void Editor::addLog(const LogMessage &message)
    {
        m_logs.push_back(message);
    }

    const std::vector<LogMessage> &Editor::getLogs() const
    {
        return m_logs;
    }

    bool Editor::isOpen() const
    {
        return !m_quit && nexo::getApp().isWindowOpen() && nexo::getApp().isRunning();
    }

    void Editor::drawMenuBar()
    {
        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("Import"))
                    m_windows["ModelViewer"]->getOpened() = true;

                if (ImGui::MenuItem("Exit"))
                    m_quit = true;

                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Window"))
            {
                for (const auto &[name, window]: m_windows)
                {
                    ImGui::MenuItem(name.c_str(), nullptr, &window->getOpened(), &window->getOpened());
                }
                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void Editor::buildDockspace() const
    {
        if (const ImGuiID dockspaceID = ImGui::GetMainViewport()->ID; !ImGui::DockBuilderGetNode(dockspaceID))
        {
            ImGui::DockBuilderRemoveNode(dockspaceID);
            ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
            ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_None);

            ImGui::DockBuilderSetNodeSize(dockspaceID, ImGui::GetMainViewport()->Size);

            ImGuiID topNode;
            ImGuiID rightNode;
            ImGuiID bottomNode;
            ImGuiID leftNode;

            // Split the main dockspace vertically (70% for the main scene)
            ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Left, 0.7f, &leftNode, &rightNode);
            // Split the right part horizontally (50/50 between the console and the scene tree)
            ImGui::DockBuilderSplitNode(rightNode, ImGuiDir_Up, 0.5f, &topNode, &bottomNode);

            // Attach the default scene to the left side of the window
            ImGui::DockBuilderDockWindow("Default scene", leftNode);
            // Attach the scene tree to the top of the right part
            ImGui::DockBuilderDockWindow("Scene Tree", topNode);
            // Attach the console to the bottom of the right part
            ImGui::DockBuilderDockWindow("Console", bottomNode);

            ImGui::DockBuilderFinish(dockspaceID);
        }
        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport()->ID);
    }

    void Editor::render()
    {
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg].w = 0.0f; // 0.0f for full transparency
        ImGuiBackend::begin();

        ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
        ImGuizmo::BeginFrame();
        buildDockspace();

        drawMenuBar();
        //ImGui::ShowDemoWindow();

        for (const auto &[_, window]: m_windows)
        {
            if (window->isOpened())
                window->show();
        }

        // Gradient background handling
        {
	        ImGuiViewport* viewport = ImGui::GetMainViewport();
	        ImGui::SetNextWindowPos(viewport->Pos);
	        ImGui::SetNextWindowSize(viewport->Size);
	        ImGui::SetNextWindowViewport(viewport->ID);
	        ImGui::Begin("Background", nullptr,
	                        ImGuiWindowFlags_NoDecoration |
	                        ImGuiWindowFlags_NoInputs |
	                        ImGuiWindowFlags_NoFocusOnAppearing |
	                        ImGuiWindowFlags_NoBringToFrontOnFocus);

	        ImDrawList* draw_list = ImGui::GetWindowDrawList();
	        ImU32 darkBase = IM_COL32(20, 20, 20, 255);

	        ImU32 col_top_left     = darkBase;
	        ImU32 col_bottom_right = darkBase;

	        // Subtle blue tint
	        ImU32 col_bottom_left  = IM_COL32(20, 20, 40, 255);

	        // Subtle fuchsia tint.
	        ImU32 col_top_right    = IM_COL32(30, 20, 30, 255);

	        draw_list->AddRectFilledMultiColor(
	            viewport->Pos,
	            ImVec2(viewport->Pos.x + viewport->Size.x, viewport->Pos.y + viewport->Size.y),
	            col_top_left, col_top_right, col_bottom_right, col_bottom_left);

	        ImGui::End();
        }

        ImGui::Render();
        ImGuiBackend::end(nexo::getApp().getWindow());
    }

    void Editor::update() const
    {
        for (const auto &[_, window]: m_windows)
        {
            window->update();
        }
    }
}
