//// ScenePreview.cpp ///////////////////////////////////////////////////////////////
//
// ⢀⢀⢀⣤⣤⣤⡀⢀⢀⢀⢀⢀⢀⢠⣤⡄⢀⢀⢀⢀⣠⣤⣤⣤⣤⣤⣤⣤⣤⣤⡀⢀⢀⢀⢠⣤⣄⢀⢀⢀⢀⢀⢀⢀⣤⣤⢀⢀⢀⢀⢀⢀⢀⢀⣀⣄⢀⢀⢠⣄⣀⢀⢀⢀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⣿⣷⡀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡟⡛⡛⡛⡛⡛⡛⡛⢁⢀⢀⢀⢀⢻⣿⣦⢀⢀⢀⢀⢠⣾⡿⢃⢀⢀⢀⢀⢀⣠⣾⣿⢿⡟⢀⢀⡙⢿⢿⣿⣦⡀⢀⢀⢀⢀
// ⢀⢀⢀⣿⣿⡛⣿⣷⡀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡙⣿⡷⢀⢀⣰⣿⡟⢁⢀⢀⢀⢀⢀⣾⣿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⣿⡆⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⡈⢿⣷⡄⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣇⣀⣀⣀⣀⣀⣀⣀⢀⢀⢀⢀⢀⢀⢀⡈⢀⢀⣼⣿⢏⢀⢀⢀⢀⢀⢀⣼⣿⡏⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⡘⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⡈⢿⣿⡄⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⣿⢿⢿⢿⢿⢿⢿⢿⢇⢀⢀⢀⢀⢀⢀⢀⢠⣾⣿⣧⡀⢀⢀⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⡈⢿⣿⢀⢀⢸⣿⡇⢀⢀⢀⢀⣿⣿⡇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣰⣿⡟⡛⣿⣷⡄⢀⢀⢀⢀⢀⢿⣿⣇⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣿⣿⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⡈⢿⢀⢀⢸⣿⡇⢀⢀⢀⢀⡛⡟⢁⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⡟⢀⢀⡈⢿⣿⣄⢀⢀⢀⢀⡘⣿⣿⣄⢀⢀⢀⢀⢀⢀⢀⢀⢀⣼⣿⢏⢀⢀⢀
// ⢀⢀⢀⣿⣿⢀⢀⢀⢀⢀⢀⢀⢀⢸⣿⡇⢀⢀⢀⢀⢀⣀⣀⣀⣀⣀⣀⣀⣀⣀⡀⢀⢀⢀⣠⣾⡿⢃⢀⢀⢀⢀⢀⢻⣿⣧⡀⢀⢀⢀⡈⢻⣿⣷⣦⣄⢀⢀⣠⣤⣶⣿⡿⢋⢀⢀⢀⢀
// ⢀⢀⢀⢿⢿⢀⢀⢀⢀⢀⢀⢀⢀⢸⢿⢃⢀⢀⢀⢀⢻⢿⢿⢿⢿⢿⢿⢿⢿⢿⢃⢀⢀⢀⢿⡟⢁⢀⢀⢀⢀⢀⢀⢀⡙⢿⡗⢀⢀⢀⢀⢀⡈⡉⡛⡛⢀⢀⢹⡛⢋⢁⢀⢀⢀⢀⢀⢀
//
//  Author:      Mehdy MORVAN
//  Date:        19/03/2025
//  Description: Source file for the scene preview utils
//
///////////////////////////////////////////////////////////////////////////////

#include "CameraFactory.hpp"
#include "LightFactory.hpp"
#include "Nexo.hpp"
#include "ScenePreview.hpp"
#include "components/Camera.hpp"
#include "components/Parent.hpp"
#include "math/Bounds.hpp"

namespace nexo::editor::utils {

    namespace constants {
        // Default bounding sphere
        constexpr float kDefaultBoundingSpherePadding = 0.8f;

        // Camera setup
        constexpr float kFramePadding = 1.12f;
        constexpr float kDefaultFov = 45.0f;
        constexpr glm::vec3 kCamThumbnailDir = glm::vec3(-0.321521f, -0.229658f, -0.91863f); // = normalize({-0.35f, -0.25f, -1.0f})
        constexpr float kPlanePadding = 1.3f;

        // Light setup
        constexpr glm::vec3 kAmbientLightColor = glm::vec3(0.5f, 0.5f, 0.5f);
        constexpr glm::vec3 kDirectionalLightDir = glm::vec3(0.2f, -1.0f, -0.3f);
        constexpr glm::vec3 kSpotLightPos = glm::vec3(0.0f, 2.0f, -5.0f);
        constexpr float kSpotLightOuterMargin = 2.5f;
        constexpr glm::vec3 kSpotLightDir = glm::vec3(0.0f, -1.0f, 0.0f);
    }

