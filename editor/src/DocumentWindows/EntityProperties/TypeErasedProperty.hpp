//// TypeErasedProperty.hpp ///////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Guillaume HEIN
//  Date:        25/06/2025
//  Description: Header file for the type erased property class
//               used to display and edit entity properties
//               for C# defined components
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AEntityProperty.hpp"

namespace nexo::editor {
    class TypeErasedProperty final : public AEntityProperty {
        public:
        	TypeErasedProperty(InspectorWindow &inspector, const ecs::ComponentType componentType, const std::shared_ptr<ecs::ComponentDescription>& description)
                : AEntityProperty(inspector)
                , m_componentType(componentType), m_description(description)
            {
            }

            /**
             * @brief Displays and edits the properties of an entity component using an ImGui interface.
             *
             * Retrieves the component data for the given entity based on the stored component type,
             * and renders editable fields according to the component description metadata.
             * Supports various field types including primitives and vector types.
             *
             * @param entity The entity whose component properties are rendered.
             */
            void show(ecs::Entity entity) override;

        private:
            const ecs::ComponentType m_componentType; // Type of the component being displayed
            const std::shared_ptr<ecs::ComponentDescription> m_description; // Description of the component being displayed
    };

}
