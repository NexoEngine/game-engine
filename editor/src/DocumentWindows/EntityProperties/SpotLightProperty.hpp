//// SpotLightProperty.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Header file for the spot light property class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "AEntityProperty.hpp"

namespace nexo::editor {
	class SpotLightProperty : public AEntityProperty {
		public:
			using AEntityProperty::AEntityProperty;

			/**
			 * @brief Renders the spotlight properties editor for the specified entity.
			 *
			 * Retrieves the spotlight component from the given entity and displays a user interface
			 * for editing its properties, including color, direction, position, and cutoff parameters.
			 * Adjusting the maximum distance automatically recalculates the linear and quadratic
			 * attenuation factors, while the cutoff angles are presented in degrees for easier modification.
			 *
			 * @param entity The entity whose spotlight component will be edited.
			 */
			void show(ecs::Entity entity) override;
	};
}
