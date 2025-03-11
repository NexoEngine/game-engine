//// CameraSystem.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/03/2025
//  Description: Source file for the camera systems
//
///////////////////////////////////////////////////////////////////////////////

#include "CameraSystem.hpp"
#include "components/RenderContext.hpp"
#include "components/SceneComponents.hpp"
#include "components/Camera.hpp"
#include "renderer/RenderCommand.hpp"
#include "ecs/Coordinator.hpp"

namespace nexo::system {
	void CameraContextSystem::update()
	{
		auto &renderContext = coord->getSingletonComponent<components::RenderContext>();
		unsigned int sceneRendered = renderContext.sceneRendered;
		if (sceneRendered == -1)
			return;

		for (const auto camera : entities)
		{
			auto tag = coord->getComponent<components::SceneTag>(camera);
			if (!tag.isRendered || sceneRendered != tag.id)
				continue;

			auto cameraComponent = coord->getComponent<components::CameraComponent>(camera);
			auto transformComponent = coord->getComponent<components::TransformComponent>(camera);
			glm::mat4 projectionMatrix = cameraComponent.getProjectionMatrix();
			glm::mat4 viewMatrix = cameraComponent.getViewMatrix(transformComponent);
			glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
			components::CameraContext context{viewProjectionMatrix, transformComponent.pos, cameraComponent.clearColor, cameraComponent.m_renderTarget};
			renderContext.cameras.push(context);
		}
	}
}
