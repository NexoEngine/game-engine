//// Model.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/05/2025
//  Description: Header file for the model component
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "assets/AssetRef.hpp"
#include "assets/Assets/Model/Model.hpp"
#include "ecs/Definitions.hpp"

#include <vector>

namespace nexo::components {

    struct SubMeshIndex {
        ecs::Entity child;
        std::vector<SubMeshIndex> children;
    };

    struct ModelComponent {
        assets::AssetRef<assets::Model> model;
        std::vector<SubMeshIndex> children;
    };
}
