//// Show.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        28/04/2025
//  Description: Source file for the material inspector rendering
//
///////////////////////////////////////////////////////////////////////////////

#include "DocumentWindows/EntityProperties/MaterialProperty.hpp"
#include "MaterialInspector.hpp"
#include "components/MaterialComponent.hpp"
#include "context/ThumbnailCache.hpp"
#include "utils/ScenePreview.hpp"
#include "DocumentWindows/InspectorWindow/InspectorWindow.hpp"
#include "ImNexo/Elements.hpp"
#include "ImNexo/Panels.hpp"
#include "context/Selector.hpp"

namespace nexo::editor {

    void MaterialInspector::renderMaterialInspector()
    {
        const auto inspectorWindow = m_windowRegistry.getWindow<InspectorWindow>(NEXO_WND_USTRID_INSPECTOR).lock();
        auto dataOpt = inspectorWindow->getSubInspectorData<MaterialInspector, MaterialInspectorData>();
        if (!dataOpt.has_value())
            return;
        const auto& data = *dataOpt;

        if (!Application::m_coordinator->entityHasComponent<components::MaterialComponent>(data.m_selectedEntity))
            return;

        unsigned int textureID = 0;
        if (m_materialModified)
        {
            textureID = ThumbnailCache::getInstance().updateMaterialThumbnail(data.material);
        } else {
            textureID = ThumbnailCache::getInstance().getMaterialThumbnail(data.material);
        }
        if (!textureID)
            return;

        // --- Material preview ---
        ImNexo::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureID)), {64, 64});
        ImGui::SameLine();

        auto material = data.material.lock();
        components::Material& materialData = *material->getData();

        m_materialModified = ImNexo::MaterialInspector(materialData);
    }

	void MaterialInspector::show()
	{
		const auto inspectorWindow = m_windowRegistry.getWindow<InspectorWindow>(NEXO_WND_USTRID_INSPECTOR).lock();
		if (!inspectorWindow)
			return;
		if (inspectorWindow->getSubInspectorVisibility<MaterialInspector>())
		{
			ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoCollapse;
			if (m_firstOpened)
				window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;

			if (ImGui::Begin("Material Inspector" NEXO_WND_USTRID_MATERIAL_INSPECTOR, &inspectorWindow->getSubInspectorVisibility<MaterialInspector>(), window_flags))
			{
				beginRender(NEXO_WND_USTRID_MATERIAL_INSPECTOR);
				renderMaterialInspector();
			}
			ImGui::End();
		}
	}
}
