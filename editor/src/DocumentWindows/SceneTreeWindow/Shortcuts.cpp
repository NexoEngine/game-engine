//// Shortcuts.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        29/04/2025
//  Description: Source file for the scene tree window shortcuts
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "components/SceneComponents.hpp"
#include "context/ActionManager.hpp"
#include "components/Uuid.hpp"
#include "context/actions/EntityActions.hpp"

namespace nexo::editor {
    void SceneTreeWindow::setupShortcuts()
    {
        setupDefaultState();
        m_windowState = m_defaultState;
    }

    void SceneTreeWindow::setupDefaultState()
    {
        m_defaultState = {static_cast<unsigned int>(SceneTreeState::GLOBAL)};

        // CTRL context
        m_defaultState.registerCommand(
            Command::create()
                .description("Control context")
                .key("Ctrl")
                .modifier(true)
                .addChild(
                    Command::create()
                        .description("Select all")
                        .key("A")
                        .onPressed([this](){
                            this->selectAllCallback(); })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Duplicate")
                        .key("D")
                        .onPressed([this](){ this->duplicateSelectedCallback(); })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Unhide all")
                        .key("H")
                        .onPressed([this](){ this->showAllCallback(); })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Create Scene")
                        .key("N")
                        .onPressed([this](){ this->m_popupManager.openPopup("Create New Scene"); })
                        .build()
                )
                .build()
        );

        // Delete selected
        m_defaultState.registerCommand(
            Command::create()
                .description("Add Entity")
                .key("A")
                .onPressed([this](){ this->addEntityCallback(); })
                .build()
        );

        // Delete selected
        m_defaultState.registerCommand(
            Command::create()
                .description("Delete")
                .key("Delete")
                .onPressed([this](){ this->deleteSelectedCallback(); })
                .build()
        );

        // Rename selected
        m_defaultState.registerCommand(
            Command::create()
                .description("Rename")
                .key("F2")
                .onPressed([this](){ this->renameSelectedCallback(); })
                .build()
        );

        // Expand all nodes
        m_defaultState.registerCommand(
            Command::create()
                .description("Expand all")
                .key("Down")
                .onPressed([this](){ this->expandAllCallback(); })
                .build()
        );

        // Collapse all nodes
        m_defaultState.registerCommand(
            Command::create()
                .description("Collapse all")
                .key("Up")
                .onPressed([this](){ this->collapseAllCallback(); })
                .build()
        );

        // Hide selected
        m_defaultState.registerCommand(
            Command::create()
                .description("Hide")
                .key("H")
                .onPressed([this](){ this->hideSelectedCallback(); })
                .build()
        );
    }

    void SceneTreeWindow::addEntityCallback()
    {
        const auto &selector = Selector::get();
        int currentSceneId = selector.getSelectedScene();
        if (currentSceneId == -1)
            return;

        m_popupManager.openPopupWithCallback(
            "Scene selection context menu",
            [this, currentSceneId]() {this->showSceneSelectionContextMenu(currentSceneId);}
        );
    }

    void SceneTreeWindow::selectAllCallback()
    {
        auto& selector = Selector::get();
        int currentSceneId = selector.getSelectedScene();

        if (currentSceneId != -1) {
            auto& app = nexo::getApp();
            auto& scene = app.getSceneManager().getScene(currentSceneId);

            selector.clearSelection();

            for (const auto entity : scene.getEntities()) {
                const auto uuidComponent = app.m_coordinator->tryGetComponent<components::UuidComponent>(entity);
                if (uuidComponent) {
                    selector.addToSelection(uuidComponent->get().uuid, entity);
                }
            }
        }
    }

    void SceneTreeWindow::deleteSelectedCallback() {
        auto& selector = Selector::get();
        const auto& selectedEntities = selector.getSelectedEntities();

        if (selectedEntities.empty()) return;

        auto& app = nexo::getApp();
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
        m_windowState = m_defaultState;
    }

    void SceneTreeWindow::expandAllCallback()
    {
        m_forceCollapseAll = false;
        m_forceExpandAll = true;
    }

    void SceneTreeWindow::collapseAllCallback() {
        m_forceCollapseAll = true;
        m_forceExpandAll = false;
    }

    void SceneTreeWindow::renameSelectedCallback()
    {
        //TODO: Implement rename callback
    }

    void SceneTreeWindow::duplicateSelectedCallback() {
        auto& selector = Selector::get();
        const auto& selectedEntities = selector.getSelectedEntities();

        if (selectedEntities.empty()) return;

        auto& app = nexo::getApp();
        auto& actionManager = ActionManager::get();
        int currentSceneId = selector.getSelectedScene();

        if (currentSceneId == -1) return;

        std::vector<ecs::Entity> newEntities;
        newEntities.reserve(selectedEntities.size());
        auto actionGroup = actionManager.createActionGroup();
        selector.clearSelection();

        for (const auto entity : selectedEntities) {
            ecs::Entity newEntity = app.m_coordinator->duplicateEntity(entity);
            components::UuidComponent uuidComponent;
            app.m_coordinator->getComponent<components::UuidComponent>(newEntity) = uuidComponent;
            app.m_coordinator->removeComponent<components::SceneTag>(newEntity);
            app.getSceneManager().getScene(currentSceneId).addEntity(newEntity);
            auto action = std::make_unique<EntityCreationAction>(newEntity);
            actionGroup->addAction(std::move(action));
            newEntities.push_back(newEntity);
        }

        actionManager.recordAction(std::move(actionGroup));

        // Select all the newly created entities
        for (const auto newEntity : newEntities) {
            const auto uuidComponent = app.m_coordinator->tryGetComponent<components::UuidComponent>(newEntity);
            if (uuidComponent) {
                selector.addToSelection(uuidComponent->get().uuid, newEntity);
            }
        }
    }

