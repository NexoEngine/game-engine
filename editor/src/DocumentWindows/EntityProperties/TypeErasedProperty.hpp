//// TypeErasedProperty.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
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
			* @brief Displays and edits the transform properties of an entity using an ImGui interface.
			*
			* Retrieves the transform component (position, scale, and rotation quaternion) of the given entity,
			* displaying the values in an ImGui table. The rotation is converted from a quaternion to Euler angles
			* to allow intuitive editing; any changes in Euler angles are applied incrementally back to the quaternion,
			* ensuring it remains normalized.
			*
			* @param entity The entity whose transform properties are rendered.
			*/
            void show(ecs::Entity entity) override;

        private:
            const ecs::ComponentType m_componentType; // Type of the component being displayed
            const std::shared_ptr<ecs::ComponentDescription> m_description; // Description of the component being displayed
    };

}
