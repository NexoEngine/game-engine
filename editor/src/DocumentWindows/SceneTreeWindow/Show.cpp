//// Show.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the scene tree window rendering
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "EntityFactory3D.hpp"
#include "LightFactory.hpp"
#include "context/actions/EntityActions.hpp"
#include "utils/EditorProps.hpp"
#include "ImNexo/Panels.hpp"
#include "context/ActionManager.hpp"

namespace nexo::editor
{
    void SceneTreeWindow::showSceneSelectionContextMenu(scene::SceneId sceneId, const std::string& uuid,
                                                        const std::string& uiName)
    {
        if (!uuid.empty() && !uiName.empty() && ImGui::MenuItem("Delete Scene"))
        {
            auto& app = Application::getInstance();
            auto& selector = Selector::get();
            selector.clearSelection();
            const std::string& sceneName = selector.getUiHandle(uuid, uiName);
            m_windowRegistry.unregisterWindow<EditorScene>(sceneName);
            app.getSceneManager().deleteScene(sceneId);
        }

        // ---- Add Entity submenu ----
        if (ImGui::BeginMenu("Add Entity"))
        {
            auto& app = Application::getInstance();
            auto& sceneManager = app.getSceneManager();

            // --- Primitives submenu ---
            const auto& selector = Selector::get();
            const int currentSceneId = selector.getSelectedScene();
            ImNexo::PrimitiveSubMenu(currentSceneId, m_popupManager);

            // --- Model item (with file‑dialog) ---
            if (ImGui::MenuItem("Model"))
            {
                //TODO: import model
            }

            // --- Lights submenu ---
            if (ImGui::BeginMenu("Lights"))
            {
                if (ImGui::MenuItem("Directional"))
                {
                    const ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.0f, -1.0f, 0.0f});
                    sceneManager.getScene(sceneId).addEntity(directionalLight);
                    auto action = std::make_unique<EntityCreationAction>(directionalLight);
                    ActionManager::get().recordAction(std::move(action));
                }
                if (ImGui::MenuItem("Point"))
                {
                    const ecs::Entity pointLight = LightFactory::createPointLight({0.0f, 0.5f, 0.0f});
                    addPropsTo(pointLight, utils::PropsType::POINT_LIGHT);
                    sceneManager.getScene(sceneId).addEntity(pointLight);
                    auto action = std::make_unique<EntityCreationAction>(pointLight);
                    ActionManager::get().recordAction(std::move(action));
                }
                if (ImGui::MenuItem("Spot"))
                {
                    const ecs::Entity spotLight =
                        LightFactory::createSpotLight({0.0f, 0.5f, 0.0f}, {0.0f, -1.0f, 0.0f});
                    addPropsTo(spotLight, utils::PropsType::SPOT_LIGHT);
                    sceneManager.getScene(sceneId).addEntity(spotLight);
                    auto action = std::make_unique<EntityCreationAction>(spotLight);
                    ActionManager::get().recordAction(std::move(action));
                }
                ImGui::EndMenu();
            }

            // --- Camera item ---
            if (ImGui::MenuItem("Camera"))
            {
                m_popupManager.openPopupWithCallback("Popup camera inspector", [this, sceneId]()
                {
                    const auto& editorScenes = m_windowRegistry.getWindows<EditorScene>();
                    for (const auto& scene : editorScenes)
                    {
                        if (scene->getSceneId() == sceneId)
                        {
                            ImNexo::CameraInspector(sceneId);
                            break;
                        }
                    }
                }, ImVec2(1440, 900));
            }

