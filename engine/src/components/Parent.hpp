//// Parent.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/05/2025
//  Description: Header file for the parent component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/Definitions.hpp"
#include "assets/AssetRef.hpp"
#include "assets/Assets/Model/Model.hpp"

namespace nexo::components {

    struct ParentComponent {
        ecs::Entity parent;
    };

    struct RootComponent {
        std::string name = "Root";
        assets::AssetRef<assets::Model> modelRef;
        int childCount = 0;
    };

}
