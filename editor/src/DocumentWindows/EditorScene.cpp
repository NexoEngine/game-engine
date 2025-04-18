//// EditorScene.cpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        10/11/2024
//  Description: Source for the editor scene document window
//
///////////////////////////////////////////////////////////////////////////////

#include "EditorScene.hpp"

#include <ImGuizmo.h>
#include <Path.hpp>

#include "ADocumentWindow.hpp"
#include "Application.hpp"
#include "EntityFactory3D.hpp"
#include "IconsFontAwesome.h"
#include "LightFactory.hpp"
#include "CameraFactory.hpp"
#include "Nexo.hpp"
#include "Texture.hpp"
#include "WindowRegistry.hpp"
#include "Components/Widgets.hpp"
#include "Components/EntityPropertiesComponents.hpp"
#include "components/Camera.hpp"
#include "components/Uuid.hpp"
#include "math/Matrix.hpp"
#include "context/Selector.hpp"
#include "utils/String.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>
#include <random>

namespace nexo::editor {

    void EditorScene::setup()
    {
        setupImguizmo();
        setupWindow();
        setupScene();
    }

    void EditorScene::setupScene()
    {
        auto &app = getApp();

        // New handling
        m_sceneId = static_cast<int>(app.getSceneManager().createScene(m_windowName));
        renderer::FramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            renderer::FrameBufferTextureFormats::RGBA8, renderer::FrameBufferTextureFormats::RED_INTEGER, renderer::FrameBufferTextureFormats::Depth
        };
        framebufferSpecs.width = static_cast<unsigned int>(m_viewSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(m_viewSize.y);
        const auto renderTarget = renderer::Framebuffer::create(framebufferSpecs);
        m_editorCamera = CameraFactory::createPerspectiveCamera({0.0f, 0.0f, 0.0f}, static_cast<unsigned int>(m_viewSize.x), static_cast<unsigned int>(m_viewSize.y), renderTarget);
        auto &cameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(m_editorCamera);
        cameraComponent.render = true;
        app.getSceneManager().getScene(m_sceneId).addEntity(static_cast<ecs::Entity>(m_editorCamera));
        components::PerspectiveCameraController controller;
        Application::m_coordinator->addComponent<components::PerspectiveCameraController>(static_cast<ecs::Entity>(m_editorCamera), controller);
        components::EditorCameraTag editorCameraTag;
        Application::m_coordinator->addComponent(m_editorCamera, editorCameraTag);
        m_activeCamera = m_editorCamera;

        m_sceneUuid = app.getSceneManager().getScene(m_sceneId).getUuid();
        if (m_defaultScene)
            loadDefaultEntities();
    }

    void EditorScene::setupImguizmo() const
    {
        ImGuizmo::SetOrthographic(true);
    }

