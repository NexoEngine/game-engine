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

#include "Editor.hpp"
#include "backends/ImGuiBackend.hpp"

#include "imgui.h"
#include <ImGuizmo.h>

namespace nexo::editor {

    void Editor::setupLogs()
    {
        loguru::add_callback(LOGURU_CALLBACK_NAME, &Editor::loguruCallback,
                             this, loguru::Verbosity_MAX);

        LOG_F(INFO, "GameEngineEditor initialized");
        VLOG_F(loguru::Verbosity_ERROR, "GameEngineEditor initialized");
    }

    void Editor::addLog(const LogMessage &message)
    {
        m_logs.push_back(message);
    }

    const std::vector<LogMessage> &Editor::getLogs() const
    {
        return m_logs;
    }

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


    Editor::Editor()
    {
        setupLogs();
        setupEngine();
        setupStyle();
        setupDockspace();
    }

    bool Editor::isOpen() const
    {
        return !m_quit && m_app->isWindowOpen();
    }

    void Editor::setupEngine()
    {
        m_app = std::make_shared<Application>(nexo::init());
        ImGuiBackend::setErrorCallback(m_app->getWindow());

        ImGui::CreateContext();

        ImGui::StyleColorsDark();
        ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
        ImGuizmo::Enable(true);
        setupFonts();
    }

    void Editor::setupStyle()
    {
        ImGui::Spectrum::StyleColorsSpectrum();

        // Retrieve DPI scale
        // float xScale, yScale = 0.0f;
        // m_app->getWindow()->getDpiScale(&xScale, &yScale);
        // if (xScale > 1.0f || yScale > 1.0f)
        //     std::cerr << "WARNING: High DPI detected! If you have rendering issues please try another scaling factor."
        //             << std::endl;

        //std::cout << "xScale " << xScale << " yScale " << yScale << std::endl;
        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(m_app->getWindow()->getWidth()),
                            static_cast<float>(m_app->getWindow()->getHeight()));
        //io.DisplayFramebufferScale = ImVec2(xScale, yScale); // Apply the DPI scale to ImGui rendering
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


        ImGuiStyle *style = &ImGui::GetStyle();
        style->CircleTessellationMaxError = 0.10f;
        style->WindowRounding = 10.0f;
        style->ChildRounding = 6.0f;
        style->PopupRounding = 4.0f;
        //style->ScaleAllSizes(xScale);

        ImVec4 *colors = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Tab] = ImVec4(0.26f, 0.52f, 0.83f, 0.93f);
        colors[ImGuiCol_TabHovered] = ImVec4(0.12f, 0.52f, 0.99f, 0.80f);
        colors[ImGuiCol_TabActive] = ImVec4(0.06f, 0.32f, 0.63f, 1.00f);
        colors[ImGuiCol_TableHeaderBg] = ImVec4(0.15f, 0.44f, 0.79f, 1.00f);

        ImGuiBackend::init(m_app->getWindow());
    }

    void Editor::setupFonts()
    {
        ImFontConfig fontConfig;
        fontConfig.OversampleH = 3; // Horizontal oversampling
        fontConfig.OversampleV = 3; // Vertical oversampling

        ImGuiIO &io = ImGui::GetIO();

        io.Fonts->AddFontDefault();

        constexpr float fontSize = 18.0f;

        ImFont *font = io.Fonts->AddFontFromFileTTF("../assets/fonts/SourceSans3-Regular.ttf", fontSize,
                                                    &fontConfig);
        IM_ASSERT(font != nullptr);
        io.FontDefault = font;

        ImGuiBackend::initFontAtlas();
    }

    void Editor::setupDockspace()
    {
        //ImGui::DockSpaceOverViewport(ImGui::GetMainViewport(), ImGuiDockNodeFlags_None);
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

    Editor::~Editor()
    {
        ImGuiBackend::shutdown();

        loguru::remove_callback(LOGURU_CALLBACK_NAME);
    }

    void Editor::init()
    {
        for (const auto &[_, window]: m_windows)
        {
            window->setup();
        }
    }

    void Editor::update()
    {
        for (const auto &[_, window]: m_windows)
        {
            window->update();
        }
    }

    void Editor::render()
    {
        ImGuiBackend::begin();

        ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
        ImGuizmo::BeginFrame();

        ImGui::DockSpaceOverViewport(ImGui::GetMainViewport());
        drawMenuBar();
        ImGui::ShowDemoWindow();

        for (const auto &[_, window] : m_windows)
        {
            if (window->isOpened())
                window->show();
        }

        ImGui::Render();
        ImGuiBackend::end(m_app->getWindow());
    }

    void Editor::destroy()
    {
        for (const auto &[_, window]: m_windows)
        {
            window->shutdown();
        }
    }

    void Editor::registerWindow(const std::string &name,
                                std::shared_ptr<IDocumentWindow> window)
    {
        m_windows[name] = std::move(window);
    }
}
