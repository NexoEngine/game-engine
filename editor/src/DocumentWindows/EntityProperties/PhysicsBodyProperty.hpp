//// PhysicsBodyProperty.hpp //////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      AI Assistant
//  Date:        08/01/2025
//  Description: Header file for the physics body property inspector
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "AEntityProperty.hpp"
#include "components/PhysicsBodyComponent.hpp"

namespace nexo::editor {

    class PhysicsBodyProperty final : public AEntityProperty {
        public:
            explicit PhysicsBodyProperty(InspectorWindow &inspector) : AEntityProperty(inspector) {}

            void show(ecs::Entity entity) override;
            
            static bool addPhysicsComponentToEntity(ecs::Entity entity, bool isDynamic);
    };

}
