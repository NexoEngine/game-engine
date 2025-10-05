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

#include <glm/glm.hpp>
#include "components/Transform.hpp"
#include "core/scene/SceneManager.hpp"

namespace nexo::editor::utils {

    /**
     * @brief Structure to store output data for a generated scene preview.
     *
     * Contains the scene ID, the preview camera entity ID, and a copy of the entity
     * being previewed. The flag @c sceneGenerated indicates whether the preview scene
     * was successfully generated.
     */
    struct ScenePreviewOut {
        scene::SceneId sceneId{};    ///< The ID of the generated preview scene.
        ecs::Entity cameraId{};      ///< The entity ID of the preview camera.
        ecs::Entity entityCopy{};    ///< A copy of the original entity for preview purposes.
        bool sceneGenerated = false; ///< Flag indicating whether the scene preview was generated.
    };

    /**
     * @brief Generate a thumbnail preview scene for a single entity.
     *
     * Creates a dedicated editor scene, duplicates the entity for isolation, spawns a camera
     * that tightly frames the entity using a bounding-sphere fit, and sets up minimal lighting.
     * Returns identifiers via the output struct for later rendering and resource management.
     *
     * @param uniqueSceneName A unique name for the preview scene (used for scene registry).
     * @param previewSize     Preview resolution in pixels (x = width, y = height).
     * @param entity          The source entity to be duplicated into the preview scene.
     * @param out             [out] Filled with created scene/camera/entity IDs and a success flag.
     * @param clearColor      Clear color used by the preview camera.
     *
     * @note The duplicated entity avoids mutating original scene state during rendering.
     * @warning Ensure scene lifecycle handles cleanup of the created entities/framebuffer as needed.
     */
    void genScenePreview(const std::string &uniqueSceneName, const glm::vec2 &previewSize, ecs::Entity entity,
                         ScenePreviewOut &out, const glm::vec4 &clearColor = {0.05f, 0.05f, 0.05f, 0.0f});

} // namespace nexo::editor::utils
