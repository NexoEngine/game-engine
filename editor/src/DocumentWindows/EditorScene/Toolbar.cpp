//// Toolbar.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the toobal rendering of the editor scene
//
///////////////////////////////////////////////////////////////////////////////

#include "EditorScene.hpp"
#include "EntityFactory3D.hpp"
#include "IconsFontAwesome.h"
#include "context/Selector.hpp"
#include "components/Uuid.hpp"
#include "context/actions/EntityActions.hpp"
#include "context/ActionManager.hpp"
#include "../GameWindow/GameWindow.hpp"
#include "Editor.hpp"
#include <imgui_internal.h>
#include <format>

namespace nexo::editor {

    void EditorScene::createOrFocusGameWindow()
    {
        // Create or focus the game window
        auto& editor = Editor::getInstance();
        const std::string gameWindowName =
            std::format("Game View - {}{}{}", m_sceneUuid, NEXO_WND_USTRID_GAME_WINDOW, m_sceneId);

        // Check if game window already exists
        if (const auto gameWindow = editor.getWindow<GameWindow>(gameWindowName).lock()) {
            // Window exists, just make it visible
            gameWindow->setOpened(true);
        } else {
            // Get current EditorScene window's dock ID for docking the game window
            const std::string currentWindowName   = m_windowName;
            const ImGuiWindow* currentImGuiWindow = ImGui::FindWindowByName(currentWindowName.c_str());

            if (currentImGuiWindow && currentImGuiWindow->DockId) {
                // Create new game window
                editor.registerWindow<GameWindow>(gameWindowName);

                if (const auto newGameWindow = editor.getWindow<GameWindow>(gameWindowName).lock()) {
                    // Configure the game window
                    newGameWindow->setSceneId(static_cast<unsigned int>(m_sceneId));
                    newGameWindow->setSceneUuid(m_sceneUuid);
                    // Note: Removed setParentEditorScene() call as requested

                    // Set up the window
                    newGameWindow->setup();
                    newGameWindow->setOpened(true);

                    // Schedule dock split for next frame
                    m_shouldSplitDock       = true;
                    m_gameWindowNameToSplit = gameWindowName;

                    // Also schedule focus for after the split
                    m_shouldFocusGameWindow = true;
                    m_gameWindowToFocus     = gameWindowName;
                }
            }
        }
    }
    void EditorScene::spawnBallsScene(const glm::vec3& offset) const
    {
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_real_distribution<float> dis(0.0f, 1.0f);

        // Balls
        for (int i = 0; i < 50; ++i) {
            float x         = -3.0f + static_cast<float>(i % 5) * 1.5f;
            float z         = static_cast<float>((i % 2 == 0) ? 1 : -1) * 0.5f;
            glm::vec3 pos   = {x, 62.0f + static_cast<float>(i), z};
            glm::vec4 color = {1.0f, dis(gen), dis(gen), 1.0f};
            createEntityWithPhysic(pos + offset, {0.4f, 0.4f, 0.4f}, {0, 0, 0}, color, system::ShapeType::Sphere,
                                   JPH::EMotionType::Dynamic);
        }
    }

