//// AEntityProperty.cpp //////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie Giacomel
//  Date:        03/01/2025
//  Description: Source file for AEntityProperty
//
///////////////////////////////////////////////////////////////////////////////

#include <imgui.h>
#include "AEntityProperty.hpp"

namespace nexo::editor {
    AEntityProperty::AEntityProperty(const std::string &name)
        : _name{name}
    {

    }

    AEntityProperty::~AEntityProperty() = default;

    void AEntityProperty::update()
    {

    }

    int AEntityProperty::show(ecs::Entity entity) {
        return 1;//ImGui::CollapsingHeader(_name.c_str(), ImGuiTreeNodeFlags_DefaultOpen);
    }

    void AEntityProperty::showEnd()
    {
        //ImGui::End();
    }
}
