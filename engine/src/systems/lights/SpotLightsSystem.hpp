//// SpotLightsSystem.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Header file for the spot lights system
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GroupSystem.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"

namespace nexo::system {

	/**
     * @brief System responsible for updating spot lights in the scene.
     *
     * This system iterates over all spot light entities and updates the RenderContext with their
     * spot light components.
     *
     * @note Required Components on spot light entities:
     *  - components::SceneTag
     *  - components::SpotLightComponent
     *
     * @note Required Singleton Component:
     *  - components::RenderContext
     */
	class SpotLightsSystem final : public ecs::GroupSystem<
		ecs::Owned<
			ecs::Read<components::SpotLightComponent>>,
        ecs::NonOwned<
        	ecs::Read<components::SceneTag>>,
    	ecs::WriteSingleton<components::RenderContext>> {
		public:
			void update();
	};
}
