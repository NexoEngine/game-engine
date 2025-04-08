//// AmbientLightSystem.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Header file for the ambient light system
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "GroupSystem.hpp"
#include "ecs/System.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"

namespace nexo::system {

	/**
     * @brief System responsible for updating ambient light data in the scene.
     *
     * This system iterates over ambient light entities and updates the global ambient light
     * in the RenderContext with the first valid ambient light component it finds.
     *
     * @note Required Components on ambient light entities:
     *  - components::SceneTag
     *  - components::AmbientLightComponent
     *
     * @note Required Singleton Component:
     *  - components::RenderContext
     */
	class AmbientLightSystem : public ecs::GroupSystem<
		ecs::Owned<
			ecs::Read<components::AmbientLightComponent>>,
        ecs::NonOwned<
        	ecs::Read<components::SceneTag>>,
    	ecs::WriteSingleton<components::RenderContext>> {
			public:
				void update();
	};
}
