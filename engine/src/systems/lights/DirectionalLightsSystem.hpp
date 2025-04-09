//// DirectionalLightsSystem.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Header file for the directional lights system
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/System.hpp"
#include "ecs/GroupSystem.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"

namespace nexo::system {

	/**
	* @brief System responsible for updating directional lights in the scene.
	*
	* This system iterates over all directional light entities in the active scene and updates
	* the RenderContext's sceneLights collection with their directional light components.
	*
	* @note Component Access Rights:
	*  - READ access to components::DirectionalLightComponent (owned)
	*  - READ access to components::SceneTag (non-owned)
	*  - WRITE access to components::RenderContext (singleton)
	*
	* @note The system uses scene partitioning to only process directional light entities
	* belonging to the currently active scene (identified by RenderContext.sceneRendered).
	*/
	class DirectionalLightsSystem final : public ecs::GroupSystem<
		ecs::Owned<
			ecs::Read<components::DirectionalLightComponent>>,
        ecs::NonOwned<
        	ecs::Read<components::SceneTag>>,
    	ecs::WriteSingleton<components::RenderContext>> {
		public:
			void update();
	};
}
