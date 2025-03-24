//// PointLightProperty.hpp ///////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Header file for the point light property
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "AEntityProperty.hpp"

namespace nexo::editor {
	class PointLightProperty : public AEntityProperty {
		public:
			using AEntityProperty::AEntityProperty;

			/**
			 * @brief Displays and edits the point light properties of a given entity.
			 *
			 * Retrieves the PointLightComponent from the specified entity and renders a UI for editing its properties.
			 * The interface allows for color adjustment, position modification via a draggable table, and max distance tuning
			 * using a slider, which in turn updates the light's attenuation coefficients.
			 *
			 * @param entity The entity owning the PointLightComponent to be modified.
			 * @return true after successfully displaying the properties.
			 */
			bool show(ecs::Entity entity) override;
	};
}
