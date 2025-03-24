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

#include "utils/Config.hpp"
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

ImGuiID g_materialInspectorDockID = 0;

namespace nexo::editor {

    void Editor::shutdown()
    {
        LOG(NEXO_INFO, "Closing editor");
        LOG(NEXO_INFO, "All windows destroyed");
        m_windowRegistry.shutdown();
        ImGuiBackend::shutdown();
    }

    Editor::Editor()
    {
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

        auto darker     = ImVec4(20.f/255.f, 20.f/255.f, 20.f/255.f, 1.0f);

            // Apply the darker color to the title bar variants:
        style->Colors[ImGuiCol_TitleBg]         = darker;
        style->Colors[ImGuiCol_TitleBgActive]   = darker;
        style->Colors[ImGuiCol_TitleBgCollapsed] = darker;

        auto creamColor   = ImVec4(1.0f, 0.992f, 0.815f, 1.0f);  // Light cream
        auto creamHovered = ImVec4(1.0f, 1.0f, 0.9f, 1.0f);        // Slightly lighter when hovered
        auto creamActive  = ImVec4(1.0f, 0.95f, 0.8f, 1.0f);       // Slightly darker when active
        auto brighterActive = ImVec4(1.0f, 1.0f, 0.95f, 1.0f);

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

    void Editor::init()
    {
		setupEngine();
		setupStyle();
		m_windowRegistry.setup();
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
                if (ImGui::MenuItem("Exit"))
                    m_quit = true;

                ImGui::EndMenu();
            }
            ImGui::EndMainMenuBar();
        }
    }

    void Editor::buildDockspace()
    {
        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        const ImGuiID dockspaceID = viewport->ID;
        static bool dockingRegistryFilled = false;

        // If the dockspace node doesn't exist yet, create it
        if (!ImGui::DockBuilderGetNode(dockspaceID))
        {
            ImGui::DockBuilderRemoveNode(dockspaceID);
            ImGui::DockSpaceOverViewport(viewport->ID);
            ImGui::DockBuilderAddNode(dockspaceID, ImGuiDockNodeFlags_None);
            ImGui::DockBuilderSetNodeSize(dockspaceID, viewport->Size);

            // ─────────────────────────────────────────────
            // Step 1: Split off the rightmost column for Material Inspector.
            // We'll reserve 20% of the width for the Material Inspector.
            ImGuiID materialInspectorNode, remainingNode;
            ImGui::DockBuilderSplitNode(dockspaceID, ImGuiDir_Right, 0.20f, &materialInspectorNode, &remainingNode);
            // 'materialInspectorNode' will hold "Material Inspector"
            // 'remainingNode' now covers the remaining 80% of the dockspace.

            // ─────────────────────────────────────────────
            // Step 2: Split the remaining node horizontally into two columns.
            // Left column (main scene and console) will take 70% of the remaining width,
            // and the middle column (scene tree and Inspector) takes the other 30%.
            ImGuiID mainSceneColumn, inspectorColumn;
            ImGui::DockBuilderSplitNode(remainingNode, ImGuiDir_Right, 0.2f, &inspectorColumn, &mainSceneColumn);
            // 'mainSceneColumn' is ~70% of the remaining space.
            // 'inspectorColumn' is ~30% of the remaining space.

            // ─────────────────────────────────────────────
            // Step 3: In the left column (main scene), split vertically.
            // The main scene will be on top (70% height) and the console below (30% height).
            ImGuiID mainSceneTop, consoleNode;
            ImGui::DockBuilderSplitNode(mainSceneColumn, ImGuiDir_Down, 0.3f, &consoleNode, &mainSceneTop);

            // ─────────────────────────────────────────────
            // Step 4: In the middle column (inspector column), split vertically.
            // The scene tree goes on top and the Inspector (replacing the old console) goes below.
            // Here, we use a 50/50 split (adjust the ratio if needed).
            ImGuiID sceneTreeNode, inspectorNode;
            ImGui::DockBuilderSplitNode(inspectorColumn, ImGuiDir_Down, 0.5f, &inspectorNode, &sceneTreeNode);

            // ─────────────────────────────────────────────
            // Dock the windows into their corresponding nodes.
            ImGui::DockBuilderDockWindow("Default scene", mainSceneTop);
            ImGui::DockBuilderDockWindow("Console", consoleNode);
            ImGui::DockBuilderDockWindow("Scene Tree", sceneTreeNode);
            ImGui::DockBuilderDockWindow("Inspector", inspectorNode);
            ImGui::DockBuilderDockWindow("Material Inspector", materialInspectorNode);

            m_windowRegistry.setDockId("Default scene", mainSceneTop);
            m_windowRegistry.setDockId("Console", consoleNode);
            m_windowRegistry.setDockId("Scene Tree", sceneTreeNode);
            m_windowRegistry.setDockId("Inspector", inspectorNode);
            m_windowRegistry.setDockId("Material Inspector", materialInspectorNode);
            dockingRegistryFilled = true;

            g_materialInspectorDockID = materialInspectorNode;

            // Finish building the dock layout.
            ImGui::DockBuilderFinish(dockspaceID);
        }
        else if (!dockingRegistryFilled)
        {
        	m_windowRegistry.setDockId("Default scene", findWindowDockIDFromConfig("Default scene"));
        	m_windowRegistry.setDockId("Console", findWindowDockIDFromConfig("Console"));
        	m_windowRegistry.setDockId("Scene Tree", findWindowDockIDFromConfig("Scene Tree"));
        	m_windowRegistry.setDockId("Inspector", findWindowDockIDFromConfig("Inspector"));
        	m_windowRegistry.setDockId("Material Inspector", findWindowDockIDFromConfig("Material Inspector"));
         	dockingRegistryFilled = true;
        }

        // Render the dockspace
        ImGui::DockSpaceOverViewport(viewport->ID);
    }


    void Editor::render()
    {
    	getApp().beginFrame();
		ImVec4* colors = ImGui::GetStyle().Colors;
		colors[ImGuiCol_WindowBg].w = 0.0f; // 0.0f for full transparency
        ImGuiBackend::begin();

        ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
        ImGuizmo::BeginFrame();
        buildDockspace();

        drawMenuBar();
        //ImGui::ShowDemoWindow();

        m_windowRegistry.render();

        // Gradient background handling
        {
	        const ImGuiViewport* viewport = ImGui::GetMainViewport();
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

    void Editor::update()
    {
    	m_windowRegistry.update();
        getApp().endFrame();
    }
}
