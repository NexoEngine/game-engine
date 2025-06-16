//// Show.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzzzzzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzzzzzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz     zzzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Marie Giacomel
//  Date:        06/06/2025
//  Description: Source file for the primitive customization window setup
//
///////////////////////////////////////////////////////////////////////////////

#include "PrimitiveWindow.hpp"
#include "DocumentWindows/InspectorWindow/InspectorWindow.hpp"

namespace nexo::editor {

	void PrimitiveWindow::renderPrimitiveWindow()
	{
		if (m_selectedPrimitive == CUBE) {
			ImGui::Text("Cube customization options");
		}
		else if (m_selectedPrimitive == SPHERE) {
			ImGui::Text("Sphere customization options");
		}
		else if (m_selectedPrimitive == CYLINDER) {
			ImGui::Text("Cylinder customization options");
		}
		else if (m_selectedPrimitive == PYRAMID) {
			ImGui::Text("Pyramid customization options");
		}
		else if (m_selectedPrimitive == TETRAHEDRON) {
			ImGui::Text("Tetrahedron customization options");
		}
	}

	void PrimitiveWindow::show()
	{
		const auto inspectorWindow = m_windowRegistry.getWindow<InspectorWindow>(NEXO_WND_USTRID_INSPECTOR).lock();
		if (!inspectorWindow)
			return;
		if (inspectorWindow->getSubInspectorVisibility<PrimitiveWindow>())
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
			if (m_firstOpened)
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

			if (ImGui::Begin("Primitive Window" NEXO_WND_USTRID_PRIMITIVE_WINDOW, &inspectorWindow->getSubInspectorVisibility<PrimitiveWindow>(), window_flags))
			{
				beginRender(NEXO_WND_USTRID_PRIMITIVE_WINDOW);
				renderPrimitiveWindow();
			}
			ImGui::End();
		}
	}
}
