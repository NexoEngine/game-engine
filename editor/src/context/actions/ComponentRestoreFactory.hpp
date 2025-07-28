//// ComponentRestoreFactory.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        30/04/2025
//  Description: Header file for the restore component action factory
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Action.hpp"
#include "ecs/Definitions.hpp"
#include <memory>
#include <typeindex>

namespace nexo::editor {
    class ComponentRestoreFactory {
        public:
            static std::unique_ptr<Action> createRestoreComponent(ecs::Entity entity, std::type_index typeIndex);
    };
}
