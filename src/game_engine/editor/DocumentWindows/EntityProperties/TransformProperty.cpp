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

            ImGui::Text("Position");
            ImGui::SameLine();
            bool posChanged = ImGui::DragFloat3("##Position", &transform.pos.x, 0.1f);

            Vector3 eulerRotation = math::quaternionToEulerSafe(transform.rotation);
            Vector3 previousRotation = eulerRotation;
            eulerRotation.x *= RAD2DEG;
            eulerRotation.y *= RAD2DEG;
            eulerRotation.z *= RAD2DEG;

            ImGui::Text("Rotation");
            ImGui::SameLine();
            bool rotChanged = ImGui::DragFloat3("##Rotation", &eulerRotation.x, 1.0f);

            if (rotChanged && !Vector3Equals(eulerRotation, previousRotation)) {
                eulerRotation.x *= DEG2RAD;
                eulerRotation.y *= DEG2RAD;
                eulerRotation.z *= DEG2RAD;
                Vector3 deltaRotation = Vector3Subtract(eulerRotation, previousRotation);
                eulerRotation = Vector3Add(previousRotation, deltaRotation);

                transform.rotation = QuaternionFromEuler(eulerRotation.x, eulerRotation.y, eulerRotation.z);
            }

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