            ImGui::EndMenu();
        }
    }

    void SceneTreeWindow::sceneContextMenu()
    {
        if (m_popupManager.showPopup("Scene Tree Context Menu"))
        {
            if (ImGui::MenuItem("Create Scene"))
                m_popupManager.openPopup("Create New Scene");
            PopupManager::closePopup();
        }

        if (m_popupManager.showPopup("Scene selection context menu"))
        {
            m_popupManager.runPopupCallback("Scene selection context menu");
            PopupManager::closePopup();
        }

        if (m_popupManager.showPopupModal("Popup camera inspector"))
        {
            m_popupManager.runPopupCallback("Popup camera inspector");
            PopupManager::closePopup();
        }
    }

    void SceneTreeWindow::sceneCreationMenu()
    {
        if (!m_popupManager.showPopupModal("Create New Scene"))
            return;

        static char sceneNameBuffer[256] = "";

        ImGui::Text("Enter Scene Name:");
        ImGui::InputText("##SceneName", sceneNameBuffer, sizeof(sceneNameBuffer));

        if (ImNexo::Button("Create") && handleSceneCreation(sceneNameBuffer))
        {
            memset(sceneNameBuffer, 0, sizeof(sceneNameBuffer));
            PopupManager::closePopupInContext();
        }

        ImGui::SameLine();
        if (ImNexo::Button("Cancel"))
            PopupManager::closePopupInContext();

        PopupManager::closePopup();
    }

    void SceneTreeWindow::showNode(SceneObject& object)
    {
        ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
            ImGuiTreeNodeFlags_SpanAvailWidth;
        // Checks if the object is at the end of a tree
        const bool leaf = object.children.empty();
        if (leaf)
            baseFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        // Check if this object is selected
        const auto& selector = Selector::get();
        const bool isSelected = selector.isEntitySelected(static_cast<int>(object.data.entity));

        if (isSelected)
            baseFlags |= ImGuiTreeNodeFlags_Selected;

        bool nodeOpen = false;
        const std::string uniqueLabel = object.uiName;

        if (m_forceExpandAll && !leaf)
        {
            ImGui::SetNextItemOpen(true);
            m_resetExpandState = true;
        }
        else if (m_forceCollapseAll)
        {
            ImGui::SetNextItemOpen(false);
            m_resetExpandState = true;
        }

        // If the user wishes to rename handle the rename, else handle the selection
        if (m_renameTarget && m_renameTarget->first == object.type && m_renameTarget->second == object.uuid)
            handleRename(object);
        else
            nodeOpen = handleSelection(object, uniqueLabel, baseFlags);

        handleHovering(object);

        handleDragSource(object);
        handleDropTarget(object);

        // Handles the right click on each different type of object
        if (object.type != SelectionType::NONE && ImGui::BeginPopupContextItem(uniqueLabel.c_str()))
        {
            // Only show rename option for the primary selected entity or for non-selected entities
            if ((!isSelected || selector.getPrimaryEntity() == static_cast<int>(object.data.entity)) &&
                ImGui::MenuItem("Rename"))
            {
                m_renameTarget = {object.type, object.uuid};
                m_renameBuffer = object.uiName;
            }

            if (object.type == SelectionType::SCENE)
                sceneSelected(object);
            else if (object.type == SelectionType::DIR_LIGHT || object.type == SelectionType::POINT_LIGHT || object.type
                == SelectionType::SPOT_LIGHT)
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
            for (auto& child : object.children)
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

        if (ImGui::Begin(ICON_FA_SITEMAP " Scene Tree" NEXO_WND_USTRID_SCENE_TREE, &m_opened,
                         ImGuiWindowFlags_NoCollapse))
        {
            beginRender(NEXO_WND_USTRID_SCENE_TREE);
            m_focused = ImGui::IsWindowFocused();
            m_hovered = ImGui::IsWindowHovered();

            const auto& selector = Selector::get();

            // Opens the right click popup when no items are hovered
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(
                ImGuiHoveredFlags_AllowWhenBlockedByPopup) && !ImGui::IsAnyItemHovered())
            {
                m_popupManager.openPopup("Scene Tree Context Menu");
            }

            // Display multi-selection count at top of window if applicable
            if (const auto& selectedEntities = selector.getSelectedEntities(); selectedEntities.size() > 1)
            {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
                ImGui::Text("%zu entities selected", selectedEntities.size());
                ImGui::PopStyleColor();
                ImGui::Separator();
            }

            if (!root_.children.empty())
            {
                for (auto& node : root_.children)
                    showNode(node);
            }
            sceneContextMenu();
            sceneCreationMenu();
            if (m_popupManager.showPopup("Sphere creation popup"))
            {
                const int sceneId = selector.getSelectedScene();
                ImNexo::PrimitiveCustomizationMenu(sceneId, SPHERE);
            }
            if (m_popupManager.showPopup("Cylinder creation popup"))
            {
                const int sceneId = selector.getSelectedScene();
                ImNexo::PrimitiveCustomizationMenu(sceneId, CYLINDER);
            }
        }
        ImGui::End();
    }
}
