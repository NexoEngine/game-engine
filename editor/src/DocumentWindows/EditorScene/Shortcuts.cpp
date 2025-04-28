//// Shortcuts.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the shortcuts init of the editor scene
//
///////////////////////////////////////////////////////////////////////////////

#include "EditorScene.hpp"
#include "context/Selector.hpp"
#include "context/ActionManager.hpp"
#include "components/Uuid.hpp"

namespace nexo::editor {

    void EditorScene::setupGlobalState()
    {
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
                "Control context",
                "Ctrl",
                nullptr,
                nullptr,
                nullptr,
                true,
                {
                    {
                        "Unhide all",
                        "H",
                        [this]{
                            auto &app = getApp();
                            const auto &entities = app.getSceneManager().getScene(m_sceneId).getEntities();
                            for (const auto entity : entities) {
                                if (app.m_coordinator->entityHasComponent<components::RenderComponent>(entity)) {
                                    auto &renderComponent = app.m_coordinator->getComponent<components::RenderComponent>(entity);
                                    renderComponent.isRendered = true;
                                }
                            }
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
                    auto &selector = Selector::get();
                    auto &app = nexo::getApp();
                    auto &scene = app.getSceneManager().getScene(m_sceneId);

                    selector.clearSelection();

                    for (const auto entity : scene.getEntities()) {
                        if (entity == m_editorCamera) continue; // Skip editor camera

                        const auto uuidComponent = app.m_coordinator->tryGetComponent<components::UuidComponent>(entity);
                        if (uuidComponent)
                            selector.addToSelection(uuidComponent->get().uuid, entity);
                    }
                    m_windowState = m_gizmoState;
                },
                nullptr,
                nullptr,
                false
            }
        );
    }

    void EditorScene::setupGizmoState()
    {
        // ================= GIZMO STATE =============================
        m_gizmoState = {static_cast<unsigned int>(EditorState::GIZMO)};
        m_gizmoState.registerCommand(
            {
                "Delete",
                "Delete",
                [this]{
                    auto &selector = Selector::get();
                    const auto &selectedEntities = selector.getSelectedEntities();
                    auto &app = nexo::getApp();
                    auto& actionManager = ActionManager::get();
                    if (selectedEntities.size() > 1) {
                        auto actionGroup = actionManager.createActionGroup();
                        for (const auto entity : selectedEntities) {
                            actionGroup->addAction(actionManager.prepareEntityDeletion(entity));
                            app.deleteEntity(entity);
                        }
                        actionManager.recordAction(std::move(actionGroup));
                    } else {
                        auto deleteAction = actionManager.prepareEntityDeletion(selectedEntities[0]);
                        app.deleteEntity(selectedEntities[0]);
                        actionManager.recordAction(std::move(deleteAction));
                    }
                    selector.clearSelection();
                    this->m_windowState = m_globalState;
                },
                nullptr,
                nullptr,
                false,
            }
        );
        m_gizmoState.registerCommand(
            {
                "Hide",
                "H",
                [this]{
                    auto &selector = Selector::get();
                    const auto &selectedEntities = selector.getSelectedEntities();
                    auto &app = nexo::getApp();
                    auto& actionManager = ActionManager::get();
                    if (selectedEntities.size() > 1) {
                        auto actionGroup = actionManager.createActionGroup();
                        for (const auto entity : selectedEntities) {
                            auto &renderComponent = app.m_coordinator->getComponent<components::RenderComponent>(entity);
                            auto beforeState = renderComponent.save();
                            renderComponent.isRendered = !renderComponent.isRendered;
                            auto afterState = renderComponent.save();
                            actionGroup->addAction(std::make_unique<ComponentChangeAction<components::RenderComponent>>(
                                entity, beforeState, afterState));
                        }
                        actionManager.recordAction(std::move(actionGroup));
                    } else {
                        auto &renderComponent = app.m_coordinator->getComponent<components::RenderComponent>(selectedEntities[0]);
                        auto beforeState = renderComponent.save();
                        renderComponent.isRendered = !renderComponent.isRendered;
                        auto afterState = renderComponent.save();
                        actionManager.recordAction(std::move(std::make_unique<ComponentChangeAction<components::RenderComponent>>(
                            selectedEntities[0], beforeState, afterState)));
                    }
                    selector.clearSelection();
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
                    },
                    {
                        "Hide all but selection",
                        "H",
                        [this]{
                            auto &app = getApp();
                            const auto &entities = app.getSceneManager().getScene(m_sceneId).getEntities();
                            auto &selector = Selector::get();
                            for (const auto entity : entities) {
                                if (app.m_coordinator->entityHasComponent<components::RenderComponent>(entity) && !selector.isEntitySelected(entity)) {
                                    auto &renderComponent = app.m_coordinator->getComponent<components::RenderComponent>(entity);
                                    renderComponent.isRendered = false;
                                }
                            }
                        },
                        nullptr,
                        nullptr,
                        false,
                    }
                }
            }
        );
    }

    void EditorScene::setupGizmoTranslateState()
    {

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
    }

    void EditorScene::setupGizmoRotateState()
    {
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
    }

    void EditorScene::setupGizmoScaleState()
    {
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

    void EditorScene::setupShortcuts()
    {
        setupGlobalState();
        m_windowState = m_globalState;

        setupGizmoState();
        setupGizmoTranslateState();
        setupGizmoRotateState();
        setupGizmoScaleState();
    }
}