    void EditorScene::initialToolbarSetup(const float buttonWidth) const
    {
        ImVec2 toolbarPos = m_windowPos;
        const ImVec2 contentMin = ImGui::GetWindowContentRegionMin();
        toolbarPos.x += contentMin.x + 10.0f;
        toolbarPos.y += contentMin.y + 20.0f;

        ImGui::SetCursorScreenPos(toolbarPos);

        const auto toolbarSize = ImVec2(m_contentSize.x - buttonWidth, 50.0f);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.0f));
        ImGui::BeginChild("##ToolbarOverlay", toolbarSize, 0,
                          ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
                          ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                          ImGuiWindowFlags_NoSavedSettings);

        ImGui::SetCursorPosY((ImGui::GetWindowHeight() - ImGui::GetFrameHeight()) * 0.5f);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
    }

    bool EditorScene::renderToolbarButton(const std::string& uniqueId, const std::string& icon,
                                          const std::string& tooltip,
                                          const std::vector<ImNexo::GradientStop>& gradientStop, bool* rightClicked)
    {
        constexpr float buttonWidth = 35.0f;
        constexpr float buttonHeight = 35.0f;
        const bool clicked =
            ImNexo::IconGradientButton(uniqueId, icon, ImVec2(buttonWidth, buttonHeight), gradientStop);
        if (!tooltip.empty() && ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", tooltip.c_str());
        if (rightClicked != nullptr)
            *rightClicked = ImGui::IsItemClicked(ImGuiMouseButton_Right);
        return clicked;
    }

    void EditorScene::renderPrimitiveSubMenu(const ImVec2& primitiveButtonPos, const ImVec2& buttonSize,
                                             bool& showPrimitiveMenu)
    {
        auto& app = getApp();
        static const std::vector<ImNexo::ButtonProps> buttonProps =
        {
            {
                .uniqueId = "cube_primitive",
                .icon = ICON_FA_CUBE,
                .onClick = [this, &app]()
                    {
                        const ecs::Entity newCube = EntityFactory3D::createCube({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
                                                                               {0.0f, 0.0f, 0.0f}, {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f});
                        app.getSceneManager().getScene(this->m_sceneId).addEntity(newCube);
                        auto createAction = std::make_unique<EntityCreationAction>(newCube);
                        ActionManager::get().recordAction(std::move(createAction));
                    },
                .tooltip = "Create Cube"
            },
            {
                .uniqueId = "sphere_primitive",
                .icon = ICON_FA_CIRCLE,
                .onClick = [this]()
                {
                    // ImNexo::PrimitiveCustomizationMenu(this->m_sceneId, SPHERE);
                    this->m_popupManager.openPopup("Sphere creation popup");
                },
                .tooltip = "Create Sphere"
            },
            {
                .uniqueId = "cylinder_primitive",
                .icon = ICON_FA_PLUS,
                .onClick = [this]()
                {
                    // ImNexo::PrimitiveCustomizationMenu(this->m_sceneId, CYLINDER);
                    this->m_popupManager.openPopup("Cylinder creation popup");
                },
                .tooltip = "Create Cylinder"
            },
            {
                .uniqueId = "pyramid_primitive",
                .icon = ICON_FA_PLUS,
                .onClick = [this, &app]()
                {
                    const ecs::Entity newPyramid = EntityFactory3D::createPyramid(
                        {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
                        {0.0f, 0.0f, 0.0f}, {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f});
                    app.getSceneManager().getScene(this->m_sceneId).addEntity(newPyramid);
                    auto createAction = std::make_unique<EntityCreationAction>(newPyramid);
                    ActionManager::get().recordAction(std::move(createAction));
                },
                .tooltip = "Create Pyramid"
            },
            {
                .uniqueId = "tetrahedron_primitive",
                .icon = ICON_FA_PLUS,
                .onClick = [this, &app]()
                {
                    const ecs::Entity newTetrahedron = EntityFactory3D::createTetrahedron(
                        {0.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 1.0f},
                        {0.0f, 0.0f, 0.0f}, {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f});
                    app.getSceneManager().getScene(this->m_sceneId).addEntity(newTetrahedron);
                    auto createAction = std::make_unique<EntityCreationAction>(newTetrahedron);
                    ActionManager::get().recordAction(std::move(createAction));
                },
                .tooltip = "Create Tetrahedron"
            },
        };
        ImNexo::ButtonDropDown(primitiveButtonPos, buttonSize, buttonProps, showPrimitiveMenu);
    }

    void EditorScene::renderSnapSubMenu(const ImVec2& snapButtonPos, const ImVec2& buttonSize, bool& showSnapMenu)
    {
        const std::vector<ImNexo::ButtonProps> buttonProps =
        {
            {
                .uniqueId = "toggle_translate_snap",
                .icon = ICON_FA_TH,
                .onClick = [this]()
                {
                    this->m_snapTranslateOn = !this->m_snapTranslateOn;
                },
                .onRightClick = [this]()
                {
                    this->m_popupManager.openPopup("Snap settings popup", ImVec2(400, 140));
                },
                .tooltip = "Toggle Translate Snap",
                .buttonGradient = m_snapTranslateOn ? m_selectedGradient : m_buttonGradient
            },
            {
                .uniqueId = "toggle_rotate_snap",
                .icon = ICON_FA_BULLSEYE,
                .onClick = [this]()
                {
                    this->m_snapRotateOn = !m_snapRotateOn;
                },
                .onRightClick = [this]()
                {
                    this->m_popupManager.openPopup("Snap settings popup", ImVec2(400, 140));
                },
                .tooltip = "Toggle Rotate Snap",
                .buttonGradient = m_snapRotateOn ? m_selectedGradient : m_buttonGradient
            }
            // Snap on scale is kinda strange, the IsOver is not able to detect it, so for now we disable it
            // {
            //     .uniqueId = "toggle_scale_snap",
            //     .icon = ICON_FA_EXPAND,
            //     .onClick = [this]()
            //         {
            //             this->m_snapScaleOn = !m_snapScaleOn;
            //         },
            //     .onRightClick = [this]()
            //         {
            //             this->m_popupManager.openPopup("Snap settings popup", ImVec2(400, 180));
            //         },
            //     .tooltip = "Toggle Scale Snap",
            //     .buttonGradient = (m_snapScaleOn) ? m_selectedGradient : buttonGradient
            // }
        };
        ImNexo::ButtonDropDown(snapButtonPos, buttonSize, buttonProps, showSnapMenu);
    }

    void EditorScene::snapSettingsPopup()
    {
        if (m_popupManager.showPopupModal("Snap settings popup"))
        {
            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
            ImGui::Indent(10.0f);

            if (ImGui::BeginTable("TranslateSnap", 4,
                                  ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn(
                    "##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn(
                    "##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn(
                    "##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn(
                    "##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImNexo::RowDragFloat3("Translate Snap", "X", "Y", "Z", &this->m_snapTranslate.x);
                ImGui::EndTable();
            }

            if (ImGui::BeginTable("ScaleAndRotateSnap", 4,
                                  ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn(
                    "##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn(
                    "##Value", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                // Empty columns to match the first table's structure
                ImGui::TableSetupColumn(
                    "##Empty1", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn(
                    "##Empty2", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

                ImNexo::RowDragFloat1("Rotate Snap", "", &this->m_angleSnap);
                ImGui::EndTable();
            }
            ImGui::Spacing();
            ImGui::Spacing();

            constexpr float buttonWidth = 120.0f;
            const float windowWidth = ImGui::GetWindowSize().x;
            ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

            if (ImNexo::Button("OK", ImVec2(buttonWidth, 0.0f)))
            {
                PopupManager::closePopup();
            }
            ImGui::Unindent(10.0f);
            ImGui::PopStyleVar();
            PopupManager::endPopup();
        }
    }

    void EditorScene::gridSettingsPopup()
    {
        if (m_popupManager.showPopupModal("Grid settings"))
        {
            components::RenderContext::GridParams& gridSettings =
                Application::m_coordinator->getSingletonComponent<components::RenderContext>().gridParams;

            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
            ImGui::Indent(10.0f);

            if (ImGui::BeginTable("GridSettings", 2,
                                  ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn(
                    "##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn(
                    "##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImNexo::RowDragFloat1("Grid size", "", &gridSettings.gridSize, 50.0f, 150.0f);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("The total size of the grid");
                ImNexo::RowDragFloat1("Pixel cell spacing", "", &gridSettings.minPixelsBetweenCells, 0.0f, 100.0f,
                                      0.1f);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Level of detail of internal cells");
                ImNexo::RowDragFloat1("Cell size", "", &gridSettings.cellSize, 0.1f, 20.0f, 0.02f);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("The size of the internal cells");
                ImGui::EndTable();
            }

            ImGui::Spacing();
            ImGui::Spacing();

            constexpr float buttonWidth = 120.0f;
            const float windowWidth = ImGui::GetWindowSize().x;
            ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

            if (ImNexo::Button("OK", ImVec2(buttonWidth, 0.0f)))
            {
                PopupManager::closePopup();
            }
            ImGui::Unindent(10.0f);
            ImGui::PopStyleVar();
            PopupManager::endPopup();
        }
    }

    void EditorScene::renderEditorCameraToolbarButton()
    {
        auto& selector = Selector::get();
        if (m_activeCamera == m_editorCamera)
        {
            if (renderToolbarButton("editor_camera", ICON_FA_CAMERA, "Edit Editor Camera Setting", m_buttonGradient))
            {
                const auto& uuidComponent = Application::m_coordinator->getComponent<components::UuidComponent>(
                    m_editorCamera);
                selector.addToSelection(uuidComponent.uuid, m_editorCamera);
            }
        }
        else
        {
            if (renderToolbarButton("switch_back", ICON_FA_EXCHANGE, "Switch back to editor camera", m_buttonGradient))
            {
                auto& oldCameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(
                    m_activeCamera);
                oldCameraComponent.active = false;
                oldCameraComponent.render = false;
                m_activeCamera = m_editorCamera;
                auto& editorCameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(
                    m_activeCamera);
                editorCameraComponent.render = true;
                editorCameraComponent.active = true;
            }
        }
    }

    bool EditorScene::renderGizmoModeToolbarButton(const bool showGizmoModeMenu, ImNexo::ButtonProps& activeGizmoMode,
                                                   ImNexo::ButtonProps& inactiveGizmoMode)
    {
        static const ImNexo::ButtonProps gizmoLocalModeButtonProps = {
            "local_coords", ICON_FA_CROSSHAIRS, [this]() { this->m_currentGizmoMode = ImGuizmo::MODE::LOCAL; }, nullptr,
            "Local coordinates"
        };
        static const ImNexo::ButtonProps gizmoWorldModeButtonProps = {
            "world_coords", ICON_FA_GLOBE, [this]() { this->m_currentGizmoMode = ImGuizmo::MODE::WORLD; }, nullptr,
            "World coordinates"
        };
        if (m_currentGizmoMode == ImGuizmo::MODE::LOCAL)
        {
            activeGizmoMode = gizmoLocalModeButtonProps;
            inactiveGizmoMode = gizmoWorldModeButtonProps;
        }
        else
        {
            activeGizmoMode = gizmoWorldModeButtonProps;
            inactiveGizmoMode = gizmoLocalModeButtonProps;
        }
        return renderToolbarButton(activeGizmoMode.uniqueId, activeGizmoMode.icon,
                                   activeGizmoMode.tooltip, showGizmoModeMenu ? m_selectedGradient : m_buttonGradient);
    }

    void EditorScene::renderToolbar()
    {
        constexpr float buttonWidth = 35.0f;
        constexpr float buttonHeight = 35.0f;
        constexpr ImVec2 buttonSize{buttonWidth, buttonHeight};
        ImVec2 originalCursorPos = ImGui::GetCursorPos();

        initialToolbarSetup(buttonWidth);

        // -------------------------------- BUTTONS -------------------------------
        // -------- Add primitve button --------
        // This can open a submenu, see at the end
        ImVec2 addPrimButtonPos = ImGui::GetCursorScreenPos();
        static bool showPrimitiveMenu = false;
        bool addPrimitiveClicked = renderToolbarButton(
            "add_primitive", ICON_FA_PLUS_SQUARE,
            "Add primitive", showPrimitiveMenu ? m_selectedGradient : m_buttonGradient);
        if (addPrimitiveClicked)
            showPrimitiveMenu = !showPrimitiveMenu;

        ImGui::SameLine();

        // -------- Editor camera settings / Switch back to editor camera button --------
        renderEditorCameraToolbarButton();

        ImGui::SameLine();

        // -------- Gizmo operation button --------
        static const auto gizmoTranslateButtonProps = ImNexo::ButtonProps{
            "translate", ICON_FA_ARROWS, [this]() { this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE; },
            nullptr, "Translate"
        };
        static const auto gizmoRotateButtonProps = ImNexo::ButtonProps{
            "rotate", ICON_FA_REFRESH, [this]() { this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE; },
            nullptr, "Rotate"
        };
        static const auto gizmoScaleButtonProps = ImNexo::ButtonProps{
            "scale", ICON_FA_EXPAND, [this]() { this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE; }, nullptr,
            "Scale"
        };
        static const auto gizmoUniversalButtonProps = ImNexo::ButtonProps{
            "universal", ICON_FA_ARROWS_ALT,
            [this]() { this->m_currentGizmoOperation = ImGuizmo::OPERATION::UNIVERSAL; }, nullptr, "Universal"
        };
        std::vector<ImNexo::ButtonProps> gizmoButtons = {
            gizmoTranslateButtonProps,
            gizmoRotateButtonProps,
            gizmoScaleButtonProps,
            gizmoUniversalButtonProps
        };

        ImNexo::ButtonProps activeOp;
        switch (m_currentGizmoOperation)
        {
        case ImGuizmo::OPERATION::TRANSLATE:
            activeOp = gizmoTranslateButtonProps;
            std::erase_if(gizmoButtons, [](const auto& prop) { return prop.uniqueId == "translate"; });
            break;
        case ImGuizmo::OPERATION::ROTATE:
            activeOp = gizmoRotateButtonProps;
            std::erase_if(gizmoButtons, [](const auto& prop) { return prop.uniqueId == "rotate"; });
            break;
        case ImGuizmo::OPERATION::SCALE:
            activeOp = gizmoScaleButtonProps;
            std::erase_if(gizmoButtons, [](const auto& prop) { return prop.uniqueId == "scale"; });
            break;
        case ImGuizmo::OPERATION::UNIVERSAL:
            activeOp = gizmoUniversalButtonProps;
            std::erase_if(gizmoButtons, [](const auto& prop) { return prop.uniqueId == "universal"; });
            break;
        default:
            break;
        }

        ImVec2 changeGizmoOpPos = ImGui::GetCursorScreenPos();
        static bool showGizmoOpMenu = false;
        bool changeGizmoOpClicked = renderToolbarButton(
            activeOp.uniqueId, activeOp.icon,
            activeOp.tooltip, showGizmoOpMenu ? m_selectedGradient : m_buttonGradient);
        if (changeGizmoOpClicked)
            showGizmoOpMenu = !showGizmoOpMenu;

        ImGui::SameLine();

        // -------- Gizmo operation button --------
        ImNexo::ButtonProps activeGizmoMode;
        ImNexo::ButtonProps inactiveGizmoMode;
        ImVec2 changeGizmoModePos = ImGui::GetCursorScreenPos();
        static bool showGizmoModeMenu = false;
        bool changeGizmoModeClicked = renderGizmoModeToolbarButton(showGizmoModeMenu, activeGizmoMode,
                                                                   inactiveGizmoMode);
        if (changeGizmoModeClicked)
            showGizmoModeMenu = !showGizmoModeMenu;

        ImGui::SameLine();

        // -------- Toggle snap button --------
        // This can open a submenu, see at the end
        ImVec2 toggleSnapPos = ImGui::GetCursorScreenPos();
        static bool showSnapToggleMenu = false;
        bool snapOn = m_snapRotateOn || m_snapTranslateOn;
        bool toggleSnapClicked = renderToolbarButton("toggle_snap", ICON_FA_MAGNET, "Toggle gizmo snap",
                                                     (showSnapToggleMenu || snapOn)
                                                         ? m_selectedGradient
                                                         : m_buttonGradient);
        if (toggleSnapClicked)
            showSnapToggleMenu = !showSnapToggleMenu;

        ImGui::SameLine();

        // -------- Grid enabled button --------
        bool rightClicked = false;
        components::RenderContext::GridParams& gridParams = Application::m_coordinator->getSingletonComponent<
            components::RenderContext>().gridParams;
        if (renderToolbarButton("grid_enabled", ICON_FA_TH_LARGE, "Enable / Disable grid",
                                gridParams.enabled ? m_selectedGradient : m_buttonGradient, &rightClicked))
        {
            gridParams.enabled = !gridParams.enabled;
        }
        if (rightClicked)
            m_popupManager.openPopup("Grid settings", ImVec2(300, 180));

        ImGui::SameLine();

        // -------- Snap to gridbutton --------
        // NOTE: This seems complicated to implement using ImGuizmo, we leave it for now but i dont know if it will be implemented
        if (renderToolbarButton("snap_to_grid", ICON_FA_TH,
                                "Enable snapping to grid\n(only horizontal translation and scaling)",
                                m_snapToGrid ? m_selectedGradient : m_buttonGradient))
        {
            m_snapToGrid = !m_snapToGrid;
        }

        ImGui::SameLine();

        // -------- Enable wireframe button --------
        if (renderToolbarButton("wireframe", ICON_FA_CUBE, "Enable / Disable wireframe",
                                m_wireframeEnabled ? m_selectedGradient : m_buttonGradient))
        {
            m_wireframeEnabled = !m_wireframeEnabled;
        }

        ImGui::SameLine();

        auto& app = getApp();
        const bool isPlaying = app.getGameState() == nexo::GameState::PLAY_MODE;
        
        const char* icon = isPlaying ? ICON_FA_STOP : ICON_FA_PLAY;
        const char* tooltip = isPlaying ? "Stop scene" : "Play scene";
        const auto& gradient = isPlaying ? m_selectedGradient : m_buttonGradient;
        
        if (renderToolbarButton("play_stop", icon, tooltip, gradient))
        {
            if (isPlaying) {
                app.setGameState(nexo::GameState::EDITOR_MODE);
            } else {
                app.setGameState(nexo::GameState::PLAY_MODE);
            }
        }

        ImGui::PopStyleVar();
        ImGui::EndChild();
        ImGui::PopStyleColor();

        // -------------------------------- SUB-MENUS -------------------------------
        // -------- Primitives sub-menus --------
        if (showPrimitiveMenu)
        {
            renderPrimitiveSubMenu(addPrimButtonPos, buttonSize, showPrimitiveMenu);
        }

        // -------- Gizmo operation sub-menu --------
        if (showGizmoOpMenu)
        {
            ImNexo::ButtonDropDown(changeGizmoOpPos, buttonSize, gizmoButtons, showGizmoOpMenu);
        }

        // -------- Gizmo mode sub-menu --------
        if (showGizmoModeMenu)
        {
            ImNexo::ButtonDropDown(changeGizmoModePos, buttonSize, {inactiveGizmoMode}, showGizmoModeMenu);
        }

        // -------- Snap sub-menu --------
        if (showSnapToggleMenu)
        {
            renderSnapSubMenu(toggleSnapPos, buttonSize, showSnapToggleMenu);
        }

        // -------- Snap settings popup --------
        snapSettingsPopup();

        // -------- Grid settings popup --------
        gridSettingsPopup();

        // IMPORTANT: Restore original cursor position so we don't affect layout
        ImGui::SetCursorPos(originalCursorPos);
    }
}
