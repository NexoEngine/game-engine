//// CameraSystem.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/03/2025
//  Description: Header file for the camera systems
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/System.hpp"
#include "Timestep.hpp"
#include "core/event/Event.hpp"
#include "core/event/WindowEvent.hpp"

namespace nexo::system {
	/**
	* @brief System responsible for updating the camera context.
	*
	* This system iterates over all active camera entities and computes their view-projection
	* matrices using the CameraComponent and TransformComponent. The computed CameraContext is
	* then pushed into the RenderContext (a singleton component).
	*
	* @note Required Components on camera entities:
	*  - components::SceneTag
	*  - components::CameraComponent
	*  - components::TransformComponent
	*
	* @note Required Singleton Component:
	*  - components::RenderContext
	*/
	class CameraContextSystem : public ecs::System {
		public:
			void update();
	};

	/**
     * @brief System for controlling perspective cameras via keyboard and mouse input.
     *
     * This system handles movement of perspective cameras based on keyboard input (e.g. WASD,
     * space, tab) and adjusts camera orientation based on mouse movement. It also processes mouse
     * scroll events for zooming.
     *
     * @note Required Components on camera entities:
     *  - components::PerspectiveCameraController
     *  - components::SceneTag
     *  - components::CameraComponent
     *  - components::TransformComponent
     */
	class PerspectiveCameraControllerSystem : public ecs::System, LISTENS_TO(
        event::EventMouseScroll,
        event::EventMouseMove) {
		public:
			PerspectiveCameraControllerSystem();
			void update(const Timestep ts);

            void handleEvent(event::EventMouseScroll &event) override;
            void handleEvent(event::EventMouseMove &event) override;
	};

	/**
     * @brief System for controlling perspective cameras that orbit around a target.
     *
     * This system processes mouse scroll and mouse move events to adjust the camera’s distance
     * from its target entity as well as its orientation to always face the target. The camera's
     * position is updated accordingly.
     *
     * @note Required Components on camera entities:
     *  - components::PerspectiveCameraTarget
     *  - components::SceneTag
     *  - components::CameraComponent
     *  - components::TransformComponent
     */
	class PerspectiveCameraTargetSystem : public ecs::System, LISTENS_TO(
		event::EventMouseScroll,
        event::EventMouseMove) {
		public:
			PerspectiveCameraTargetSystem();
            void handleEvent(event::EventMouseMove &event) override;
            void handleEvent(event::EventMouseScroll &event) override;
	};
}
