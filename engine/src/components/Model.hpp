//// Model.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        06/05/2025
//  Description: Header file for the model component
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "assets/AssetRef.hpp"
#include "assets/Assets/Model/Model.hpp"

namespace nexo::components {

    struct ModelComponent {
        assets::AssetRef<assets::Model> model;
    };
}