    void EditorScene::loadDefaultEntities() const
    {
        auto &app = getApp();
        scene::Scene &scene = app.getSceneManager().getScene(m_sceneId);
        const ecs::Entity ambientLight = LightFactory::createAmbientLight({0.5f, 0.5f, 0.5f});
        scene.addEntity(ambientLight);
        const ecs::Entity pointLight = LightFactory::createPointLight({1.2f, 5.0f, 0.1f});
        scene.addEntity(pointLight);
        const ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.2f, -1.0f, -0.3f});
        scene.addEntity(directionalLight);
        const ecs::Entity spotLight = LightFactory::createSpotLight({0.0f, 0.5f, -2.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
        scene.addEntity(spotLight);
        const ecs::Entity basicCube = EntityFactory3D::createCube({0.0f, -5.0f, -5.0f}, {20.0f, 1.0f, 20.0f},
                                                               {0.0f, 0.0f, 0.0f}, {1.0f, 0.5f, 0.31f, 1.0f});
        app.getSceneManager().getScene(m_sceneId).addEntity(basicCube);
    }

    void EditorScene::setupWindow()
    {
        constexpr auto size = ImVec2(1280, 720);
        m_viewSize = size;
    }

    void EditorScene::shutdown()
    {
    	// Should probably check if it is necessary to delete the scene here ? (const for now)
    }

    void EditorScene::handleKeyEvents() const
    {
    	// Will be implemeneted later
    }

    void EditorScene::setCamera(ecs::Entity cameraId)
    {
        auto &app = getApp();
        auto &oldCameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);
        oldCameraComponent.active = false;
        oldCameraComponent.render = false;
        m_activeCamera = cameraId;
    }

    void EditorScene::initialToolbarSetup(const float buttonWidth, const float buttonHeight)
    {
        ImVec2 toolbarPos = m_viewPosition;
        toolbarPos.x += 10.0f;

        ImGui::SetCursorScreenPos(toolbarPos);

        ImVec2 toolbarSize = ImVec2(m_viewSize.x - buttonWidth, 50.0f);

        ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.1f, 0.1f, 0.1f, 0.0f));
        ImGui::BeginChild("##ToolbarOverlay", toolbarSize, 0,
                         ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove |
                         ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize |
                         ImGuiWindowFlags_NoSavedSettings);

        ImGui::SetCursorPosY((ImGui::GetWindowHeight() - ImGui::GetFrameHeight()) * 0.5f);

        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(8, 0));
    }

    bool EditorScene::renderToolbarButton(const std::string &uniqueId, const std::string &icon, const std::string &tooltip, const std::vector<Components::GradientStop> & gradientStop)
    {
        constexpr float buttonWidth = 35.0f;
        constexpr float buttonHeight = 35.0f;
        bool clicked = Components::drawToolbarButton(uniqueId, icon, ImVec2(buttonWidth, buttonHeight), gradientStop);
        if (!tooltip.empty() && ImGui::IsItemHovered())
            ImGui::SetTooltip(tooltip.c_str());
        return clicked;
    }

    void EditorScene::renderPrimitiveSubMenu(const ImVec2 &primitiveButtonPos, const ImVec2 &buttonSize, bool &showPrimitiveMenu)
    {
        auto &app = getApp();
        static const std::vector<Widgets::ButtonProps> buttonProps =
        {
            {
                .uniqueId = "cube_primitive",
                .icon = ICON_FA_CUBE,
                .onClick = [this, &app]()
                    {
                        const ecs::Entity newCube = EntityFactory3D::createCube({0.0f, 0.0f, -5.0f}, {1.0f, 1.0f, 1.0f},
                                                                               {0.0f, 0.0f, 0.0f}, {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f});
                        app.getSceneManager().getScene(this->m_sceneId).addEntity(newCube);
                    },
                .tooltip = "Create Cube"
            }
        };
        Widgets::drawVerticalButtonDropDown(primitiveButtonPos, buttonSize, buttonProps, showPrimitiveMenu);
    }

    void EditorScene::renderSnapSubMenu(const ImVec2 &snapButtonPos, const ImVec2 &buttonSize, bool &showSnapMenu)
    {
        const std::vector<Widgets::ButtonProps> buttonProps =
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
                .buttonGradient = (m_snapTranslateOn) ? m_selectedGradient : m_buttonGradient
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
                .buttonGradient = (m_snapRotateOn) ? m_selectedGradient : m_buttonGradient
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
        Widgets::drawVerticalButtonDropDown(snapButtonPos, buttonSize, buttonProps, showSnapMenu);
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
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                EntityPropertiesComponents::drawRowDragFloat3("Translate Snap", "X", "Y", "Z", &this->m_snapTranslate.x);
                ImGui::EndTable();
            }

            if (ImGui::BeginTable("ScaleAndRotateSnap", 4,
                ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Value", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                // Empty columns to match the first table's structure
                ImGui::TableSetupColumn("##Empty1", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Empty2", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);

                EntityPropertiesComponents::drawRowDragFloat1("Rotate Snap", "", &this->m_angleSnap);
                ImGui::EndTable();
            }
            ImGui::Spacing();
            ImGui::Spacing();

            float buttonWidth = 120.0f;
            float windowWidth = ImGui::GetWindowSize().x;
            ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

            if (ImGui::Button("OK", ImVec2(buttonWidth, 0.0f)))
            {
                m_popupManager.closePopupInContext();
            }
            ImGui::Unindent(10.0f);
            ImGui::PopStyleVar();
            m_popupManager.closePopup();
        }
    }

    void EditorScene::renderEditorCameraToolbarButton()
    {
        auto &app = getApp();
        auto &selector = Selector::get();
        bool editorMode = m_activeCamera == m_editorCamera;
        if (m_activeCamera == m_editorCamera) {
            if (renderToolbarButton("editor_camera", ICON_FA_CAMERA, "Edit Editor Camera Setting", m_buttonGradient)) {
                const auto &uuidComponent = app.m_coordinator->getComponent<components::UuidComponent>(m_editorCamera);
                selector.setSelectedEntity(uuidComponent.uuid, m_editorCamera);
            }
        } else {
            if (renderToolbarButton("switch_back", ICON_FA_EXCHANGE, "Switch back to editor camera", m_buttonGradient)) {
                auto &oldCameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);
                oldCameraComponent.active = false;
                oldCameraComponent.render = false;
                m_activeCamera = m_editorCamera;
                auto &editorCameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);
                editorCameraComponent.render = true;
                editorCameraComponent.active = true;
            }
        }
    }

    bool EditorScene::renderGizmoModeToolbarButton(const bool showGizmoModeMenu, Widgets::ButtonProps &activeGizmoMode, Widgets::ButtonProps &inactiveGizmoMode)
    {
        static const Widgets::ButtonProps gizmoLocalModeButtonProps = {"local_coords", ICON_FA_CROSSHAIRS, [this]() {this->m_currentGizmoMode = ImGuizmo::MODE::LOCAL;}, nullptr, "Local coordinates"};
        static const Widgets::ButtonProps gizmoWorldModeButtonProps = {"world_coords", ICON_FA_GLOBE, [this]() {this->m_currentGizmoMode = ImGuizmo::MODE::WORLD;}, nullptr, "World coordinates"};
        if (m_currentGizmoMode == ImGuizmo::MODE::LOCAL) {
            activeGizmoMode = gizmoLocalModeButtonProps;
            inactiveGizmoMode = gizmoWorldModeButtonProps;
        } else {
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

        initialToolbarSetup(buttonWidth, buttonHeight);

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
        static const Widgets::ButtonProps gizmoTranslateButtonProps = Widgets::ButtonProps{"translate", ICON_FA_ARROWS, [this]() {this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;}, nullptr, "Translate"};
        static const Widgets::ButtonProps gizmoRotateButtonProps = Widgets::ButtonProps{"rotate", ICON_FA_REFRESH, [this]() {this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;}, nullptr, "Rotate"};
        static const Widgets::ButtonProps gizmoScaleButtonProps = Widgets::ButtonProps{"scale", ICON_FA_EXPAND, [this]() {this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;}, nullptr, "Scale"};
        static const Widgets::ButtonProps gizmoUniversalButtonProps = Widgets::ButtonProps{"universal", ICON_FA_ARROWS_ALT, [this]() {this->m_currentGizmoOperation = ImGuizmo::OPERATION::UNIVERSAL;}, nullptr, "Universal"};
        std::vector<Widgets::ButtonProps> gizmoButtons = {
            gizmoTranslateButtonProps,
            gizmoRotateButtonProps,
            gizmoScaleButtonProps,
            gizmoUniversalButtonProps
        };

        Widgets::ButtonProps activeOp;
        switch (m_currentGizmoOperation) {
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
        Widgets::ButtonProps activeGizmoMode;
        Widgets::ButtonProps inactiveGizmoMode;
        ImVec2 changeGizmoModePos = ImGui::GetCursorScreenPos();
        static bool showGizmoModeMenu = false;
        bool changeGizmoModeClicked = renderGizmoModeToolbarButton(showGizmoModeMenu, activeGizmoMode, inactiveGizmoMode);
        if (changeGizmoModeClicked)
            showGizmoModeMenu = !showGizmoModeMenu;

        ImGui::SameLine();

        // -------- Toggle snap button --------
        // This can open a submenu, see at the end
        ImVec2 toggleSnapPos = ImGui::GetCursorScreenPos();
        static bool showSnapToggleMenu = false;
        bool snapOn = m_snapRotateOn || m_snapTranslateOn;
        bool toggleSnapClicked = renderToolbarButton("toggle_snap", ICON_FA_MAGNET, "Toggle gizmo snap", (showSnapToggleMenu || snapOn) ? m_selectedGradient : m_buttonGradient);
        if (toggleSnapClicked)
            showSnapToggleMenu = !showSnapToggleMenu;

        ImGui::SameLine();

        // -------- Grid enabled button --------
        if (renderToolbarButton("grid_enabled", ICON_FA_TH_LARGE, "Enable / Disable grid", m_gridEnabled ? m_selectedGradient : m_buttonGradient))
        {
            m_gridEnabled = !m_gridEnabled;
        }

        ImGui::SameLine();

        // -------- Snap to gridbutton --------
        if (renderToolbarButton("snap_to_grid", ICON_FA_TH, "Enable snapping to grid", m_snapToGrid ? m_selectedGradient : m_buttonGradient))
        {
            m_snapToGrid = !m_snapToGrid;
        }

        ImGui::SameLine();

        // -------- Enable wireframe button --------
        if (renderToolbarButton("wireframe", ICON_FA_CUBE, "Enable / Disable wireframe", m_wireframeEnabled ? m_selectedGradient : m_buttonGradient))
        {
            m_wireframeEnabled = !m_wireframeEnabled;
        }

        ImGui::SameLine();

        // -------- Play button button --------
        renderToolbarButton("play", ICON_FA_PLAY, "Play scene", m_buttonGradient);

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
            Widgets::drawVerticalButtonDropDown(changeGizmoOpPos, buttonSize, gizmoButtons, showGizmoOpMenu);
        }

        // -------- Gizmo mode sub-menu --------
        if (showGizmoModeMenu)
        {
            Widgets::drawVerticalButtonDropDown(changeGizmoModePos, buttonSize, {inactiveGizmoMode}, showGizmoModeMenu);
        }

        // -------- Snap sub-menu --------
        if (showSnapToggleMenu)
        {
            renderSnapSubMenu(toggleSnapPos, buttonSize, showSnapToggleMenu);
        }

        // -------- Snap settings popup --------
        snapSettingsPopup();

        // IMPORTANT: Restore original cursor position so we don't affect layout
        ImGui::SetCursorPos(originalCursorPos);
    }

    void EditorScene::renderGizmo()
    {
        const auto &coord = nexo::Application::m_coordinator;
        auto const &selector = Selector::get();
        if (selector.getSelectionType() != SelectionType::ENTITY ||
            selector.getSelectedScene() != m_sceneId)
            return;
        const ecs::Entity entity = selector.getSelectedEntity();
        const auto &transformCameraComponent = coord->getComponent<components::TransformComponent>(m_activeCamera);
        auto &cameraComponent = coord->getComponent<components::CameraComponent>(m_activeCamera);
        ImGuizmo::SetOrthographic(cameraComponent.type == components::CameraType::ORTHOGRAPHIC);
        ImGuizmo::SetDrawlist();
        ImGuizmo::SetID(static_cast<int>(entity));
        ImGuizmo::SetRect(m_viewPosition.x, m_viewPosition.y, m_viewSize.x, m_viewSize.y);
        glm::mat4 viewMatrix = cameraComponent.getViewMatrix(transformCameraComponent);
        glm::mat4 projectionMatrix = cameraComponent.getProjectionMatrix();
        const auto transf = coord->tryGetComponent<components::TransformComponent>(entity);
        if (!transf)
            return;
        const glm::mat4 rotationMat = glm::toMat4(transf->get().quat);
        glm::mat4 transformMatrix = glm::translate(glm::mat4(1.0f), transf->get().pos) *
                                    rotationMat *
                                    glm::scale(glm::mat4(1.0f), {transf->get().size.x, transf->get().size.y, transf->get().size.z});

        static ImGuizmo::OPERATION lastOperation;
        if (!ImGuizmo::IsUsing())
        {
            if (ImGuizmo::IsOver(ImGuizmo::OPERATION::TRANSLATE))
            {
                lastOperation = ImGuizmo::OPERATION::TRANSLATE;
            }
            else if (ImGuizmo::IsOver(ImGuizmo::OPERATION::ROTATE))
            {
                lastOperation = ImGuizmo::OPERATION::ROTATE;
            }
        }


        float *snap = nullptr;
        if (m_snapTranslateOn && lastOperation == ImGuizmo::OPERATION::TRANSLATE) {
            snap = &m_snapTranslate.x;
        } else if (m_snapRotateOn && lastOperation == ImGuizmo::OPERATION::ROTATE) {
            snap = &m_angleSnap;
        }

        ImGuizmo::Enable(true);
        ImGuizmo::Manipulate(glm::value_ptr(viewMatrix), glm::value_ptr(projectionMatrix),
                             m_currentGizmoOperation,
                             m_currentGizmoMode,
                             glm::value_ptr(transformMatrix),
                             nullptr, snap);

        glm::vec3 translation(0);
        glm::vec3 scale(0);
        glm::quat quaternion;

        math::decomposeTransformQuat(transformMatrix, translation, quaternion, scale);

        if (ImGuizmo::IsUsing())
        {
            cameraComponent.active = false;
            transf->get().pos = translation;
            transf->get().quat = quaternion;
            transf->get().size = scale;
        } else
            cameraComponent.active = true;
    }

    void EditorScene::renderView()
    {
        const auto viewPortOffset = ImGui::GetCursorPos();
       	auto &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(m_activeCamera);
        const ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();

        // Resize handling
        if ((viewportPanelSize.x > 0 && viewportPanelSize.y > 0) && (m_viewSize.x != viewportPanelSize.x || m_viewSize.y != viewportPanelSize.y))
        {
        	cameraComponent.resize(static_cast<unsigned int>(viewportPanelSize.x),
        							static_cast<unsigned int>(viewportPanelSize.y));

            m_viewSize.x = viewportPanelSize.x;
            m_viewSize.y = viewportPanelSize.y;
        }

        // Render framebuffer
        const unsigned int textureId = cameraComponent.m_renderTarget->getColorAttachmentId(0);
        ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)), m_viewSize, ImVec2(0, 1), ImVec2(1, 0));

        const auto windowSize = ImGui::GetWindowSize();
        auto minBounds = ImGui::GetWindowPos();

        minBounds.x += viewPortOffset.x;
        minBounds.y += viewPortOffset.y;

        const ImVec2 maxBounds = {minBounds.x + windowSize.x, minBounds.y + windowSize.y};
        m_viewportBounds[0] = minBounds;
        m_viewportBounds[1] = maxBounds;
    }

    void EditorScene::show()
    {
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
        ImGui::SetNextWindowSizeConstraints(ImVec2(480, 270), ImVec2(1920, 1080));
        auto &selector = Selector::get();
        m_windowName = selector.getUiHandle(m_sceneUuid, std::string(ICON_FA_GLOBE) + "   " + m_windowName);
        const std::string &sceneWindowName = m_windowName + std::string(NEXO_WND_USTRID_DEFAULT_SCENE) + std::to_string(m_sceneId);

        if (ImGui::Begin(sceneWindowName.c_str(), &m_opened, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoCollapse))
        {
            firstDockSetup(std::string(NEXO_WND_USTRID_DEFAULT_SCENE) + std::to_string(m_sceneId));
        	auto &app = getApp();

            // Add some spacing after the toolbar
            ImGui::Dummy(ImVec2(0, 5));
            m_viewPosition = ImGui::GetCursorScreenPos();


            m_focused = ImGui::IsWindowFocused();
            m_hovered = ImGui::IsWindowHovered();
            app.getSceneManager().getScene(m_sceneId).setActiveStatus(m_focused);
            if (m_focused && selector.getSelectedScene() != m_sceneId)
            {
                selector.setSelectedScene(m_sceneId);
                selector.unselectEntity();
            }

            if (m_activeCamera == -1)
            {
	            // No active camera, render the text at the center of the screen
	            ImVec2 textSize = ImGui::CalcTextSize("No active camera");
	            auto textPos = ImVec2((m_viewSize.x - textSize.x) / 2, (m_viewSize.y - textSize.y) / 2);

	            ImGui::SetCursorScreenPos(textPos);
	            ImGui::Text("No active camera");
            }
            else
            {
	            renderView();
	            renderGizmo();
	            renderToolbar();

            }
        }
        ImGui::End();
        ImGui::PopStyleVar();
    }

    void EditorScene::update()
    {
    	auto &selector = Selector::get();
    	//m_windowName = selector.getUiHandle(m_sceneUuid, m_windowName);
        if (!m_opened || m_activeCamera == -1)
            return;
        if (m_focused && m_hovered)
            handleKeyEvents();
        SceneType sceneType = m_activeCamera == m_editorCamera ? SceneType::EDITOR : SceneType::GAME;
        runEngine(m_sceneId, RenderingType::FRAMEBUFFER, sceneType);
        auto const &cameraComponent = Application::m_coordinator->getComponent<components::CameraComponent>(static_cast<ecs::Entity>(m_activeCamera));
        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left) && !ImGuizmo::IsUsing() && m_focused)
        {
            auto [mx, my] = ImGui::GetMousePos();
            mx -= m_viewportBounds[0].x;
            my -= m_viewportBounds[0].y;

            // Flip the y-coordinate to match opengl texture format (maybe make it modular in some way)
            my = m_viewSize.y - my;

            // Mouse is not inside the viewport
            if (!(mx >= 0 && my >= 0 && mx < m_viewSize.x && my < m_viewSize.y))
                return;

            cameraComponent.m_renderTarget->bind();
            int data = cameraComponent.m_renderTarget->getPixel<int>(1, static_cast<int>(mx), static_cast<int>(my));
            cameraComponent.m_renderTarget->unbind();
            if (data == -1)
            {
            	selector.unselectEntity();
             	return;
            }
            const auto uuid = Application::m_coordinator->tryGetComponent<components::UuidComponent>(data);
            if (uuid)
            {
               	selector.setSelectedEntity(uuid->get().uuid, data);
                selector.setSelectionType(SelectionType::ENTITY);
            }
            selector.setSelectedScene(m_sceneId);
        }
    }

}
