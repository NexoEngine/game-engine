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
	void MaterialInspector::setup()
	{
		renderer::FramebufferSpecs framebufferSpecs;
		framebufferSpecs.attachments = {renderer::FrameBufferTextureFormats::RGBA8, renderer::FrameBufferTextureFormats::Depth};
		framebufferSpecs.width = static_cast<unsigned int>(64);
		framebufferSpecs.height = static_cast<unsigned int>(64);
		m_framebuffer = renderer::Framebuffer::create(framebufferSpecs);
		m_framebuffer->setClearColor({0.05f, 0.05f, 0.05f, 0.0f});
	}

	void MaterialInspector::show(const int selectedEntity)
	{
		static bool materialModified = true;
		static utils::ScenePreviewOut previewParams;

		if (selectedEntity != -1)
		{
			if (m_ecsEntity != selectedEntity)
				m_ecsEntity = selectedEntity;
   		}

		if (m_ecsEntity == -1)
			return;

		if (materialModified)
		{
			utils::genScenePreview("Modify material inspector", {64, 64}, m_ecsEntity, previewParams);
			auto &app = nexo::getApp();
			auto &cameraComponent = nexo::Application::m_coordinator->getComponent<components::CameraComponent>(previewParams.cameraId);
			cameraComponent.clearColor = {0.05f, 0.05f, 0.05f, 0.0f};
			app.run(previewParams.sceneId, RenderingType::FRAMEBUFFER);
			m_framebuffer = cameraComponent.m_renderTarget;
			materialModified = false;
			app.getSceneManager().deleteScene(previewParams.sceneId);
		}

		// --- Material preview ---
		if (m_framebuffer->getColorAttachmentId(0) != 0)
			ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(m_framebuffer->getColorAttachmentId(0))), {64, 64}, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();

		auto materialVariant = m_inspector.getSubInspectorData<MaterialInspector>();
		if (std::holds_alternative<components::Material*>(materialVariant)) {
		    auto materialPtr = std::get<components::Material*>(materialVariant);
			materialModified = Widgets::drawMaterialInspector(materialPtr);
		}
	}

    InspectorWindow::~InspectorWindow() = default;

    void InspectorWindow::setup()
    {

    	m_materialInspector = std::make_shared<MaterialInspector>(*this);
    	m_materialInspector->setup();


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
    }

    void InspectorWindow::show()
    {
        ImGui::Begin("Inspector", &m_opened, ImGuiWindowFlags_NoCollapse);
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

        if (getSubInspectorVisibility<MaterialInspector>())
        {
        	static bool first = true;

            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
            if (first)
            	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

            if (ImGui::Begin("Material Inspector", &getSubInspectorVisibility<MaterialInspector>(), window_flags))
            {
	            if (ImGuiWindow* currentWindow = ImGui::GetCurrentWindow(); currentWindow && first)
	            {
		            const bool isDocked = currentWindow->DockIsActive;
	                const ImGuiID currentDockID = currentWindow->DockId;

					//TODO: Implement a docking registry
	                if (!isDocked || currentDockID != g_materialInspectorDockID)
						currentWindow->DockId = g_materialInspectorDockID;
	            }
                m_materialInspector->show(selectedEntity);
                first = false;
            }
            ImGui::End();
        }
    }

    void InspectorWindow::showSceneProperties(const scene::SceneId sceneId)
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
    }
}
