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
#include "IconsFontAwesome.h"

#include <imgui.h>
#include <raylib.h>

namespace nexo::editor {
    SceneTreeWindow::SceneTreeWindow()
    {
        // Initialize the scene with some random objects

        /*root_.children = {
            {ICON_FA_CAMERA " Camera"},
            {ICON_FA_LIGHTBULB " Lights", {
                {ICON_FA_SUN " Directional Light"},
                {ICON_FA_BOLT " Spot Light"}
            }},
            {"Static Meshes", {{"Cube"}, {"Sphere"}, {"Plane"}}},
            {"Dynamic Objects", {{"Player"}, {"Enemy #1"}, {"Enemy #2"}}},
            // ... add more as needed
        };*/
    }

    SceneTreeWindow::~SceneTreeWindow()
    {
        // Destructor
    }

    void SceneTreeWindow::setup()
    {
        // setup code
    }

    void SceneTreeWindow::shutdown()
    {
        // shutdown code
    }

    void SceneTreeWindow::handleRename(SceneObject &obj)
    {
        ImGui::BeginGroup(); // Group the icon and input text
        ImGui::TextUnformatted(ObjectTypeToIcon.at(obj.type).c_str());
        ImGui::SameLine();

        char buffer[256];
        std::string editableName = obj.name.substr(strlen(ObjectTypeToIcon.at(obj.type).c_str()));
        strncpy(buffer, editableName.c_str(), sizeof(buffer));
        buffer[sizeof(buffer) - 1] = '\0'; // Ensure null-termination

        ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 0.0f); // Remove border
        ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 0.0f);   // Optional: No rounding
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(0, 0));

        if (ImGui::InputText("##Rename", buffer, sizeof(buffer), ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_AutoSelectAll))
        {
            obj.name = ObjectTypeToIcon.at(obj.type) + std::string(buffer);
            m_sceneManagerBridge.renameObject(obj.id, obj.type, obj.data, std::string(buffer));
            m_renameTarget.reset();
        }
        if (ImGui::IsKeyPressed(ImGuiKey_Escape))
            m_renameTarget.reset();
        ImGui::PopStyleVar(3);
        ImGui::EndGroup();
    }


