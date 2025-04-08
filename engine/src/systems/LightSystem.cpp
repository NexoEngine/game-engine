//// LightSystem.cpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/03/2025
//  Description: Source file for the light system
//
///////////////////////////////////////////////////////////////////////////////

#include "LightSystem.hpp"

namespace nexo::system {
	void LightSystem::update()
	{
		m_ambientLightSystem->update();
		m_directionalLightSystem->update();
		m_pointLightSystem->update();
		m_spotLightSystem->update();
	}
}
