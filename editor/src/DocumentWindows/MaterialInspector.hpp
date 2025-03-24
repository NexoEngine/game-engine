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
			void show() override;
        	void shutdown() override;
         	void update() override;

		private:
			void renderMaterialInspector(int selectedEntity);

			std::shared_ptr<renderer::Framebuffer> m_framebuffer = nullptr;
			int m_ecsEntity = -1;
			bool m_materialModified = true;
	};

}
