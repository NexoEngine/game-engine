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
#include "components/Camera.hpp"
#include "components/RenderContext.hpp"
#include "components/Uuid.hpp"
#include "components/Editor.hpp"
#include "math/Matrix.hpp"
#include "context/Selector.hpp"
#include "utils/String.hpp"
#include "utils/EditorProps.hpp"
#include "ImNexo/Widgets.hpp"
#include "ImNexo/Panels.hpp"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/quaternion.hpp>
#include <imgui.h>
#include <random>

namespace nexo::editor {

    void EditorScene::setup()
    {
        setupWindow();
        setupScene();

        // ================= GLOBAL STATE =============================
        m_globalState = {static_cast<unsigned int>(EditorState::GLOBAL)};
        m_globalState.registerCommand(
            {
                "Shift context",
                "Shift",
                nullptr,
                nullptr,
                nullptr,
                true,
                {
                    {
                        "Add entity",
                        "A",
                        [this]{
                            this->m_popupManager.openPopup("Add new entity popup");
                        },
                        nullptr,
                        nullptr,
                        false,
                    }
                }
            }
        );
        m_globalState.registerCommand(
            {
                "Select all",
                "A",
                [this]{
                    LOG(NEXO_WARN, "Select all not implemented yet");
                },
                nullptr,
                nullptr,
                false
            }
        );
        m_windowState = m_globalState;

        // ================= GIZMO STATE =============================
        m_gizmoState = {static_cast<unsigned int>(EditorState::GIZMO)};
        m_gizmoState.registerCommand(
            {
                "Delete",
                "X",
                [this]{
                        auto &selector = Selector::get();
                        ecs::Entity selectedEntity = selector.getSelectedEntity();
                        selector.unselectEntity();
                        auto &app = nexo::getApp();
                        app.deleteEntity(selectedEntity);
                        this->m_windowState = m_globalState;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoState.registerCommand(
            {
                "Translate",
                "G",
                [this]{
                    this->m_windowState = m_gizmoTranslateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoState.registerCommand(
            {
                "Rotate",
                "R",
                [this]{
                    this->m_windowState = m_gizmoRotateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoState.registerCommand(
            {
                "Scale",
                "S",
                [this]{
                    this->m_windowState = m_gizmoScaleState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoState.registerCommand(
            {
                "Shift context",
                "Shift",
                nullptr,
                nullptr,
                nullptr,
                true,
                {
                    {
                        "Toggle snapping",
                        "S",
                        [this]{
                            m_snapTranslateOn = true;
                            m_snapRotateOn = true;
                        },
                        [this]{
                            m_snapTranslateOn = false;
                            m_snapRotateOn = false;
                        },
                        nullptr,
                        false,
                    }
                }
            }
        );

        // ================= TRANSLATE STATE =============================
        m_gizmoTranslateState = {static_cast<unsigned int>(EditorState::GIZMO_TRANSLATE)};
        m_gizmoTranslateState.registerCommand(
            {
                "Universal",
                "U",
                [this]{
                    this->m_windowState = m_gizmoState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::UNIVERSAL;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoTranslateState.registerCommand(
            {
                "Translate",
                "G",
                [this]{
                    this->m_windowState = m_gizmoTranslateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                },
                nullptr,
                [this]{
                    if (this->m_currentGizmoMode == ImGuizmo::MODE::LOCAL)
                        this->m_currentGizmoMode = ImGuizmo::MODE::WORLD;
                    else
                        this->m_currentGizmoMode = ImGuizmo::MODE::LOCAL;
                },
                false,
            }
        );
        m_gizmoTranslateState.registerCommand(
            {
                "Rotate",
                "R",
                [this]{
                    this->m_windowState = m_gizmoRotateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoTranslateState.registerCommand(
            {
                "Scale",
                "S",
                [this]{
                    this->m_windowState = m_gizmoScaleState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoTranslateState.registerCommand(
            {
                "Shift context",
                "Shift",
                nullptr,
                nullptr,
                nullptr,
                true,
                {
                    {
                        "Exclude X",
                        "X",
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::TRANSLATE_X);
                        },
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::TRANSLATE_X);
                        },
                        nullptr,
                        false,
                    },
                    {
                        "Exclude Y",
                        "Y",
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::TRANSLATE_Y);
                        },
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::TRANSLATE_Y);
                        },
                        nullptr,
                        false,
                    },
                    {
                        "Exclude Z",
                        "Z",
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::TRANSLATE_Z);
                        },
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::TRANSLATE_Z);
                        },
                        nullptr,
                        false,
                    },
                    {
                        "Toggle snapping",
                        "S",
                        [this]{
                            m_snapTranslateOn = true;
                        },
                        [this]{
                            m_snapTranslateOn = false;
                        },
                        nullptr,
                        false,
                    }
                }
            }
        );
        m_gizmoTranslateState.registerCommand(
            {
                "Lock X",
                "X",
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE_X;
                },
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                },
                nullptr,
                false,
            }
        );
        m_gizmoTranslateState.registerCommand(
            {
                "Lock Y",
                "Y",
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE_Y;
                },
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                },
                nullptr,
                false,
            }
        );
        m_gizmoTranslateState.registerCommand(
            {
                "Lock Z",
                "Z",
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE_Z;
                },
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                },
                nullptr,
                false,
            }
        );

        // ================= ROTATE STATE =============================
        m_gizmoRotateState = {static_cast<unsigned int>(EditorState::GIZMO_ROTATE)};
        m_gizmoRotateState.registerCommand(
            {
                "Universal",
                "U",
                [this]{
                    this->m_windowState = m_gizmoState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::UNIVERSAL;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoRotateState.registerCommand(
            {
                "Rotate",
                "R",
                [this]{
                    this->m_windowState = m_gizmoRotateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                }, // << Key pressed
                nullptr, // << Key released
                [this]{
                    if (this->m_currentGizmoMode == ImGuizmo::MODE::LOCAL)
                        this->m_currentGizmoMode = ImGuizmo::MODE::WORLD;
                    else
                        this->m_currentGizmoMode = ImGuizmo::MODE::LOCAL;
                }, // << Key repeat
                false,
            }
        );
        m_gizmoRotateState.registerCommand(
            {
                "Translate",
                "G",
                [this]{
                    this->m_windowState = m_gizmoTranslateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoRotateState.registerCommand(
            {
                "Scale",
                "S",
                [this]{
                    this->m_windowState = m_gizmoScaleState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoRotateState.registerCommand(
            {
                "Shift context",
                "Shift",
                nullptr,
                nullptr,
                nullptr,
                true,
                {
                    {
                        "Exclude X",
                        "X",
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::ROTATE_X);
                        },
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::ROTATE_X);
                        },
                        nullptr,
                        false,
                    },
                    {
                        "Exclude Y",
                        "Y",
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::ROTATE_Y);
                        },
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::ROTATE_Y);
                        },
                        nullptr,
                        false,
                    },
                    {
                        "Exclude Z",
                        "Z",
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::ROTATE_Z);
                        },
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::ROTATE_Z);
                        },
                        nullptr,
                        false,
                    },
                    {
                        "Toggle snapping",
                        "S",
                        [this]{
                            m_snapRotateOn = true;
                        },
                        [this]{
                            m_snapRotateOn = false;
                        },
                        nullptr,
                        false,
                    },
                }
            }
        );
        m_gizmoRotateState.registerCommand(
            {
                "Lock X",
                "X",
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE_X;
                },
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                },
                nullptr,
                false,
            }
        );
        m_gizmoRotateState.registerCommand(
            {
                "Lock Y",
                "Y",
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE_Y;
                },
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                },
                nullptr,
                false,
            }
        );
        m_gizmoRotateState.registerCommand(
            {
                "Lock Z",
                "Z",
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE_Z;
                },
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                },
                nullptr,
                false,
            }
        );

        // ================= SCALE STATE =============================
        m_gizmoScaleState = {static_cast<unsigned int>(EditorState::GIZMO_SCALE)};
        m_gizmoScaleState.registerCommand(
            {
                "Universal",
                "U",
                [this]{
                    this->m_windowState = m_gizmoState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::UNIVERSAL;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoScaleState.registerCommand(
            {
                "Scale",
                "S",
                [this]{
                    this->m_windowState = m_gizmoScaleState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                },
                nullptr,
                [this]{
                    if (this->m_currentGizmoMode == ImGuizmo::MODE::LOCAL)
                        this->m_currentGizmoMode = ImGuizmo::MODE::WORLD;
                    else
                        this->m_currentGizmoMode = ImGuizmo::MODE::LOCAL;
                },
                false,
            }
        );
        m_gizmoScaleState.registerCommand(
            {
                "Translate",
                "G",
                [this]{
                    this->m_windowState = m_gizmoTranslateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoScaleState.registerCommand(
            {
                "Rotate",
                "R",
                [this]{
                    this->m_windowState = m_gizmoRotateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoScaleState.registerCommand(
            {
                "Shift context",
                "Shift",
                nullptr,
                nullptr,
                nullptr,
                true,
                {
                    {
                        "Exclude X",
                        "X",
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::SCALE_X);
                        },
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::SCALE_X);
                        },
                        nullptr,
                        false,
                    },
                    {
                        "Exclude Y",
                        "Y",
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::SCALE_Y);
                        },
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::SCALE_Y);
                        },
                        nullptr,
                        false,
                    },
                    {
                        "Exclude Z",
                        "Z",
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::SCALE_Z);
                        },
                        [this]{
                            m_currentGizmoOperation = static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::SCALE_Z);
                        },
                        nullptr,
                        false,
                    }
                }
            }
        );
        m_gizmoScaleState.registerCommand(
            {
                "Lock X",
                "X",
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE_X;
                },
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                },
                nullptr,
                false,
            }
        );
        m_gizmoScaleState.registerCommand(
            {
                "Lock Y",
                "Y",
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE_Y;
                },
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                },
                nullptr,
                false,
            }
        );
        m_gizmoScaleState.registerCommand(
            {
                "Lock Z",
                "Z",
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE_Z;
                },
                [this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                },
                nullptr,
                false,
            }
        );
    }

    void EditorScene::setupScene()
    {
        auto &app = getApp();

        m_sceneId = static_cast<int>(app.getSceneManager().createScene(m_windowName));
        renderer::NxFramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            renderer::NxFrameBufferTextureFormats::RGBA8, renderer::NxFrameBufferTextureFormats::RED_INTEGER, renderer::NxFrameBufferTextureFormats::Depth
        };
        framebufferSpecs.width = static_cast<unsigned int>(m_viewSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(m_viewSize.y);
        const auto renderTarget = renderer::NxFramebuffer::create(framebufferSpecs);
        m_editorCamera = CameraFactory::createPerspectiveCamera({0.0f, 3.0f, -2.0f}, static_cast<unsigned int>(m_viewSize.x), static_cast<unsigned int>(m_viewSize.y), renderTarget);
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

    void EditorScene::loadDefaultEntities() const
    {
        auto &app = getApp();
        scene::Scene &scene = app.getSceneManager().getScene(m_sceneId);
        const ecs::Entity ambientLight = LightFactory::createAmbientLight({0.5f, 0.5f, 0.5f});
        scene.addEntity(ambientLight);
        const ecs::Entity pointLight = LightFactory::createPointLight({2.0f, 5.0f, 0.0f});
        utils::addPropsTo(pointLight, utils::PropsType::POINT_LIGHT);
        scene.addEntity(pointLight);
        const ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.2f, -1.0f, -0.3f});
        scene.addEntity(directionalLight);
        const ecs::Entity spotLight = LightFactory::createSpotLight({-2.0f, 5.0f, 0.0f}, {0.0f, -1.0f, 0.0f}, {0.0f, 0.0f, 1.0f});
        utils::addPropsTo(spotLight, utils::PropsType::SPOT_LIGHT);
        scene.addEntity(spotLight);
        const ecs::Entity basicCube = EntityFactory3D::createCube({0.0f, 0.25f, 0.0f}, {20.0f, 0.5f, 20.0f},
                                                               {0.0f, 0.0f, 0.0f}, {0.05f * 1.7, 0.09f * 1.35, 0.13f * 1.45, 1.0f});
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

    bool EditorScene::renderToolbarButton(const std::string &uniqueId, const std::string &icon, const std::string &tooltip, const std::vector<ImNexo::GradientStop> & gradientStop, bool *rightClicked)
    {
        constexpr float buttonWidth = 35.0f;
        constexpr float buttonHeight = 35.0f;
        bool clicked = ImNexo::IconGradientButton(uniqueId, icon, ImVec2(buttonWidth, buttonHeight), gradientStop);
        if (!tooltip.empty() && ImGui::IsItemHovered())
            ImGui::SetTooltip("%s", tooltip.c_str());
        if (rightClicked != nullptr)
            *rightClicked = ImGui::IsItemClicked(ImGuiMouseButton_Right);
        return clicked;
    }

    void EditorScene::renderPrimitiveSubMenu(const ImVec2 &primitiveButtonPos, const ImVec2 &buttonSize, bool &showPrimitiveMenu)
    {
        auto &app = getApp();
        static const std::vector<ImNexo::ButtonProps> buttonProps =
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
        ImNexo::ButtonDropDown(primitiveButtonPos, buttonSize, buttonProps, showPrimitiveMenu);
    }

    void EditorScene::renderSnapSubMenu(const ImVec2 &snapButtonPos, const ImVec2 &buttonSize, bool &showSnapMenu)
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
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Y", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##Z", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImNexo::RowDragFloat3("Translate Snap", "X", "Y", "Z", &this->m_snapTranslate.x);
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

                ImNexo::RowDragFloat1("Rotate Snap", "", &this->m_angleSnap);
                ImGui::EndTable();
            }
            ImGui::Spacing();
            ImGui::Spacing();

            float buttonWidth = 120.0f;
            float windowWidth = ImGui::GetWindowSize().x;
            ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

            if (ImNexo::Button("OK", ImVec2(buttonWidth, 0.0f)))
            {
                m_popupManager.closePopupInContext();
            }
            ImGui::Unindent(10.0f);
            ImGui::PopStyleVar();
            m_popupManager.closePopup();
        }
    }

    void EditorScene::gridSettingsPopup()
    {
        if (m_popupManager.showPopupModal("Grid settings"))
        {
            auto &app = getApp();
            components::RenderContext::GridParams &gridSettings =
                app.m_coordinator->getSingletonComponent<components::RenderContext>().gridParams;

            ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(5.0f, 10.0f));
            ImGui::Indent(10.0f);

            if (ImGui::BeginTable("GridSettings", 2,
                ImGuiTableFlags_SizingStretchProp))
            {
                ImGui::TableSetupColumn("##Label", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImGui::TableSetupColumn("##X", ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_NoHeaderLabel);
                ImNexo::RowDragFloat1("Grid size", "", &gridSettings.gridSize, 50.0f, 150.0f);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("The total size of the grid");
                ImNexo::RowDragFloat1("Pixel cell spacing", "", &gridSettings.minPixelsBetweenCells, 0.0f, 100.0f, 0.1f);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("Level of detail of internal cells");
                ImNexo::RowDragFloat1("Cell size", "", &gridSettings.cellSize, 0.1f, 20.0f, 0.02f);
                if (ImGui::IsItemHovered())
                    ImGui::SetTooltip("The size of the internal cells");
                ImGui::EndTable();
            }

            ImGui::Spacing();
            ImGui::Spacing();

            float buttonWidth = 120.0f;
            float windowWidth = ImGui::GetWindowSize().x;
            ImGui::SetCursorPosX((windowWidth - buttonWidth) * 0.5f);

            if (ImNexo::Button("OK", ImVec2(buttonWidth, 0.0f)))
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

    bool EditorScene::renderGizmoModeToolbarButton(const bool showGizmoModeMenu, ImNexo::ButtonProps &activeGizmoMode, ImNexo::ButtonProps &inactiveGizmoMode)
    {
        static const ImNexo::ButtonProps gizmoLocalModeButtonProps = {"local_coords", ICON_FA_CROSSHAIRS, [this]() {this->m_currentGizmoMode = ImGuizmo::MODE::LOCAL;}, nullptr, "Local coordinates"};
        static const ImNexo::ButtonProps gizmoWorldModeButtonProps = {"world_coords", ICON_FA_GLOBE, [this]() {this->m_currentGizmoMode = ImGuizmo::MODE::WORLD;}, nullptr, "World coordinates"};
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
        auto &app = getApp();
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
        static const ImNexo::ButtonProps gizmoTranslateButtonProps = ImNexo::ButtonProps{"translate", ICON_FA_ARROWS, [this]() {this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;}, nullptr, "Translate"};
        static const ImNexo::ButtonProps gizmoRotateButtonProps = ImNexo::ButtonProps{"rotate", ICON_FA_REFRESH, [this]() {this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;}, nullptr, "Rotate"};
        static const ImNexo::ButtonProps gizmoScaleButtonProps = ImNexo::ButtonProps{"scale", ICON_FA_EXPAND, [this]() {this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;}, nullptr, "Scale"};
        static const ImNexo::ButtonProps gizmoUniversalButtonProps = ImNexo::ButtonProps{"universal", ICON_FA_ARROWS_ALT, [this]() {this->m_currentGizmoOperation = ImGuizmo::OPERATION::UNIVERSAL;}, nullptr, "Universal"};
        std::vector<ImNexo::ButtonProps> gizmoButtons = {
            gizmoTranslateButtonProps,
            gizmoRotateButtonProps,
            gizmoScaleButtonProps,
            gizmoUniversalButtonProps
        };

        ImNexo::ButtonProps activeOp;
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
        ImNexo::ButtonProps activeGizmoMode;
        ImNexo::ButtonProps inactiveGizmoMode;
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
        bool rightClicked = false;
        components::RenderContext::GridParams &gridParams = app.m_coordinator->getSingletonComponent<components::RenderContext>().gridParams;
        if (renderToolbarButton("grid_enabled", ICON_FA_TH_LARGE, "Enable / Disable grid", gridParams.enabled ? m_selectedGradient : m_buttonGradient, &rightClicked))
        {
            gridParams.enabled = !gridParams.enabled;

        }
        if (rightClicked)
            m_popupManager.openPopup("Grid settings", ImVec2(300, 180));

        ImGui::SameLine();

        // -------- Snap to gridbutton --------
        // NOTE: This seems complicated to implement using ImGuizmo, we leave it for now but i dont know if it will be implemented
        if (renderToolbarButton("snap_to_grid", ICON_FA_TH, "Enable snapping to grid\n(only horizontal translation and scaling)", m_snapToGrid ? m_selectedGradient : m_buttonGradient))
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

    ImGuizmo::OPERATION EditorScene::getLastGuizmoOperation()
    {
        for (int bitPos = 0; bitPos <= 13; bitPos++)
        {
            // Create a mask for this bit position
            ImGuizmo::OPERATION op = static_cast<ImGuizmo::OPERATION>(1u << bitPos);

            // Check if this bit is set
            if (ImGuizmo::IsOver(op))
                return op;
        }
        return ImGuizmo::OPERATION::UNIVERSAL;
    }

    void EditorScene::renderGizmo()
    {
        const auto &coord = nexo::Application::m_coordinator;
        auto const &selector = Selector::get();
        if (selector.getSelectionType() == SelectionType::SCENE ||
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
        if (!ImGuizmo::IsUsing()) {
            lastOperation = getLastGuizmoOperation();
            //m_windowState = m_globalState;
        }

        float *snap = nullptr;
        if (m_snapTranslateOn && lastOperation & ImGuizmo::OPERATION::TRANSLATE) {
            snap = &m_snapTranslate.x;
        } else if (m_snapRotateOn && lastOperation & ImGuizmo::OPERATION::ROTATE) {
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

            m_focused = ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows);
            m_hovered = ImGui::IsWindowHovered(ImGuiFocusedFlags_RootAndChildWindows);
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

            if (m_popupManager.showPopup("Add new entity popup"))
            {
                auto &app = Application::getInstance();
                auto &sceneManager = app.getSceneManager();
                const auto sceneId = m_sceneId;

                // --- Primitives submenu ---
                if (ImGui::BeginMenu("Primitives")) {
                    if (ImGui::MenuItem("Cube")) {
                        const ecs::Entity newCube = EntityFactory3D::createCube({0.0f, 0.0f, -5.0f}, {1.0f, 1.0f, 1.0f},
                                                                               {0.0f, 0.0f, 0.0f}, {0.05f * 1.5, 0.09f * 1.15, 0.13f * 1.25, 1.0f});
                        sceneManager.getScene(sceneId).addEntity(newCube);
                    }
                    ImGui::EndMenu();
                }

                // --- Model item (with file‑dialog) ---
                if (ImGui::MenuItem("Model")) {
                    //TODO: import model
                }

                // --- Lights submenu ---
                if (ImGui::BeginMenu("Lights")) {
                    if (ImGui::MenuItem("Directional")) {
                        const ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.0f, -1.0f, 0.0f});
                        sceneManager.getScene(sceneId).addEntity(directionalLight);
                    }
                    if (ImGui::MenuItem("Point")) {
                        const ecs::Entity pointLight = LightFactory::createPointLight({0.0f, 0.5f, 0.0f});
                        utils::addPropsTo(pointLight, utils::PropsType::POINT_LIGHT);
                        sceneManager.getScene(sceneId).addEntity(pointLight);
                    }
                    if (ImGui::MenuItem("Spot")) {
                        const ecs::Entity spotLight = LightFactory::createSpotLight({0.0f, 0.5f, 0.0f}, {0.0f, -1.0f, 0.0f});
                        utils::addPropsTo(spotLight, utils::PropsType::SPOT_LIGHT);
                        sceneManager.getScene(sceneId).addEntity(spotLight);
                    }
                    ImGui::EndMenu();
                }

                // --- Camera item ---
                if (ImGui::MenuItem("Camera")) {
                    m_popupManager.openPopupWithCallback("Popup camera inspector", [this]() {
                        ImNexo::CameraInspector(this->m_sceneId, this->m_viewSize);
                    }, ImVec2(1440,900));
                }
                m_popupManager.closePopup();
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
                m_windowState = m_globalState;
             	return;
            }
            const auto uuid = Application::m_coordinator->tryGetComponent<components::UuidComponent>(data);
            if (uuid)
            {
                if (m_currentGizmoOperation == ImGuizmo::OPERATION::TRANSLATE)
                    m_windowState = m_gizmoTranslateState;
                else if (m_currentGizmoOperation == ImGuizmo::OPERATION::ROTATE)
                    m_windowState = m_gizmoRotateState;
                else
                    m_windowState = m_gizmoState;
                auto &app = getApp();
               	selector.setSelectedEntity(uuid->get().uuid, data);
                if (app.m_coordinator->entityHasComponent<components::CameraComponent>(data))
                    selector.setSelectionType(SelectionType::CAMERA);
                else
                    selector.setSelectionType(SelectionType::ENTITY);
            }
            selector.setSelectedScene(m_sceneId);
        }
    }

}
