//// SceneTreeWindow.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/11/2024
//  Description: Source for the scene tree document window
//
///////////////////////////////////////////////////////////////////////////////

#include "SceneTreeWindow.hpp"
#include "EntityFactory2D.hpp"
#include "IconsFontAwesome.h"

#include <imgui_internal.h>

#include "SceneViewManager.hpp"

namespace nexo::editor {
    SceneTreeWindow::SceneTreeWindow()
    {}

    SceneTreeWindow::~SceneTreeWindow()
    {}

    void SceneTreeWindow::setup()
    {}

    void SceneTreeWindow::shutdown()
    {}

    void SceneTreeWindow::handleRename(SceneObject &obj)
    {
        ImGui::BeginGroup();
        ImGui::TextUnformatted(ObjectTypeToIcon.at(obj.type).c_str());
        ImGui::SameLine();

        char buffer[256];
        std::string editableName = obj.name.substr(strlen(ObjectTypeToIcon.at(obj.type).c_str()));
        strncpy(buffer, editableName.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0';

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f); // Remove border
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f); // No rounding
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

        if (ImGui::InputText("##Rename", buffer, sizeof(buffer),
                             ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            obj.name = ObjectTypeToIcon.at(obj.type) + std::string(buffer);
            m_sceneManagerBridge->renameObject(obj.id, obj.type, obj.data, std::string(buffer));
            m_renameTarget.reset();
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            m_renameTarget.reset();
        ImGui::PopStyleVar(3);
        ImGui::EndGroup();
    }

    bool SceneTreeWindow::handleSelection(SceneObject &obj, std::string &uniqueLabel, ImGuiTreeNodeFlags baseFlags)
    {
        bool nodeOpen = ImGui::TreeNodeEx(uniqueLabel.c_str(), baseFlags);
        auto &viewManager = SceneViewManager::getInstance();
        if (ImGui::IsItemClicked())
        {
            if (obj.type == SelectionType::ENTITY)
            {
                m_sceneManagerBridge->setSelectedEntity(obj.id);
                m_sceneManagerBridge->setSelectionType(SelectionType::ENTITY);
            } else if (obj.type == SelectionType::CAMERA)
            {
                if (!std::holds_alternative<CameraProperties>(obj.data))
                    return nodeOpen;
                CameraProperties props = std::get<CameraProperties>(obj.data);
                viewManager->setSelectedScene(props.sceneId);

                m_sceneManagerBridge->setSelectedEntity(obj.id);
                m_sceneManagerBridge->setData(obj.data);
                m_sceneManagerBridge->setSelectionType(SelectionType::CAMERA);
            } else if (obj.type == SelectionType::LAYER)
            {
                if (!std::holds_alternative<LayerProperties>(obj.data))
                    return nodeOpen;
                LayerProperties props = std::get<LayerProperties>(obj.data);
                viewManager->setSelectedScene(props.sceneId);

                m_sceneManagerBridge->setSelectedEntity(obj.id);
                m_sceneManagerBridge->setData(obj.data);
                m_sceneManagerBridge->setSelectionType(SelectionType::LAYER);
            } else if (obj.type == SelectionType::SCENE)
            {
                viewManager->setSelectedScene(obj.id);
                m_sceneManagerBridge->setSelectedEntity(obj.id);
                m_sceneManagerBridge->setSelectionType(SelectionType::SCENE);
            }
        }
        return nodeOpen;
    }

    void SceneTreeWindow::sceneSelected(SceneObject &obj)
    {


        if (ImGui::MenuItem("Delete Scene"))
        {
            char buffer[256];
            std::string realName = obj.name.substr(strlen(ObjectTypeToIcon.at(obj.type).c_str()));
            strncpy(buffer, realName.c_str(), sizeof(buffer));
            buffer[sizeof(buffer) - 1] = '\0';
            SceneViewManager::getInstance()->removeScene(realName);
        }
        if (ImGui::MenuItem("Add layer"))
        {
            m_popupManager.openPopup("Add New Layer");
            m_popupManager.setUserData("Add New Layer", obj.id);
        }
    }

    void SceneTreeWindow::layerSelected(SceneObject &obj)
    {
        if (!std::holds_alternative<LayerProperties>(obj.data))
            return;
        LayerProperties props = std::get<LayerProperties>(obj.data);

        if (ImGui::MenuItem("Delete Layer"))
        {
            auto &app = nexo::getApp();
            app.removeLayer(props.sceneId, props.layerName);
            m_sceneManagerBridge->unselectEntity();
        }
        if (ImGui::MenuItem("Add entity"))
        {
            ecs::Entity basicQuad = EntityFactory2D::createQuad({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f, {0.0f, 1.0f, 0.0f, 1.0f});
            auto &app = getApp();
            app.addEntityToScene(basicQuad, props.sceneId, props.layerName);
            m_sceneManagerBridge->setSelectedEntity(basicQuad);
            m_sceneManagerBridge->setSelectionType(SelectionType::ENTITY);
        }
    }

    void SceneTreeWindow::cameraSelected(SceneObject &obj)
    {
        if (!std::holds_alternative<CameraProperties>(obj.data))
            return;
        CameraProperties props = std::get<CameraProperties>(obj.data);

        if (ImGui::MenuItem("Delete Camera"))
        {
            m_sceneManagerBridge->unselectEntity();
            auto &app = nexo::getApp();
            app.detachCamera(props.sceneId, props.layerName);
        }
    }

    void SceneTreeWindow::entitySelected(SceneObject &obj)
    {
        if (ImGui::MenuItem("Delete Entity"))
        {
            m_sceneManagerBridge->unselectEntity();
            auto &app = nexo::getApp();
            app.destroyEntity(obj.id);
        }
    }

    void SceneTreeWindow::showNode(SceneObject &object)
    {
        ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                       ImGuiTreeNodeFlags_SpanAvailWidth;
        // Checks if the object is at the end of a tree
        bool leaf = object.children.empty();
        if (leaf)
            baseFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        // Checks if the object is selected
        if (m_sceneManagerBridge->isEntitySelected() && object.id ==
                m_sceneManagerBridge->getSelectedEntity() && object.type == m_sceneManagerBridge->getSelectionType())
            baseFlags |= ImGuiTreeNodeFlags_Selected;

        bool nodeOpen = false;
        std::string uniqueLabel = object.name + "##" + std::to_string(object.id);

        // If the user wishes to rename handle the rename, else handle the selection
        if (m_renameTarget && m_renameTarget->first == object.type && m_renameTarget->second == object.id)
            handleRename(object);
        else
            nodeOpen = handleSelection(object, uniqueLabel, baseFlags);

        // Handles the right click on each different type of object
        if (object.type != SelectionType::NONE && ImGui::BeginPopupContextItem(uniqueLabel.c_str()))
        {
            // Renaming works on every object excepts entities and cameras
            if (object.type != SelectionType::ENTITY && object.type != SelectionType::CAMERA &&
                ImGui::MenuItem("Rename"))
            {
                m_renameTarget = {object.type, object.id};
                m_renameBuffer = object.name;
            }
            if (object.type == SelectionType::SCENE)
                sceneSelected(object);
            else if (object.type == SelectionType::LAYER)
                layerSelected(object);
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

    void SceneTreeWindow::sceneContextMenu()
    {
        if (m_popupManager.showPopup("Scene Tree Context Menu"))
        {
            if (ImGui::MenuItem("Create Scene"))
            {
                m_popupManager.openPopup("Create New Scene");
            }
            m_popupManager.closePopup();
        }
    }

    void SceneTreeWindow::sceneCreationMenu()
    {
        if (m_popupManager.showPopupModal("Create New Scene"))
        {
            static char sceneNameBuffer[256] = "";

            ImGui::Text("Enter Scene Name:");
            ImGui::InputText("##SceneName", sceneNameBuffer, sizeof(sceneNameBuffer));

            if (ImGui::Button("Create"))
            {
                std::string newSceneName(sceneNameBuffer);

                if (!newSceneName.empty())
                {
                    auto &viewManager = SceneViewManager::getInstance();
                    viewManager->addNewScene(sceneNameBuffer);
                    memset(sceneNameBuffer, 0, sizeof(sceneNameBuffer));

                    m_popupManager.closePopupInContext();
                } else
                {
                    LOG(NEXO_WARN, "Scene name is empty !");
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                m_popupManager.closePopupInContext();
            }

            m_popupManager.closePopup();
        }
    }

    void SceneTreeWindow::layerCreationMenu()
    {
        if (m_popupManager.showPopupModal("Add New Layer"))
        {
            static char layerNameBuffer[256] = "";

            ImGui::Text("Enter layer Name:");
            ImGui::InputText("##LayerName", layerNameBuffer, sizeof(layerNameBuffer));

            if (ImGui::Button("Create"))
            {
                std::string newLayerName(layerNameBuffer);

                if (!newLayerName.empty())
                {
                    if (std::holds_alternative<int>(m_popupManager.getUserData("Add New Layer")))
                    {
                        int sceneId = std::get<int>(m_popupManager.getUserData("Add New Layer"));
                        auto &app = getApp();
                        app.addNewLayer(sceneId, layerNameBuffer);
                        memset(layerNameBuffer, 0, sizeof(layerNameBuffer));
                    }

                    m_popupManager.closePopupInContext();
                } else
                {
                    LOG(NEXO_WARN, "Layer name is empty !");
                }
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
            {
                m_popupManager.closePopupInContext();
            }

            m_popupManager.closePopup();
        }
    }

    void SceneTreeWindow::show()
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 40), ImGuiCond_FirstUseEver);

        ImGui::Begin("Scene Tree", &m_opened, ImGuiWindowFlags_NoCollapse);

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            // Opens the right click popup when no items is hovered
            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && !ImGui::IsAnyItemHovered())
                m_popupManager.openPopup("Scene Tree Context Menu");
        }

        showNode(root_);
        sceneContextMenu();
        sceneCreationMenu();
        layerCreationMenu();

        ImGui::End();
    }

    SceneObject SceneTreeWindow::newSceneNode(scene::SceneId id, const std::string &uiName)
    {
        SceneObject sceneNode;
        sceneNode.name = ObjectTypeToIcon.at(SelectionType::SCENE) + uiName;
        sceneNode.id = id;
        sceneNode.type = SelectionType::SCENE;
        return sceneNode;
    }

    SceneObject SceneTreeWindow::newLayerNode(scene::SceneId id, std::shared_ptr<layer::Layer> layer)
    {
        SceneObject layerNode;
        layerNode.name = ObjectTypeToIcon.at(SelectionType::LAYER) + layer->name;
        layerNode.id = layer->id;
        layerNode.type = SelectionType::LAYER;
        layerNode.data = LayerProperties(id, layer->name);
        return layerNode;
    }

    SceneObject SceneTreeWindow::newCameraNode(scene::SceneId id, std::shared_ptr<layer::Layer> layer)
    {
        SceneObject cameraNode;
        cameraNode.name = ObjectTypeToIcon.at(SelectionType::CAMERA) + std::string("Camera");
        auto camera = m_sceneManagerBridge->getCameraLayer(id, layer->name);
        if (camera)
        {
            cameraNode.id = camera->getCameraID();
            cameraNode.type = SelectionType::CAMERA;
            cameraNode.data = CameraProperties(id, layer->name, camera);
            return cameraNode;
        }
        return SceneObject();
    }

    SceneObject SceneTreeWindow::newEntityNode(ecs::Entity entity)
    {
        //TODO: Should store the scene id also
        SceneObject entityNode;
        entityNode.name = ObjectTypeToIcon.at(SelectionType::ENTITY) + std::to_string(entity);
        entityNode.id = entity;
        entityNode.type = SelectionType::ENTITY;
        return entityNode;
    }

    void SceneTreeWindow::update()
    {
        root_.name = "Scenes";
        root_.children.clear();

        // Retrieves the scenes that are displayed on the GUI
        auto scenes = SceneViewManager::getInstance()->getOpenScenes();

        for (const auto &scene: scenes)
        {
            SceneObject sceneNode = newSceneNode(scene.sceneId, scene.uiName);

            // Retrieve the global entities of the scene (i.e entities that are not part of a layer)
            auto sceneEntities = m_sceneManagerBridge->getSceneGlobalEntities(scene.sceneId);
            for (auto entity: sceneEntities)
                sceneNode.children.push_back(newEntityNode(entity));

            auto layers = m_sceneManagerBridge->getSceneLayers(scene.sceneId);
            for (const auto &layer: layers)
            {
                SceneObject layerNode = newLayerNode(scene.sceneId, layer);

                SceneObject cameraNode = newCameraNode(scene.sceneId, layer);
                // Checks if a camera is attached to the layer
                if (std::holds_alternative<CameraProperties>(cameraNode.data))
                    layerNode.children.push_back(cameraNode);

                auto entities = m_sceneManagerBridge->getLayerEntities(scene.sceneId, layer->name);
                for (const auto &entity: entities)
                {
                    SceneObject entityNode = newEntityNode(entity);

                    layerNode.children.push_back(entityNode);
                }
                sceneNode.children.push_back(layerNode);
            }
            root_.children.push_back(sceneNode);
        }
    }

}
