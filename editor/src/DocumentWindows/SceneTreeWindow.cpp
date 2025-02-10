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
#include <random>

#include "SceneViewManager.hpp"

namespace nexo::editor {
    SceneTreeWindow::SceneTreeWindow()
    = default;

    SceneTreeWindow::~SceneTreeWindow()
    = default;

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
        const std::string editableName = obj.uiName.substr(ObjectTypeToIcon.at(obj.type).size());
        buffer[sizeof(buffer) - 1] = '\0';
        strncpy(buffer, editableName.c_str(), sizeof(buffer));

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f); // Remove border
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f); // No rounding
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

        if (ImGui::InputText("##Rename", buffer, sizeof(buffer),
                             ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            obj.uiName = ObjectTypeToIcon.at(obj.type) + std::string(buffer);
            m_sceneManagerBridge->renameObject(obj.type, obj.data, std::string(buffer));
            m_renameTarget.reset();
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            m_renameTarget.reset();
        ImGui::PopStyleVar(3);
        ImGui::EndGroup();
    }

    bool SceneTreeWindow::handleSelection(const SceneObject &obj, const std::string &uniqueLabel,
                                          const ImGuiTreeNodeFlags baseFlags) const
    {
        const bool nodeOpen = ImGui::TreeNodeEx(uniqueLabel.c_str(), baseFlags);
        if (!nodeOpen)
            return nodeOpen;
        const auto &viewManager = SceneViewManager::getInstance();
        if (ImGui::IsItemClicked())
        {
            if (obj.type == SelectionType::ENTITY)
            {
                if (!std::holds_alternative<EntityProperties>(obj.data))
                    return nodeOpen;
                auto [layerProps, entity] = std::get<EntityProperties>(obj.data);
                m_sceneManagerBridge->setSelectedEntity(obj.nodeId);
                viewManager->setSelectedScene(layerProps.sceneProps.sceneId);
                m_sceneManagerBridge->setData(obj.data);
                m_sceneManagerBridge->setSelectionType(SelectionType::ENTITY);
            } else if (obj.type == SelectionType::CAMERA)
            {
                if (!std::holds_alternative<CameraProperties>(obj.data))
                    return nodeOpen;
                const auto &[layerProps, camera] = std::get<CameraProperties>(obj.data);
                viewManager->setSelectedScene(layerProps.sceneProps.sceneId);

                m_sceneManagerBridge->setSelectedEntity(obj.nodeId);
                m_sceneManagerBridge->setData(obj.data);
                m_sceneManagerBridge->setSelectionType(SelectionType::CAMERA);
            } else if (obj.type == SelectionType::LAYER)
            {
                if (!std::holds_alternative<LayerProperties>(obj.data))
                    return nodeOpen;
                auto [sceneProps, layerId] = std::get<LayerProperties>(obj.data);
                viewManager->setSelectedScene(sceneProps.sceneId);

                m_sceneManagerBridge->setSelectedEntity(obj.nodeId);
                m_sceneManagerBridge->setData(obj.data);
                m_sceneManagerBridge->setSelectionType(SelectionType::LAYER);
            } else if (obj.type == SelectionType::SCENE)
            {
                if (!std::holds_alternative<SceneProperties>(obj.data))
                    return nodeOpen;
                auto [sceneId, windowId] = std::get<SceneProperties>(obj.data);
                viewManager->setSelectedScene(sceneId);
                m_sceneManagerBridge->setSelectedEntity(obj.nodeId);
                m_sceneManagerBridge->setSelectionType(SelectionType::SCENE);
            }
        }
        return nodeOpen;
    }

    void SceneTreeWindow::sceneSelected(const SceneObject &obj)
    {
        if (!std::holds_alternative<SceneProperties>(obj.data))
            return;
        auto props = std::get<SceneProperties>(obj.data);

        if (ImGui::MenuItem("Delete Scene"))
            SceneViewManager::getInstance()->removeScene(props.windowId);
        if (ImGui::MenuItem("Duplicate Scene view"))
            SceneViewManager::getInstance()->duplicateSceneView(props.windowId);
        if (ImGui::MenuItem("Add layer"))
        {
            m_popupManager.openPopup("Add New Layer");
            m_popupManager.setUserData("Add New Layer", props);
        }
    }

    float randomFloat()
    {
        static std::mt19937 rng(std::random_device{}());
        static std::uniform_real_distribution<float> dist(0.0f, 1.0f);
        return dist(rng);
    }

    void SceneTreeWindow::layerSelected(const SceneObject &obj) const
    {
        if (!std::holds_alternative<LayerProperties>(obj.data))
            return;
        auto [sceneProps, layerId] = std::get<LayerProperties>(obj.data);

        if (ImGui::MenuItem("Delete Layer"))
        {
            auto &app = nexo::getApp();
            app.removeLayer(sceneProps.sceneId, layerId);
            m_sceneManagerBridge->unselectEntity();
        }
        if (ImGui::MenuItem("Add entity"))
        {
            const ecs::Entity basicQuad = EntityFactory2D::createQuad({0.0f, 0.0f, 0.0f}, {1.0f, 1.0f}, 0.0f,
                                                                      {
                                                                          randomFloat(), randomFloat(), randomFloat(),
                                                                          1.0f
                                                                      });
            auto &app = getApp();
            app.addEntityToScene(basicQuad, sceneProps.sceneId, layerId);
        }

        const auto &viewManager = SceneViewManager::getInstance();

        if (ImGui::MenuItem("Add camera"))
            viewManager->addDefaultCameraToLayer(sceneProps.windowId, layerId);

        if (const bool isLayerHidden = viewManager->isLayerHidden(sceneProps.windowId, layerId); ImGui::MenuItem(
            "Hide layer", nullptr, isLayerHidden))
        {
            if (isLayerHidden)
            {
                viewManager->showLayer(sceneProps.windowId, layerId);
                LOG(NEXO_DEBUG, "Layer {} in view {} now rendered", layerId, sceneProps.windowId);
            } else
            {
                viewManager->hideLayer(sceneProps.windowId, layerId);
                LOG(NEXO_INFO, "Layer {} in view {} now hidden", layerId, sceneProps.windowId);
            }
        }
    }

    void SceneTreeWindow::cameraSelected(const SceneObject &obj) const
    {
        if (!std::holds_alternative<CameraProperties>(obj.data))
            return;
        const auto &[layerProps, camera] = std::get<CameraProperties>(obj.data);

        if (ImGui::MenuItem("Delete Camera"))
        {
            m_sceneManagerBridge->unselectEntity();
            auto &app = nexo::getApp();
            app.detachCamera(layerProps.sceneProps.sceneId, layerProps.layerId);
        }
    }

    void SceneTreeWindow::entitySelected(const SceneObject &obj) const
    {
        if (ImGui::MenuItem("Delete Entity") && std::holds_alternative<EntityProperties>(obj.data))
        {
            auto [layerProps, entity] = std::get<EntityProperties>(obj.data);
            m_sceneManagerBridge->unselectEntity();
            auto &app = nexo::getApp();
            app.removeEntityFromScene(entity, layerProps.sceneProps.sceneId,
                                      layerProps.layerId);
        }
    }

    void SceneTreeWindow::showNode(SceneObject &object)
    {
        ImGuiTreeNodeFlags baseFlags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                       ImGuiTreeNodeFlags_SpanAvailWidth;
        // Checks if the object is at the end of a tree
        const bool leaf = object.children.empty();
        if (leaf)
            baseFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;

        // Checks if the object is selected
        if (m_sceneManagerBridge->isEntitySelected() && static_cast<int>(object.nodeId) ==
            m_sceneManagerBridge->getSelectedEntity())
            baseFlags |= ImGuiTreeNodeFlags_Selected;

        bool nodeOpen = false;
        const std::string uniqueLabel = object.uiName;

        // If the user wishes to rename handle the rename, else handle the selection
        if (m_renameTarget && m_renameTarget->first == object.type && m_renameTarget->second == static_cast<int>(object.
                nodeId))
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
                m_renameTarget = {object.type, object.nodeId};
                m_renameBuffer = object.uiName;
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
                m_popupManager.openPopup("Create New Scene");
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
                if (const std::string newSceneName(sceneNameBuffer); !newSceneName.empty())
                {
                    const auto &viewManager = SceneViewManager::getInstance();
                    viewManager->addNewScene(sceneNameBuffer);
                    memset(sceneNameBuffer, 0, sizeof(sceneNameBuffer));

                    m_popupManager.closePopupInContext();
                } else
                    LOG(NEXO_WARN, "Scene name is empty !");
            }

            ImGui::SameLine();
            if (ImGui::Button("Cancel"))
                m_popupManager.closePopupInContext();

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
                if (const std::string newLayerName(layerNameBuffer); !newLayerName.empty())
                {
                    if (std::holds_alternative<SceneProperties>(m_popupManager.getUserData("Add New Layer")))
                    {
                        auto [sceneId, windowId] = std::get<SceneProperties>(
                            m_popupManager.getUserData("Add New Layer"));
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

        if (ImGui::Begin("Scene Tree", &m_opened, ImGuiWindowFlags_NoCollapse))
        {
            // Opens the right click popup when no items are hovered
            if (ImGui::IsMouseClicked(ImGuiMouseButton_Right) && ImGui::IsWindowHovered(
                    ImGuiHoveredFlags_AllowWhenBlockedByPopup) && !ImGui::IsAnyItemHovered())
                m_popupManager.openPopup("Scene Tree Context Menu");
            if (!root_.children.empty())
                showNode(root_);
            sceneContextMenu();
            sceneCreationMenu();
            layerCreationMenu();

            ImGui::End();
        }
    }

    SceneObject SceneTreeWindow::newSceneNode(const scene::SceneId sceneId, const WindowId uiId)
    {
        SceneObject sceneNode;
        sceneNode.nodeId = nextNodeId++;
        sceneNode.uiName = ObjectTypeToIcon.at(SelectionType::SCENE) + SceneViewManager::getInstance()->
                           getSceneName(uiId);
        sceneNode.data = SceneProperties(sceneId, uiId);
        sceneNode.type = SelectionType::SCENE;
        return sceneNode;
    }

    SceneObject SceneTreeWindow::newLayerNode(const scene::SceneId sceneId, const WindowId uiId,
                                              const layer::LayerId layerId,
                                              const std::string &layerName)
    {
        SceneObject layerNode;
        layerNode.nodeId = nextNodeId++;
        layerNode.uiName = ObjectTypeToIcon.at(SelectionType::LAYER) + layerName;
        layerNode.type = SelectionType::LAYER;
        const SceneProperties sceneProperties(sceneId, uiId);
        layerNode.data = LayerProperties(sceneProperties, static_cast<int>(layerId));
        return layerNode;
    }

    SceneObject SceneTreeWindow::newCameraNode(const scene::SceneId sceneId, const WindowId uiId,
                                               const layer::LayerId layerId)
    {
        SceneObject cameraNode;
        cameraNode.uiName = ObjectTypeToIcon.at(SelectionType::CAMERA) + std::string("Camera");
        if (const auto camera = m_sceneManagerBridge->getCameraLayer(sceneId, layerId))
        {
            cameraNode.nodeId = nextNodeId++;
            cameraNode.type = SelectionType::CAMERA;
            const SceneProperties sceneProperties(sceneId, uiId);
            const LayerProperties layerProperties(sceneProperties, static_cast<int>(layerId));
            cameraNode.data = CameraProperties(layerProperties, camera);
            return cameraNode;
        }
        return SceneObject{};
    }

    SceneObject SceneTreeWindow::newEntityNode(const scene::SceneId sceneId, const WindowId uiId,
                                               const unsigned int layerId,
                                               const ecs::Entity entity)
    {
        SceneObject entityNode;
        entityNode.nodeId = nextNodeId++;
        entityNode.uiName = std::format("{}{}", ObjectTypeToIcon.at(SelectionType::ENTITY), entity);
        entityNode.type = SelectionType::ENTITY;
        const SceneProperties sceneProperties(sceneId, uiId);
        const LayerProperties layerProperties(sceneProperties, static_cast<int>(layerId));
        entityNode.data = EntityProperties(layerProperties, entity);
        if (m_sceneManagerBridge->isEntitySelected() && m_sceneManagerBridge->getSelectionType() == SelectionType::ENTITY && m_sceneManagerBridge->getSelectedEntity() == entity)
        {
            const auto &viewManager = SceneViewManager::getInstance();
            m_sceneManagerBridge->setSelectedEntity(entityNode.nodeId);
            viewManager->setSelectedScene(layerProperties.sceneProps.sceneId);
            m_sceneManagerBridge->setData(entityNode.data);
            m_sceneManagerBridge->setSelectionType(SelectionType::ENTITY);
        }
        return entityNode;
    }

    void SceneTreeWindow::update()
    {
        nextNodeId = 0;
        root_.uiName = "Scenes";
        root_.nodeId = nextNodeId++;
        root_.children.clear();

        // Retrieves the scenes that are displayed on the GUI
        auto scenes = SceneViewManager::getInstance()->getOpenScenes();

        for (const auto &[sceneId, windowId]: scenes)
        {
            SceneObject sceneNode = newSceneNode(sceneId, windowId);

            // Retrieve the global entities of the scene (i.e. entities that are not part of a layer)
            auto sceneEntities = m_sceneManagerBridge->getSceneGlobalEntities(sceneId);
            for (auto entity: sceneEntities)
                sceneNode.children.push_back(newEntityNode(sceneId, windowId, -1, entity));

            auto layers = m_sceneManagerBridge->getSceneLayers(sceneId);
            for (const auto &layer: layers)
            {
                SceneObject layerNode = newLayerNode(sceneId, windowId, layer->id, layer->name);

                // Checks if a camera is attached to the layer
                if (SceneObject cameraNode = newCameraNode(sceneId, windowId, layer->id); std::holds_alternative<
                    CameraProperties>(cameraNode.data))
                    layerNode.children.push_back(cameraNode);

                auto entities = m_sceneManagerBridge->getLayerEntities(sceneId, layer->id);
                for (const auto &entity: entities)
                {
                    SceneObject entityNode = newEntityNode(sceneId, windowId, layer->id, entity);

                    layerNode.children.push_back(entityNode);
                }
                sceneNode.children.push_back(layerNode);
            }
            root_.children.push_back(sceneNode);
        }
    }

}