    /**
     * @brief Compute the world-space bounding sphere of an entity.
     *
     * Prefers the model’s precomputed root bounding sphere if the entity has a RootComponent,
     * conservatively transformed to world space (center transformed by world matrix, radius scaled
     * by the max axis scale). If no model is present, a fallback sphere is synthesized from the
     * entity’s TransformComponent::size (scaled by 0.8) and transformed to world.
     *
     * @param entity The entity whose bounds are requested.
     * @return math::BSphere The world-space bounding sphere (never empty).
     */
    static math::BSphere getWorldSphere(const ecs::Entity entity)
    {
        const auto& transform = Application::m_coordinator->getComponent<components::TransformComponent>(entity);
        const glm::mat4 worldMatrix = transform.worldMatrix;

        if (const auto root = Application::m_coordinator->tryGetComponent<components::RootComponent>(entity)) {
            if (auto model = root->get().modelRef.lock()) {
                return sphereTransform(model->rootSphere, worldMatrix);
            }
        }

        // Use transform size to compute a basic bounding sphere if the entity is not a model
        const glm::vec3 half = constants::kDefaultBoundingSpherePadding * transform.size;
        math::BSphere bsLocal;
        bsLocal.c = glm::vec3(0.0f);
        bsLocal.r = glm::length(half);
        return sphereTransform(bsLocal, worldMatrix);
    }

    /**
     * @brief Compute the half-angle for a spotlight that tightly covers an entity.
     *
     * Uses the entity’s world-space bounding sphere radius and the distance from light to the
     * entity’s position to derive the required half-angle: atan(radius / distance). A minimum
     * distance guard avoids divide-by-zero and returns a reasonable fallback.
     *
     * @param entity        The target entity (bounds and transform are read).
     * @param lightPosition The spotlight world position.
     * @return float        The spotlight half-angle in radians.
     *
     * @note The returned angle already matches GL/lighting conventions (radians).
     */
    static float computeSpotlightHalfAngle(ecs::Entity entity,
                                    const glm::vec3 &lightPosition)
    {
        const float radius = getWorldSphere(entity).r;
        const auto &transformComponent =
            Application::m_coordinator->getComponent<components::TransformComponent>(entity);
        const float distance = glm::length(transformComponent.pos - lightPosition);
        // Prevent division by zero
        if (distance < 0.001f)
            return glm::radians(15.0f);
        return atanf(radius / distance);
    }

    /**
     * @brief Fit a perspective camera to a world-space bounding sphere.
     *
     * Computes a camera position along a fixed viewing direction so that the entire sphere
     * fits within both the vertical and horizontal FOV. Also produces tight near/far planes.
     * A small padding factor is applied so the object doesn’t touch the viewport edges.
     *
     * @param bs            World-space bounding sphere (center/radius in world units).
     * @param aspect        Viewport aspect ratio (width / height).
     * @param outCamPos     [out] Computed camera world position.
     * @param outNearPlane  [out] Suggested near plane distance.
     * @param outFarPlane   [out] Suggested far plane distance.
     *
     * @details
     * The required distance is d = max( R / tan(fovy/2), R / tan(fovx/2) ), where
     * R is the padded radius and fovx = 2 * atan( tan(fovy/2) * aspect ).
     * The camera is placed at C - d * camDir, where C is the sphere center.
     *
     * @note The viewing direction is a fixed 3/4 view for consistent thumbnails.
     * @warning near/far are chosen tightly around the sphere to improve depth precision;
     *          clamp near to a small epsilon to avoid clipping at the camera.
     */
    static void fitCameraToBoundingSphere(const math::BSphere& bs,
                                       float aspect,
                                       glm::vec3 &outCamPos,
                                       float &outNearPlane,
                                       float &outFarPlane)
    {

        const float fovy = glm::radians(constants::kDefaultFov);
        const float fovx = 2.0f * std::atan(std::tan(fovy * 0.5f) * aspect);

        const float radius = bs.r * constants::kFramePadding;
        const float dV = radius / std::tan(fovy * 0.5f);
        const float dH = radius / std::tan(fovx * 0.5f);
        const float d  = std::max(dV, dH);

        const glm::vec3 camPos = bs.c - d * constants::kCamThumbnailDir;

        const float nearP = std::max(0.01f, d - radius * constants::kPlanePadding);
        const float farP  = d + radius * constants::kPlanePadding;

        outCamPos = camPos;
        outNearPlane = nearP;
        outFarPlane = farP;
    }

