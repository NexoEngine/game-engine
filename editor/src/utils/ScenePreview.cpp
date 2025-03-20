//// ScenePreview.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        19/03/2025
//  Description: Source file for the scene preview utils
//
///////////////////////////////////////////////////////////////////////////////

#include "ScenePreview.hpp"
#include "Nexo.hpp"
#include "CameraFactory.hpp"
#include "LightFactory.hpp"
#include "components/Camera.hpp"

namespace nexo::editor::utils {
	float computeBoundingSphereRadius(const components::TransformComponent &objectTransform)
	{
	    // Assuming objectTransform.size contains the dimensions,
	    // a simple approximation is to take half the maximum dimension.
	    float halfX = objectTransform.size.x * 0.5f;
	    float halfY = objectTransform.size.y * 0.5f;
	    float halfZ = objectTransform.size.z * 0.5f;
	    return glm::max(halfX, glm::max(halfY, halfZ));
	}

	float computeSpotlightHalfAngle(const components::TransformComponent &objectTransform, const glm::vec3 &lightPosition)
	{
	    float radius = computeBoundingSphereRadius(objectTransform);
	    float distance = glm::length(objectTransform.pos - lightPosition);
	    // Prevent division by zero
	    if (distance < 0.001f)
	        return glm::radians(15.0f); // default value if too close
	    return atan(radius / distance);
	}

	static ecs::Entity copyEntity(ecs::Entity entity)
	{
		auto &app = getApp();

		ecs::Entity entityCopy = app.m_coordinator->createEntity();
        auto renderComponentCopy = app.m_coordinator->getComponent<components::RenderComponent>(entity).clone();
        auto &transformComponentBase = app.m_coordinator->getComponent<components::TransformComponent>(entity);
        components::TransformComponent transformComponent;
        transformComponent.pos = {0.0f, 0.0f, -transformComponentBase.size.z * 2.0f};
        transformComponent.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        transformComponent.size = transformComponentBase.size;
        app.m_coordinator->addComponent<components::RenderComponent>(entityCopy, renderComponentCopy);
        app.m_coordinator->addComponent<components::TransformComponent>(entityCopy, transformComponent);
        return entityCopy;
	}

	static ecs::Entity createPreviewCamera(scene::SceneId sceneId, ecs::Entity entity, ecs::Entity entityCopy, const glm::vec2 &previewSize)
	{
		auto &app = getApp();
  		renderer::FramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {
            renderer::FrameBufferTextureFormats::RGBA8,
            renderer::FrameBufferTextureFormats::RED_INTEGER,
            renderer::FrameBufferTextureFormats::Depth
        };
        framebufferSpecs.width = static_cast<unsigned int>(previewSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(previewSize.y);
        auto &transformComponentBase = app.m_coordinator->getComponent<components::TransformComponent>(entity);
        auto &transformComponent = app.m_coordinator->getComponent<components::TransformComponent>(entityCopy);

        // Create the render target for the preview scene.
        auto framebuffer = renderer::Framebuffer::create(framebufferSpecs);

        float distance = transformComponentBase.size.z * 2.0f;

        // Define default angular offsets (in degrees) for yaw and pitch
        float defaultYawDeg   = 30.0f; // horizontal offset
        float defaultPitchDeg = -20.0f; // vertical offset

        // Convert the angles to radians
        float defaultYaw   = glm::radians(defaultYawDeg);
        float defaultPitch = glm::radians(defaultPitchDeg);

        // Set the target position for the camera.
        // In this preview, the target is the copied entity, whose transform we set above.
        glm::vec3 targetPos = transformComponent.pos;

        // Start with an initial offset vector.
        // Here we assume the camera initially lies along the positive Z axis (relative to the target).
        glm::vec3 initialOffset = glm::vec3(0.0f, 0.0f, distance);

        // Create an incremental quaternion for horizontal rotation (yaw) about the world up.
        glm::quat qYaw = glm::angleAxis(defaultYaw, glm::vec3(0, 1, 0));

        // For the pitch (vertical rotation), compute the right axis.
        glm::vec3 rightAxis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), initialOffset));
        if (glm::length(rightAxis) < 0.001f)  // Fallback if the vector is degenerate.
            rightAxis = glm::vec3(1, 0, 0);
        glm::quat qPitch = glm::angleAxis(defaultPitch, rightAxis);

        // Combine the yaw and pitch rotations, similar to the event handler logic.
        glm::quat incrementalRotation = qYaw * qPitch;

        // Apply the incremental rotation to the initial offset to get the final offset.
        glm::vec3 newOffset = incrementalRotation * initialOffset;
        // Normalize and apply the desired distance (optional if you need to clamp or adjust)
        newOffset = glm::normalize(newOffset) * distance;

        // Compute the camera's starting position.
        glm::vec3 cameraPos = targetPos + newOffset;

        // Create the perspective camera using the computed position.
        ecs::Entity cameraId = CameraFactory::createPerspectiveCamera(cameraPos,
            framebufferSpecs.width, framebufferSpecs.height, framebuffer);

        // Update the camera's transform.
        auto &cameraTransform = app.m_coordinator->getComponent<components::TransformComponent>(cameraId);
        cameraTransform.pos = cameraPos;

        // Compute the camera's orientation so that it looks at the target.
        glm::vec3 newFront = glm::normalize(targetPos - cameraPos);
        cameraTransform.quat = glm::normalize(glm::quatLookAt(newFront, glm::vec3(0.0f, 1.0f, 0.0f)));

        components::PerspectiveCameraTarget cameraTarget;
        cameraTarget.targetEntity = entityCopy;
        cameraTarget.distance = transformComponentBase.size.z * 2.0f;
        app.m_coordinator->addComponent<components::PerspectiveCameraTarget>(cameraId, cameraTarget);
        app.getSceneManager().getScene(sceneId).addEntity(cameraId);
        return cameraId;
	}

	static void setupPreviewLights(scene::SceneId sceneId, ecs::Entity entityCopy)
	{
		auto &app = getApp();
		auto &transformComponent = app.m_coordinator->getComponent<components::TransformComponent>(entityCopy);

		app.getSceneManager().getScene(sceneId).addEntity(entityCopy);
        ecs::Entity ambientLight = LightFactory::createAmbientLight({0.5f, 0.5f, 0.5f});
        app.getSceneManager().getScene(sceneId).addEntity(ambientLight);
        ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.2f, -1.0f, -0.3f});
        app.getSceneManager().getScene(sceneId).addEntity(directionalLight);
        float spotLightHalfAngle = utils::computeSpotlightHalfAngle(transformComponent, {0.0, 2.0f, -5.0f});
        float margin = glm::radians(2.5f);
        ecs::Entity spotLight = LightFactory::createSpotLight({0.0f, 2.0f, -5.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 0.0900000035F, 0.0320000015F, glm::cos(spotLightHalfAngle), glm::cos(spotLightHalfAngle + margin));
        app.getSceneManager().getScene(sceneId).addEntity(spotLight);
	}

	void genScenePreview(const std::string &uniqueSceneName, const glm::vec2 &previewSize, ecs::Entity entity, ScenePreviewOut &out)
	{
		auto &app = getApp();

        out.sceneId = app.getSceneManager().createEditorScene(uniqueSceneName);

        out.entityCopy = copyEntity(entity);

        out.cameraId = createPreviewCamera(out.sceneId, entity, out.entityCopy, previewSize);

        setupPreviewLights(out.sceneId, out.entityCopy);
        out.sceneGenerated = true;
	}
}
