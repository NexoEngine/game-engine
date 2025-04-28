//// Show.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the scene tree window rendering
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "components/Uuid.hpp"

namespace nexo::editor {

    void SceneTreeWindow::sceneContextMenu()
    {
        if (m_popupManager.showPopup("Scene Tree Context Menu"))
        {
            if (ImGui::MenuItem("Create Scene"))
                m_popupManager.openPopup("Create New Scene");
            m_popupManager.closePopup();
        }

        if (m_popupManager.showPopupModal("Popup camera inspector")) {
            m_popupManager.runPopupCallback("Popup camera inspector");
            m_popupManager.closePopup();
        }
    }

    void SceneTreeWindow::sceneCreationMenu()
    {
        if (!m_popupManager.showPopupModal("Create New Scene"))
            return;

        static char sceneNameBuffer[256] = "";

        ImGui::Text("Enter Scene Name:");
        ImGui::InputText("##SceneName", sceneNameBuffer, sizeof(sceneNameBuffer));

        if (ImNexo::Button("Create")) {
            if (handleSceneCreation(sceneNameBuffer)) {
                memset(sceneNameBuffer, 0, sizeof(sceneNameBuffer));
                m_popupManager.closePopupInContext();
            }
        }

        ImGui::SameLine();
        if (ImNexo::Button("Cancel"))
            m_popupManager.closePopupInContext();

        m_popupManager.closePopup();
    }

    void SceneTreeWindow::showNode(SceneObject &object)
    {
        ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                       ImGuiTreeNodeFlags_SpanAvailWidth;
        // Checks if the object is at the end of a tree
        const bool leaf = object.children.empty();
        if (leaf)
            baseFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        // Check if this object is selected
        auto const &selector = Selector::get();
        bool isSelected = selector.isEntitySelected(object.data.entity);

        if (isSelected)
            baseFlags |= ImGuiTreeNodeFlags_Selected;

        bool nodeOpen = false;
        const std::string uniqueLabel = object.uiName;

        // If the user wishes to rename handle the rename, else handle the selection
        if (m_renameTarget && m_renameTarget->first == object.type && m_renameTarget->second == object.uuid)
            handleRename(object);
        else
            nodeOpen = handleSelection(object, uniqueLabel, baseFlags);

        handleHovering(object);

        // Handles the right click on each different type of object
        if (object.type != SelectionType::NONE && ImGui::BeginPopupContextItem(uniqueLabel.c_str()))
        {
            // Only show rename option for the primary selected entity or for non-selected entities
            if ((!isSelected || (isSelected && selector.getPrimaryEntity() == object.data.entity)) &&
                ImGui::MenuItem("Rename"))
            {
                m_renameTarget = {object.type, object.uuid};
                m_renameBuffer = object.uiName;
            }

            if (object.type == SelectionType::SCENE)
                sceneSelected(object);
            else if (object.type == SelectionType::DIR_LIGHT || object.type == SelectionType::POINT_LIGHT || object.type == SelectionType::SPOT_LIGHT)
                lightSelected(object);
            else if (object.type == SelectionType::CAMERA)
                cameraSelected(object);
            else if (object.type == SelectionType::ENTITY)
                entitySelected(object);
            ImGui::EndPopup();
        }

        // Go further into the tree
        if (nodeOpen && !leaf)
        {
            for (auto &child: object.children)
            {
                showNode(child);
            }
            ImGui::TreePop();
        }
    }

    void SceneTreeWindow::show()
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 40), ImGuiCond_FirstUseEver);

        if (ImGui::Begin(ICON_FA_SITEMAP " Scene Tree" NEXO_WND_USTRID_SCENE_TREE, &m_opened, ImGuiWindowFlags_NoCollapse))
        {
            firstDockSetup(NEXO_WND_USTRID_SCENE_TREE);

            auto &selector = Selector::get();
            bool isCtrlPressed = ImGui::IsKeyDown(ImGuiKey_LeftCtrl) || ImGui::IsKeyDown(ImGuiKey_RightCtrl);

            // Ctrl+A to select all entities in current scene
            if (isCtrlPressed && ImGui::IsKeyPressed(ImGuiKey_A) && ImGui::IsWindowFocused()) {
                // Get current scene ID
                int currentSceneId = selector.getSelectedScene();
                if (currentSceneId != -1) {
                    auto &app = nexo::getApp();
                    auto &scene = app.getSceneManager().getScene(currentSceneId);

                    selector.clearSelection();

                    // Add all entities in the scene to selection
                    for (const auto entity : scene.getEntities()) {
                        const auto uuidComponent = app.m_coordinator->tryGetComponent<components::UuidComponent>(entity);
                        if (uuidComponent) {
                            selector.addToSelection(uuidComponent->get().uuid, entity);
                        }
                    }
                }
            }

            // Opens the right click popup when no items are hovered
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(
                    ImGuiHoveredFlags_AllowWhenBlockedByPopup) && !ImGui::IsAnyItemHovered()) {
                m_popupManager.openPopup("Scene Tree Context Menu");
            }

            // Display multi-selection count at top of window if applicable
            const auto& selectedEntities = selector.getSelectedEntities();
            if (selectedEntities.size() > 1) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
                ImGui::Text("%zu entities selected", selectedEntities.size());
                ImGui::PopStyleColor();
                ImGui::Separator();
            }

            if (!root_.children.empty()) {
                for (auto &node : root_.children)
                    showNode(node);
            }
            sceneContextMenu();
            sceneCreationMenu();
        }
        ImGui::End();
    }
}
