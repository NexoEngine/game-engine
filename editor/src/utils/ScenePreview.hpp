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

	struct ScenePreviewOut {
		scene::SceneId sceneId;
		ecs::Entity cameraId;
		ecs::Entity entityCopy;

		bool sceneGenerated = false;
	};

	float computeBoundingSphereRadius(const components::TransformComponent &objectTransform);
	float computeSpotlightHalfAngle(const components::TransformComponent &objectTransform, const glm::vec3 &lightPosition);

	void genScenePreview(const std::string &uniqueSceneName, const glm::vec2 &previewSize, ecs::Entity entity, ScenePreviewOut &out);

}
