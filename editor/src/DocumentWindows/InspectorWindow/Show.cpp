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
//  Description: Source file for the inspector window rendering
//
///////////////////////////////////////////////////////////////////////////////

#include "InspectorWindow.hpp"
#include "context/Selector.hpp"
#include "ImNexo/Components.hpp"
#include "IconsFontAwesome.h"

namespace nexo::editor {

    void InspectorWindow::showSceneProperties(const scene::SceneId sceneId) const
    {
        auto &app = getApp();
        auto &selector = Selector::get();
        scene::SceneManager &manager = app.getSceneManager();
        scene::Scene &scene = manager.getScene(sceneId);
        std::string uiHandle = selector.getUiHandle(scene.getUuid(), "");

        // Remove the icon prefix
        if (size_t spacePos = uiHandle.find(' '); spacePos != std::string::npos)
            uiHandle = uiHandle.substr(spacePos + 1);

        if (ImNexo::Header("##SceneNode", uiHandle))
        {
            ImGui::Spacing();
            ImGui::Columns(2, "sceneProps");
            ImGui::SetColumnWidth(0, 80);

            ImGui::Text("Hide");
            ImGui::NextColumn();
            bool hidden = !scene.isRendered();
            ImGui::Checkbox("##HideCheckBox", &hidden);
            scene.setRenderStatus(!hidden);
            ImGui::NextColumn();

            ImGui::Text("Pause");
            ImGui::NextColumn();
            bool paused = !scene.isActive();
            ImGui::Checkbox("##PauseCheckBox", &paused);
            scene.setActiveStatus(!paused);
            ImGui::NextColumn();

            ImGui::Columns(1);
            ImGui::TreePop();
        }
    }

    void InspectorWindow::showEntityProperties(const ecs::Entity entity)
    {
        const std::vector<std::type_index> componentsType = nexo::Application::getAllEntityComponentTypes(entity);
        for (auto& type : componentsType)
        {
            if (m_entityProperties.contains(type))
            {
                m_entityProperties[type]->show(entity);
            }
        }
    }

    void InspectorWindow::show()
    {
        ImGui::Begin(ICON_FA_SLIDERS " Inspector" NEXO_WND_USTRID_INSPECTOR, &m_opened, ImGuiWindowFlags_NoCollapse);
        firstDockSetup(NEXO_WND_USTRID_INSPECTOR);
        auto const &selector = Selector::get();

        if (selector.getPrimarySelectionType() == SelectionType::SCENE) {
            // Scene selection stays the same - only show the selected scene
            showSceneProperties(selector.getSelectedScene());
        }
        else if (selector.hasSelection()) {
            const ecs::Entity primaryEntity = selector.getPrimaryEntity();

            const auto& selectedEntities = selector.getSelectedEntities();
            if (selectedEntities.size() > 1) {
                ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f));
                ImGui::TextWrapped("%zu entities selected. Displaying properties for the primary entity.",
                                  selectedEntities.size());
                ImGui::PopStyleColor();
                ImGui::Separator();
            }

            showEntityProperties(primaryEntity);
        }

        ImGui::End();
    }

}
