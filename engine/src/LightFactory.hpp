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

	/**
	* @brief A static factory class for creating light entities.
	*
	* The LightFactory provides methods to create different types of lights (ambient, directional,
	* point, and spot lights) by setting up the necessary components (such as the corresponding
	* LightComponent and UuidComponent) for each light entity.
	*
	* @note Each created light entity will have a components::UuidComponent attached automatically.
	*/
	class LightFactory {
		public:
			/**
			* @brief Creates an ambient light entity.
			*
			* This function creates a new entity and attaches an AmbientLightComponent with the given color.
			*
			* @param color The RGB color of the ambient light.
			* @return ecs::Entity The newly created ambient light entity.
			*
			* @note Required Components added:
			*  - components::AmbientLightComponent
			*  - components::UuidComponent
			*/
			static ecs::Entity createAmbientLight(glm::vec3 color);

			/**
			* @brief Creates a directional light entity.
			*
			* This function creates a new entity and attaches a DirectionalLightComponent configured with
			* the specified light direction and color.
			*
			* @param lightDir The direction vector of the directional light.
			* @param color The RGB color of the directional light (default is white).
			* @return ecs::Entity The newly created directional light entity.
			*
			* @note Required Components added:
			*  - components::DirectionalLightComponent
			*  - components::UuidComponent
			*/
			static ecs::Entity createDirectionalLight(glm::vec3 lightDir, glm::vec3 color = {1.0f, 1.0f, 1.0f});

			/**
             * @brief Creates a point light entity.
             *
             * This function creates a new entity and attaches a PointLightComponent configured with
             * the specified position, color, and attenuation parameters.
             *
             * @param position The position of the point light.
             * @param color The RGB color of the point light (default is white).
             * @param linear The linear attenuation factor (default is 0.09f).
             * @param quadratic The quadratic attenuation factor (default is 0.032f).
             * @return ecs::Entity The newly created point light entity.
             *
             * @note Required Components added:
             *  - components::PointLightComponent
             *  - components::UuidComponent
             */
			static ecs::Entity createPointLight(glm::vec3 position, glm::vec3 color = {1.0f, 1.0f, 1.0f}, float linear = 0.09f, float quadratic = 0.032f);

			/**
             * @brief Creates a spot light entity.
             *
             * This function creates a new entity and attaches a SpotLightComponent configured with
             * the specified position, direction, color, and cutoff parameters.
             *
             * @param position The position of the spot light.
             * @param direction The direction vector of the spot light.
             * @param color The RGB color of the spot light (default is white).
             * @param linear The linear attenuation factor (default is 0.09f).
             * @param quadratic The quadratic attenuation factor (default is 0.032f).
             * @param cutOff The cosine of the inner cutoff angle (default is cos(12.5°)).
             * @param outerCutOff The cosine of the outer cutoff angle (default is cos(15.0°)).
             * @return ecs::Entity The newly created spot light entity.
             *
             * @note Required Components added:
             *  - components::SpotLightComponent
             *  - components::UuidComponent
             */
			static ecs::Entity createSpotLight(glm::vec3 position, glm::vec3 direction, glm::vec3 color = {1.0f, 1.0f, 1.0f}, float linear = 0.09f, float quadratic = 0.032f, float cutOff = glm::cos(glm::radians(12.5f)), float outerCutOff = glm::cos(glm::radians(15.0f)));
	};
}
