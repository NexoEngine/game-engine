//// Editor.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        09/11/2024
//  Description: Source file for the main editor class
//
//////////////////////////////////////////////////////////////////////////////

#include "ADocumentWindow.hpp"

#include "utils/Config.hpp"
#include "Nexo.hpp"
#include "Editor.hpp"
#include "Logger.hpp"
#include "Path.hpp"
#include "backends/ImGuiBackend.hpp"
#include "IconsFontAwesome.h"
#include "ImNexo/Elements.hpp"
#include "context/ActionManager.hpp"
#include "DocumentWindows/TestWindow/TestWindow.hpp"

#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui.h"
#include <imgui_internal.h>
#include <ImGuizmo.h>
#include <algorithm>

#include "DocumentWindows/EditorScene/EditorScene.hpp"
#include "DocumentWindows/InspectorWindow/InspectorWindow.hpp"

namespace nexo::editor {

    void Editor::shutdown() const
    {
        Application& app = Application::getInstance();

        app.shutdownScripting();
        LOG(NEXO_INFO, "Closing editor");
        LOG(NEXO_INFO, "All windows destroyed");
        m_windowRegistry.shutdown();
        ImGuiBackend::shutdown();
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
        getApp().getWindow()->getDpiScale(&scaleFactorX, &scaleFactorY);
        getApp().getWindow()->setWindowIcon(Path::resolvePathRelativeToExe(
            "../resources/nexo.png"));
        if (scaleFactorX > 1.0f || scaleFactorY > 1.0f)
        {
            LOG(NEXO_WARN,
                "Scale factor is greater than 1.0, if you have any issue try adjusting the system's scale factor");
            LOG(NEXO_INFO, "DPI scale: x: {}, y: {}", scaleFactorX, scaleFactorY);
        }

        LOG(NEXO_INFO, "ImGui version: {}", IMGUI_VERSION);

        ImGuiIO &io = ImGui::GetIO();
        io.DisplaySize = ImVec2(static_cast<float>(getApp().getWindow()->getWidth()),
                                static_cast<float>(getApp().getWindow()->getHeight()));
        io.DisplayFramebufferScale = ImVec2(scaleFactorX, scaleFactorY); // Apply the DPI scale to ImGui rendering
        io.ConfigWindowsMoveFromTitleBarOnly = true;
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;


        ImGuiStyle *style = &ImGui::GetStyle();
        style->CircleTessellationMaxError = 0.10f;
        style->WindowRounding = 10.0f;
        style->ChildRounding = 6.0f;
        style->PopupRounding = 4.0f;
        style->WindowMenuButtonPosition = ImGuiDir_Right;
        style->ScaleAllSizes(std::max(scaleFactorX, scaleFactorY));

        // Setup NEXO Color Scheme
        ImVec4* colors = style->Colors;
        constexpr auto colWindowBg                  = ImVec4(0.02f, 0.02f, 0.04f, 0.59f); // Every color above it will depend on it because of the alpha
        constexpr auto colTitleBg                   = ImVec4(0.00f, 0.00f, 0.00f, 0.28f);
        constexpr auto colTitleBgActive             = ImVec4(0.00f, 0.00f, 0.00f, 0.31f);
        constexpr auto colTabSelectedOverline       = ImVec4(0.30f, 0.12f, 0.45f, 0.85f);
        constexpr auto colTabDimmedSelectedOverline = ImVec4(0.29f, 0.12f, 0.43f, 0.15f);

        // Dependent colors
        // We want the tabs to have the same color as colWindowBg, but titleBg is under tabs, so we subtract titleBg
        constexpr auto colTab               = ImVec4(0, 0, 0, (colWindowBg.w - colTitleBgActive.w) * 0.60f);
        constexpr auto colTabDimmed         = ImVec4(0, 0, 0, colTab.w * 0.90f);
        constexpr auto colTabSelected       = ImVec4(0, 0, 0, colWindowBg.w - colTitleBg.w);
        constexpr auto colTabDimmedSelected = ImVec4(0, 0, 0, colTabSelected.w);
        constexpr auto colTabHovered        = ImVec4(0.33f, 0.25f, 0.40f, colWindowBg.w - colTitleBg.w);

        // Depending definitions
        colors[ImGuiCol_WindowBg]               = colWindowBg;
        colors[ImGuiCol_TitleBg]                = colTitleBg;
        colors[ImGuiCol_TitleBgActive]          = colTitleBgActive;
        colors[ImGuiCol_TitleBgCollapsed]       = colTitleBg;
        colors[ImGuiCol_Tab]                    = colTab;
        colors[ImGuiCol_TabSelected]            = colTabSelected;
        colors[ImGuiCol_TabDimmed]              = colTabDimmed;
        colors[ImGuiCol_TabDimmedSelected]      = colTabDimmedSelected;
        colors[ImGuiCol_TabSelectedOverline]    = colTabSelectedOverline;
        colors[ImGuiCol_TabDimmedSelectedOverline]  = colTabDimmedSelectedOverline;
        colors[ImGuiCol_TabHovered]             = colTabHovered;

        // Static definitions
        const ImVec4 whiteText = colors[ImGuiCol_Text];

        colors[ImGuiCol_Border]                 = ImVec4(0.08f, 0.08f, 0.25f, 0.19f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.49f, 0.63f, 0.71f, 0.15f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.49f, 0.63f, 0.71f, 0.15f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.59f, 0.73f, 0.81f, 0.15f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.58f, 0.14f, 0.14f, 0.10f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.20f, 0.20f, 0.20f, 0.34f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.30f, 0.30f, 0.30f, 0.69f);
        colors[ImGuiCol_TextTab]                = whiteText;
        colors[ImGuiCol_TextTabDimmed]          = whiteText;
        colors[ImGuiCol_TextTabHovered]         = whiteText;
        colors[ImGuiCol_TextTabSelected]        = whiteText;
        colors[ImGuiCol_TextTabDimmedSelected]  = whiteText;
        colors[ImGuiCol_Header]                 = ImVec4(0.49f, 0.63f, 0.71f, 0.15f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.49f, 0.63f, 0.71f, 0.30f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.49f, 0.63f, 0.71f, 0.45f);
        colors[ImGuiCol_Button]                 = ImVec4(0.49f, 0.63f, 0.71f, 0.15f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.49f, 0.63f, 0.71f, 0.30f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.49f, 0.63f, 0.71f, 0.45f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.05f * 1.5f, 0.09f * 1.15f, 0.13f * 1.25, 1.0f);

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
        float iconFontSize = fontSize * 2.0f / 3.0f;

        static const std::string sourceSansPath = Path::resolvePathRelativeToExe(
            "../resources/fonts/SourceSans3-Regular.ttf").string();
        ImFont *font = io.Fonts->AddFontFromFileTTF(sourceSansPath.c_str(), fontSize,
                                                    &fontConfig);
        LOG(NEXO_DEBUG, "Font path: {}", sourceSansPath);
        IM_ASSERT(font != nullptr);
        io.FontDefault = font;

        ImGuiBackend::initFontAtlas();

        ImFontConfig fontawesome_config;
        fontawesome_config.MergeMode = true; // Merge fontawesome with the default font
        fontawesome_config.OversampleH = 3; // Horizontal oversampling
        fontawesome_config.OversampleV = 3; // Vertical oversampling
        //fontawesome_config.GlyphMinAdvanceX = 7.0f; // Use if you want to make the icon monospaced
        //fontawesome_config.GlyphMaxAdvanceX = 7.0f; // Use if you want to make the icon monospaced
        fontawesome_config.PixelSnapH = true; // Snap to pixel grid, useful for pixel-perfect rendering
        //fontawesome_config.GlyphExtraSpacing = ImVec2(0.0f, 0.0f); // Adds space between icon and text

        fontawesome_config.GlyphMinAdvanceX = iconFontSize; // Make the icons monospaced and aligned
        fontawesome_config.GlyphMaxAdvanceX = iconFontSize; // Make the icons monospaced and aligned



        static constexpr ImWchar icon_ranges[] = { ICON_MIN_FA, ICON_MAX_FA, 0 };
        static const std::string fontawesomePath = Path::resolvePathRelativeToExe(
            "../resources/fonts/fontawesome4.ttf").string();
        io.Fonts->AddFontFromFileTTF(fontawesomePath.c_str(), iconFontSize, &fontawesome_config, icon_ranges);

        LOG(NEXO_DEBUG, "Fonts initialized");
    }

