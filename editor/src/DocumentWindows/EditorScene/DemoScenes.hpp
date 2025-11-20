//// MainScene.hpp ////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Marie GIACOMEL
//  Date:        20/11/2025
//  Description: Header file for the demo scenes
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <Nexo.hpp>
#include <glm/vec3.hpp>

namespace nexo::editor {
    /**
     * @brief Creates a physics-enabled entity in the scene.
     *
     * This method creates an entity with a mesh, material, transform, and physics components
     * based on the provided parameters. The entity is added to the current scene and configured
     * with the specified position, size, rotation, color, shape type, and motion type.
     *
     * @param sceneId The ID of the scene where the entity will be created.
     * @param pos The position of the entity in world space.
     * @param size The size (scale) of the entity.
     * @param rotation The rotation of the entity in Euler angles (degrees).
     * @param color The color of the entity as a glm::vec4 (RGBA).
     * @param shapeType The shape type for the physics collider (e.g., box, sphere).
     * @param motionType The motion type for the physics body (e.g., static, dynamic).
     */
    void createEntityWithPhysic(int sceneId, const glm::vec3& pos, const glm::vec3& size, const glm::vec3& rotation,
                                const glm::vec4& color, system::ShapeType shapeType, JPH::EMotionType motionType);

    /**
     * @brief Adds a 3D model to the scene at the specified position, scale, and rotation.
     *
     * This method creates an entity with a model component using the provided model path,
     * and sets its transform based on the given position, scale, and rotation. The entity
     * is then added to the current scene.
     *
     * @param sceneId The ID of the scene where the entity will be created.
     * @param modelPath The file path to the 3D model asset.
     * @param position The position of the model in world space.
     * @param scale The scale of the model.
     * @param rotation The rotation of the model in Euler angles (degrees).
     */
    void addModelToScene(int sceneId, const std::string& modelPath, const glm::vec3& position,
                         const glm::vec3& scale = {1.0f, 1.0f, 1.0f}, const glm::vec3& rotation = {0.0f, 0.0f, 0.0f});

    /**
     * @brief Populates the scene with default entities.
     *
     * @param sceneId The ID of the scene where the chamboule setup will be created.
     * @param offset A glm::vec3 representing the positional offset to apply to all spheres.
     */
    void loadDefaultEntities(int sceneId, const glm::vec3& offset = {0.0f, 0.0f, 0.0f});

    /**
     * @brief Creates a chamboule scene with physics entities.
     *
     * This function sets up a chamboule scene by creating a series of physics-enabled entities
     * arranged in a specific pattern. It includes a base, walls, and pyramid-like structures,
     * all with specified positions, sizes, rotations, and colors.
     *
     * @param sceneId The ID of the scene where the chamboule setup will be created.
     * @param offset A glm::vec3 representing the positional offset to apply to all entities in the scene.
     */
    void chambouleScene(int sceneId, const glm::vec3& offset = {0.0f, 0.0f, 0.0f});

    /**
     * @brief Populates a scene with representing all features (lights, scripting, physic, model, textures).
     *
     * @param sceneId The ID of the scene where the entity will be created.
     * @param offset A glm::vec3 representing the positional offset to apply to demo scene.
     */
    void fullScene(int sceneId, const glm::vec3& offset = {0.0f, 0.0f, 0.0f});

    /** @brief Spawns a scene with multiple balls for physics testing.
     *
     * Creates a grid of sphere entities with physics properties, arranged in
     * a 5x5x5 formation. Each sphere is given a random color and positioned
     * based on the provided offset.
     *
     * @param sceneId The ID of the scene where the entity will be created.
     * @param offset A glm::vec3 representing the positional offset to apply to all spheres.
     */
    void spawnBallsScene(int sceneId, const glm::vec3& offset = {0.0f, 0.0f, 0.0f});

    /**
     * @brief Creates a scene with multiple light sources.
     *
     * Adds various types of lights (directional, point, spot, etc.) to the scene,
     * positioned according to the given offset. Useful for testing lighting and visual effects.
     *
     * @param sceneId The ID of the scene where the entity will be created.
     * @param offset Position offset to apply to all lights.
     */
    void lightsScene(int sceneId, const glm::vec3& offset = {0.0f, 0.0f, 0.0f});

    /**
     * @brief Creates a scene spawning different point lights sources turning around models.
     *
     * @param sceneId The ID of the scene where the entity will be created.
     * @param offset Position offset to apply to the scene.
     */
    void scriptedLightsOnModelsScene(int sceneId, const glm::vec3& offset = {0.0f, 0.0f, 0.0f});

    /**
     * @brief Creates a scene of a fakir game. (physic demo)
     *
     * Adds cylinder with physical properties (rigid, dynamic) to the scene,
     * positioned according to create a fakir game.
     *
     * @param sceneId The ID of the scene where the entity will be created.
     * @param offset Position offset to apply to all physics entities.
     */
    void fakirGameScene(int sceneId, const glm::vec3& offset = {0.0f, 0.0f, 0.0f});

    /**
     * @brief Creates a domino scene in the current scene. (physic demo)
     *
     * This method generates a series of domino entities arranged along a Bézier curve,
     * each with physics properties, at the specified offset position.
     *
     * @param sceneId The ID of the scene where the entity will be created.
     * @param offset A glm::vec3 representing the positional offset to apply to all dominoes.
     */
    void dominoScene(int sceneId, const glm::vec3& offset = {0.0f, 0.0f, 0.0f});

    /**
     * @brief Creates a scene containing video textures.
     *
     * Adds entities with animated (video) textures to the scene, positioned according to the offset.
     * Useful for testing video playback and synchronization in the engine.
     *
     * @param sceneId The ID of the scene where the entity will be created.
     * @param offset Position offset to apply to all video textures.
     */
    void videoScene(int sceneId, const glm::vec3& offset = {0.0f, 0.0f, 0.0f});

    /**
     * @brief Creates a forest-type scene.
     *
     * Adds many vegetation objects (trees, bushes) to the scene, positioned according to the offset.
     * Useful for testing rendering and management of large numbers of entities.
     *
     * @param sceneId The ID of the scene where the entity will be created.
     * @param offset Position offset to apply to all forest elements.
     */
    void forestScene(int sceneId, const glm::vec3& offset = {0.0f, 0.0f, 0.0f});
} // namespace nexo::editor
