//// SpotLightsSystem.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Source file for the spot lights system
//
///////////////////////////////////////////////////////////////////////////////

#include "SpotLightsSystem.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "ecs/Coordinator.hpp"


namespace nexo::system {
	void SpotLightsSystem::update() const
	{
		auto &renderContext = coord->getSingletonComponent<components::RenderContext>();
		const unsigned int sceneRendered = renderContext.sceneRendered;
		if (sceneRendered == -1)
			return;

		for (const auto &spotLights : entities)
		{
			auto tag = coord->getComponent<components::SceneTag>(spotLights);
			if (!tag.isRendered || sceneRendered != tag.id)
				continue;
			const auto &spotComponent = coord->getComponent<components::SpotLightComponent>(spotLights);
			renderContext.sceneLights.spotLights[renderContext.sceneLights.spotLightCount++] = spotComponent;
		}
	}
}
