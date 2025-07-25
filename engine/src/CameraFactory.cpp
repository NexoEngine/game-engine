//// CameraFactory.cpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Source file for the camera factory static class
//
///////////////////////////////////////////////////////////////////////////////

#include "CameraFactory.hpp"

#include <utility>
#include "Application.hpp"
#include "components/Transform.hpp"
#include "components/Camera.hpp"
#include "components/Uuid.hpp"
#include "renderPasses/ForwardPass.hpp"

namespace nexo {
	ecs::Entity CameraFactory::createPerspectiveCamera(glm::vec3 pos, unsigned int width,
       									               unsigned int height, std::shared_ptr<renderer::NxFramebuffer> renderTarget,
                             				           const glm::vec4 &clearColor, float fov, float nearPlane, float farPlane)
	{
		components::TransformComponent transform{};
		transform.pos = pos;

		components::CameraComponent camera{};
		camera.width = width;
		camera.height = height;
		camera.fov = fov;
		camera.nearPlane = nearPlane;
		camera.farPlane = farPlane;
		camera.type = components::CameraType::PERSPECTIVE;

		auto forwardPass = std::make_shared<renderer::ForwardPass>(width, height);
		renderer::PassId forwardPassId = camera.pipeline.addRenderPass(forwardPass);
		camera.pipeline.setFinalOutputPass(forwardPassId);
		camera.pipeline.setCameraClearColor(clearColor);
		if (renderTarget) {
		    camera.m_renderTarget = std::move(renderTarget);
			camera.pipeline.setFinalRenderTarget(camera.m_renderTarget);
		}
		camera.clearColor = clearColor;

		ecs::Entity newCamera = Application::m_coordinator->createEntity();
		Application::m_coordinator->addComponent<components::TransformComponent>(newCamera, transform);
		Application::m_coordinator->addComponent<components::CameraComponent>(newCamera, camera);
		components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newCamera, uuid);
		return newCamera;
	}
}
