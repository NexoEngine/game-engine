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
#include "components/Transform.hpp"
#include "core/event/Input.hpp"
#include "core/event/KeyCodes.hpp"
#include "Application.hpp"
#include "math/Vector.hpp"
#include <glm/gtc/quaternion.hpp>

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

	PerspectiveCameraControllerSystem::PerspectiveCameraControllerSystem()
	{
        Application::getInstance().getEventManager()->registerListener<event::EventMouseScroll>(this);
        Application::getInstance().getEventManager()->registerListener<event::EventMouseMove>(this);
	}

    void PerspectiveCameraControllerSystem::update(const Timestep ts)
    {
        auto &renderContext = coord->getSingletonComponent<components::RenderContext>();
        unsigned int sceneRendered = renderContext.sceneRendered;
        if (sceneRendered == -1)
            return;

        // Movement speed (units per second)
        const float translationSpeed = 5.0f;
        float deltaTime = static_cast<float>(ts);

        for (const auto &entity : entities)
        {
            auto tag = coord->getComponent<components::SceneTag>(entity);
            if (!tag.isActive || sceneRendered != tag.id)
                continue;
            auto &cameraComponent = coord->getComponent<components::CameraComponent>(entity);
            cameraComponent.resizing = false;
            auto &transform = coord->getComponent<components::TransformComponent>(entity);

   	        glm::vec3 front;
            glm::vec3 right;
            glm::vec3 up;
            math::extractCameraComponents(transform.rotation, front, right, up);

            if (event::isKeyPressed(NEXO_KEY_Z))
                transform.pos += front * translationSpeed * deltaTime; // Forward
            if (event::isKeyPressed(NEXO_KEY_S))
                transform.pos -= front * translationSpeed * deltaTime; // Backward
            if (event::isKeyPressed(NEXO_KEY_Q))
                transform.pos -= right * translationSpeed * deltaTime; // Left
            if (event::isKeyPressed(NEXO_KEY_D))
                transform.pos += right * translationSpeed * deltaTime; // Right
            if (event::isKeyPressed(NEXO_KEY_SPACE))
                transform.pos += up * translationSpeed * deltaTime;    // Up
            if (event::isKeyPressed(NEXO_KEY_TAB))
                transform.pos -= up * translationSpeed * deltaTime;    // Down
        }
    }

	void PerspectiveCameraControllerSystem::handleEvent(event::EventMouseScroll &event)
	{
		auto &renderContext = coord->getSingletonComponent<components::RenderContext>();
		unsigned int sceneRendered = renderContext.sceneRendered;
		if (sceneRendered == -1)
			return;

		for (const auto &camera : entities)
		{
			auto tag = coord->getComponent<components::SceneTag>(camera);
			if (!tag.isActive || sceneRendered != tag.id)
				continue;
			constexpr float zoomSpeed = 0.5f;
			auto& transform = coord->getComponent<components::TransformComponent>(camera);
			glm::vec3 front;
			glm::vec3 right;
			glm::vec3 up;
			math::extractCameraComponents(transform.rotation, front, right, up);
			transform.pos += front * event.y * zoomSpeed;
			event.consumed = true;
		}
	}

	void PerspectiveCameraControllerSystem::handleEvent(event::EventMouseMove &event)
	{
		auto &renderContext = coord->getSingletonComponent<components::RenderContext>();
		unsigned int sceneRendered = renderContext.sceneRendered;
		if (sceneRendered == -1)
			return;

		glm::vec2 currentMousePosition(event.x, event.y);
		for (const auto &camera : entities)
		{
			auto &controller = coord->getComponent<components::PerspectiveCameraController>(camera);
			auto &cameraComponent = coord->getComponent<components::CameraComponent>(camera);
			auto tag = coord->getComponent<components::SceneTag>(camera);
 			glm::vec2 mouseDelta = (currentMousePosition - controller.lastMousePosition) * controller.mouseSensitivity;
			controller.lastMousePosition = currentMousePosition;
			if (!tag.isActive || sceneRendered != tag.id || cameraComponent.resizing || !event::isMouseDown(NEXO_MOUSE_LEFT))
				continue;
      		auto& transform = coord->getComponent<components::TransformComponent>(camera);

	        // Update yaw and pitch based on the mouse delta.
	        transform.rotation.y += mouseDelta.x;   // Yaw update
	        transform.rotation.x += mouseDelta.y;   // Pitch update
			transform.rotation.x = glm::clamp(transform.rotation.x, -89.0f, 89.0f);
		}
	}
}
