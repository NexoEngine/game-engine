//// CameraFactory.hpp ////////////////////////////////////////////////////////
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
        /**
         * @brief Creates a perspective camera entity with specified parameters.
         * @param pos The position of the camera in 3D space.
         * @param width The width of the camera viewport.
         * @param height The height of the camera viewport.
         * @param renderTarget Optional framebuffer to render the camera's view to.
         * @param clearColor The color to clear the camera's view with.
         * @param fov The field of view angle in degrees.
         * @param nearPlane The near clipping plane distance.
         * @param farPlane The far clipping plane distance.
         * @return The created camera entity.
         */
        static ecs::Entity createPerspectiveCamera(glm::vec3 pos, unsigned int width, unsigned int height,
                                                   std::shared_ptr<renderer::NxFramebuffer> renderTarget = nullptr,
                                                   const glm::vec4 &clearColor = {37.0f / 255.0f, 35.0f / 255.0f,
                                                                                  50.0f / 255.0f, 111.0f / 255.0f},
                                                   float fov = 45.0f, float nearPlane = 1.0f, float farPlane = 100.0f);
    };
} // namespace nexo
