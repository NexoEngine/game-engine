//// LightFactory.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/O3/2025
//  Description: Header for the light factory static class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/Entity.hpp"

#include <glm/glm.hpp>

namespace nexo {
	class LightFactory {
		public:
			static ecs::Entity createAmbientLight(glm::vec3 color);
			static ecs::Entity createDirectionalLight(glm::vec3 lightDir, glm::vec3 color = {1.0f, 1.0f, 1.0f});
			static ecs::Entity createPointLight(glm::vec3 position, glm::vec3 color = {1.0f, 1.0f, 1.0f}, float linear = 0.09f, float quadratic = 0.032f);
			static ecs::Entity createSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color = {1.0f, 1.0f, 1.0f}, float linear = 0.09f, float quadratic = 0.032f, float cutOff = glm::cos(glm::radians(12.5f)), float outerCutOff = glm::cos(glm::radians(15.0f)));
	};
}
