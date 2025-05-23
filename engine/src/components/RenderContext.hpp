//// RenderContext.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        09/03/2025
//  Description: Header file for the render context singleton component
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <queue>
#include "Camera.hpp"
#include "renderer/Renderer3D.hpp"
#include "Light.hpp"

namespace nexo::components {
	struct RenderContext {
		int sceneRendered = -1;
		renderer::Renderer3D renderer3D;
		std::queue<CameraContext> cameras;
		LightContext sceneLights;

		RenderContext()
		{
			renderer3D.init();
		}

		RenderContext(RenderContext&& other) noexcept
		    : sceneRendered(other.sceneRendered),
		        renderer3D(std::move(other.renderer3D)),
		        cameras(std::move(other.cameras)),
		        sceneLights(std::move(other.sceneLights))
		{
		}

        ~RenderContext()
        {
            renderer3D.shutdown();

            reset();
        }

		void reset()
		{
			sceneRendered = -1;
			std::queue<CameraContext> empty;
			std::swap(cameras, empty);
			sceneLights.ambientLight = glm::vec3(0.0f);
			sceneLights.pointLightCount = 0;
			sceneLights.spotLightCount = 0;
			sceneLights.directionalLightCount = 0;
		}
	};
}
