/*
** EPITECH PROJECT, 2024
** game_engine
** File description:
** TransformProperty.cpp
*/

#include <imgui.h>

#include "game_engine/editor/DocumentWindows/EntityProperties/TransformProperty.hpp"
#include "EntityHandling.hpp"
#include "Math.hpp"

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
            Matrix objectMatrix = engine::entity::getTransformMatrix(_sceneManagerBridge.getSelectedEntity());
            Vector3 translation, rotation, scale;
            math::decomposeTransformMatrixEuler(objectMatrix, translation, rotation, scale);

            ImGui::Text("Position");
            ImGui::SameLine();
            bool posChanged = ImGui::DragFloat3("##Position", &transform.pos.x, 0.1f);

            Vector3 rotationInDegree = Vector3Scale(transform.rotation, RAD2DEG);
            ImGui::Text("Rotation");
            ImGui::SameLine();
            bool rotChanged = ImGui::DragFloat3("##Rotation", &rotationInDegree.x, 0.05f);

            transform.rotation = Vector3Scale(rotationInDegree, DEG2RAD);

            ImGui::Text("Scale");
            ImGui::SameLine();
            bool scaleChanged = ImGui::DragFloat3("##Scale", &transform.scale.x, 0.1f);

            if (posChanged || rotChanged || scaleChanged) 
            {
                entity::updateEntityTransformMatrix(_sceneManagerBridge.getSelectedEntity());
            }

            return true;
        }
        return false;
    }

    void TransformProperty::showEnd()
    {
        AEntityProperty::showEnd();
    }
}
