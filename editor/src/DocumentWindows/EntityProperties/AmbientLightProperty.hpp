//// AmbientLightProperty.hpp /////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Header file for the ambient light property class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "AEntityProperty.hpp"

namespace nexo::editor {
	class AmbientLightProperty final : public AEntityProperty {
		public:
			using AEntityProperty::AEntityProperty;

			/**
			 * @brief Renders the ambient light properties UI for the specified entity.
			 *
			 * Retrieves the ambient light component associated with the entity and displays
			 * a header along with a color editor widget. When the header is open, the widget
			 * allows the user to modify the ambient light color, which is then applied to the component.
			 *
			 * @param entity The ECS entity whose ambient light properties are to be displayed.
			 */
			void show(ecs::Entity entity) override;
	};
}
