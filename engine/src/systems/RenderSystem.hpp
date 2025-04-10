//// RenderSystem.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/03/2025
//  Description: Header file for the render system
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/System.hpp"

namespace nexo::system {

	/**
     * @brief System responsible for rendering the scene.
     *
     * The RenderSystem iterates over the active cameras stored in the RenderContext singleton,
     * sets up lighting uniforms using the sceneLights data, and then renders entities that have
     * a valid RenderComponent. The system binds each camera's render target, clears the buffers,
     * and then draws each renderable entity.
     *
     * @note Required Singleton Component:
     *  - components::RenderContext
     *
     * @note Required Components on renderable entities:
     *  - components::SceneTag
     *  - components::CameraComponent (for camera entities stored in the RenderContext)
     *  - components::TransformComponent
     *  - components::RenderComponent
     */
	class RenderSystem : public ecs::System {
		public:
			void update() const;
	};
}
