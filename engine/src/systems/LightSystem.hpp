//// LightSystem.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/03/2025
//  Description: Header file for the light system
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "lights/AmbientLightSystem.hpp"
#include "lights/DirectionalLightsSystem.hpp"
#include "lights/PointLightsSystem.hpp"
#include "lights/SpotLightsSystem.hpp"

namespace nexo::system {
	class LightSystem {
		public:
			LightSystem(const std::shared_ptr<AmbientLightSystem> &ambientSystem,
						const std::shared_ptr<DirectionalLightsSystem> &directionalSystem,
						const std::shared_ptr<PointLightsSystem> &pointSystem,
						const std::shared_ptr<SpotLightsSystem> &spotSystem) :
			m_ambientLightSystem(ambientSystem),
			m_directionalLightSystem(directionalSystem),
			m_pointLightSystem(pointSystem),
			m_spotLightSystem(spotSystem) {}

			void update();
		private:
			std::shared_ptr<AmbientLightSystem> m_ambientLightSystem = nullptr;
			std::shared_ptr<DirectionalLightsSystem> m_directionalLightSystem = nullptr;
			std::shared_ptr<PointLightsSystem> m_pointLightSystem = nullptr;
			std::shared_ptr<SpotLightsSystem> m_spotLightSystem = nullptr;
	};
}
