//// LightFactory.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Source file for the light factory static class
//
///////////////////////////////////////////////////////////////////////////////

#include "LightFactory.hpp"
#include "Application.hpp"

#include "components/Light.hpp"
#include "components/Uuid.hpp"

namespace nexo {
	ecs::Entity LightFactory::createAmbientLight(glm::vec3 color)
	{
		ecs::Entity newAmbientLight = Application::m_coordinator->createEntity();
		components::AmbientLightComponent newAmbientLightComponent{color};
		Application::m_coordinator->addComponent<components::AmbientLightComponent>(newAmbientLight, newAmbientLightComponent);
  		components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newAmbientLight, uuid);
		return newAmbientLight;
	}

	ecs::Entity LightFactory::createDirectionalLight(glm::vec3 lightDir, glm::vec3 color)
	{
		ecs::Entity newDirectionalLight = Application::m_coordinator->createEntity();
		components::DirectionalLightComponent newDirectionalLightComponent(lightDir, color);
		Application::m_coordinator->addComponent<components::DirectionalLightComponent>(newDirectionalLight, newDirectionalLightComponent);
		components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newDirectionalLight, uuid);
		return newDirectionalLight;
	}

	ecs::Entity LightFactory::createPointLight(glm::vec3 position, glm::vec3 color, float linear, float quadratic)
	{
		ecs::Entity newPointLight = Application::m_coordinator->createEntity();
		components::PointLightComponent newPointLightComponent(position, color, linear, quadratic);
		Application::m_coordinator->addComponent<components::PointLightComponent>(newPointLight, newPointLightComponent);
		components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newPointLight, uuid);
		return newPointLight;
	}

	ecs::Entity LightFactory::createSpotLight(glm::vec3 position, glm::vec3 direction,
											  glm::vec3 color, float linear,
											  float quadratic, float cutOff,
											  float outerCutOff)
	{
		ecs::Entity newSpotLight = Application::m_coordinator->createEntity();
		components::SpotLightComponent newSpotLightComponent(position, direction, color, cutOff, outerCutOff, linear, quadratic);
		Application::m_coordinator->addComponent<components::SpotLightComponent>(newSpotLight, newSpotLightComponent);
		components::UuidComponent uuid;
        Application::m_coordinator->addComponent<components::UuidComponent>(newSpotLight, uuid);
		return newSpotLight;
	}
}
