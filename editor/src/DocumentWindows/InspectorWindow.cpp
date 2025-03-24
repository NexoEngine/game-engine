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
    /**
 * @brief Default destructor for InspectorWindow.
 *
 * This destructor relies on compiler-generated cleanup and does not perform any custom actions.
 */
InspectorWindow::~InspectorWindow() = default;

    /**
     * @brief Initializes the mapping between component types and their respective property handlers.
     *
     * This method registers each supported component type with an instance of its corresponding property class,
     * storing them in the internal `m_entityProperties` map. Each property is constructed using the current instance
     * of InspectorWindow, ensuring the inspector can later display and manage the properties of selected entities.
     */
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

    /**
     * @brief Shuts down the InspectorWindow.
     *
     * This function finalizes the shutdown procedures, though no cleanup actions are currently implemented.
     */
    void InspectorWindow::shutdown()
    {
    }

    /**
     * @brief Renders the Inspector window and displays properties based on the current selection.
     *
     * Opens an ImGui window titled "Inspector" and, if it is opened for the first time, initializes it via a dock setup.
     * The method retrieves the currently selected item from the Selector singleton and, if a valid selection exists,
     * displays either scene-specific properties (if the selection type is SCENE) or entity-specific properties.
     */
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

    /**
     * @brief Displays a UI panel for managing scene properties.
     *
     * Retrieves the scene using the provided scene identifier and renders a two-column ImGui panel
     * where users can toggle the scene's visibility ("Hide") and activity status ("Pause").
     * The function processes a UI handle by removing any icon prefix before rendering the header.
     *
     * @param sceneId Identifier of the scene whose properties are to be displayed.
     */
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

    /**
     * @brief Displays the property panels for each component attached to the specified entity.
     *
     * This function retrieves all component types associated with the entity and, for each type registered in the
     * property mapping, invokes the corresponding display method to render its properties.
     *
     * @param entity The entity whose component properties are to be shown.
     */
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
    }
}
