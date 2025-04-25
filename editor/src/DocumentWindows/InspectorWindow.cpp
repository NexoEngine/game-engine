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

#include <IconsFontAwesome.h>
#include <imgui.h>
#include <imgui_internal.h>
#include <variant>

#include "Application.hpp"
#include "ImNexo/Elements.hpp"
#include "EntityProperties/RenderProperty.hpp"
#include "EntityProperties/TransformProperty.hpp"
#include "EntityProperties/AmbientLightProperty.hpp"
#include "EntityProperties/DirectionalLightProperty.hpp"
#include "EntityProperties/PointLightProperty.hpp"
#include "EntityProperties/SpotLightProperty.hpp"
#include "EntityProperties/CameraProperty.hpp"
#include "EntityProperties/CameraController.hpp"
#include "components/Transform.hpp"
#include "utils/ScenePreview.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "context/Selector.hpp"
#include "core/scene/SceneManager.hpp"

extern ImGuiID g_materialInspectorDockID;

namespace nexo::editor
{

    InspectorWindow::~InspectorWindow() = default;

    void InspectorWindow::setup()
    {
        registerProperty<components::TransformComponent, TransformProperty>();
        registerProperty<components::RenderComponent, RenderProperty>();
        registerProperty<components::AmbientLightComponent, AmbientLightProperty>();
        registerProperty<components::DirectionalLightComponent, DirectionalLightProperty>();
        registerProperty<components::PointLightComponent, PointLightProperty>();
        registerProperty<components::SpotLightComponent, SpotLightProperty>();
        registerProperty<components::CameraComponent, CameraProperty>();
        registerProperty<components::PerspectiveCameraController, CameraController>();
    }

    void InspectorWindow::shutdown()
    {
        // Nothing to clear for now
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

    void InspectorWindow::update()
    {
        // Nothing to update here
    }
}
