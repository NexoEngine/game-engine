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

#include "Access.hpp"
#include "GroupSystem.hpp"
#include "components/Camera.hpp"
#include "components/Render.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/Transform.hpp"

namespace nexo::system {

	/**
	* @brief System responsible for rendering the scene.
	*
	* The RenderSystem iterates over the active cameras stored in the RenderContext singleton,
	* sets up lighting uniforms using the sceneLights data, and then renders entities that have
	* a valid RenderComponent. The system binds each camera's render target, clears the buffers,
	* and then draws each renderable entity.
	*
	* @note Component Access Rights:
	*  - READ access to components::TransformComponent (owned)
	*  - READ access to components::RenderComponent (owned)
	*  - READ access to components::SceneTag (non-owned)
	*  - WRITE access to components::RenderContext (singleton)
	*
	* @note The system uses scene partitioning to only render entities belonging to the
	* currently active scene (identified by RenderContext.sceneRendered).
	*/
	class RenderSystem final : public ecs::GroupSystem<
		ecs::Owned<
			ecs::Read<components::TransformComponent>,
	        ecs::Read<components::RenderComponent>>,
        ecs::NonOwned<
        	ecs::Read<components::SceneTag>>,
    	ecs::WriteSingleton<components::RenderContext>> {
			public:
                RenderSystem();
                void update();

			private:
			    void setupLights(const std::shared_ptr<renderer::Shader>& shader, const components::LightContext& lightContext);
				void renderGrid(const components::CameraContext &camera, components::RenderContext &renderContext);
				void renderOutline(
				    components::RenderContext &renderContext,
					const components::CameraContext &camera,
					const components::RenderComponent &renderComponent,
					const components::TransformComponent &transformComponent
				);
                std::shared_ptr<renderer::VertexArray> m_fullscreenQuad;
                std::shared_ptr<renderer::Framebuffer> m_maskFramebuffer;
	};
}