    void Editor::init() const
    {
		setupEngine();
		setupStyle();
		m_windowRegistry.setup();

        const Application& app = Application::getInstance();
        app.initScripting(); // TODO: scripting is init here since it requires a scene, later scenes shouldn't be created in the editor window
        for (const auto inspectorWindow : m_windowRegistry.getWindows<InspectorWindow>())
            inspectorWindow->registerTypeErasedProperties(); // TODO: this should be done in the InspectorWindow constructor, but we need the scripting to init
    }

    bool Editor::isOpen() const
    {
        return !m_quit && getApp().isWindowOpen() && getApp().isRunning();
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
        if (!ImGui::DockBuilderGetNode(dockspaceID)) {
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
            const std::string defaultSceneUniqueStrId = std::format("{}{}", NEXO_WND_USTRID_DEFAULT_SCENE, 0); // for the default scene
            ImGui::DockBuilderDockWindow(defaultSceneUniqueStrId.c_str(), mainSceneTop);
            ImGui::DockBuilderDockWindow(NEXO_WND_USTRID_CONSOLE, consoleNode);
            ImGui::DockBuilderDockWindow(NEXO_WND_USTRID_SCENE_TREE, sceneTreeNode);
            ImGui::DockBuilderDockWindow(NEXO_WND_USTRID_INSPECTOR, inspectorNode);
            ImGui::DockBuilderDockWindow(NEXO_WND_USTRID_MATERIAL_INSPECTOR, materialInspectorNode);
            ImGui::DockBuilderDockWindow(NEXO_WND_USTRID_ASSET_MANAGER, consoleNode);

            m_windowRegistry.setDockId(defaultSceneUniqueStrId, mainSceneTop);
            m_windowRegistry.setDockId(NEXO_WND_USTRID_CONSOLE, consoleNode);
            m_windowRegistry.setDockId(NEXO_WND_USTRID_SCENE_TREE, sceneTreeNode);
            m_windowRegistry.setDockId(NEXO_WND_USTRID_INSPECTOR, inspectorNode);
            m_windowRegistry.setDockId(NEXO_WND_USTRID_MATERIAL_INSPECTOR, materialInspectorNode);
            m_windowRegistry.setDockId(NEXO_WND_USTRID_ASSET_MANAGER, consoleNode);
            dockingRegistryFilled = true;

            // Finish building the dock layout.
            ImGui::DockBuilderFinish(dockspaceID);
        }
        else if (!dockingRegistryFilled) {
            setAllWindowDockIDsFromConfig(m_windowRegistry);
         	dockingRegistryFilled = true;
        }

        // Render the dockspace
        ImGui::DockSpaceOverViewport(viewport->ID);
    }

