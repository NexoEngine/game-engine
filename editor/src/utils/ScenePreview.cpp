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
#include "CameraFactory.hpp"
#include "LightFactory.hpp"
#include "Nexo.hpp"
#include "components/Camera.hpp"
#include "components/MaterialComponent.hpp"
#include "components/StaticMesh.hpp"

namespace nexo::editor::utils {

    float computeBoundingSphereRadius(const components::TransformComponent &objectTransform)
    {
        const float halfX = objectTransform.size.x * 0.5f;
        const float halfY = objectTransform.size.y * 0.5f;
        const float halfZ = objectTransform.size.z * 0.5f;
        return glm::max(halfX, glm::max(halfY, halfZ));
    }

    float computeSpotlightHalfAngle(const components::TransformComponent &objectTransform,
                                    const glm::vec3 &lightPosition)
    {
        const float radius   = computeBoundingSphereRadius(objectTransform);
        const float distance = glm::length(objectTransform.pos - lightPosition);
        // Prevent division by zero
        if (distance < 0.001f) return glm::radians(15.0f);
        return atanf(radius / distance);
    }

    static ecs::Entity copyEntity(const ecs::Entity entity)
    {
        // const ecs::Entity entityCopy = Application::m_coordinator->createEntity();
        // const auto staticMeshCopy =
        // Application::m_coordinator->getComponent<components::StaticMeshComponent>(entity); const auto materialCopy =
        // Application::m_coordinator->getComponent<components::MaterialComponent>(entity); const auto
        // &transformComponentBase =
        //     Application::m_coordinator->getComponent<components::TransformComponent>(entity);
        // components::TransformComponent transformComponent;
        // transformComponent.pos  = {0.0f, 0.0f, -transformComponentBase.size.z * 2.0f};
        // transformComponent.quat = glm::quat(1.0f, 0.0f, 0.0f, 0.0f);
        // transformComponent.size = transformComponentBase.size;
        // Application::m_coordinator->addComponent(entityCopy, staticMeshCopy);
        // Application::m_coordinator->addComponent(entityCopy, materialCopy);
        // Application::m_coordinator->addComponent(entityCopy, transformComponent);
        // return entityCopy;
        return Application::m_coordinator->duplicateEntity(entity);
    }

    glm::vec3 oldComputeCameraPosition(const ecs::Entity entity)
    {
        const auto &transformComponentBase =
            Application::m_coordinator->getComponent<components::TransformComponent>(entity);
        // If no vertices are available, use the transform component's size to compute the camera position
        float distance = transformComponentBase.size.z * 3.0f;

        float defaultYawDeg   = 30.0f;  // horizontal offset
        float defaultPitchDeg = -20.0f; // vertical offset

        float defaultYaw   = glm::radians(defaultYawDeg);
        float defaultPitch = glm::radians(defaultPitchDeg);

        glm::vec3 targetPos = transformComponentBase.pos;

        glm::vec3 initialOffset = {0.0f, 0.0f, distance};

        glm::quat qYaw = glm::angleAxis(defaultYaw, glm::vec3(0, 1, 0));

        glm::vec3 rightAxis = glm::normalize(glm::cross(glm::vec3(0, 1, 0), initialOffset));
        if (glm::length(rightAxis) < 0.001f) // Fallback if the vector is degenerate.
            rightAxis = glm::vec3(1, 0, 0);
        glm::quat qPitch = glm::angleAxis(defaultPitch, rightAxis);

        glm::quat incrementalRotation = qYaw * qPitch;

        glm::vec3 newOffset = incrementalRotation * initialOffset;
        newOffset           = glm::normalize(newOffset) * distance;

        glm::vec3 cameraPos = targetPos + newOffset;
        std::cout << "Camera position computed: " << cameraPos.x << ", " << cameraPos.y << ", " << cameraPos.z
                  << std::endl;
        return cameraPos;
    }

    glm::vec3 computeCameraPosition(const ecs::Entity entity, const float verticalFovDeg, const float aspectRatio,
                                    const glm::vec3 &camForward = glm::vec3(0, 0, -1))
    {
        const auto &modelComponent = Application::m_coordinator->tryGetComponent<components::ModelComponent>(entity);
        if (!modelComponent) {
            LOG(NEXO_ERROR, "Entity {} does not have model component, using default camera position computation",
                entity);
            return oldComputeCameraPosition(entity);
        }
        // const auto vertices = modelComponent.model.getVertices(); // TODO: Create get vertices method for the model
        const std::vector<glm::vec3> &vertices = {};

        if (vertices.empty()) {
            LOG(NEXO_ERROR, "No vertices available for entity {}, using default camera position computation", entity);
            return oldComputeCameraPosition(entity);
        }

        // 1) Find AABB min/max
        glm::vec3 vMin{std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(),
                       std::numeric_limits<float>::infinity()};
        glm::vec3 vMax{-std::numeric_limits<float>::infinity(), -std::numeric_limits<float>::infinity(),
                       -std::numeric_limits<float>::infinity()};

        for (const auto &v : vertices) {
            vMin.x = std::min(vMin.x, v.x);
            vMin.y = std::min(vMin.y, v.y);
            vMin.z = std::min(vMin.z, v.z);
            vMax.x = std::max(vMax.x, v.x);
            vMax.y = std::max(vMax.y, v.y);
            vMax.z = std::max(vMax.z, v.z);
        }

        // 2) Compute center & half‐extents
        const glm::vec3 center{(vMin.x + vMax.x) * 0.5f, (vMin.y + vMax.y) * 0.5f, (vMin.z + vMax.z) * 0.5f};
        const glm::vec3 extents{(vMax.x - vMin.x) * 0.5f, (vMax.y - vMin.y) * 0.5f, (vMax.z - vMin.z) * 0.5f};

        // 3) Compute half‐angles in radians
        constexpr float deg2rad = 3.14159265f / 180.0f;
        const float halfVFovRad = 0.5f * verticalFovDeg * deg2rad;
        const float halfHFovRad = std::atan(std::tan(halfVFovRad) * aspectRatio);

        // 4) Compute distances needed to fit height & width
        const float dVert    = extents.y / std::tan(halfVFovRad);
        const float dHoriz   = extents.x / std::tan(halfHFovRad);
        const float distance = std::max(dVert, dHoriz);

        // 5) Position camera: move back from center along the NEGATIVE of the forward vector
        const glm::vec3 forwardN = normalize(camForward);
        return center - forwardN * distance;
    }

