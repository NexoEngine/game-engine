//// RenderCommand.hpp ////////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for render command class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "RendererAPI.hpp"

namespace nexo::renderer {
    class RenderCommand {
        public:
            static void init();
            static void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) { _rendererApi->setViewport(x, y, width, height); };
            static void setClearColor(const glm::vec4 &color) { _rendererApi->setClearColor(color); };
            static void clear() { _rendererApi->clear(); };

            static void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray, unsigned int indexCount = 0)
            {
                _rendererApi->drawIndexed(vertexArray, indexCount);
            }

        private:
            static RendererApi *_rendererApi;
    };
}
