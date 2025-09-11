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

    static void hideCallback()
    {
        auto &selector = Selector::get();
        const auto &selectedEntities = selector.getSelectedEntities();
        auto& actionManager = ActionManager::get();
        auto actionGroup = ActionManager::createActionGroup();
        for (const auto entity : selectedEntities) {
            auto &renderComponent = Application::m_coordinator->getComponent<components::RenderComponent>(entity);
            auto beforeState = renderComponent.save();
            renderComponent.isRendered = !renderComponent.isRendered;
            auto afterState = renderComponent.save();
            actionGroup->addAction(std::make_unique<ComponentChangeAction<components::RenderComponent>>(
                entity, beforeState, afterState));
        }
        actionManager.recordAction(std::move(actionGroup));
        selector.clearSelection();
    }

    void EditorScene::selectAllCallback()
    {
        auto &selector = Selector::get();
        auto &app = getApp();
        const auto &scene = app.getSceneManager().getScene(m_sceneId);

        selector.clearSelection();

        for (const auto entity : scene.getEntities()) {
            if (static_cast<int>(entity) == m_editorCamera) continue; // Skip editor camera

            const auto uuidComponent = Application::m_coordinator->tryGetComponent<components::UuidComponent>(entity);
            if (uuidComponent)
                selector.addToSelection(uuidComponent->get().uuid, static_cast<int>(entity));
        }
        m_windowState = m_gizmoState;
    }

    void EditorScene::hideAllButSelectionCallback() const
    {
        auto &app = getApp();
        const auto &entities = app.getSceneManager().getScene(m_sceneId).getEntities();
        const auto &selector = Selector::get();
        auto &actionManager = ActionManager::get();
        auto actionGroup = ActionManager::createActionGroup();
        for (const auto entity : entities) {
            if (Application::m_coordinator->entityHasComponent<components::RenderComponent>(entity) && !selector.isEntitySelected(static_cast<int>(entity))) {
                auto &renderComponent = Application::m_coordinator->getComponent<components::RenderComponent>(entity);
                if (renderComponent.isRendered) {
                    auto beforeState = renderComponent.save();
                    renderComponent.isRendered = false;
                    auto afterState = renderComponent.save();
                    actionGroup->addAction(std::make_unique<ComponentChangeAction<components::RenderComponent>>(
                        entity, beforeState, afterState));
                }
            }
        }
        actionManager.recordAction(std::move(actionGroup));
    }

    void EditorScene::deleteCallback()
    {
        auto &selector = Selector::get();
        const auto &selectedEntities = selector.getSelectedEntities();
        auto &app = getApp();
        auto& actionManager = ActionManager::get();

        if (selectedEntities.empty())
            return;

        if (selectedEntities.size() > 1) {
            auto actionGroup = ActionManager::createActionGroup();
            for (const auto entity : selectedEntities) {
                actionGroup->addAction(ActionManager::prepareEntityHierarchyDeletion(entity));
                app.deleteEntity(entity);
            }
            actionManager.recordAction(std::move(actionGroup));
        } else {
            auto deleteAction = ActionManager::prepareEntityHierarchyDeletion(selectedEntities[0]);
            app.deleteEntity(selectedEntities[0]);
            actionManager.recordAction(std::move(deleteAction));
        }

        selector.clearSelection();
        this->m_windowState = m_globalState;
    }


    void EditorScene::unhideAllCallback() const
    {
        auto &app = getApp();
        const auto &entities = app.getSceneManager().getScene(m_sceneId).getEntities();
        auto &actionManager = ActionManager::get();
        auto actionGroup = ActionManager::createActionGroup();
        for (const auto entity : entities) {
            if (Application::m_coordinator->entityHasComponent<components::RenderComponent>(entity)) {
                auto &renderComponent = Application::m_coordinator->getComponent<components::RenderComponent>(entity);
                if (!renderComponent.isRendered) {
                    auto beforeState = renderComponent.save();
                    renderComponent.isRendered = true;
                    auto afterState = renderComponent.save();
                    actionGroup->addAction(std::make_unique<ComponentChangeAction<components::RenderComponent>>(
                        entity, beforeState, afterState));
                }
            }
        }
        actionManager.recordAction(std::move(actionGroup));
    }

    void EditorScene::setupGlobalState()
    {
        // ================= GLOBAL STATE =============================
        m_globalState = {static_cast<unsigned int>(EditorState::GLOBAL)};

        // Shift context
        m_globalState.registerCommand(
            Command::create()
                .description("Shift context")
                .key("Shift")
                .modifier(true)
                .addChild(
                    Command::create()
                        .description("Add entity")
                        .key("A")
                        .onPressed([this]{ this->m_popupManager.openPopup("Add new entity popup"); })
                        .build()
                )
                .build()
        );

        // Control context
        m_globalState.registerCommand(
            Command::create()
                .description("Control context")
                .key("Ctrl")
                .modifier(true)
                .addChild(
                    Command::create()
                        .description("Unhide all")
                        .key("H")
                        .onPressed([this]() { this->unhideAllCallback(); })
                        .build()
                )
                .build()
        );

        // Select all
        m_globalState.registerCommand(
            Command::create()
                .description("Select all")
                .key("A")
                .onPressed([this]{ this->selectAllCallback(); })
                .build()
        );

        m_globalState.registerCommand(
            Command::create()
                .description("Start next timecode")
                .key("Space")
                .onPressed([this]{ this->startNextTimecode(); })
                .build()
        );
    }

    void EditorScene::setupGizmoState()
    {
        // ================= GIZMO STATE =============================
        m_gizmoState = {static_cast<unsigned int>(EditorState::GIZMO)};

        // Delete
        m_gizmoState.registerCommand(
            Command::create()
                .description("Delete")
                .key("Delete")
                .onPressed([this]() { this->deleteCallback(); })
                .build()
        );

        // Hide
        m_gizmoState.registerCommand(
            Command::create()
                .description("Hide")
                .key("H")
                .onPressed(&hideCallback)
                .build()
        );

        // Translate
        m_gizmoState.registerCommand(
            Command::create()
                .description("Translate")
                .key("G")
                .onPressed([this]{
                    this->m_windowState = m_gizmoTranslateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                })
                .build()
        );

        // Rotate
        m_gizmoState.registerCommand(
            Command::create()
                .description("Rotate")
                .key("R")
                .onPressed([this]{
                    this->m_windowState = m_gizmoRotateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                })
                .build()
        );

        // Scale
        m_gizmoState.registerCommand(
            Command::create()
                .description("Scale")
                .key("S")
                .onPressed([this]{
                    this->m_windowState = m_gizmoScaleState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                })
                .build()
        );

        // Shift context
        m_gizmoState.registerCommand(
            Command::create()
                .description("Shift context")
                .key("Shift")
                .modifier(true)
                .addChild(
                    Command::create()
                        .description("Toggle snapping")
                        .key("S")
                        .onPressed([this]{
                            m_snapTranslateOn = true;
                            m_snapRotateOn = true;
                        })
                        .onReleased([this]{
                            m_snapTranslateOn = false;
                            m_snapRotateOn = false;
                        })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Hide all but selection")
                        .key("H")
                        .onPressed([this]{ this->hideAllButSelectionCallback(); })
                        .build()
                )
                .build()
        );

        m_gizmoState.registerCommand(
            Command::create()
                .description("Start next timecode")
                .key("Space")
                .onPressed([this]{ this->startNextTimecode(); })
                .build()
        );
    }

    void EditorScene::setupGizmoTranslateState()
    {
        // ================= TRANSLATE STATE =============================
        m_gizmoTranslateState = {static_cast<unsigned int>(EditorState::GIZMO_TRANSLATE)};

        // Universal
        m_gizmoTranslateState.registerCommand(
            Command::create()
                .description("Universal")
                .key("U")
                .onPressed([this]{
                    this->m_windowState = m_gizmoState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::UNIVERSAL;
                })
                .build()
        );

        // Translate
        m_gizmoTranslateState.registerCommand(
            Command::create()
                .description("Translate")
                .key("G")
                .onPressed([this]{
                    this->m_windowState = m_gizmoTranslateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                })
                .onRepeat([this]{
                    if (this->m_currentGizmoMode == ImGuizmo::MODE::LOCAL)
                        this->m_currentGizmoMode = ImGuizmo::MODE::WORLD;
                    else
                        this->m_currentGizmoMode = ImGuizmo::MODE::LOCAL;
                })
                .build()
        );

        // Rotate
        m_gizmoTranslateState.registerCommand(
            Command::create()
                .description("Rotate")
                .key("R")
                .onPressed([this]{
                    this->m_windowState = m_gizmoRotateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                })
                .build()
        );

        // Scale
        m_gizmoTranslateState.registerCommand(
            Command::create()
                .description("Scale")
                .key("S")
                .onPressed([this]{
                    this->m_windowState = m_gizmoScaleState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                })
                .build()
        );

        // Shift context
        m_gizmoTranslateState.registerCommand(
            Command::create()
                .description("Shift context")
                .key("Shift")
                .modifier(true)
                .addChild(
                    Command::create()
                        .description("Exclude X")
                        .key("X")
                        .onPressed([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::TRANSLATE_X);
                        })
                        .onReleased([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::TRANSLATE_X);
                        })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Exclude Y")
                        .key("Y")
                        .onPressed([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::TRANSLATE_Y);
                        })
                        .onReleased([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::TRANSLATE_Y);
                        })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Exclude Z")
                        .key("Z")
                        .onPressed([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::TRANSLATE_Z);
                        })
                        .onReleased([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::TRANSLATE_Z);
                        })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Toggle snapping")
                        .key("S")
                        .onPressed([this]{
                            m_snapTranslateOn = true;
                        })
                        .onReleased([this]{
                            m_snapTranslateOn = false;
                        })
                        .build()
                )
                .build()
        );

        // Lock X
        m_gizmoTranslateState.registerCommand(
            Command::create()
                .description("Lock X")
                .key("X")
                .onPressed([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE_X;
                })
                .onReleased([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                })
                .build()
        );

        // Lock Y
        m_gizmoTranslateState.registerCommand(
            Command::create()
                .description("Lock Y")
                .key("Y")
                .onPressed([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE_Y;
                })
                .onReleased([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                })
                .build()
        );

        // Lock Z
        m_gizmoTranslateState.registerCommand(
            Command::create()
                .description("Lock Z")
                .key("Z")
                .onPressed([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE_Z;
                })
                .onReleased([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                })
                .build()
        );
    }

    void EditorScene::setupGizmoRotateState()
    {
        // ================= ROTATE STATE =============================
        m_gizmoRotateState = {static_cast<unsigned int>(EditorState::GIZMO_ROTATE)};

        // Universal
        m_gizmoRotateState.registerCommand(
            Command::create()
                .description("Universal")
                .key("U")
                .onPressed([this]{
                    this->m_windowState = m_gizmoState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::UNIVERSAL;
                })
                .build()
        );

        // Rotate
        m_gizmoRotateState.registerCommand(
            Command::create()
                .description("Rotate")
                .key("R")
                .onPressed([this]{
                    this->m_windowState = m_gizmoRotateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                })
                .onRepeat([this]{
                    if (this->m_currentGizmoMode == ImGuizmo::MODE::LOCAL)
                        this->m_currentGizmoMode = ImGuizmo::MODE::WORLD;
                    else
                        this->m_currentGizmoMode = ImGuizmo::MODE::LOCAL;
                })
                .build()
        );

        // Translate
        m_gizmoRotateState.registerCommand(
            Command::create()
                .description("Translate")
                .key("G")
                .onPressed([this]{
                    this->m_windowState = m_gizmoTranslateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                })
                .build()
        );

        // Scale
        m_gizmoRotateState.registerCommand(
            Command::create()
                .description("Scale")
                .key("S")
                .onPressed([this]{
                    this->m_windowState = m_gizmoScaleState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                })
                .build()
        );

        // Shift context
        m_gizmoRotateState.registerCommand(
            Command::create()
                .description("Shift context")
                .key("Shift")
                .modifier(true)
                .addChild(
                    Command::create()
                        .description("Exclude X")
                        .key("X")
                        .onPressed([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::ROTATE_X);
                        })
                        .onReleased([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::ROTATE_X);
                        })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Exclude Y")
                        .key("Y")
                        .onPressed([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::ROTATE_Y);
                        })
                        .onReleased([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::ROTATE_Y);
                        })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Exclude Z")
                        .key("Z")
                        .onPressed([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::ROTATE_Z);
                        })
                        .onReleased([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation | ImGuizmo::OPERATION::ROTATE_Z);
                        })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Toggle snapping")
                        .key("S")
                        .onPressed([this]{
                            m_snapRotateOn = true;
                        })
                        .onReleased([this]{
                            m_snapRotateOn = false;
                        })
                        .build()
                )
                .build()
        );

        // Lock X
        m_gizmoRotateState.registerCommand(
            Command::create()
                .description("Lock X")
                .key("X")
                .onPressed([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE_X;
                })
                .onReleased([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                })
                .build()
        );

        // Lock Y
        m_gizmoRotateState.registerCommand(
            Command::create()
                .description("Lock Y")
                .key("Y")
                .onPressed([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE_Y;
                })
                .onReleased([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                })
                .build()
        );

        // Lock Z
        m_gizmoRotateState.registerCommand(
            Command::create()
                .description("Lock Z")
                .key("Z")
                .onPressed([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE_Z;
                })
                .onReleased([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                })
                .build()
        );
    }

    void EditorScene::setupGizmoScaleState()
    {
        // ================= SCALE STATE =============================
        m_gizmoScaleState = {static_cast<unsigned int>(EditorState::GIZMO_SCALE)};

        // Universal
        m_gizmoScaleState.registerCommand(
            Command::create()
                .description("Universal")
                .key("U")
                .onPressed([this]{
                    this->m_windowState = m_gizmoState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::UNIVERSAL;
                })
                .build()
        );

        // Scale
        m_gizmoScaleState.registerCommand(
            Command::create()
                .description("Scale")
                .key("S")
                .onPressed([this]{
                    this->m_windowState = m_gizmoScaleState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                })
                .onRepeat([this]{
                    if (this->m_currentGizmoMode == ImGuizmo::MODE::LOCAL)
                        this->m_currentGizmoMode = ImGuizmo::MODE::WORLD;
                    else
                        this->m_currentGizmoMode = ImGuizmo::MODE::LOCAL;
                })
                .build()
        );

        // Translate
        m_gizmoScaleState.registerCommand(
            Command::create()
                .description("Translate")
                .key("G")
                .onPressed([this]{
                    this->m_windowState = m_gizmoTranslateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::TRANSLATE;
                })
                .build()
        );

        // Rotate
        m_gizmoScaleState.registerCommand(
            Command::create()
                .description("Rotate")
                .key("R")
                .onPressed([this]{
                    this->m_windowState = m_gizmoRotateState;
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::ROTATE;
                })
                .build()
        );

        // Shift context
        m_gizmoScaleState.registerCommand(
            Command::create()
                .description("Shift context")
                .key("Shift")
                .modifier(true)
                .addChild(
                    Command::create()
                        .description("Exclude X")
                        .key("X")
                        .onPressed([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::SCALE_X);
                        })
                        .onReleased([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::SCALE_X);
                        })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Exclude Y")
                        .key("Y")
                        .onPressed([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::SCALE_Y);
                        })
                        .onReleased([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::SCALE_Y);
                        })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Exclude Z")
                        .key("Z")
                        .onPressed([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ~ImGuizmo::OPERATION::SCALE_Z);
                        })
                        .onReleased([this]{
                            m_currentGizmoOperation =
                                static_cast<ImGuizmo::OPERATION>(m_currentGizmoOperation & ImGuizmo::OPERATION::SCALE_Z);
                        })
                        .build()
                )
                .build()
        );

        // Lock X
        m_gizmoScaleState.registerCommand(
            Command::create()
                .description("Lock X")
                .key("X")
                .onPressed([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE_X;
                })
                .onReleased([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                })
                .build()
        );

        // Lock Y
        m_gizmoScaleState.registerCommand(
            Command::create()
                .description("Lock Y")
                .key("Y")
                .onPressed([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE_Y;
                })
                .onReleased([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                })
                .build()
        );

        // Lock Z
        m_gizmoScaleState.registerCommand(
            Command::create()
                .description("Lock Z")
                .key("Z")
                .onPressed([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE_Z;
                })
                .onReleased([this]{
                    this->m_currentGizmoOperation = ImGuizmo::OPERATION::SCALE;
                })
                .build()
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
