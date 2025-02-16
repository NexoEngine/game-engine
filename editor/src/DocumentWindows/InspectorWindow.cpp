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

#include "EntityProperties/RenderProperty.hpp"
#include "EntityProperties/TransformProperty.hpp"

namespace nexo::editor
{
    InspectorWindow::InspectorWindow()
    {
        m_componentShowFunctions[typeid(components::TransformComponent)] = &TransformProperty::show;
        m_componentShowFunctions[typeid(components::RenderComponent)] = &RenderProperty::show;
    }

    InspectorWindow::~InspectorWindow() = default;

    void InspectorWindow::setup()
    {
    }

    void InspectorWindow::shutdown()
    {
    }

    void InspectorWindow::show()
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 400, ImGui::GetIO().DisplaySize.y - 500),
                                ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);

        ImGui::Begin("Inspector", &m_opened, ImGuiWindowFlags_NoCollapse);

        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            LOG(NEXO_INFO, "right click on inspector\n");
        }

        showEntityProperties();
        ImGui::End();
    }

    void InspectorWindow::showEntityProperties()
    {
        const int selectedEntity = m_sceneManagerBridge->getSelectedEntity();
        auto const& App = getApp();
        if (selectedEntity == -1) return;
        if (!std::holds_alternative<EntityProperties>(m_sceneManagerBridge->getData())) return;
        const auto& [layerProps, entity] = std::get<EntityProperties>(m_sceneManagerBridge->getData());

        const std::vector<std::type_index> componentsType = App.getAllEntityComponentTypes(entity);
        for (auto& type : componentsType)
        {
            if (m_componentShowFunctions.contains(type))
            {
                m_componentShowFunctions[type](entity);
            }
        }
    }

    // void InspectorWindow::showComponent(const std::type_index& type)
    // {
    //     if (m_componentShowFunctions.contains(type))
    //     {
    //         m_componentShowFunctions[type](m_sceneManagerBridge->getSelectedEntity());
    //     }
    // }

    // void InspectorWindow::showTransformComponent()
    // {
    //     const int selectedEntity = m_sceneManagerBridge->getSelectedEntity();
    //     auto const& App = getApp();
    //     if (selectedEntity == -1)
    //         return ;
    //
    //     if (!std::holds_alternative<EntityProperties>(m_sceneManagerBridge->getData()))
    //         return;
    //
    //     const auto& [layerProps, entity] = std::get<EntityProperties>(m_sceneManagerBridge->getData());
    //     auto& [pos, size, rotation] = App.getEntityComponent<components::TransformComponent>(entity);
    //
    //     if (ImGui::TreeNodeEx("Transform", ImGuiTreeNodeFlags_DefaultOpen))
    //     {
    //         if (ImGui::BeginTable("InspectorTransformTable", 4, ImGuiTableFlags_SizingStretchProp))
    //         {
    //             auto showRow = [](const char* label, float* values)
    //             {
    //                 ImGui::PushID("%s", label);
    //                 ImGui::TableNextRow(ImGuiTableRowFlags_None);
    //                 ImGui::TableNextColumn();
    //                 ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 11.0f);
    //                 ImGui::Text("%s", label);
    //                 ImGui::SameLine();
    //
    //                 ImGui::TableNextColumn();
    //                 ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 11.0f);
    //                 ImGui::Text("X");
    //                 ImGui::SameLine();
    //                 ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 11.0f);
    //                 ImGui::DragFloat("##X", &values[0], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");
    //
    //                 ImGui::TableNextColumn();
    //                 ImGui::Text("Y");
    //                 ImGui::SameLine();
    //                 ImGui::DragFloat("##Y", &values[1], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");
    //
    //                 ImGui::TableNextColumn();
    //                 ImGui::Text("Z");
    //                 ImGui::SameLine();
    //                 ImGui::DragFloat("##Z", &values[2], 0.3f, -FLT_MAX, FLT_MAX, "%.2f");
    //                 ImGui::PopID();
    //             };
    //
    //             showRow("Position", &pos.x);
    //             showRow("Rotation", &rotation.x);
    //             showRow("Scale", &size.x);
    //             ImGui::EndTable();
    //         }
    //         ImGui::TreePop();
    //     }
    // }
    //
    // void InspectorWindow::showRendererComponent()
    // {
    //     static bool active = true;
    //     const int selectedEntity = m_sceneManagerBridge->getSelectedEntity();
    //     auto const& App = getApp();
    //     if (selectedEntity == -1)
    //         return;
    //
    //     if (!std::holds_alternative<EntityProperties>(m_sceneManagerBridge->getData()))
    //         return;
    //
    //     const auto& [layerProps, entity] = std::get<EntityProperties>(m_sceneManagerBridge->getData());
    //     const auto& renderComponent = App.getEntityComponent<components::RenderComponent>(entity);
    //
    //     ImVec4* selectedEntityColor = nullptr;
    //     if (renderComponent.type == components::RenderType::RENDER_3D)
    //     {
    //         auto renderable3D = std::dynamic_pointer_cast<components::Renderable3D>(renderComponent.renderable);
    //         if (renderable3D)
    //         {
    //             auto& [color] = renderable3D->material;
    //             selectedEntityColor = reinterpret_cast<ImVec4*>(&color);
    //         }
    //     }
    //     if (renderComponent.type == components::RenderType::RENDER_2D)
    //     {
    //         auto renderable2D = std::dynamic_pointer_cast<components::Renderable2D>(renderComponent.renderable);
    //         if (renderable2D)
    //         {
    //             auto& [color, texture, sprite] = renderable2D->sprite;
    //             selectedEntityColor = reinterpret_cast<ImVec4*>(&color);
    //         }
    //     }
    //     if (!selectedEntityColor)
    //         return;
    //
    //     ImGui::Checkbox("##RenderComponentActive", &active);
    //     ImGui::SameLine(ImGui::GetCursorPosX());
    //     ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(30.0f, 11.0f));
    //     if (ImGui::TreeNodeEx("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
    //     {
    //         ImGui::ColorEdit4("##InspectorRenderColorPicker", reinterpret_cast<float*>(selectedEntityColor),
    //                           ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB |
    //                           ImGuiColorEditFlags_NoSidePreview |
    //                           ImGuiColorEditFlags_NoTooltip);
    //         ImGui::TreePop();
    //     }
    //     ImGui::PopStyleVar();
    // }

    void InspectorWindow::update()
    {
    }
}
