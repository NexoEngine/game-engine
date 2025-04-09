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

#include "components/Light.hpp"
#include "ecs/Coordinator.hpp"

namespace nexo::system {
	void AmbientLightSystem::update()
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

		const auto ambientSpan = get<components::AmbientLightComponent>();
		if (ambientSpan.size())
			renderContext.sceneLights.ambientLight = ambientSpan[0].color;

	}
}
