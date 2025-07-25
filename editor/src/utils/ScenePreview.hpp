//// ScenePreview.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        19/03/2025
//  Description: Header file for the scene preview utils
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "components/Transform.hpp"
#include "core/scene/SceneManager.hpp"
#include <glm/glm.hpp>

namespace nexo::editor::utils {

	/**
     * @brief Structure to store output data for a generated scene preview.
     *
     * Contains the scene ID, the preview camera entity ID, and a copy of the entity
     * being previewed. The flag @c sceneGenerated indicates whether the preview scene
     * was successfully generated.
     */
    struct ScenePreviewOut {
        scene::SceneId sceneId{};  ///< The ID of the generated preview scene.
        ecs::Entity cameraId{};    ///< The entity ID of the preview camera.
        ecs::Entity entityCopy{};  ///< A copy of the original entity for preview purposes.
        bool sceneGenerated = false;  ///< Flag indicating whether the scene preview was generated.
    };

    /**
     * @brief Computes an approximate bounding sphere radius for an object.
     *
     * The radius is approximated by taking half the maximum dimension (x, y, or z)
     * of the object's transform size.
     *
     * @param objectTransform The transform component of the object.
     * @return float The computed bounding sphere radius.
     */
	float computeBoundingSphereRadius(const components::TransformComponent &objectTransform);

	/**
	* @brief Computes the half-angle of a spotlight based on an object's transform.
	*
	* Uses the bounding sphere radius of the object and the distance between the object
	* and the light position to compute the half-angle of the spotlight.
	*
	* @param objectTransform The transform component of the object.
	* @param lightPosition The position of the light.
	* @return float The computed half-angle in radians.
	*/
	float computeSpotlightHalfAngle(const components::TransformComponent &objectTransform, const glm::vec3 &lightPosition);

	/**
     * @brief Generates a scene preview.
     *
     * Creates an editor scene with a copy of the given entity, a preview camera, and some default lights.
     * The generated scene's ID, camera entity, and entity copy are stored in @p out.
     *
     * @param uniqueSceneName A unique name for the preview scene.
     * @param previewSize The size (width, height) of the preview.
     * @param entity The entity to generate the preview from.
     * @param out Output structure containing preview scene details.
     */
	void genScenePreview(const std::string &uniqueSceneName, const glm::vec2 &previewSize, ecs::Entity entity, ScenePreviewOut &out, const glm::vec4 &clearColor = {0.05f, 0.05f, 0.05f, 0.0f});

}
