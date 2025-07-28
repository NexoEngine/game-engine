//// LightSystem.cpp //////////////////////////////////////////////////////////
//
//  zzzzz      zzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//  zzz zzz    zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz   zzz  zzz   zzzzzzzzz            zzzz        zzzz          zzzz
//  zzz    zzz zzz   zzzz              zzzz  zzzz      zzzz        zzzz
//  zzz      zzzzz   zzzzzzzzzzzzz   zzzz      zzzz      zzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/03/2025
//  Description: Source file for the light system
//
///////////////////////////////////////////////////////////////////////////////

#include "LightSystem.hpp"

namespace nexo::system {
	void LightSystem::update() const
	{
		m_ambientLightSystem->update();
		m_directionalLightSystem->update();
		m_pointLightSystem->update();
		m_spotLightSystem->update();
	}
}
