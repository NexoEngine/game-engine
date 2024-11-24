//// OpenGlRendererAPI.hpp ////////////////////////////////////////////////////
//
//  zzzzz       zzz  zzzzzzzzzzzzz    zzzz      zzzz       zzzzzz  zzzzz
//  zzzzzzz     zzz  zzzz                    zzzz       zzzz           zzzz
//  zzz   zzz   zzz  zzzzzzzzzzzzz         zzzz        zzzz             zzz
//  zzz    zzz  zzz  z                  zzzz  zzzz      zzzz           zzzz
//  zzz         zzz  zzzzzzzzzzzzz    zzzz       zzz      zzzzzzz  zzzzz
//
//  Author:      Mehdy MORVAN
//  Date:        08/11/2024
//  Description: Header file for open gl renderer api class
//
///////////////////////////////////////////////////////////////////////////////
#pragma once

#include "renderer/RendererAPI.hpp"

namespace nexo::renderer {

    class OpenGlRendererApi final : public RendererApi {
        public:
            void init() override;
            void setViewport(unsigned int x, unsigned int y, unsigned int width, unsigned int height) override;
            void getMaxViewportSize(unsigned int *width, unsigned int *height) override;
            void clear() override ;
            void setClearColor(const glm::vec4 &color) override;
            void setClearDepth(float depth) override;

            void drawIndexed(const std::shared_ptr<VertexArray> &vertexArray, unsigned int indexCount = 0) override;
        private:
            bool m_initialized = false;
            int m_maxWidth = 0;
            int m_maxHeight = 0;
    };
}
