///////////////////////////////////////////////////////////////////////////////
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
#include "core/event/WindowEvent.hpp"
#include <glm/gtc/quaternion.hpp>
#include <numbers>

namespace nexo::system {

	void CameraContextSystem::update()
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
            LOG_ONCE(NEXO_WARN, "No camera found in scene {}, skipping", sceneName);
            return;
        }
        nexo::Logger::resetOnce(NEXO_LOG_ONCE_KEY("No camera found in scene {}, skipping", sceneName));

		const auto cameraSpan = get<components::CameraComponent>();
		const auto transformComponentArray = get<components::TransformComponent>();
		const auto entitySpan = m_group->entities();
		renderContext.cameras.reserve(partition->count);

		for (size_t i = partition->startIndex; i < partition->startIndex + partition->count; ++i)
		{
			const auto &cameraComponent = cameraSpan[i];
			if (!cameraComponent.render)
				continue;
			const auto &transformComponent = transformComponentArray->get(entitySpan[i]);
			glm::mat4 projectionMatrix = cameraComponent.getProjectionMatrix();
			glm::mat4 viewMatrix = cameraComponent.getViewMatrix(transformComponent);
			const glm::mat4 viewProjectionMatrix = projectionMatrix * viewMatrix;
			components::CameraContext context{viewProjectionMatrix, transformComponent.pos, cameraComponent.clearColor, cameraComponent.m_renderTarget, cameraComponent.pipeline};
			renderContext.cameras.push_back(context);
		}
	}

	PerspectiveCameraControllerSystem::PerspectiveCameraControllerSystem()
	{
		Application::getInstance().getEventManager()->registerListener<event::EventMouseScroll>(this);
		Application::getInstance().getEventManager()->registerListener<event::EventMouseMove>(this);
	}

	void PerspectiveCameraControllerSystem::update(const Timestep ts)
	{
		const auto &renderContext = getSingleton<components::RenderContext>();
		if (renderContext.sceneRendered == -1)
			return;

		const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);
		const auto deltaTime = static_cast<float>(ts);

	    for (const ecs::Entity entity : entities)
		{
			auto &sceneTag = getComponent<components::SceneTag>(entity);
			if (!sceneTag.isActive || sceneTag.id != sceneRendered)
				continue;
			auto &cameraComponent = getComponent<components::CameraComponent>(entity);
			if (!cameraComponent.active)
				continue;
			auto &transform = getComponent<components::TransformComponent>(entity);
			auto &cameraController = getComponent<components::PerspectiveCameraController>(entity);

			cameraComponent.resizing = false;

            if (event::isKeyPressed(NEXO_KEY_SHIFT))
                cameraController.translationSpeed = 10.0f;
            if (event::isKeyReleased(NEXO_KEY_SHIFT))
                cameraController.translationSpeed = 5.0f;

			glm::vec3 front = transform.quat * glm::vec3(0.0f, 0.0f, -1.0f);
			glm::vec3 up    = transform.quat * glm::vec3(0.0f, 1.0f,  0.0f);
			glm::vec3 right = transform.quat * glm::vec3(1.0f, 0.0f,  0.0f);

			if (event::isKeyPressed(NEXO_KEY_Z))
				transform.pos += front * cameraController.translationSpeed * deltaTime; // Forward
			if (event::isKeyPressed(NEXO_KEY_S))
				transform.pos -= front * cameraController.translationSpeed * deltaTime; // Backward
			if (event::isKeyPressed(NEXO_KEY_Q))
				transform.pos -= right * cameraController.translationSpeed * deltaTime; // Left
			if (event::isKeyPressed(NEXO_KEY_D))
				transform.pos += right * cameraController.translationSpeed * deltaTime; // Right
			if (event::isKeyPressed(NEXO_KEY_SPACE))
				transform.pos += up * cameraController.translationSpeed * deltaTime;    // Up
			if (event::isKeyPressed(NEXO_KEY_TAB))
				transform.pos -= up * cameraController.translationSpeed * deltaTime;    // Down
		}
	}

	void PerspectiveCameraControllerSystem::handleEvent(event::EventMouseScroll &event)
	{
		const auto &renderContext = getSingleton<components::RenderContext>();
		if (renderContext.sceneRendered == -1)
			return;

		const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

		for (const ecs::Entity entity : entities)
		{
			constexpr float zoomSpeed = 0.5f;
			auto &sceneTag = getComponent<components::SceneTag>(entity);
			const auto &cameraComponent = getComponent<components::CameraComponent>(entity);
			if (!sceneTag.isActive || sceneTag.id != sceneRendered || !cameraComponent.active)
				continue;
			auto &transform = getComponent<components::TransformComponent>(entity);
			glm::vec3 front = transform.quat * glm::vec3(0.0f, 0.0f, -1.0f);
			transform.pos += front * event.y * zoomSpeed;
			event.consumed = true;
		}
	}

	void PerspectiveCameraControllerSystem::handleEvent(event::EventMouseMove &event)
	{
        auto const &renderContext = getSingleton<components::RenderContext>();
        if (renderContext.sceneRendered == -1)
            return;

        const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);
        const glm::vec2 currentMousePosition(event.x, event.y);

        for (const ecs::Entity entity : entities)
        {
            auto &controller = getComponent<components::PerspectiveCameraController>(entity);
            const auto &sceneTag = getComponent<components::SceneTag>(entity);
            const auto &cameraComponent = getComponent<components::CameraComponent>(entity);
            const bool isActiveScene = sceneTag.isActive && sceneTag.id == sceneRendered;
            const bool isActiveCamera = isActiveScene && cameraComponent.active;
            const bool mouseDown = event::isMouseDown(NEXO_MOUSE_LEFT);

            // Check for scene transition - if the camera wasn't active before but is now
            const bool sceneTransition = isActiveCamera && !controller.wasActiveLastFrame;
            controller.wasActiveLastFrame = isActiveCamera;

            // Reset position on scene transition to prevent abrupt rotation
            if (sceneTransition) {
                controller.lastMousePosition = currentMousePosition;
                controller.wasMouseReleased = true;
                continue;
            }

            if (!isActiveCamera)
                continue;

            // Always update lastMousePosition if this is the active scene, even if not moving the camera
            // This ensures the position is current when we start dragging
            if (!mouseDown || controller.wasMouseReleased) {
                controller.lastMousePosition = currentMousePosition;
                controller.wasMouseReleased = false;
                continue;
            }

            if (cameraComponent.resizing) {
                controller.lastMousePosition = currentMousePosition;
                continue;
            }

            auto &transform = getComponent<components::TransformComponent>(entity);
            const glm::vec2 mouseDelta = (currentMousePosition - controller.lastMousePosition) * controller.mouseSensitivity;

            // Extract camera orientation vectors from current quaternion
            glm::vec3 right = transform.quat * glm::vec3(1.0f, 0.0f, 0.0f);


            // Create rotation quaternions based on mouse movement
            glm::quat pitchRotation = glm::angleAxis(glm::radians(-mouseDelta.y), right);
            glm::quat yawRotation = glm::angleAxis(glm::radians(-mouseDelta.x), glm::vec3(0.0f, 1.0f, 0.0f)); // World up for yaw
            glm::quat newQuat = glm::normalize(yawRotation * pitchRotation * transform.quat);
            const glm::vec3 newFront = newQuat * glm::vec3(0.0f, 0.0f, -1.0f);

            // Check if the resulting orientation would flip the camera (pitch constraint)
            const float pitchAngle = glm::degrees(std::asin(newFront.y));
            if (pitchAngle < -85.0f || pitchAngle > 85.0f)
                transform.quat = glm::normalize(yawRotation * transform.quat);
            else
                transform.quat = newQuat;

            controller.lastMousePosition = currentMousePosition;
            event.consumed = true;
        }
	}

	PerspectiveCameraTargetSystem::PerspectiveCameraTargetSystem()
	{
		Application::getInstance().getEventManager()->registerListener<event::EventMouseMove>(this);
		Application::getInstance().getEventManager()->registerListener<event::EventMouseScroll>(this);
	}

	void PerspectiveCameraTargetSystem::handleEvent(event::EventMouseScroll &event)
	{
		auto const &renderContext = getSingleton<components::RenderContext>();
		if (renderContext.sceneRendered == -1)
			return;

		const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

		for (const ecs::Entity entity : entities)
		{
			constexpr float zoomSpeed = 0.5f;
			auto &tag = getComponent<components::SceneTag>(entity);
			const auto &cameraComponent = getComponent<components::CameraComponent>(entity);
			if (!tag.isActive || sceneRendered != tag.id || !cameraComponent.active)
				continue;
			auto &target = getComponent<components::PerspectiveCameraTarget>(entity);
			target.distance -= event.y * zoomSpeed;
			if (target.distance < 0.1f)
				target.distance = 0.1f;

			auto &transformCamera = getComponent<components::TransformComponent>(entity);
			const auto &transformTarget = getComponent<components::TransformComponent>(target.targetEntity);

			glm::vec3 offset = transformCamera.pos - transformTarget.pos;
			// If offset is near zero, choose a default direction.
			if(glm::length(offset) < 0.001f)
				offset = glm::vec3(0, 0, 1);

			offset = glm::normalize(offset) * target.distance;

			transformCamera.pos = transformTarget.pos + offset;

			glm::vec3 newFront = glm::normalize(transformTarget.pos - transformCamera.pos);
			transformCamera.quat = glm::normalize(glm::quatLookAt(newFront, glm::vec3(0,1,0)));

			event.consumed = true;
        }
	}

	void PerspectiveCameraTargetSystem::handleEvent(event::EventMouseMove &event)
	{
		const auto &renderContext = getSingleton<components::RenderContext>();
		if (renderContext.sceneRendered == -1)
			return;

		const auto sceneRendered = static_cast<unsigned int>(renderContext.sceneRendered);

		glm::vec2 currentMousePosition(event.x, event.y);

		for (const ecs::Entity entity : entities)
		{
			const auto &sceneTag = getComponent<components::SceneTag>(entity);
			const auto &cameraComponent = getComponent<components::CameraComponent>(entity);
			auto &targetComponent = getComponent<components::PerspectiveCameraTarget>(entity);
			if (!sceneTag.isActive || sceneTag.id != sceneRendered || cameraComponent.resizing || !event::isMouseDown(NEXO_MOUSE_RIGHT) || !cameraComponent.active)
			{
				targetComponent.lastMousePosition = currentMousePosition;
				continue;
			}

			auto &transformCameraComponent = getComponent<components::TransformComponent>(entity);
			const auto &transformTargetComponent = getComponent<components::TransformComponent>(targetComponent.targetEntity);

			float deltaX = targetComponent.lastMousePosition.x - currentMousePosition.x;
			float deltaY = targetComponent.lastMousePosition.y - currentMousePosition.y;

			// Compute rotation angles based on screen dimensions.
			float xAngle = deltaX * (2.0f * std::numbers::pi_v<float> / static_cast<float>(cameraComponent.width));
			float yAngle = deltaY * (std::numbers::pi_v<float> / static_cast<float>(cameraComponent.height));

			// Prevent excessive pitch rotation when the camera is nearly vertical.
			glm::vec3 front = glm::normalize(transformTargetComponent.pos - transformCameraComponent.pos);
			auto sgn = [](const float x) { return (x >= 0.0f ? 1.0f : -1.0f); };
			if (glm::dot(front, glm::vec3(0, 1, 0)) * sgn(yAngle) > 0.99f)
				yAngle = 0.0f;

			glm::vec3 offset = (transformCameraComponent.pos - transformTargetComponent.pos);

			glm::quat qYaw = glm::angleAxis(xAngle, glm::vec3(0, 1, 0));

			// For the pitch (vertical rotation), compute the right axis.
			// This is the normalized cross product between the world up and the offset vector.
			glm::vec3 rightAxis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), offset));
			if (glm::length(rightAxis) < 0.001f)  // Fallback if the vector is degenerate.
				rightAxis = glm::vec3(1, 0, 0);
			glm::quat qPitch = glm::angleAxis(yAngle, rightAxis);

			glm::quat incrementalRotation = qYaw * qPitch;

			glm::vec3 newOffset = incrementalRotation * offset;

			newOffset = glm::normalize(newOffset) * targetComponent.distance;

			transformCameraComponent.pos = transformTargetComponent.pos + newOffset;

			glm::vec3 newFront = glm::normalize(transformTargetComponent.pos - transformCameraComponent.pos);
			transformCameraComponent.quat = glm::normalize(glm::quatLookAt(newFront, glm::vec3(0, 1, 0)));

			targetComponent.lastMousePosition = currentMousePosition;
			event.consumed = true;
		}
	}
}
