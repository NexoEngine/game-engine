//// Selection.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the selection of the scene tree
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "EntityFactory3D.hpp"
#include "LightFactory.hpp"
#include "utils/EditorProps.hpp"
#include "ImNexo/Panels.hpp"
#include "context/ActionManager.hpp"
#include "context/actions/EntityActions.hpp"

#include <memory>

namespace nexo::editor {

    void SceneTreeWindow::entitySelected(const SceneObject &obj) const
    {
        auto &selector = Selector::get();
        auto &app = nexo::getApp();

        // Check if we're operating on a single item or multiple items
        const auto& selectedEntities = selector.getSelectedEntities();
        bool multipleSelected = selectedEntities.size() > 1;

        std::string menuText = multipleSelected ?
            "Delete Selected Entities (" + std::to_string(selectedEntities.size()) + ")" :
            "Delete Entity";

        if (ImGui::MenuItem(menuText.c_str()))
        {
            if (multipleSelected) {
                // Delete all selected entities
                auto actionGroup = ActionManager::get().createActionGroup();
                for (const auto& entityId : selectedEntities) {
                    auto deleteAction = std::make_unique<EntityDeletionAction>(entityId);
                    actionGroup->addAction(std::move(deleteAction));
                    app.deleteEntity(entityId);
                }
                ActionManager::get().recordAction(std::move(actionGroup));
                selector.clearSelection();
            } else {
                // Delete just this entity
                selector.clearSelection();
                auto action = std::make_unique<EntityDeletionAction>(obj.data.entity);
                ActionManager::get().recordAction(std::move(action));
                app.deleteEntity(obj.data.entity);
            }
        }
    }

    void SceneTreeWindow::cameraSelected(const SceneObject &obj) const
    {
        auto &app = Application::getInstance();
        auto &selector = Selector::get();

        // Check if we're operating on a single item or multiple items
        const auto& selectedEntities = selector.getSelectedEntities();
        bool multipleSelected = selectedEntities.size() > 1;

        std::string deleteMenuText = multipleSelected ?
            "Delete Selected Entities (" + std::to_string(selectedEntities.size()) + ")" :
            "Delete Camera";

        if (ImGui::MenuItem(deleteMenuText.c_str()))
        {
            if (multipleSelected) {
                // Delete all selected entities
                auto actionGroup = ActionManager::get().createActionGroup();
                for (const auto& entityId : selectedEntities) {
                    auto deleteAction = std::make_unique<EntityDeletionAction>(entityId);
                    actionGroup->addAction(std::move(deleteAction));
                    app.deleteEntity(entityId);
                }
                ActionManager::get().recordAction(std::move(actionGroup));
                selector.clearSelection();
            } else {
                // Delete just this entity
                selector.clearSelection();
                auto action = std::make_unique<EntityDeletionAction>(obj.data.entity);
                ActionManager::get().recordAction(std::move(action));
                app.deleteEntity(obj.data.entity);
            }
        }

        // Switch to camera only makes sense for a single camera
        if (!multipleSelected && ImGui::MenuItem("Switch to"))
        {
            auto &cameraComponent = app.m_coordinator->getComponent<components::CameraComponent>(obj.data.entity);
            cameraComponent.render = true;
            cameraComponent.active = true;
            const auto &scenes = m_windowRegistry.getWindows<EditorScene>();
            for (const auto &scene : scenes) {
                if (scene->getSceneId() == obj.data.sceneProperties.sceneId) {
                    scene->setCamera(obj.data.entity);
                    break;
                }
            }
        }
    }

    void SceneTreeWindow::lightSelected(const SceneObject &obj) const
    {
        auto &app = Application::getInstance();
        auto &selector = Selector::get();

        // Check if we're operating on a single item or multiple items
        const auto& selectedEntities = selector.getSelectedEntities();
        bool multipleSelected = selectedEntities.size() > 1;

        std::string menuText = multipleSelected ?
            "Delete Selected Entities (" + std::to_string(selectedEntities.size()) + ")" :
            "Delete Light";

        if (ImGui::MenuItem(menuText.c_str()))
        {
            if (multipleSelected) {
                // Delete all selected entities
                auto actionGroup = ActionManager::get().createActionGroup();
                for (const auto& entityId : selectedEntities) {
                    auto deleteAction = std::make_unique<EntityDeletionAction>(entityId);
                    actionGroup->addAction(std::move(deleteAction));
                    app.deleteEntity(entityId);
                }
                ActionManager::get().recordAction(std::move(actionGroup));
                selector.clearSelection();
            } else {
                // Delete just this entity
                selector.clearSelection();
                auto action = std::make_unique<EntityDeletionAction>(obj.data.entity);
                ActionManager::get().recordAction(std::move(action));
                app.deleteEntity(obj.data.entity);
            }
        }
    }

    void SceneTreeWindow::sceneSelected([[maybe_unused]] const SceneObject &obj)
    {
        m_popupManager.openPopupWithCallback(
            "Scene selection context menu",
            [this, obj]() {this->showSceneSelectionContextMenu(obj.data.sceneProperties.sceneId, obj.uuid, obj.uiName);}
        );
    }

    bool SceneTreeWindow::handleSelection(const SceneObject &obj, const std::string &uniqueLabel,
                                          const ImGuiTreeNodeFlags baseFlags) const
    {
        const bool nodeOpen = ImGui::TreeNodeEx(uniqueLabel.c_str(), baseFlags);
        if (!nodeOpen)
            return nodeOpen;

        if (ImGui::IsItemClicked())
        {
            auto &selector = Selector::get();
            bool isShiftPressed = ImGui::IsKeyDown(ImGuiKey_LeftShift) || ImGui::IsKeyDown(ImGuiKey_RightShift);
            bool isCtrlPressed = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);

            if (isCtrlPressed)
                selector.toggleSelection(obj.uuid, obj.data.entity, obj.type);
            else if (isShiftPressed)
                selector.addToSelection(obj.uuid, obj.data.entity, obj.type);
            else
                selector.selectEntity(obj.uuid, obj.data.entity, obj.type);
            selector.setSelectedScene(obj.data.sceneProperties.sceneId);
        }
        return nodeOpen;
    }
}
