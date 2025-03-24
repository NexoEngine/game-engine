//// DirectionalLightProperty.hpp /////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Header file for the directional light property
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "AEntityProperty.hpp"

namespace nexo::editor {
	class DirectionalLightProperty : public AEntityProperty {
		public:
			using AEntityProperty::AEntityProperty;

			/**
			 * @brief Displays and enables editing of an entity's directional light properties.
			 *
			 * Retrieves the directional light component from the specified entity and, if the header is expanded,
			 * renders a GUI section that includes controls for modifying the light's color and direction. The color
			 * editor uses a customizable picker while the direction is adjusted via draggable float inputs arranged in a table.
			 *
			 * @param entity The entity whose directional light properties are to be displayed.
			 * @return true if the directional light properties section is (or has been) rendered.
			 */
			bool show(ecs::Entity entity) override;
	};
}
