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

#include "EntityProperties/RenderProperty.hpp"
#include "EntityProperties/TransformProperty.hpp"
#include "tinyfiledialogs.h"

#include "Components/Components.hpp"
#include "Components/Widgets.hpp"

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

	bool MaterialInspector::drawTextureButton(const std::string &label, std::shared_ptr<renderer::Texture2D> &texture)
	{
		bool textureModified = false;
		ImVec2 previewSize(32, 32);
        ImGui::PushID(label.c_str());

        ImTextureID textureId = texture ? static_cast<ImTextureID>(static_cast<intptr_t>(texture->getId())) : 0;
        std::string textureButton = std::string("##TextureButton") + label;

        if (ImGui::ImageButton(textureButton.c_str(), textureId, previewSize))
        {
            const char* filePath = tinyfd_openFileDialog(
                "Open Texture",
                "",
                0,
                nullptr,
                nullptr,
                0
            );

            if (filePath)
            {
                std::string path(filePath);
                std::shared_ptr<renderer::Texture2D> newTexture = renderer::Texture2D::create(path);
                if (newTexture)
                {
                    texture = newTexture;
                    textureModified = true;
                }
            }
        }
        Components::drawButtonBorder(IM_COL32(255,255,255,0), IM_COL32(255,255,255,255), IM_COL32(255,255,255,0), 0.0f, 0, 2.0f);
		ImGui::PopID();
		ImGui::SameLine();
		ImGui::Text("%s", label.c_str());
		return textureModified;
	}

	void MaterialInspector::show(int selectedEntity, const VariantData &selectedData)
	{
		auto app = nexo::getApp();
		static bool materialModified = true;
		if (selectedEntity != -1)
		{
			if (std::holds_alternative<EntityProperties>(selectedData))
			{
				const auto& [layerProps, entity] = std::get<EntityProperties>(selectedData);
				if (m_ecsEntity != entity)
					m_ecsEntity = entity;
			}
   		}

		if (m_ecsEntity == -1)
			return;

		if (materialModified)
		{
			m_framebuffer->bind();
			app.genAssetPreview(m_ecsEntity);
			m_framebuffer->unbind();
			materialModified = false;
		}

		// --- Material preview ---
		const unsigned int textureId = m_framebuffer->getColorAttachmentId(0);
		ImGui::Image(static_cast<ImTextureID>(static_cast<intptr_t>(textureId)), {64, 64}, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::SameLine();

		// --- Shader Selection ---
		ImGui::BeginGroup();
		{
			ImGui::Text("Shader:");
			ImGui::SameLine();

			static int currentShaderIndex = 0;
			const char* shaderOptions[] = { "Standard", "Unlit", "CustomPBR" };
			float availableWidth = ImGui::GetContentRegionAvail().x;
			ImGui::SetNextItemWidth(availableWidth);

			if (ImGui::Combo("##ShaderCombo", &currentShaderIndex, shaderOptions, IM_ARRAYSIZE(shaderOptions)))
			{
				//TODO: implement shader selection
			}

		}
		ImGui::EndGroup();
		ImGui::Spacing();

		// --- Rendering mode selection ---
		ImGui::Text("Rendering mode:");
		ImGui::SameLine();
		static int currentRenderingModeIndex = 0;
		const char* renderingModeOptions[] = { "Opaque", "Transparent", "Refraction" };
		float availableWidth = ImGui::GetContentRegionAvail().x;

		ImGui::SetNextItemWidth(availableWidth);
		if (ImGui::Combo("##RenderingModeCombo", &currentRenderingModeIndex, renderingModeOptions, IM_ARRAYSIZE(renderingModeOptions)))
		{
			//TODO: implement rendering mode
		}

		// --- Albedo texture ---
  		static ImGuiColorEditFlags colorPickerModeAlbedo = ImGuiColorEditFlags_PickerHueBar;
    	static bool showColorPickerAlbedo = false;
		materialModified = drawTextureButton("Albedo texture", RenderProperty::selectedMaterial->albedoTexture) || materialModified;
		ImGui::SameLine();
		materialModified = Widgets::drawColorEditor("##ColorEditor Albedo texture", &RenderProperty::selectedMaterial->albedoColor, &colorPickerModeAlbedo, &showColorPickerAlbedo) || materialModified;

		// --- Specular texture ---
		static ImGuiColorEditFlags colorPickerModeSpecular = ImGuiColorEditFlags_PickerHueBar;
		static bool showColorPickerSpecular = false;
		materialModified = drawTextureButton("Specular texture", RenderProperty::selectedMaterial->metallicMap) || materialModified;
		ImGui::SameLine();
		materialModified = Widgets::drawColorEditor("##ColorEditor Specular texture", &RenderProperty::selectedMaterial->specularColor, &colorPickerModeSpecular, &showColorPickerSpecular) || materialModified;
	}

    InspectorWindow::InspectorWindow()
    {
    	m_materialInspector = std::make_shared<MaterialInspector>();
        m_componentShowFunctions[typeid(components::TransformComponent)] = &TransformProperty::show;
        m_componentShowFunctions[typeid(components::RenderComponent)] = &RenderProperty::show;
    }

    InspectorWindow::~InspectorWindow() = default;

    void InspectorWindow::setup()
    {
    	m_materialInspector->setup();
    }

    void InspectorWindow::shutdown()
    {
    }

    void InspectorWindow::show()
    {
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - 400, ImGui::GetIO().DisplaySize.y - 500),
                                ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400, 500), ImGuiCond_FirstUseEver);
        ImGui::Begin("Inspector", &m_opened, ImGuiWindowFlags_NoCollapse);


        if (ImGui::IsMouseClicked(ImGuiMouseButton_Right))
        {
            LOG(NEXO_INFO, "right click on inspector\n");
        }

        showEntityProperties();
        ImGui::End();

        if (RenderProperty::showMaterialInspector)
        {
			ImGui::Begin("Material Inspector", &RenderProperty::showMaterialInspector);
			{
				m_materialInspector->show(m_sceneManagerBridge->getSelectedEntity(), m_sceneManagerBridge->getData());
			}
			ImGui::End();
        }

    }

    void InspectorWindow::showEntityProperties()
    {
        const int selectedEntity = m_sceneManagerBridge->getSelectedEntity();
        auto const& App = getApp();
        if (selectedEntity == -1) return;
        if (!std::holds_alternative<EntityProperties>(m_sceneManagerBridge->getData())) return;
        const auto& [layerProps, entity] = std::get<EntityProperties>(m_sceneManagerBridge->getData());

        const std::vector<std::type_index> componentsType = App.getAllEntityComponentTypes(entity);
        for (auto& type : componentsType)
        {
            if (m_componentShowFunctions.contains(type))
            {
                m_componentShowFunctions[type](entity);
            }
        }
    }

    void InspectorWindow::update()
    {
    }
}
