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
#include "core/event/WindowEvent.hpp"
#include "math/Vector.hpp"
#include <glm/gtc/quaternion.hpp>
#include <numbers>

namespace nexo::system {
	void CameraContextSystem::update() const
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

	void PerspectiveCameraControllerSystem::update(const Timestep ts) const
    {
        auto &renderContext = coord->getSingletonComponent<components::RenderContext>();
        const unsigned int sceneRendered = renderContext.sceneRendered;
        if (sceneRendered == -1)
            return;

        const auto deltaTime = static_cast<float>(ts);

        for (const auto &entity : entities)
        {
	        constexpr float translationSpeed = 5.0f;
	        auto tag = coord->getComponent<components::SceneTag>(entity);
            if (!tag.isActive || sceneRendered != tag.id)
                continue;

            auto &cameraComponent = coord->getComponent<components::CameraComponent>(entity);
            cameraComponent.resizing = false;
            auto &transform = coord->getComponent<components::TransformComponent>(entity);

            glm::vec3 front = transform.quat * glm::vec3(0.0f, 0.0f, -1.0f);
            glm::vec3 up    = transform.quat * glm::vec3(0.0f, 1.0f,  0.0f);
            glm::vec3 right = transform.quat * glm::vec3(1.0f, 0.0f,  0.0f);

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
        auto const &renderContext = coord->getSingletonComponent<components::RenderContext>();
        const unsigned int sceneRendered = renderContext.sceneRendered;
        if (sceneRendered == -1)
            return;

        for (const auto &camera : entities)
        {
            auto tag = coord->getComponent<components::SceneTag>(camera);
            if (!tag.isActive || sceneRendered != tag.id)
                continue;
            constexpr float zoomSpeed = 0.5f;
            auto &transform = coord->getComponent<components::TransformComponent>(camera);
            glm::vec3 front = transform.quat * glm::vec3(0.0f, 0.0f, -1.0f);
            transform.pos += front * event.y * zoomSpeed;
            event.consumed = true;
        }
    }

    void PerspectiveCameraControllerSystem::handleEvent(event::EventMouseMove &event)
    {
        auto const &renderContext = coord->getSingletonComponent<components::RenderContext>();
        const unsigned int sceneRendered = renderContext.sceneRendered;

        if (sceneRendered == -1)
            return;

        glm::vec2 currentMousePosition(event.x, event.y);
        for (const auto &camera : entities)
        {
            auto &controller = coord->getComponent<components::PerspectiveCameraController>(camera);
            auto const &cameraComponent = coord->getComponent<components::CameraComponent>(camera);
            auto tag = coord->getComponent<components::SceneTag>(camera);
            const glm::vec2 mouseDelta = (currentMousePosition - controller.lastMousePosition) * controller.mouseSensitivity;
            controller.lastMousePosition = currentMousePosition;


            if (!tag.isActive || sceneRendered != tag.id || cameraComponent.resizing || !event::isMouseDown(NEXO_MOUSE_LEFT))
                continue;

            controller.yaw   += -mouseDelta.x;
            controller.pitch += -mouseDelta.y;

            // Clamp pitch to avoid flipping
            if (controller.pitch > 89.0f)
                controller.pitch = 89.0f;
            if (controller.pitch < -89.0f)
                controller.pitch = -89.0f;

            // Rebuild the quaternion from yaw and pitch.
            glm::quat qPitch = glm::angleAxis(glm::radians(controller.pitch), glm::vec3(1.0f, 0.0f, 0.0f));
            glm::quat qYaw   = glm::angleAxis(glm::radians(controller.yaw),   glm::vec3(0.0f, 1.0f, 0.0f));

            auto &transform = coord->getComponent<components::TransformComponent>(camera);
            transform.quat = glm::normalize(qYaw * qPitch);
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
		auto const &renderContext = coord->getSingletonComponent<components::RenderContext>();
        unsigned int sceneRendered = renderContext.sceneRendered;
        if (sceneRendered == -1)
            return;

        for (const auto &camera : entities)
        {
            auto tag = coord->getComponent<components::SceneTag>(camera);
            if (!tag.isActive || sceneRendered != tag.id)
                continue;
            constexpr float zoomSpeed = 0.5f;
            auto &target = coord->getComponent<components::PerspectiveCameraTarget>(camera);
            target.distance -= event.y * zoomSpeed;
            if(target.distance < 0.1f)
                target.distance = 0.1f;

            auto &transformCamera = coord->getComponent<components::TransformComponent>(camera);
            auto const &transformTarget = coord->getComponent<components::TransformComponent>(target.targetEntity);

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
	    auto const &renderContext = coord->getSingletonComponent<components::RenderContext>();
	    unsigned int sceneRendered = renderContext.sceneRendered;
	    if (sceneRendered == -1)
	        return;

	    glm::vec2 currentMousePosition(event.x, event.y);

	    for (const auto &entity : entities)
	    {
	        auto &targetComp = coord->getComponent<components::PerspectiveCameraTarget>(entity);
	        auto tag = coord->getComponent<components::SceneTag>(entity);
	        auto const &cameraComponent = coord->getComponent<components::CameraComponent>(entity);

	        if (!tag.isActive || sceneRendered != tag.id || cameraComponent.resizing)
	        {
	            targetComp.lastMousePosition = currentMousePosition;
	            continue;
	        }

	        if (!event::isMouseDown(NEXO_MOUSE_RIGHT))
	        {
	            targetComp.lastMousePosition = currentMousePosition;
	            continue;
	        }

	        auto &transformCamera = coord->getComponent<components::TransformComponent>(entity);
	        auto const &transformTarget = coord->getComponent<components::TransformComponent>(targetComp.targetEntity);

	        float deltaX = targetComp.lastMousePosition.x - currentMousePosition.x;
	        float deltaY = targetComp.lastMousePosition.y - currentMousePosition.y;

	        // Compute rotation angles based on screen dimensions.
	        float xAngle = deltaX * (2.0f * std::numbers::pi_v<float> / static_cast<float>(cameraComponent.width));
	        float yAngle = deltaY * (std::numbers::pi_v<float> / static_cast<float>(cameraComponent.height));

	        // Prevent excessive pitch rotation when the camera is nearly vertical.
	        glm::vec3 front = glm::normalize(transformTarget.pos - transformCamera.pos);
	        auto sgn = [](float x) { return (x >= 0.0f ? 1.0f : -1.0f); };
	        if (glm::dot(front, glm::vec3(0, 1, 0)) * sgn(yAngle) > 0.99f)
	            yAngle = 0.0f;

	        glm::vec3 offset = (transformCamera.pos - transformTarget.pos);

	        glm::quat qYaw = glm::angleAxis(xAngle, glm::vec3(0, 1, 0));

	        // For the pitch (vertical rotation), compute the right axis.
	        // This is the normalized cross product between the world up and the offset vector.
	        glm::vec3 rightAxis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), offset));
	        if (glm::length(rightAxis) < 0.001f)  // Fallback if the vector is degenerate.
	            rightAxis = glm::vec3(1, 0, 0);
	        glm::quat qPitch = glm::angleAxis(yAngle, rightAxis);

	        glm::quat incrementalRotation = qYaw * qPitch;

	        glm::vec3 newOffset = incrementalRotation * offset;

			newOffset = glm::normalize(newOffset) * targetComp.distance;

			transformCamera.pos = transformTarget.pos + newOffset;

	        glm::vec3 newFront = glm::normalize(transformTarget.pos - transformCamera.pos);
	        transformCamera.quat = glm::normalize(glm::quatLookAt(newFront, glm::vec3(0, 1, 0)));

	        targetComp.lastMousePosition = currentMousePosition;
			event.consumed = true;
	    }
	}
}