    /**
     * @brief Create and register a preview camera that frames an entity for thumbnail rendering.
     *
     * Allocates a framebuffer with color, ID, and depth attachments, computes the entity’s
     * world-space bounding sphere, fits a camera to it (position and near/far), and orients
     * the camera to look at the sphere center. The camera is added to the given editor scene.
     *
     * @param sceneId     Target editor scene ID to receive the camera.
     * @param entityCopy  The (possibly duplicated) entity to frame in the preview.
     * @param previewSize Preview resolution in pixels (x = width, y = height).
     * @param clearColor  Clear color applied to the camera’s framebuffer.
     * @return ecs::Entity The created camera entity ID.
     *
     * @note Uses a fixed vertical FOV of 45° for consistent previews.
     */
    static ecs::Entity createPreviewCamera(const scene::SceneId sceneId,
                                           const ecs::Entity entityCopy, const glm::vec2 &previewSize,
                                           const glm::vec4 &clearColor)
    {
        auto &app = getApp();
        renderer::NxFramebufferSpecs framebufferSpecs;
        framebufferSpecs.attachments = renderer::NxFrameBufferAttachmentsSpecifications(
            {renderer::NxFrameBufferTextureSpecifications(renderer::NxFrameBufferTextureFormats::RGBA8),
             renderer::NxFrameBufferTextureSpecifications(renderer::NxFrameBufferTextureFormats::RED_INTEGER),
             renderer::NxFrameBufferTextureSpecifications(renderer::NxFrameBufferTextureFormats::Depth)});
        framebufferSpecs.width  = static_cast<unsigned int>(previewSize.x);
        framebufferSpecs.height = static_cast<unsigned int>(previewSize.y);

        const auto framebuffer = renderer::NxFramebuffer::create(framebufferSpecs);

        const math::BSphere worldSphere = getWorldSphere(entityCopy);

        const float aspect  = framebufferSpecs.width / float(std::max(1u, framebufferSpecs.height));
        glm::vec3 camPos;
        float farPlane;
        float nearPlane;
        fitCameraToBoundingSphere(worldSphere, aspect, camPos, nearPlane, farPlane);

        const ecs::Entity cameraId = CameraFactory::createPerspectiveCamera(
            camPos, framebufferSpecs.width, framebufferSpecs.height, framebuffer, clearColor, constants::kDefaultFov, nearPlane, farPlane);

        Application::m_coordinator->getComponent<components::CameraComponent>(cameraId).render = true;

        const glm::vec3 forward = glm::normalize(worldSphere.c - camPos);
        auto& cameraTransform = Application::m_coordinator->getComponent<components::TransformComponent>(cameraId);
        cameraTransform.quat = glm::normalize(glm::quatLookAt(forward, glm::vec3(0,1,0)));

        app.getSceneManager().getScene(sceneId).addEntity(cameraId);
        return cameraId;
    }

    /**
     * @brief Add basic lighting to a preview scene for consistent, readable thumbnails.
     *
     * Inserts the target entity into the scene, then creates:
     *  - an ambient light,
     *  - a directional light,
     *  - a spotlight with cone sized to cover the entity via its bounding sphere.
     *
     * @param sceneId    The editor scene receiving the lights.
     * @param entityCopy The entity instance to be lit in the preview scene.
     *
     * @note The spotlight’s inner/outer cones include a small angular margin to avoid edge clipping.
     */
    static void setupPreviewLights(const scene::SceneId sceneId, const ecs::Entity entityCopy)
    {
        auto &app = getApp();

        app.getSceneManager().getScene(sceneId).addEntity(entityCopy);
        const ecs::Entity ambientLight = LightFactory::createAmbientLight(constants::kAmbientLightColor);
        app.getSceneManager().getScene(sceneId).addEntity(ambientLight);
        const ecs::Entity directionalLight = LightFactory::createDirectionalLight(constants::kDirectionalLightDir);
        app.getSceneManager().getScene(sceneId).addEntity(directionalLight);
        const float spotLightHalfAngle = computeSpotlightHalfAngle(entityCopy, constants::kSpotLightPos);
        constexpr float margin = glm::radians(constants::kSpotLightOuterMargin);
        const ecs::Entity spotLight = LightFactory::createSpotLight(
            constants::kSpotLightPos, constants::kSpotLightDir, {1.0f, 1.0f, 1.0f}, 0.0900000035F, 0.0320000015F,
            glm::cos(spotLightHalfAngle), glm::cos(spotLightHalfAngle + margin));
        app.getSceneManager().getScene(sceneId).addEntity(spotLight);
    }

    void genScenePreview(const std::string &uniqueSceneName, const glm::vec2 &previewSize, const ecs::Entity entity,
                         ScenePreviewOut &out, const glm::vec4 &clearColor)
    {
        auto &app = getApp();

        out.sceneId = app.getSceneManager().createEditorScene(uniqueSceneName);

        out.entityCopy = Application::m_coordinator->duplicateEntity(entity);

        out.cameraId = createPreviewCamera(out.sceneId, out.entityCopy, previewSize, clearColor);

        setupPreviewLights(out.sceneId, out.entityCopy);
        out.sceneGenerated = true;
    }
} // namespace nexo::editor::utils
