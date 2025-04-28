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
                for (const auto& entityId : selectedEntities) {
                    app.deleteEntity(entityId);
                }
                selector.clearSelection();
            } else {
                // Delete just this entity
                selector.clearSelection();
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
            "Delete Selected Cameras (" + std::to_string(selectedEntities.size()) + ")" :
            "Delete Camera";

        if (ImGui::MenuItem(deleteMenuText.c_str()))
        {
            if (multipleSelected) {
                // Delete all selected cameras
                for (const auto& entityId : selectedEntities) {
                    app.deleteEntity(entityId);
                }
                selector.clearSelection();
            } else {
                // Delete just this camera
                selector.clearSelection();
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
            "Delete Selected Lights (" + std::to_string(selectedEntities.size()) + ")" :
            "Delete Light";

        if (ImGui::MenuItem(menuText.c_str()))
        {
            if (multipleSelected) {
                // Delete all selected lights
                for (const auto& entityId : selectedEntities) {
                    app.deleteEntity(entityId);
                }
                selector.clearSelection();
            } else {
                // Delete just this light
                selector.clearSelection();
                app.deleteEntity(obj.data.entity);
            }
        }
    }

    void SceneTreeWindow::sceneSelected([[maybe_unused]] const SceneObject &obj)
    {
        if (ImGui::MenuItem("Delete Scene")) {
            auto &app = Application::getInstance();
            auto &selector = Selector::get();
            selector.clearSelection();
            const std::string &sceneName = selector.getUiHandle(obj.uuid, obj.uiName);
            m_windowRegistry.unregisterWindow<EditorScene>(sceneName);
            app.getSceneManager().deleteScene(obj.data.sceneProperties.sceneId);
        }

        // ---- Add Entity submenu ----
        if (ImGui::BeginMenu("Add Entity")) {
            auto &app = Application::getInstance();
            auto &sceneManager = app.getSceneManager();
            const auto sceneId = obj.data.sceneProperties.sceneId;

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
                m_popupManager.openPopupWithCallback("Popup camera inspector", [this, obj]() {
                    const auto &editorScenes = m_windowRegistry.getWindows<EditorScene>();
                    for (const auto &scene : editorScenes) {
                        if (scene->getSceneId() == obj.data.sceneProperties.sceneId) {
                            ImNexo::CameraInspector(obj.data.sceneProperties.sceneId, scene->getViewportSize());
                            break;
                        }
                    }
                }, ImVec2(1440,900));
            }

            ImGui::EndMenu();
        }
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