void SceneTreeWindow::ShowNode(SceneObject& object)
{
    ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                    ImGuiTreeNodeFlags_SpanAvailWidth;
    bool leaf = object.children.empty();
    if (leaf)
    {
        base_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    if (m_sceneManagerBridge.isEntitySelected() && object.id ==
        m_sceneManagerBridge.getSelectedEntity() && object.type == m_sceneManagerBridge.getSelectionType())
    {
        base_flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool nodeOpen = false;
    std::string uniqueLabel = object.name + "##" + std::to_string(object.id);
    if (m_renameTarget && m_renameTarget->first == object.type && m_renameTarget->second == object.id)
        handleRename(object);
    else
    {
        nodeOpen = ImGui::TreeNodeEx(uniqueLabel.c_str(), base_flags);
        if (ImGui::IsItemClicked())
        {
            if (object.type == SelectionType::ENTITY)
            {
                m_sceneManagerBridge.setSelectedEntity(object.id);
                m_sceneManagerBridge.setSelectionType(SelectionType::ENTITY);
            }
            else if (object.type == SelectionType::CAMERA)
            {
                m_sceneManagerBridge.setSelectedEntity(object.id);
                m_sceneManagerBridge.setData(object.data);
                m_sceneManagerBridge.setSelectionType(SelectionType::CAMERA);
            }
            else if (object.type == SelectionType::LAYER)
            {
                m_sceneManagerBridge.setSelectedEntity(object.id);
                m_sceneManagerBridge.setData(object.data);
                m_sceneManagerBridge.setSelectionType(SelectionType::LAYER);
            }
            else if (object.type == SelectionType::SCENE)
            {
                m_sceneManagerBridge.setSelectedEntity(object.id);
                m_sceneManagerBridge.setSelectionType(SelectionType::SCENE);
            }
        }
    }


    if (object.type != SelectionType::NONE && ImGui::BeginPopupContextItem(uniqueLabel.c_str()))
    {
        if (object.type != SelectionType::ENTITY && object.type != SelectionType::CAMERA  && ImGui::MenuItem("Rename"))
        {
            m_renameTarget = {object.type, object.id};
            m_renameBuffer = object.name;
        }
        if (object.type == SelectionType::SCENE)
        {
            if (ImGui::MenuItem("Delete Scene"))
            {
                m_sceneManagerBridge.unselectEntity();
                auto &app = nexo::getApp();
                app.deleteScene(object.id);
            }
        }
        else if (object.type == SelectionType::LAYER)
        {
            if (ImGui::MenuItem("Delete Layer"))
            {
                m_sceneManagerBridge.unselectEntity();
                auto &app = nexo::getApp();
                std::string layerName = object.name.substr(strlen(ObjectTypeToIcon.at(SelectionType::LAYER).c_str()));
                app.removeLayer(object.id, layerName);
            }
        }
        else if (object.type == SelectionType::CAMERA)
        {
            if (ImGui::MenuItem("Delete Camera"))
            {
                if (std::holds_alternative<CameraProperties>(object.data))
                {
                    std::string layerName = std::get<CameraProperties>(object.data).layerName;
                    m_sceneManagerBridge.unselectEntity();
                    auto &app = nexo::getApp();
                    app.detachCamera(object.id, layerName);
                }
            }
        }
        else if (object.type == SelectionType::ENTITY)
        {
            if (ImGui::MenuItem("Delete Entity"))
            {
                m_sceneManagerBridge.unselectEntity();
                auto &app = nexo::getApp();
                app.destroyEntity(object.id);
            }
        }
        ImGui::EndPopup();

    }

    if (nodeOpen && !leaf)
    {
        for (auto& child : object.children)
        {
            ShowNode(child);
        }
        ImGui::TreePop();
    }
}

    void SceneTreeWindow::show()
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 300, 20), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(300, ImGui::GetIO().DisplaySize.y - 40), ImGuiCond_FirstUseEver);

        ImGui::Begin("Scene Tree", &m_opened, ImGuiWindowFlags_NoCollapse);
        ImVec2 winPos = ImGui::GetWindowPos();
        ImVec2 winSize = ImGui::GetWindowSize();
        ImVec2 winMin = winPos;
        ImVec2 winMax = ImVec2(winPos.x + winSize.x, winPos.y + winSize.y);

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            if (ImGui::IsWindowHovered(ImGuiHoveredFlags_AllowWhenBlockedByPopup) && !ImGui::IsAnyItemHovered())
                m_sceneManagerBridge.unselectEntity();
            else
            {
                ImVec2 mousePos = ImGui::GetMousePos();
                bool outsideWindow = mousePos.x < winMin.x || mousePos.x > winMax.x ||
                                     mousePos.y < winMin.y || mousePos.y > winMax.y;

                if (outsideWindow)
                    m_sceneManagerBridge.unselectEntity();
            }
        }
        ShowNode(root_);
        ImGui::End();
    }

    void SceneTreeWindow::update()
    {
        root_.name = "Scenes";
        root_.children.clear();

        auto sceneIDs = m_sceneManagerBridge.getSceneIDs();

        for (const auto &sceneID: sceneIDs)
        {
            SceneObject sceneNode;
            sceneNode.name = ObjectTypeToIcon.at(SelectionType::SCENE) + m_sceneManagerBridge.getSceneName(sceneID);
            sceneNode.id = sceneID;
            sceneNode.type = SelectionType::SCENE;
            auto sceneEntities = m_sceneManagerBridge.getSceneGlobalEntities(sceneID);
            for (auto entity : sceneEntities)
            {
                SceneObject entityNode;
                entityNode.name = ObjectTypeToIcon.at(SelectionType::ENTITY) +  std::to_string(entity);
                entityNode.id = entity;
                entityNode.type = SelectionType::ENTITY;

                sceneNode.children.push_back(entityNode);
            }

            auto layers = m_sceneManagerBridge.getSceneLayers(sceneID);
            for (const auto &layer : layers)
            {
                SceneObject layerNode;
                layerNode.name = ObjectTypeToIcon.at(SelectionType::LAYER) + layer->name;
                layerNode.id = sceneID;
                layerNode.type = SelectionType::LAYER;
                layerNode.data = layer->name;

                SceneObject cameraNode;
                cameraNode.name = ObjectTypeToIcon.at(SelectionType::CAMERA) + std::string("Camera");
                auto camera = m_sceneManagerBridge.getCameraLayer(sceneID, layer->name);
                if (camera)
                {
                    cameraNode.id = camera->getCameraID();
                    cameraNode.type = SelectionType::CAMERA;
                    cameraNode.data = CameraProperties(sceneID, layer->name, camera);;

                    layerNode.children.push_back(cameraNode);
                }
                auto entities = m_sceneManagerBridge.getLayerEntities(sceneID, layer->name);
                for (const auto &entity: entities)
                {
                    SceneObject entityNode;
                    entityNode.name = ObjectTypeToIcon.at(SelectionType::ENTITY) +  std::to_string(entity);
                    entityNode.id = entity;
                    entityNode.type = SelectionType::ENTITY;

                    layerNode.children.push_back(entityNode);
                }
                sceneNode.children.push_back(layerNode);
            }
            root_.children.push_back(sceneNode);
        }
    }

}
