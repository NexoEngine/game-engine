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
#include "SystemProfiler.hpp"
#include "components/Light.hpp"
#include "renderPasses/GPUResources.hpp"

namespace nexo::system {

    void LightSystem::update(components::RenderContext &renderContext) const
    {
        PROFILE_SYSTEM("LightSystem", 0);
        renderer::GpuLightBlock gpuLightBlock{};
        gpuLightBlock.ambientLight = m_ambientLightSystem->update();
        gpuLightBlock.dirLight = m_directionalLightSystem->update();
        auto pointLightContext = m_pointLightSystem->update();
        gpuLightBlock.numPointLights = pointLightContext.nbPointLights;
        for (unsigned int i = 0; i < MAX_POINT_LIGHTS; ++i)
        {
            gpuLightBlock.pointLights[i] = pointLightContext.pointLights[i];
        }
        auto spotLightContext = m_spotLightSystem->update();
        for (unsigned int i = 0; i < MAX_SPOT_LIGHTS; ++i)
        {
            gpuLightBlock.spotLights[i] = spotLightContext.spotLights[i];
        }
        gpuLightBlock.numSpotLights = spotLightContext.nbSpotLights;

        for (auto &camera : renderContext.cameras) {
            camera.pipeline.setUniformBufferData(LIGHT_UBO, &gpuLightBlock, sizeof(renderer::GpuLightBlock));
        }
    }
} // namespace nexo::system
