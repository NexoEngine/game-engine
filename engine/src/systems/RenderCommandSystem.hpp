//// RenderCommandSystem.hpp //////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        13/05/2025
//  Description: Header file for the render command system
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "Access.hpp"
#include "DrawCommand.hpp"
#include "GroupSystem.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/MaterialComponent.hpp"
#include "components/StaticMesh.hpp"
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
	class RenderCommandSystem final : public ecs::GroupSystem<
		ecs::Owned<
			ecs::Read<components::TransformComponent>,
	        ecs::Read<components::StaticMeshComponent>,
			ecs::Read<components::MaterialComponent>>,
        ecs::NonOwned<
        	ecs::Read<components::SceneTag>>,
    	ecs::WriteSingleton<components::RenderContext>> {
			public:
                void update();

			private:
			    static void setupLights(renderer::DrawCommand &cmd, const components::LightContext& lightContext);
	};
}
