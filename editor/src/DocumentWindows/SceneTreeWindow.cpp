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

void SceneTreeWindow::ShowNode(SceneObject& object)
{
    ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick |
                                    ImGuiTreeNodeFlags_SpanAvailWidth;
    bool leaf = object.children.empty();
    if (leaf)
    {
        base_flags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
    }

    if (object.type == SCENE_OBJECT_TYPE_ENTITY && m_sceneManagerBridge.isEntitySelected() && object.id ==
        m_sceneManagerBridge.getSelectedEntity())
    {
        base_flags |= ImGuiTreeNodeFlags_Selected;
    }

    bool node_open = ImGui::TreeNodeEx(object.name.c_str(), base_flags);
    if (ImGui::IsItemClicked())
    {
        if (object.type == SCENE_OBJECT_TYPE_ENTITY)
        {
            m_sceneManagerBridge.setSelectedEntity(object.id);
        }
        else if (object.type == SCENE_OBJECT_TYPE_LAYER)
        {
            // Logic to handle layer selection (optional)
        }
        else if (object.type == SCENE_OBJECT_TYPE_SCENE)
        {
            // Logic to handle scene selection (optional)
        }
    }

    if (ImGui::BeginPopupContextItem())
    {
        if (object.type == SCENE_OBJECT_TYPE_SCENE)
        {
            if (ImGui::MenuItem("Rename Scene"))
            {
                // Rename scene logic
            }
            if (ImGui::MenuItem("Delete Scene"))
            {
                // Delete scene logic
            }
        }
        else if (object.type == SCENE_OBJECT_TYPE_LAYER)
        {
            if (ImGui::MenuItem("Rename Layer"))
            {
                // Rename layer logic
            }
            if (ImGui::MenuItem("Delete Layer"))
            {
                // Delete layer logic
            }
        }
        else if (object.type == SCENE_OBJECT_TYPE_ENTITY)
        {
            if (ImGui::MenuItem("Rename Entity"))
            {
                // Rename entity logic
            }
            if (ImGui::MenuItem("Delete Entity"))
            {
                m_sceneManagerBridge.unselectEntity();
                auto app = nexo::getApp();
                app.destroyEntity(object.id);
            }
        }
        ImGui::EndPopup();
    }

    if (node_open && !leaf)
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
            sceneNode.name = ICON_FA_MAP_O " " + m_sceneManagerBridge.getSceneName(sceneID);
            sceneNode.id = sceneID;
            sceneNode.type = SCENE_OBJECT_TYPE_SCENE;

            auto layers = m_sceneManagerBridge.getSceneLayers(sceneID);
            for (const auto &layer : layers)
            {
                SceneObject layerNode;
                layerNode.name = ICON_FA_BARS " " + layer->name;
                layerNode.id = 0;
                layerNode.type = SCENE_OBJECT_TYPE_LAYER;

                auto entities = m_sceneManagerBridge.getLayerEntities(sceneID, layer->name);
                for (const auto &entity: entities)
                {
                    SceneObject entityNode;
                    entityNode.name = ICON_FA_CUBES " " +  std::to_string(entity);
                    entityNode.id = entity;
                    entityNode.type = SCENE_OBJECT_TYPE_ENTITY;

                    layerNode.children.push_back(entityNode);
                }
                sceneNode.children.push_back(layerNode);
            }
            root_.children.push_back(sceneNode);
        }
    }

}
