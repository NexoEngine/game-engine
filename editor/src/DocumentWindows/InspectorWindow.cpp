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

    void InspectorWindow::update()
    {
    }
}
