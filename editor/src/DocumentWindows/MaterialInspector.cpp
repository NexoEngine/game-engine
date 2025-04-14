//// MaterialInspector.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/03/2025
//  Description: Source file for the material inspector window
//
///////////////////////////////////////////////////////////////////////////////

#include "MaterialInspector.hpp"
#include "DocumentWindows/InspectorWindow.hpp"
#include "Exception.hpp"
#include "components/Render.hpp"
#include "utils/ScenePreview.hpp"
#include "components/Camera.hpp"
#include "Components/Widgets.hpp"
#include "context/Selector.hpp"
#include "exceptions/Exceptions.hpp"

namespace nexo::editor {

	void MaterialInspector::setup()
	{
		// No need to setup anything
	}

	void MaterialInspector::shutdown()
	{
		// No need to delete anything since the destructor of the framebuffer will handle it
	}

	void MaterialInspector::renderMaterialInspector(int selectedEntity)
	{
		bool &materialModified = m_materialModified;
		static utils::ScenePreviewOut previewParams;

		if (selectedEntity != -1 && m_ecsEntity != selectedEntity)
		{
			auto renderComp = Application::m_coordinator->tryGetComponent<components::RenderComponent>(selectedEntity);
			if (renderComp)
			{
				m_ecsEntity = selectedEntity;
				materialModified = true;
			}
			else
			{
				m_ecsEntity = -1;
			}
   		}

		if (m_ecsEntity == -1)
			return;

		if (materialModified)
		{
			utils::genScenePreview("Modify material inspector", {64, 64}, m_ecsEntity, previewParams);
			auto &app = nexo::getApp();
			app.run(previewParams.sceneId, RenderingType::FRAMEBUFFER);
			auto &cameraComponent = nexo::Application::m_coordinator->getComponent<components::CameraComponent>(previewParams.cameraId);
			m_framebuffer = cameraComponent.m_renderTarget;
			materialModified = false;
			app.getSceneManager().deleteScene(previewParams.sceneId);
		}

		if (!m_framebuffer)
			THROW_EXCEPTION(BackendRendererApiFatalFailure, "OPENGL", "Failed to initialize framebuffer in Material Inspector window");
		// --- Material preview ---
		if (m_framebuffer->getColorAttachmentId(0) != 0)
			ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(m_framebuffer->getColorAttachmentId(0))), {64, 64}, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();

		auto inspectorWindow = m_windowRegistry.getWindow<InspectorWindow>(NEXO_WND_USTRID_INSPECTOR).lock();
		if (!inspectorWindow)
			return;
		auto materialVariant = inspectorWindow->getSubInspectorData<MaterialInspector>();
		if (std::holds_alternative<components::Material*>(materialVariant)) {
		    auto materialPtr = std::get<components::Material*>(materialVariant);
			materialModified = Widgets::drawMaterialInspector(materialPtr);
		}
	}

	void MaterialInspector::show()
	{
  		auto const &selector = Selector::get();
        const int selectedEntity = selector.getSelectedEntity();
        auto inspectorWindow = m_windowRegistry.getWindow<InspectorWindow>(NEXO_WND_USTRID_INSPECTOR).lock();
        if (!inspectorWindow)
            return;
		if (inspectorWindow->getSubInspectorVisibility<MaterialInspector>())
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
            if (m_firstOpened)
            	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

            if (ImGui::Begin("Material Inspector" "###" NEXO_WND_USTRID_MATERIAL_INSPECTOR, &inspectorWindow->getSubInspectorVisibility<MaterialInspector>(), window_flags))
            {
            	firstDockSetup(NEXO_WND_USTRID_MATERIAL_INSPECTOR);
                renderMaterialInspector(selectedEntity);
            }
            ImGui::End();
        }
	}

	void MaterialInspector::update()
	{
		// No need to update anything
	}
}
