//// RendererAPI.hpp //////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for renderer api interface
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include <glm/glm.hpp>
#include <memory>

#include "VertexArray.hpp"

namespace nexo::renderer {

    class RendererApi {
        public:
            virtual ~RendererApi() = default;

            virtual void init() = 0;
            virtual void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) = 0;
            virtual void clear() = 0;
            virtual void setClearColor(const glm::vec4 &color) = 0;

            virtual void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray, unsigned int count = 0) = 0;
    };
}
