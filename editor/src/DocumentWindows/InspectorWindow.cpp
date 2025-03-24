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
 * @brief Default destructor for the InspectorWindow.
 *
 * The default implementation ensures proper cleanup of InspectorWindow instances without any explicit resource deallocation.
 */
InspectorWindow::~InspectorWindow() = default;

    /**
     * @brief Initializes the property handlers for various entity component types.
     *
     * This method populates the internal map that links component type identifiers (obtained via
     * typeid) to their corresponding property display handlers, such as TransformProperty,
     * RenderProperty, and various light and camera properties. These handlers are constructed
     * with the current InspectorWindow instance and later used to display component-specific
     * properties in the inspector UI.
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
     * @brief Shuts down the inspector window.
     *
     * This method is intended for cleanup operations when the inspector window is closed.
     * Currently, it does not perform any actions.
     */
    void InspectorWindow::shutdown()
    {
    }

    /**
     * @brief Renders the Inspector window.
     *
     * Opens an ImGui window titled "Inspector" and, on its first display, configures docking by calling
     * firstDockSetup("Inspector"). It retrieves the currently selected entity via the Selector singleton and,
     * if a valid selection exists, displays either scene or entity properties depending on the selection type.
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
     * @brief Displays the scene's properties in the inspector UI.
     *
     * Retrieves the scene corresponding to the provided SceneId and renders UI controls that allow toggling the scene's
     * render and active statuses. The UI is configured with two columns where the "Hide" checkbox inverts the scene's
     * rendering state and the "Pause" checkbox inverts its active state. An icon prefix is removed from the scene's UI handle
     * before display.
     *
     * @param sceneId The identifier of the scene whose properties are to be displayed.
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
     * @brief Renders the UI for the properties of an entity's components.
     *
     * Iterates through all component types associated with the given entity and,
     * for each type that has a registered property handler in the m_entityProperties map,
     * invokes its show() method to display the component's properties in the inspector.
     *
     * @param entity The entity whose component properties are being displayed.
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