    void SceneTreeWindow::focusOnSelectedCallback() {
        auto& selector = Selector::get();
        const auto& selectedEntities = selector.getSelectedEntities();

        if (selectedEntities.empty()) return;

        // Implementation would focus the editor camera on the selected entity/entities
        // This depends on your camera system
        // For now, just log it
        LOG(NEXO_INFO, "Focusing on {} selected entities", selectedEntities.size());
    }

    void SceneTreeWindow::deselectAllCallback() {
        auto& selector = Selector::get();
        selector.clearSelection();
        m_windowState = m_defaultState;
    }

    void SceneTreeWindow::groupEntitiesCallback() {
        auto& selector = Selector::get();
        const auto& selectedEntities = selector.getSelectedEntities();

        if (selectedEntities.size() <= 1) return; // Need at least 2 entities to group

        // Implementation would create a parent entity and reparent all selected entities to it
        // This depends on your scene hierarchy system
        // For now, just log it
        LOG(NEXO_INFO, "Grouping {} entities", selectedEntities.size());
    }

    void SceneTreeWindow::ungroupEntitiesCallback() {
        auto& selector = Selector::get();
        const auto& selectedEntities = selector.getSelectedEntities();

        if (selectedEntities.empty()) return;

        // Implementation would unparent all children of the selected groups
        // This depends on your scene hierarchy system
        // For now, just log it
        LOG(NEXO_INFO, "Ungrouping {} entities", selectedEntities.size());
    }

    void SceneTreeWindow::invertSelectionCallback() {
        auto& selector = Selector::get();
        int currentSceneId = selector.getSelectedScene();

        if (currentSceneId == -1) return;

        auto& app = nexo::getApp();
        auto& scene = app.getSceneManager().getScene(currentSceneId);

        // Get all entities in the scene
        const std::set<ecs::Entity> &allEntities = scene.getEntities();

        // Get currently selected entities
        const auto& selectedEntities = selector.getSelectedEntities();
        std::unordered_set<ecs::Entity> selectedSet(selectedEntities.begin(), selectedEntities.end());

        // Clear current selection
        selector.clearSelection();

        // Select entities that weren't selected before
        for (const auto entity : allEntities) {
            if (selectedSet.find(entity) == selectedSet.end()) {
                const auto uuidComponent = app.m_coordinator->tryGetComponent<components::UuidComponent>(entity);
                if (uuidComponent) {
                    selector.addToSelection(uuidComponent->get().uuid, entity);
                }
            }
        }
    }

    void SceneTreeWindow::hideSelectedCallback() {
        auto& selector = Selector::get();
        const auto& selectedEntities = selector.getSelectedEntities();

        if (selectedEntities.empty()) return;

        auto& app = nexo::getApp();
        auto& actionManager = ActionManager::get();
        auto actionGroup = actionManager.createActionGroup();

        for (const auto entity : selectedEntities) {
            if (app.m_coordinator->entityHasComponent<components::RenderComponent>(entity)) {
                auto& renderComponent = app.m_coordinator->getComponent<components::RenderComponent>(entity);
                if (renderComponent.isRendered) {
                    auto beforeState = renderComponent.save();
                    renderComponent.isRendered = !renderComponent.isRendered;
                    auto afterState = renderComponent.save();
                    actionGroup->addAction(std::make_unique<ComponentChangeAction<components::RenderComponent>>(
                        entity, beforeState, afterState));
                }
            }
        }

        actionManager.recordAction(std::move(actionGroup));
    }

    void SceneTreeWindow::showSelectedCallback() {
        auto& selector = Selector::get();
        const auto& selectedEntities = selector.getSelectedEntities();

        if (selectedEntities.empty()) return;

        auto& app = nexo::getApp();
        auto& actionManager = ActionManager::get();
        auto actionGroup = actionManager.createActionGroup();

        for (const auto entity : selectedEntities) {
            if (app.m_coordinator->entityHasComponent<components::RenderComponent>(entity)) {
                auto& renderComponent = app.m_coordinator->getComponent<components::RenderComponent>(entity);
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

    void SceneTreeWindow::showAllCallback() {
        auto& selector = Selector::get();
        int currentSceneId = selector.getSelectedScene();

        if (currentSceneId == -1) return;

        auto& app = nexo::getApp();
        auto& scene = app.getSceneManager().getScene(currentSceneId);
        auto& actionManager = ActionManager::get();
        auto actionGroup = actionManager.createActionGroup();

        for (const auto entity : scene.getEntities()) {
            if (app.m_coordinator->entityHasComponent<components::RenderComponent>(entity)) {
                auto& renderComponent = app.m_coordinator->getComponent<components::RenderComponent>(entity);
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
}
