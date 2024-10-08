/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** RenderProperty.cpp
*/

#include <imgui.h>

#include "game_engine/editor/DocumentWindows/EntityProperties/RenderProperty.hpp"
#include <EntityHandling.hpp>

#include <loguru/loguru.hpp>

namespace engine::editor {
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

    int RenderProperty::show()
    {
        if (AEntityProperty::show()) {
            auto &render = engine::entity::getComponent<ecs::components::render::render_t>(_sceneManagerBridge.getSelectedEntity());

            ImGui::Text("Configure the rendering of this entity");

            ImGui::Checkbox("Visible", &render.isRendered);
            if (ImGui::IsItemHovered())
                ImGui::SetTooltip("Check this box to make the entity visible");



            return true;
        }
        return false;
    }

    void RenderProperty::showEnd()
    {
        AEntityProperty::showEnd();
    }
}
