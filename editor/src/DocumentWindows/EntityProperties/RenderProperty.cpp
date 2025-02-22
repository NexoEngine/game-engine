/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** RenderProperty.cpp
*/

#include <imgui.h>

#include "RenderProperty.hpp"
#include "AEntityProperty.hpp"
#include "Components/EntityPropertiesComponents.hpp"

#include <imgui_internal.h>
#include <loguru/loguru.hpp>

namespace nexo::editor {
	bool RenderProperty::showMaterialInspector = false;
	components::Material *RenderProperty::selectedMaterial = nullptr;

    RenderProperty::RenderProperty(const std::string& name)
        : AEntityProperty(name)
    {
    }

    RenderProperty::~RenderProperty()
    {
    }

    void RenderProperty::update()
    {
    }

    int RenderProperty::show(ecs::Entity entity)
    {
    	auto const& App = getApp();
        auto& renderComponent = App.getEntityComponent<components::RenderComponent>(entity);

        glm::vec4 *selectedEntityColor = nullptr;
        static components::Material *selectedMaterial = nullptr;
        static bool showMaterialInspector = false;
        if (renderComponent.type == components::RenderType::RENDER_3D)
        {
            auto renderable3D = std::dynamic_pointer_cast<components::Renderable3D>(renderComponent.renderable);
            if (renderable3D)
            {
            	selectedMaterial = &renderable3D->material;
            }
        }
        else if (renderComponent.type == components::RenderType::RENDER_2D)
        {
            auto renderable2D = std::dynamic_pointer_cast<components::Renderable2D>(renderComponent.renderable);
            if (renderable2D)
            {
                auto& [color, texture, sprite] = renderable2D->sprite;
                selectedEntityColor = &color;
            }
        }
        bool open = EntityPropertiesComponents::drawHeader("##RenderNode", "Render Component");
        static ImGuiColorEditFlags colorPickerMode = ImGuiColorEditFlags_PickerHueBar;
        static bool sectionOpen = true;

        if (open)
        {
            ImGui::SetWindowFontScale(1.15f);
        	ImGui::Text("Hide");
         	ImGui::SameLine(0, 12);
         	bool hidden = !renderComponent.isRendered;
            ImGui::Checkbox("##HideCheckBox", &hidden);
            renderComponent.isRendered = !hidden;

            EntityPropertiesComponents::drawToggleButtonWithSeparator("Material", &sectionOpen);
            if (sectionOpen)
            {
                // --- Material Preview (Placeholder) ---
                // For now, we use a dummy texture id (0), maybe store the asset image in the asset manager later ?
                ImGui::Image(0, ImVec2(64, 64));
                ImGui::SameLine();

                ImGui::BeginGroup();
                {
                    // --- Dropdown for Material Types ---
                    static int selectedMaterialIndex = 0;
                    const char* materialTypes[] = { "Default", "Metal", "Wood", "Plastic" };
                    ImGui::Combo("##MaterialType", &selectedMaterialIndex, materialTypes, IM_ARRAYSIZE(materialTypes));

                    // --- Material Action Buttons ---
                    if (ImGui::Button("Create new material"))
                    {
                    	ImGui::OpenPopup("Create Material");
                    }
                    ImGui::SameLine();
                    if (ImGui::Button("Modify Material"))
                    {
                    	RenderProperty::selectedMaterial = selectedMaterial;
                     	RenderProperty::showMaterialInspector = true;
                    }
                }
                ImGui::EndGroup();
                ImVec2 center = ImGui::GetMainViewport()->GetCenter();
	            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	            if (ImGui::BeginPopupModal("Create Material", nullptr, ImGuiWindowFlags_AlwaysAutoResize))
	            {
	                ImGui::Text("Create New Material");
	                ImGui::Separator();
	                static char materialName[128] = "";
	                ImGui::InputText("Name", materialName, IM_ARRAYSIZE(materialName));

	                ImGui::Spacing();

	                if (ImGui::Button("OK", ImVec2(120, 0)))
	                {
	                    // TODO: Insert logic to create the new material
	                    ImGui::CloseCurrentPopup();
	                }
	                ImGui::SameLine();
	                if (ImGui::Button("Cancel", ImVec2(120, 0)))
	                {
	                    ImGui::CloseCurrentPopup();
	                }
	                ImGui::EndPopup();
	            }
            }
            ImGui::TreePop();
        }

        return true;
    }

    void RenderProperty::showEnd()
    {
        AEntityProperty::showEnd();
    }
}