    void Editor::handleGlobalCommands()
    {
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey_Z))
        {
            if (ImGui::IsKeyDown(ImGuiKey_LeftShift))
            {
                ActionManager::get().redo();
            }
            else
            {
                ActionManager::get().undo();
            }
        }
        if (ImGui::IsKeyDown(ImGuiKey_LeftCtrl) && ImGui::IsKeyDown(ImGuiKey_LeftShift) && ImGui::IsKeyPressed(ImGuiKey_T))
        {
            if (auto testWindow = getWindow<nexo::editor::TestWindow>(NEXO_WND_USTRID_TEST).lock()) {
                testWindow->setOpened(true);
            } else {
                registerWindow<nexo::editor::TestWindow>(NEXO_WND_USTRID_TEST);
                getWindow<nexo::editor::TestWindow>(NEXO_WND_USTRID_TEST).lock()->setup();
            }
        }
    }

    std::vector<CommandInfo> Editor::handleFocusedWindowCommands()
    {
        std::vector<CommandInfo> possibleCommands;
        static std::vector<CommandInfo> lastValidCommands; // Store the last valid set of commands
        static float commandDisplayTimer = 0.0f;           // Track how long to show last commands

        auto focusedWindow = m_windowRegistry.getFocusedWindow();
        if (focusedWindow)
        {
            const WindowState currentState = m_windowRegistry.getFocusedWindow()->getWindowState();
            m_inputManager.processInputs(currentState);
            possibleCommands = m_inputManager.getPossibleCommands(currentState);

            // Update the last valid commands if we have any
            if (!possibleCommands.empty())
            {
                constexpr float commandPersistTime = 2.0f;
                lastValidCommands = possibleCommands;
                commandDisplayTimer = commandPersistTime; // Reset timer
            }
            else if (commandDisplayTimer > 0.0f)
            {
                // Use the last valid commands if timer is still active
                possibleCommands = lastValidCommands;
                commandDisplayTimer -= ImGui::GetIO().DeltaTime;
            }
            else if (lastValidCommands.empty())
            {
                // Fallback: If we've never had commands, grab all possible commands from the window
                // This is a more complex operation but ensures we always have something to show
                possibleCommands = m_inputManager.getAllPossibleCommands(currentState);
                lastValidCommands = possibleCommands;
            }
            else
            {
                // Use the last valid set of commands
                possibleCommands = lastValidCommands;
            }
        }
        return possibleCommands;
    }

    void Editor::drawShortcutBar(const std::vector<CommandInfo> &possibleCommands) const
    {
        constexpr float bottomBarHeight = 38.0f;
        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.05f, 0.06f, 0.12f, 0.85f)); // Matches your dark blue theme
        ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.75f, 0.75f, 0.75f, 1.0f));
        const auto viewport = ImGui::GetMainViewport();

        ImGui::SetNextWindowPos(ImVec2(viewport->Pos.x, viewport->Pos.y + viewport->Size.y - bottomBarHeight));
        ImGui::SetNextWindowSize(ImVec2(viewport->Size.x, bottomBarHeight));
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGuiWindowFlags bottomBarFlags =
            ImGuiWindowFlags_NoDecoration |
            ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoResize |
            ImGuiWindowFlags_NoScrollbar |
            ImGuiWindowFlags_NoNav |
            ImGuiWindowFlags_NoDocking |
            ImGuiWindowFlags_NoFocusOnAppearing |
            ImGuiWindowFlags_NoInputs |
            ImGuiWindowFlags_NoBackground;

        if (ImGui::Begin(NEXO_WND_USTRID_BOTTOM_BAR, nullptr, bottomBarFlags))
        {
            constexpr float textScaleFactor = 0.90f; // 15% larger text
            ImGui::SetWindowFontScale(textScaleFactor);
            // Vertically center the content
            const float windowHeight = ImGui::GetWindowHeight();
            const float textHeight = ImGui::GetTextLineHeight();
            const float paddingY = (windowHeight - textHeight) * 0.5f;

            // Apply the vertical padding
            ImGui::SetCursorPosY(paddingY);

            // Start with a small horizontal padding
            ImGui::SetCursorPosX(10.0f);

            if (!possibleCommands.empty())
            {
                ImDrawList* drawList = ImGui::GetWindowDrawList();

                // Use horizontal layout for commands, left-aligned
                for (const auto& cmd : possibleCommands)
                {
                    // Calculate text sizes for proper positioning and border sizing
                    ImVec2 keySize = ImGui::CalcTextSize(cmd.key.c_str());
                    ImVec2 colonSize = ImGui::CalcTextSize(":");
                    ImVec2 descSize = ImGui::CalcTextSize(cmd.description.c_str());

                    // Position of the start of this command
                    const ImVec2 commandStart = ImGui::GetCursorScreenPos();

                    // Total size of command group with padding
                    const float commandWidth = keySize.x + colonSize.x + 5.0f + descSize.x;
                    const float commandHeight = std::max(keySize.y, std::max(colonSize.y, descSize.y));

                    // Add padding around the entire command
                    constexpr float borderPadding = 6.0f;
                    constexpr float borderCornerRadius = 3.0f;

                    // Draw the gradient border rectangle
                    const auto rectMin = ImVec2(commandStart.x - borderPadding, commandStart.y - borderPadding);
                    const auto rectMax = ImVec2(commandStart.x + commandWidth + borderPadding,
                                           commandStart.y + commandHeight + borderPadding);

                    // Draw gradient border rectangle
                    drawList->AddRect(
                        rectMin,
                        rectMax,
                        IM_COL32(58, 124, 161, 200),  // Gradient start color
                        borderCornerRadius,
                        0,
                        1.5f  // Border thickness
                    );

                    // Dark inner background
                    drawList->AddRectFilled(
                        ImVec2(rectMin.x + 1, rectMin.y + 1),
                        ImVec2(rectMax.x - 1, rectMax.y - 1),
                        IM_COL32(10, 11, 25, 200),  // Dark inner background
                        borderCornerRadius - 0.5f
                    );

                    // Draw the command components
                    const std::string &key = cmd.key + ":";
                    ImGui::TextColored(ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "%s", key.c_str());
                    ImGui::SameLine(0.0f, 5.0f);
                    ImGui::Text("%s", cmd.description.c_str());

                    // Add space between commands
                    ImGui::SameLine(0.0f, 20.0f);

                    // Update cursor position to account for the border we added
                    ImGui::SetCursorScreenPos(ImVec2(ImGui::GetCursorScreenPos().x, commandStart.y));
                }
            }
        }
        ImGui::End();
        ImGui::PopStyleColor(2); // Pop both text and bg colors
    }

    void Editor::drawBackground() const
    {
        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->Pos);
        ImGui::SetNextWindowSize(viewport->Size);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::Begin("Background", nullptr,
                        ImGuiWindowFlags_NoDecoration |
                        ImGuiWindowFlags_NoInputs |
                        ImGuiWindowFlags_NoFocusOnAppearing |
                        ImGuiWindowFlags_NoBringToFrontOnFocus |
                        ImGuiWindowFlags_NoMove |
                        ImGuiWindowFlags_NoResize |
                        ImGuiWindowFlags_NoSavedSettings |
                        ImGuiWindowFlags_NoScrollbar |
                        ImGuiWindowFlags_NoBackground);

        const std::vector<ImNexo::GradientStop> stops = {
            { 0.06f, IM_COL32(58, 124, 161, 255) },
            {0.26f, IM_COL32(88, 87, 154, 255) },
            { 0.50f, IM_COL32(88, 87, 154, 255) },
            {0.73f, IM_COL32(58, 124, 161, 255) },
        };

        constexpr float angle = 148;

        ImNexo::RectFilledLinearGradient(viewport->Pos,
                ImVec2(viewport->Pos.x + viewport->Size.x, viewport->Pos.y + viewport->Size.y), angle, stops);

        ImGui::End();
    }

    void Editor::render()
    {
    	getApp().beginFrame();

        ImGuiBackend::begin();

        ImGuizmo::SetImGuiContext(ImGui::GetCurrentContext());
        ImGuizmo::BeginFrame();
        buildDockspace();

        drawMenuBar();
        m_windowRegistry.render();

        handleGlobalCommands();

        // Get the commands to display in the bottom bar
        const std::vector<CommandInfo> possibleCommands = handleFocusedWindowCommands();
        drawShortcutBar(possibleCommands);
        drawBackground();

        ImGui::Render();

        ImGuiBackend::end(nexo::getApp().getWindow());
    }

    void Editor::update() const
    {
    	m_windowRegistry.update();
        Application& app = Application::getInstance();
        getApp().endFrame();
    }
}
