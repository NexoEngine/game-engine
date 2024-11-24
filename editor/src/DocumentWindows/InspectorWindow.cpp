//// InspectorWindow.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        23/11/2024
//  Description: Inspector window source file
//
///////////////////////////////////////////////////////////////////////////////

#include "InspectorWindow.hpp"

#include <imgui_internal.h>

namespace nexo::editor
{
    InspectorWindow::InspectorWindow()
    {
    }

    InspectorWindow::~InspectorWindow()
    {
    }

    void InspectorWindow::setup()
    {
    }

    void InspectorWindow::shutdown()
    {
    }

    void InspectorWindow::show()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.WindowPadding = ImVec2(15, 15); // Padding des fenêtres
        style.FramePadding = ImVec2(5, 5); // Padding interne des widgets
        style.ItemSpacing = ImVec2(10, 10); // Espacement entre les widgets
        style.ScrollbarSize = 10.0f; // Taille de la barre de défilement
        style.WindowRounding = 10.0f; // Arrondi des coins des fenêtres
        style.FrameRounding = 5.0f; // Arrondi des widgets (boutons, sliders)
        style.GrabRounding = 2.0f; // Arrondi des curseurs (drag, sliders)

        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 400, ImGui::GetIO().DisplaySize.y - 500),
                                ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);

        ImGui::Begin("Inspector", &m_opened, ImGuiWindowFlags_NoCollapse);

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            printf("right click on inspector\n");
        }

        showTransform();
        ImGui::End();
    }

    void InspectorWindow::showTransform()
    {
        static bool active = true;
        int selectedEntity = m_sceneManagerBridge->getSelectedEntity();
        auto App = getApp();
        if (selectedEntity == -1)
            return;

        auto &transform = App.getEntityComponent<components::TransformComponent>(selectedEntity);

        ImGui::Checkbox("##active", &active); ImGui::SameLine(ImGui::GetCursorPosX());
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(30.0f, 11.0f));
        if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
        {
            if (ImGui::BeginTable("Table", 4, ImGuiTableFlags_SizingStretchProp))
            {
                auto showRow = [](const char* label, float *values) {
                    ImGui::PushID(label);
                    ImGui::TableNextRow(ImGuiTableRowFlags_None);
                    ImGui::TableNextColumn();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 11.0f);
                    ImGui::Text(label);ImGui::SameLine();

                    ImGui::TableNextColumn();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 11.0f);
                    ImGui::Text("X"); ImGui::SameLine();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 11.0f);
                    ImGui::DragFloat("##X", &values[0], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");

                    ImGui::TableNextColumn();
                    ImGui::Text("Y"); ImGui::SameLine();
                    ImGui::DragFloat("##Y", &values[1], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");

                    ImGui::TableNextColumn();
                    ImGui::Text("Z"); ImGui::SameLine();
                    ImGui::DragFloat("##Z", &values[2], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");
                    ImGui::PopID();
                };

                showRow("Position", &transform.pos.x);
                showRow("Rotation", &transform.rotation.x);
                showRow("Scale", &transform.size.x);
                ImGui::EndTable();
            }
            ImGui::TreePop();
        }
        ImGui::PopStyleVar();
    }

    void InspectorWindow::update()
    {
    }
}
