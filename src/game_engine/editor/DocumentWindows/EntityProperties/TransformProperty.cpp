/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** TransformProperty.cpp
*/

#include <imgui.h>

#include "game_engine/editor/DocumentWindows/EntityProperties/TransformProperty.hpp"
#include <EntityHandling.hpp>

#include <loguru/loguru.hpp>

namespace engine::editor {
    TransformProperty::TransformProperty(const std::string& name)
        : AEntityProperty(name)
    {

    }

    TransformProperty::~TransformProperty()
    {
    }

    void TransformProperty::update()
    {

    }

    int TransformProperty::show()
    {
        if (AEntityProperty::show()) {
            auto &transform = engine::entity::getComponent<ecs::components::physics::transform_t>(_sceneManagerBridge.getSelectedEntity());
            // Position
            ImGui::Text("Position");
            ImGui::SameLine(); ImGui::DragFloat3("##Position", &transform.pos.x, 0.1f);

            // Rotation
            ImGui::Text("Rotation");
            ImGui::SameLine(); ImGui::DragFloat3("##Rotation", &transform.rotation.x, 0.1f);
            // Scale
            ImGui::Text("Scale");
            ImGui::SameLine(); ImGui::DragFloat3("##Scale", &transform.scale.x, 0.1f);
            return true;
        }
        return false;
    }

    void TransformProperty::showEnd()
    {
        AEntityProperty::showEnd();
    }
}
