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
	class CameraContextSystem : public ecs::System {
		public:
			void update();
	};

	class PerspectiveCameraControllerSystem : public ecs::System, LISTENS_TO(
        event::EventMouseScroll,
        event::EventMouseMove) {
		public:
			PerspectiveCameraControllerSystem();
			void update(const Timestep ts);

            void handleEvent(event::EventMouseScroll &event) override;
            void handleEvent(event::EventMouseMove &event) override;
	};

	class PerspectiveCameraTargetSystem : public ecs::System, LISTENS_TO(
		event::EventMouseScroll,
        event::EventMouseMove) {
		public:
			PerspectiveCameraTargetSystem();
            void handleEvent(event::EventMouseMove &event) override;
            void handleEvent(event::EventMouseScroll &event) override;
	};
}
