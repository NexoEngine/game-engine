//// PhysicsBodyProperty.hpp //////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Thomas PARENTEAU
//  Date:        09/01/2025
//  Description: Header file for the physics body property class
//               Handles proper synchronization with Jolt Physics
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AEntityProperty.hpp"
#include "components/PhysicsBodyComponent.hpp"

namespace nexo::editor {

    class PhysicsBodyProperty final : public AEntityProperty {
        public:
            explicit PhysicsBodyProperty(InspectorWindow &inspector)
                : AEntityProperty(inspector)
            {
            }

            /**
             * @brief Displays and edits the physics body properties with proper Jolt synchronization.
             *
             * Shows the physics body type (Static/Dynamic) and handles the recreation
             * of the physics body in Jolt when properties are modified.
             *
             * @param entity The entity whose physics body properties are rendered.
             */
            void show(ecs::Entity entity) override;

        private:
            /**
             * @brief Recreates the physics body in Jolt with new properties.
             * 
             * Properly removes the old body and creates a new one with updated settings.
             * 
             * @param entity The entity whose physics body needs recreation
             * @param newType The new physics body type
             */
            void recreatePhysicsBody(ecs::Entity entity, components::PhysicsBodyComponent::Type newType);
    };

}
