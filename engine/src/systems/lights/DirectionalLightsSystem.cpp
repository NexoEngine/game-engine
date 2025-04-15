//// DirectionalLights.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Source file for the directional lights system
//
///////////////////////////////////////////////////////////////////////////////

#include "DirectionalLightsSystem.hpp"
#include "components/Light.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "core/exceptions/Exceptions.hpp"
#include "ecs/Coordinator.hpp"

namespace nexo::system {
	void DirectionalLightsSystem::update()
	{
		auto &renderContext = getSingleton<components::RenderContext>();
		if (renderContext.sceneRendered == -1)
			return;

		const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

		const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
			[](const components::SceneTag& tag) { return tag.id; }
		);

		const auto *partition = scenePartition.getPartition(sceneRendered);

		//TODO: Throw exception here ?
		if (!partition)
			return;

		if (partition->count > MAX_DIRECTIONAL_LIGHTS)
		    THROW_EXCEPTION(core::TooManyDirectionalLightsException, sceneRendered, partition->count);

		const auto directionalLightSpan = get<components::DirectionalLightComponent>();

		for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i)
		{
			renderContext.sceneLights.directionalLights[renderContext.sceneLights.directionalLightCount] = directionalLightSpan[i];
			renderContext.sceneLights.directionalLightCount++;
		}
	}
}
