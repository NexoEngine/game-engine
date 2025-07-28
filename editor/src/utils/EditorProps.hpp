//// EditorProps.hpp //////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        19/04/2025
//  Description: Header file for the utils function to create editor props
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/Coordinator.hpp"

namespace nexo::editor::utils {

    enum class PropsType {
        CAMERA,
        POINT_LIGHT,
        SPOT_LIGHT
    };

    void addPropsTo(ecs::Entity entity, PropsType type);
}
