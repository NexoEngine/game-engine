//// MaterialInspector.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        25/03/2025
//  Description: Header file for the material inspector window
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ADocumentWindow.hpp"

namespace nexo::editor {

	class MaterialInspector : public ADocumentWindow {
		public:
			using ADocumentWindow::ADocumentWindow;
			void setup() override;

			/**
			 * @brief Displays the Material Inspector window.
			 *
			 * This method retrieves the selected entity and the inspector window, then renders the Material Inspector UI using ImGui.
			 * It sets up appropriate window flags, performs a first-time docking setup when necessary, and delegates material
			 * rendering to renderMaterialInspector() if the Material Inspector is visible.
			 */
			void show() override;
        	void shutdown() override;
         	void update() override;

		private:

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
			void renderMaterialInspector(int selectedEntity);

			std::shared_ptr<renderer::Framebuffer> m_framebuffer = nullptr;
			int m_ecsEntity = -1;
			bool m_materialModified = true;
	};

}
