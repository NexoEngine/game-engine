//// CameraFactory.hpp ///////////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        11/03/2025
//  Description: Header file for the camera factory static class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "ecs/Entity.hpp"
#include "renderer/Framebuffer.hpp"

#include <glm/glm.hpp>

namespace nexo {
	class CameraFactory {
		public:
			static ecs::Entity createPerspectiveCamera(glm::vec3 pos, unsigned int width,
       									               unsigned int height, std::shared_ptr<renderer::Framebuffer> renderTarget = nullptr,
                               				           const glm::vec4 &clearColor = {37.0f/255.0f, 35.0f/255.0f, 50.0f/255.0f, 111.0f/255.0f}, float fov = 45.0f, float nearPlane = 0.1f, float farPlane = 1000.0f);
	};
}
