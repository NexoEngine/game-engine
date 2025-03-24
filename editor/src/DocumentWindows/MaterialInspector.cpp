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
#include "utils/ScenePreview.hpp"
#include "components/Camera.hpp"
#include "Components/Widgets.hpp"
#include "context/Selector.hpp"
#include "exceptions/Exceptions.hpp"

namespace nexo::editor {
	/**
	 * @brief No-op setup method.
	 *
	 * This method intentionally does nothing since no initialization is required for the MaterialInspector.
	 */
	void MaterialInspector::setup()
	{
		// No need to setup anything
	}

	/**
	 * @brief Shuts down the Material Inspector.
	 *
	 * No explicit cleanup is required as the framebuffer's destructor automatically handles resource deallocation.
	 */
	void MaterialInspector::shutdown()
	{
		// No need to delete anything since the destructor of the framebuffer will handle it
	}

	/**
	 * @brief Renders the material inspector view for the selected entity.
	 *
	 * This function updates and displays the material inspector window. If the provided
	 * entity identifier differs from the current one, it updates the target and marks the
	 * material as modified. When a modification occurs, the function generates a scene preview,
	 * updates the associated camera's clear color, and executes the renderer to refresh the framebuffer.
	 * It subsequently cleans up the temporary preview scene and, if a valid framebuffer is available,
	 * displays its content using ImGui. Finally, it retrieves and forwards the material data to the
	 * material inspector widget for further modifications.
	 *
	 * @param selectedEntity The identifier of the entity whose material is being inspected.
	 *                       A value of -1 indicates an invalid entity, resulting in an early return.
	 *
	 * @throws BackendRendererApiFatalFailure If the framebuffer fails to initialize.
	 */
	void MaterialInspector::renderMaterialInspector(int selectedEntity)
	{
		bool &materialModified = m_materialModified;
		static utils::ScenePreviewOut previewParams;

		if (selectedEntity != -1)
		{
			if (m_ecsEntity != selectedEntity)
			{
				m_ecsEntity = selectedEntity;
				materialModified = true;
			}
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
		if (!m_framebuffer)
			THROW_EXCEPTION(BackendRendererApiFatalFailure, "OPENGL", "Failedd to initialize framebuffer in Material Inspector window");
		// --- Material preview ---
		if (m_framebuffer->getColorAttachmentId(0) != 0)
			ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(m_framebuffer->getColorAttachmentId(0))), {64, 64}, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();

		auto inspectorWindow = m_windowRegistry.getWindow<InspectorWindow>();
		auto materialVariant = inspectorWindow->getSubInspectorData<MaterialInspector>();
		if (std::holds_alternative<components::Material*>(materialVariant)) {
		    auto materialPtr = std::get<components::Material*>(materialVariant);
			materialModified = Widgets::drawMaterialInspector(materialPtr);
		}
	}

	/**
	 * @brief Displays and renders the Material Inspector window.
	 *
	 * Retrieves the currently selected entity and checks whether the Material Inspector
	 * sub-window is visible within the Inspector window. If visible, it opens an ImGui window
	 * with proper window flags, performs an initial docking setup on the first invocation, 
	 * and then calls renderMaterialInspector() to render the material details.
	 */
	void MaterialInspector::show()
	{
  		auto const &selector = Selector::get();
        const int selectedEntity = selector.getSelectedEntity();
        auto inspectorWindow = m_windowRegistry.getWindow<InspectorWindow>();
		if (inspectorWindow->getSubInspectorVisibility<MaterialInspector>())
        {
            ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
            if (m_firstOpened)
            	window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

            if (ImGui::Begin("Material Inspector", &inspectorWindow->getSubInspectorVisibility<MaterialInspector>(), window_flags))
            {
            	if (m_firstOpened)
             		firstDockSetup("Material Inspector");
                renderMaterialInspector(selectedEntity);
            }
            ImGui::End();
        }
	}

	/**
	 * @brief No update actions required.
	 *
	 * This method is intentionally left empty as the material inspector does not require periodic updates.
	 */
	void MaterialInspector::update()
	{
		// No need to update anything
	}
}
