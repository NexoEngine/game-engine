//// SpotLightsSystem.cpp /////////////////////////////////////////////////////
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
#include "core/exceptions/Exceptions.hpp"
#include "Application.hpp"

namespace nexo::system {
	void SpotLightsSystem::update()
	{
		auto &renderContext = getSingleton<components::RenderContext>();
		if (renderContext.sceneRendered == -1)
			return;

		const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

		const auto scenePartition = m_group->getPartitionView<components::SceneTag, unsigned int>(
			[](const components::SceneTag& tag) { return tag.id; }
		);

		const auto *partition = scenePartition.getPartition(sceneRendered);

		auto &app = Application::getInstance();
        const std::string &sceneName = app.getSceneManager().getScene(sceneRendered).getName();
		if (!partition) {
            LOG_ONCE(NEXO_WARN, "No spot light found in scene {}, skipping", sceneName);
            return;
        }
        nexo::Logger::resetOnce(NEXO_LOG_ONCE_KEY("No spot light found in scene {}, skipping", sceneName));

		if (partition->count > MAX_SPOT_LIGHTS)
		    THROW_EXCEPTION(core::TooManySpotLightsException, sceneRendered, partition->count);

		const std::span<const ecs::Entity> entitySpan = m_group->entities();

		for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i)
		{
			renderContext.sceneLights.spotLights[renderContext.sceneLights.spotLightCount] = entitySpan[i];
			renderContext.sceneLights.spotLightCount++;
		}
	}
}
