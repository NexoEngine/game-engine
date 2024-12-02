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

    void Editor::shutdown()
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

    void Editor::setupEngine()
    {
        auto &app = Application::getInstance();
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

    void Editor::setupStyle()
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

        ImVec4 *colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Tab] = ImVec4(0.26f, 0.52f, 0.83f, 0.93f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.12f, 0.52f, 0.99f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.06f, 0.32f, 0.63f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.15f, 0.44f, 0.79f, 1.00f);
        setupFonts(scaleFactorX, scaleFactorY);
    }

    void Editor::setupFonts(const float scaleFactorX, const float scaleFactorY)
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

    void Editor::init()
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

    void Editor::buildDockspace()
    {
        const ImGuiID dockspaceID = ImGui::GetMainViewport()->ID;

        if (!ImGui::DockBuilderGetNode(dockspaceID))
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

        ImGui::Render();
        ImGuiBackend::end(nexo::getApp().getWindow());
    }

    void Editor::update()
    {
        for (const auto &[_, window]: m_windows)
        {
            window->update();
        }
    }
}
