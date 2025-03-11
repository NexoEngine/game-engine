//// AmbientLightSystem.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Source file for the ambient light system
//
///////////////////////////////////////////////////////////////////////////////

#include "AmbientLightSystem.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "ecs/Coordinator.hpp"

namespace nexo::system {
	void AmbientLightSystem::update()
	{
		auto &renderContext = coord->getSingletonComponent<components::RenderContext>();
		unsigned int sceneRendered = renderContext.sceneRendered;
		if (sceneRendered == -1)
			return;

		for (const auto &ambientLights : entities)
		{
			auto tag = coord->getComponent<components::SceneTag>(ambientLights);
			if (!tag.isRendered || sceneRendered != tag.id)
				continue;
			auto &ambientComponent = coord->getComponent<components::AmbientLightComponent>(ambientLights);
			renderContext.sceneLights.ambientLight = ambientComponent.color;
			break;
		}
	}
}
