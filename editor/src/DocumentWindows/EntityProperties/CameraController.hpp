//// CameraController.hpp /////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Header file for the camera controller property
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "AEntityProperty.hpp"

namespace nexo::editor {
	class CameraController : public AEntityProperty {
		public:
			using AEntityProperty::AEntityProperty;

			/**
			 * @brief Renders the camera controller UI.
			 *
			 * Retrieves the PerspectiveCameraController component from the specified entity and displays its settings,
			 * including a control for adjusting mouse sensitivity, using an ImGui layout.
			 *
			 * @param entity The entity that holds the camera controller component.
			 * @return true after processing the UI elements.
			 */
			bool show(ecs::Entity entity) override;
	};
}
