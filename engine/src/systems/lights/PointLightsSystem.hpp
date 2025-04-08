//// PointLightsSystem.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Header file for the point lights system
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/System.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "ecs/GroupSystem.hpp"

namespace nexo::system {

	/**
     * @brief System responsible for updating point lights in the scene.
     *
     * This system iterates over all point light entities and updates the RenderContext with
     * their point light components.
     *
     * @note Required Components on point light entities:
     *  - components::SceneTag
     *  - components::PointLightComponent
     *
     * @note Required Singleton Component:
     *  - components::RenderContext
     */
	class PointLightsSystem : public ecs::GroupSystem<
		ecs::Owned<
			ecs::Read<components::PointLightComponent>>,
        ecs::NonOwned<
        	ecs::Read<components::SceneTag>>,
    	ecs::WriteSingleton<components::RenderContext>> {
		public:
			void update();
	};
}
