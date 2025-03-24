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

#include <imgui.h>
#include <imgui_internal.h>
#include <variant>

#include "Application.hpp"
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
#include "Components/EntityPropertiesComponents.hpp"
#include "components/Camera.hpp"
#include "components/Light.hpp"
#include "context/Selector.hpp"
#include "core/scene/SceneManager.hpp"

#include "Components/Widgets.hpp"

extern ImGuiID g_materialInspectorDockID;

namespace nexo::editor
{

	InspectorWindow::~InspectorWindow() = default;

    void InspectorWindow::setup()
    {
		m_entityProperties[typeid(components::TransformComponent)] = std::make_shared<TransformProperty>(*this);
		m_entityProperties[typeid(components::RenderComponent)] = std::make_shared<RenderProperty>(*this);
		m_entityProperties[typeid(components::AmbientLightComponent)] = std::make_shared<AmbientLightProperty>(*this);
		m_entityProperties[typeid(components::DirectionalLightComponent)] = std::make_shared<DirectionalLightProperty>(*this);
		m_entityProperties[typeid(components::PointLightComponent)] = std::make_shared<PointLightProperty>(*this);
		m_entityProperties[typeid(components::SpotLightComponent)] = std::make_shared<SpotLightProperty>(*this);
		m_entityProperties[typeid(components::CameraComponent)] = std::make_shared<CameraProperty>(*this);
		m_entityProperties[typeid(components::PerspectiveCameraController)] = std::make_shared<CameraController>(*this);
    }

    void InspectorWindow::shutdown()
    {
    	// Nothing to clear for now
    }

    void InspectorWindow::show()
    {
        ImGui::Begin("Inspector", &m_opened, ImGuiWindowFlags_NoCollapse);
       	if (m_firstOpened)
        	firstDockSetup("Inspector");
        auto const &selector = Selector::get();
        const int selectedEntity = selector.getSelectedEntity();

        if (selectedEntity != -1)
        {
            if (selector.getSelectionType() == SelectionType::SCENE)
            {
                showSceneProperties(selectedEntity);
            }
            else
            {
                showEntityProperties(selectedEntity);
            }
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

		if (EntityPropertiesComponents::drawHeader("##SceneNode", uiHandle))
		{
			ImGui::Spacing();
	  		ImGui::SetWindowFontScale(1.15f);
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
