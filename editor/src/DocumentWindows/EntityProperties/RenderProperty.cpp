/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** RenderProperty.cpp
*/

#include <imgui.h>

#include "RenderProperty.hpp"
#include "AEntityProperty.hpp"

#include <loguru/loguru.hpp>

namespace nexo::editor {
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

        ImVec4* selectedEntityColor = nullptr;
        if (renderComponent.type == components::RenderType::RENDER_3D)
        {
            auto renderable3D = std::dynamic_pointer_cast<components::Renderable3D>(renderComponent.renderable);
            if (renderable3D)
            {
                //auto& [color] = renderable3D->material;
                ImVec4 tempColor(1.0f, 0.0f, 0.0f, 1.0f);
                selectedEntityColor = &tempColor;
            }
        }
        if (renderComponent.type == components::RenderType::RENDER_2D)
        {
            auto renderable2D = std::dynamic_pointer_cast<components::Renderable2D>(renderComponent.renderable);
            if (renderable2D)
            {
                auto& [color, texture, sprite] = renderable2D->sprite;
                selectedEntityColor = reinterpret_cast<ImVec4*>(&color);
            }
        }
        if (!selectedEntityColor)
            return false;

        ImGui::Checkbox("##RenderComponentActive", &renderComponent.isRendered);

        ImGui::SameLine(ImGui::GetCursorPosX());
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(30.0f, 11.0f));
        if (ImGui::TreeNodeEx("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::ColorEdit4("##InspectorRenderColorPicker", reinterpret_cast<float*>(selectedEntityColor),
                              ImGuiColorEditFlags_DisplayRGB | ImGuiColorEditFlags_InputRGB |
                              ImGuiColorEditFlags_NoSidePreview |
                              ImGuiColorEditFlags_NoTooltip);
            ImGui::TreePop();
        }

        ImGui::PopStyleVar();
        return true;
    }

    void RenderProperty::showEnd()
    {
        AEntityProperty::showEnd();
    }
}
