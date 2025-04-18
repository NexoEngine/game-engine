//// TransformProperty.cpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/03/2025
//  Description: Source file for the transform property inspector
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>

#include "TransformProperty.hpp"
#include "ImNexo/Elements.hpp"
#include "ImNexo/EntityProperties.hpp"

namespace nexo::editor {

    void TransformProperty::show(ecs::Entity entity)
    {
        auto& transformComponent = Application::getEntityComponent<components::TransformComponent>(entity);
        static glm::vec3 lastDisplayedEuler(0.0f);

        if (ImNexo::Header("##TransformNode", "Transform Component"))
        {
            ImNexo::Transform(transformComponent, lastDisplayedEuler);
            ImGui::TreePop();
        }
    }
}
