//// AmbientLightSystem.cpp ///////////////////////////////////////////////////
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

#include "Logger.hpp"
#include "Application.hpp"
#include "components/Light.hpp"

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

        auto &app = Application::getInstance();
        const std::string &sceneName = app.getSceneManager().getScene(sceneRendered).getName();
		if (!partition) {
            LOG_ONCE(NEXO_WARN, "No ambient light found in scene {}, skipping", sceneName);
            return;
        }
        Logger::resetOnce(NEXO_LOG_ONCE_KEY("No ambient light found in scene {}, skipping", sceneName));

        if (partition->count != 1)
            LOG_ONCE(NEXO_WARN, "For scene {}, found {} ambient lights, only one is supported, picking the first one", sceneName, partition->count);
        else
            Logger::resetOnce(NEXO_LOG_ONCE_KEY("For scene {}, found {} ambient lights, only one is supported, picking the first one", sceneName, partition->count));

        renderContext.sceneLights.ambientLight = get<components::AmbientLightComponent>()[0].color;
	}
}
