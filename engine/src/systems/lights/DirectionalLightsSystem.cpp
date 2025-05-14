//// DirectionalLights.cpp ////////////////////////////////////////////////////
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
#include "Application.hpp"

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

        auto &app = Application::getInstance();
        const std::string &sceneName = app.getSceneManager().getScene(sceneRendered).getName();
		if (!partition) {
            LOG_ONCE(NEXO_WARN, "No directional light found in scene {}, skipping", sceneName);
            return;
        }
        nexo::Logger::resetOnce(NEXO_LOG_ONCE_KEY("No directional light found in scene {}, skipping", sceneName));

        if (partition->count != 1)
            LOG_ONCE(NEXO_WARN, "For scene {}, found {} directional lights, only one is supported, picking the first one", sceneName, partition->count);
        else
            nexo::Logger::resetOnce(NEXO_LOG_ONCE_KEY("For scene {}, found {} directional lights, only one is supported, picking the first one", sceneName, partition->count));

        const auto &lights = get<components::DirectionalLightComponent>();  // now 'lights' names the container
        const auto &dirLight = lights[0];
		renderContext.sceneLights.dirLight = dirLight;
	}
}
