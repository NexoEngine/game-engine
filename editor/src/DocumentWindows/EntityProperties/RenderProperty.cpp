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
                static ImVec4 tempColor(1.0f, 0.0f, 0.0f, 1.0f);
                selectedEntityColor = &tempColor;
            }
        }
        else if (renderComponent.type == components::RenderType::RENDER_2D)
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

        if (ImGui::CollapsingHeader("Renderer", ImGuiTreeNodeFlags_DefaultOpen))
        {
            ImGui::Checkbox("Active", &renderComponent.isRendered);

            ImGui::ColorEdit4("Color", reinterpret_cast<float*>(selectedEntityColor),
                                ImGuiColorEditFlags_DisplayRGB
                            | ImGuiColorEditFlags_InputRGB
                            | ImGuiColorEditFlags_NoSidePreview
                            | ImGuiColorEditFlags_NoTooltip);
        }

        return true;
    }

    void RenderProperty::showEnd()
    {
        AEntityProperty::showEnd();
    }
}