    static ecs::Entity createPreviewCamera(const scene::SceneId sceneId, const ecs::Entity entity,
                                           const ecs::Entity entityCopy, const glm::vec2 &previewSize,
                                           const glm::vec4 &clearColor)
    {
        auto &app = getApp();
        renderer::NxFramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = {renderer::NxFrameBufferTextureFormats::RGBA8,
                                        renderer::NxFrameBufferTextureFormats::RED_INTEGER,
                                        renderer::NxFrameBufferTextureFormats::Depth};
        framebufferSpecs.width       = static_cast<unsigned int>(previewSize.x);
        framebufferSpecs.height      = static_cast<unsigned int>(previewSize.y);
        const auto &transformComponentBase =
            Application::m_coordinator->getComponent<components::TransformComponent>(entity);
        const auto &transformComponent =
            Application::m_coordinator->getComponent<components::TransformComponent>(entityCopy);

        const auto framebuffer = renderer::NxFramebuffer::create(framebufferSpecs);

        const glm::vec3 cameraPos = computeCameraPosition(entity, 45.0f, previewSize.x / previewSize.y,
                                                    transformComponentBase.pos - transformComponent.pos);

        const ecs::Entity cameraId = CameraFactory::createPerspectiveCamera(cameraPos, framebufferSpecs.width,
                                                                      framebufferSpecs.height, framebuffer, clearColor);

        auto &cameraTransform  = Application::m_coordinator->getComponent<components::TransformComponent>(cameraId);
        cameraTransform.pos    = cameraPos;
        auto &cameraComponent  = Application::m_coordinator->getComponent<components::CameraComponent>(cameraId);
        cameraComponent.render = true;

        const glm::vec3 newFront   = glm::normalize(transformComponent.pos - cameraPos);
        cameraTransform.quat = glm::normalize(glm::quatLookAt(newFront, glm::vec3(0.0f, 1.0f, 0.0f)));

        components::PerspectiveCameraTarget cameraTarget;
        cameraTarget.targetEntity = entityCopy;
        cameraTarget.distance     = transformComponentBase.size.z * 2.0f;
        Application::m_coordinator->addComponent<components::PerspectiveCameraTarget>(cameraId, cameraTarget);
        app.getSceneManager().getScene(sceneId).addEntity(cameraId);
        return cameraId;
    }

    static void setupPreviewLights(const scene::SceneId sceneId, const ecs::Entity entityCopy)
    {
        auto &app = getApp();
        const auto &transformComponent =
            Application::m_coordinator->getComponent<components::TransformComponent>(entityCopy);

        app.getSceneManager().getScene(sceneId).addEntity(entityCopy);
        const ecs::Entity ambientLight = LightFactory::createAmbientLight({0.5f, 0.5f, 0.5f});
        app.getSceneManager().getScene(sceneId).addEntity(ambientLight);
        const ecs::Entity directionalLight = LightFactory::createDirectionalLight({0.2f, -1.0f, -0.3f});
        app.getSceneManager().getScene(sceneId).addEntity(directionalLight);
        const float spotLightHalfAngle = utils::computeSpotlightHalfAngle(transformComponent, {0.0, 2.0f, -5.0f});
        constexpr float margin         = glm::radians(2.5f);
        const ecs::Entity spotLight    = LightFactory::createSpotLight(
            {0.0f, 2.0f, -5.0f}, {0.0f, -1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, 0.0900000035F, 0.0320000015F,
            glm::cos(spotLightHalfAngle), glm::cos(spotLightHalfAngle + margin));
        app.getSceneManager().getScene(sceneId).addEntity(spotLight);
    }

    void genScenePreview(const std::string &uniqueSceneName, const glm::vec2 &previewSize, const ecs::Entity entity,
                         ScenePreviewOut &out, const glm::vec4 &clearColor)
    {
        auto &app = getApp();

        out.sceneId = app.getSceneManager().createEditorScene(uniqueSceneName);

        out.entityCopy = copyEntity(entity);

        out.cameraId = createPreviewCamera(out.sceneId, entity, out.entityCopy, previewSize, clearColor);

        setupPreviewLights(out.sceneId, out.entityCopy);
        out.sceneGenerated = true;
    }
} // namespace nexo::editor::utils
