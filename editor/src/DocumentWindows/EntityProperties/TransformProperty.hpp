//// TransformProperty.hpp ////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        20/03/2025
//  Description: Header file for the transform property inspector
//
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "AEntityProperty.hpp"

namespace nexo::editor {
    class TransformProperty final : public AEntityProperty {
        public:
        	using AEntityProperty::AEntityProperty;

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
    };
}
