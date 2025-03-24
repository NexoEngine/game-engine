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
	 * @brief No setup actions are required for the MaterialInspector.
	 *
	 * This method is intentionally left empty as the initialization needs are handled elsewhere.
	 */
	void MaterialInspector::setup()
	{
		// No need to setup anything
	}

	/**
	 * @brief Shuts down the Material Inspector.
	 *
	 * No explicit cleanup is performed as the framebuffer's destructor automatically handles resource deallocation.
	 */
	void MaterialInspector::shutdown()
	{
		// No need to delete anything since the destructor of the framebuffer will handle it
	}

	/**
	 * @brief Renders the material inspector for the selected entity.
	 *
	 * When a valid entity is provided (i.e., `selectedEntity` is not -1) and differs from the current entity,
	 * this method marks the material as modified and regenerates a scene preview. If the material has been modified,
	 * it updates the preview by running a framebuffer render pass, displays the resulting image using ImGui,
	 * and processes potential material changes through the inspector widget.
	 *
	 * @param selectedEntity The identifier of the selected entity. A value of -1 indicates no valid selection.
	 *
	 * @throw BackendRendererApiFatalFailure Thrown if the framebuffer fails to initialize.
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
			THROW_EXCEPTION(BackendRendererApiFatalFailure, "OPENGL", "Failed to initialize framebuffer in Material Inspector window");
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
	 * @brief Displays the Material Inspector window.
	 *
	 * This method retrieves the selected entity and the inspector window, then renders the Material Inspector UI using ImGui.
	 * It sets up appropriate window flags, performs a first-time docking setup when necessary, and delegates material
	 * rendering to renderMaterialInspector() if the Material Inspector is visible.
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
	 * @brief No update actions are performed.
	 *
	 * This method is provided to adhere to the interface without requiring any update logic.
	 */
	void MaterialInspector::update()
	{
		// No need to update anything
	}
}
