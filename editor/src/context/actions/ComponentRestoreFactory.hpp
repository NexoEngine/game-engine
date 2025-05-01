//// ComponentRestoreFactory.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
