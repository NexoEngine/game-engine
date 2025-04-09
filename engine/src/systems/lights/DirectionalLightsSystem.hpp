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
     * This system iterates over all directional light entities and updates the RenderContext with the
     * directional light components from those entities.
     *
     * @note Required Components on directional light entities:
     *  - components::SceneTag
     *  - components::DirectionalLightComponent
     *
     * @note Required Singleton Component:
     *  - components::RenderContext
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
