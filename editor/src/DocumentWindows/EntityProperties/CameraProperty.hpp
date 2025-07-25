//// CameraProperty.hpp ///////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        17/03/2025
//  Description: Header file for the camera property class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "AEntityProperty.hpp"

namespace nexo::editor {
	class CameraProperty final : public AEntityProperty {
		public:
			using AEntityProperty::AEntityProperty;

			/**
			 * @brief Displays and updates the camera properties UI.
			 *
			 * Retrieves the camera component from the given ECS entity and renders a user interface for adjusting
			 * camera settings. The UI includes controls for modifying viewport dimensions (with a lock toggle), field of view,
			 * near plane, far plane, and the clear color using ImGui.
			 *
			 * @param entity The entity containing the camera component.
			 */
			void show(ecs::Entity entity) override;
	};
}
