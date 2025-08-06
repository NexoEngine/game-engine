//// LightSystem.cpp //////////////////////////////////////////////////////////
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
#include <tracy/Tracy.hpp>

namespace nexo::system {
	void LightSystem::update() const
	{
        ZoneScoped;
        ZoneName("Light System Update", 19);

        {
            ZoneScopedN("Ambient Light Update");
            m_ambientLightSystem->update();
        }

        {
            ZoneScopedN("Directional Light Update");
            m_directionalLightSystem->update();
        }

        {
            ZoneScopedN("Point Light Update");
            m_pointLightSystem->update();
        }

        {
            ZoneScopedN("Spot Light Update");
            m_spotLightSystem->update();
        }
	}
}
