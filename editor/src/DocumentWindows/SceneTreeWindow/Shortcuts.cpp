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
                        .onPressed([](){
                            selectAllCallback(); })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Duplicate")
                        .key("D")
                        .onPressed([](){ duplicateSelectedCallback(); })
                        .build()
                )
                .addChild(
                    Command::create()
                        .description("Unhide all")
                        .key("H")
                        .onPressed([](){ showAllCallback(); })
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
                .onPressed([this](){ addEntityCallback(); })
                .build()
        );

        // Delete selected
        m_defaultState.registerCommand(
            Command::create()
                .description("Delete")
                .key("Delete")
                .onPressed([this](){ deleteSelectedCallback(); })
                .build()
        );

        // Rename selected
        m_defaultState.registerCommand(
            Command::create()
                .description("Rename")
                .key("F2")
                .onPressed([this](){ renameSelectedCallback(); })
                .build()
        );

        // Expand all nodes
        m_defaultState.registerCommand(
            Command::create()
                .description("Expand all")
                .key("Down")
                .onPressed([this](){ expandAllCallback(); })
                .build()
        );

        // Collapse all nodes
        m_defaultState.registerCommand(
            Command::create()
                .description("Collapse all")
                .key("Up")
                .onPressed([this](){ collapseAllCallback(); })
                .build()
        );

        // Hide selected
        m_defaultState.registerCommand(
            Command::create()
                .description("Hide")
                .key("H")
                .onPressed([](){ hideSelectedCallback(); })
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
            [this, currentSceneId]() {showSceneSelectionContextMenu(currentSceneId);}
        );
    }

    void SceneTreeWindow::selectAllCallback()
    {
        auto& selector = Selector::get();
        const int currentSceneId = selector.getSelectedScene();

        if (currentSceneId != -1) {
            auto& app = getApp();
            const auto& scene = app.getSceneManager().getScene(currentSceneId);

            selector.clearSelection();

            for (const auto entity : scene.getEntities()) {
                const auto uuidComponent = Application::m_coordinator->tryGetComponent<components::UuidComponent>(entity);
                if (uuidComponent) {
                    selector.addToSelection(uuidComponent->get().uuid, static_cast<int>(entity));
                }
            }
        }
    }

    void SceneTreeWindow::deleteSelectedCallback()
    {
        auto& selector = Selector::get();
        const auto& selectedEntities = selector.getSelectedEntities();

        if (selectedEntities.empty())
            return;

        auto& app = getApp();
        auto& actionManager = ActionManager::get();

        if (selectedEntities.size() > 1) {
            auto actionGroup = ActionManager::createActionGroup();
            for (const auto entity : selectedEntities) {
                actionGroup->addAction(std::make_unique<EntityHierarchyDeletionAction>(entity));
                app.deleteEntity(entity);
            }
            actionManager.recordAction(std::move(actionGroup));
        } else {
            auto deleteAction = std::make_unique<EntityHierarchyDeletionAction>(selectedEntities[0]);
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

    void SceneTreeWindow::collapseAllCallback()
    {
        m_forceCollapseAll = true;
        m_forceExpandAll = false;
    }

    void SceneTreeWindow::renameSelectedCallback()
    {
        //TODO: Implement rename callback
    }

    void SceneTreeWindow::duplicateSelectedCallback()
    {
        auto& selector = Selector::get();
        const auto& selectedEntities = selector.getSelectedEntities();

        if (selectedEntities.empty())
            return;

        auto& app = nexo::getApp();
        auto& actionManager = ActionManager::get();
        const int currentSceneId = selector.getSelectedScene();

        if (currentSceneId == -1)
            return;

        std::vector<ecs::Entity> newEntities;
        newEntities.reserve(selectedEntities.size());
        auto actionGroup = ActionManager::createActionGroup();
        selector.clearSelection();

        for (const auto entity : selectedEntities) {
            ecs::Entity newEntity = Application::m_coordinator->duplicateEntity(entity);
            const components::UuidComponent uuidComponent;
            Application::m_coordinator->getComponent<components::UuidComponent>(newEntity) = uuidComponent;
            Application::m_coordinator->removeComponent<components::SceneTag>(newEntity);
            app.getSceneManager().getScene(currentSceneId).addEntity(newEntity);
            auto action = std::make_unique<EntityCreationAction>(newEntity);
            actionGroup->addAction(std::move(action));
            newEntities.push_back(newEntity);
        }

        actionManager.recordAction(std::move(actionGroup));

        // Select all the newly created entities
        for (const auto newEntity : newEntities) {
            const auto uuidComponent = Application::m_coordinator->tryGetComponent<components::UuidComponent>(newEntity);
            if (uuidComponent) {
                selector.addToSelection(uuidComponent->get().uuid, static_cast<int>(newEntity));
            }
        }
    }

    void SceneTreeWindow::hideSelectedCallback()
    {
        const auto& selector = Selector::get();
        const auto& selectedEntities = selector.getSelectedEntities();

        if (selectedEntities.empty())
            return;

        auto& actionManager = ActionManager::get();
        auto actionGroup = ActionManager::createActionGroup();

        for (const auto entity : selectedEntities) {
            if (Application::m_coordinator->entityHasComponent<components::RenderComponent>(entity)) {
                auto& renderComponent = Application::m_coordinator->getComponent<components::RenderComponent>(entity);
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

    void SceneTreeWindow::showAllCallback()
    {
        const auto& selector = Selector::get();
        const int currentSceneId = selector.getSelectedScene();

        if (currentSceneId == -1) return;

        auto& app = getApp();
        const auto& scene = app.getSceneManager().getScene(currentSceneId);
        auto& actionManager = ActionManager::get();
        auto actionGroup = ActionManager::createActionGroup();

        for (const auto entity : scene.getEntities()) {
            if (Application::m_coordinator->entityHasComponent<components::RenderComponent>(entity)) {
                auto& renderComponent = Application::m_coordinator->getComponent<components::RenderComponent>(entity);
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
